// Copyright 2021 Esri.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
//
#include "SampleCppOAuth.h"

// Unreal

#include "Engine/SkyLight.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// ArcGISMapsSDK

#include "ArcGISMapsSDK/API/ArcGISRuntime/Geometry/SpatialReference.h"
#include "ArcGISMapsSDK/API/GameEngine/Camera/ArcGISCamera.h"
#include "ArcGISMapsSDK/API/GameEngine/Elevation/ArcGISImageElevationSource.h"
#include "ArcGISMapsSDK/API/GameEngine/Layers/ArcGISImageLayer.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISBasemap.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISMap.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISMapElevation.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISMapType.h"
#include "ArcGISMapsSDK/API/GameEngine/Security/ArcGISAuthenticationConfiguration.h"
#include "ArcGISMapsSDK/API/GameEngine/Security/ArcGISAuthenticationManager.h"
#include "ArcGISMapsSDK/API/GameEngine/Security/ArcGISOAuthAuthenticationConfiguration.h"
#include "ArcGISMapsSDK/API/GameEngine/View/ArcGISRendererView.h"
#include "ArcGISMapsSDK/API/Unreal/Collection.h"
#include "ArcGISMapsSDK/Actors/ArcGISMapView.h"
#include "ArcGISMapsSDK/Components/ArcGISCameraComponent.h"
#include "ArcGISMapsSDK/Components/ArcGISRendererComponent.h"
#include "ArcGISMapsSDK/Security/AuthenticationChallengeManager.h"

#if PLATFORM_ANDROID || PLATFORM_IOS
#include "Security/MobileOAuthAuthenticationChallengeHandler.h"
#else
#include "Security/DesktopOAuthAuthenticationChallengeHandler.h"
#endif

// Sets default values
ASampleCppOAuth::ASampleCppOAuth()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArcGISRendererComponent = CreateDefaultSubobject<UArcGISRendererComponent>(TEXT("ArcGISRendererComponent"));

	AddOwnedComponent(ArcGISRendererComponent);
}

// Called when the game starts or when spawned
void ASampleCppOAuth::BeginPlay()
{
	Super::BeginPlay();

#if PLATFORM_ANDROID || PLATFORM_IOS
	auto arcGISOAuthAuthenticationChallengeHandler = ::MakeShared<Esri::ArcGISMapsSDK::Security::FMobileOAuthAuthenticationChallengeHandler>();
#else
	auto arcGISOAuthAuthenticationChallengeHandler = ::MakeShared<Esri::ArcGISMapsSDK::Security::FDesktopOAuthAuthenticationChallengeHandler>();
#endif

	Esri::ArcGISMapsSDK::Security::AuthenticationChallengeManager::SetOAuthChallengeHandler(arcGISOAuthAuthenticationChallengeHandler);

	Esri::GameEngine::Security::ArcGISAuthenticationManager::GetAuthenticationConfigurations().RemoveAll();

	Esri::GameEngine::Security::ArcGISOAuthAuthenticationConfiguration authenticationConfiguration;

	auto trimmedClientID = ClientID.TrimStartAndEnd();
	auto trimmedRedirectURI = RedirectURI.TrimStartAndEnd();

	// ArcGIS Identity
	authenticationConfiguration =
		Esri::GameEngine::Security::ArcGISOAuthAuthenticationConfiguration(TCHAR_TO_ANSI(*trimmedClientID), "", TCHAR_TO_ANSI(*trimmedRedirectURI));

	Esri::GameEngine::Security::ArcGISAuthenticationManager::GetAuthenticationConfigurations().Insert(TCHAR_TO_ANSI(*ServiceURL),
																									  authenticationConfiguration);

	// Get planet shape
	auto ArcGISMapView = AArcGISMapView::GetArcGISMapView(this);
	check(ArcGISMapView);
	auto IsGlobal = ArcGISMapView->PlanetShape == EPlanetShape::RoundPlanet;

	// Create the map document
	auto mapType = IsGlobal ? Esri::GameEngine::Map::ArcGISMapType::Global : Esri::GameEngine::Map::ArcGISMapType::Local;
	auto arcGISMap = ::MakeShared<Esri::GameEngine::Map::ArcGISMap>(mapType);

	// Create and set a basemap
	auto arcGISBasemap =
		Esri::GameEngine::Map::ArcGISBasemap("https://www.arcgis.com/sharing/rest/content/items/716b600dbbac433faa4bec9220c76b3a/data", "");
	arcGISMap->SetBasemap(arcGISBasemap);

	// Create and set an elevation layer
	auto elevationLayer = Esri::GameEngine::Elevation::ArcGISImageElevationSource(
		"https://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer", "Elevation", "");
	arcGISMap->SetElevation(elevationLayer);

	// Create and set an OAuth-protected image layer
	auto imageLayer = ::MakeShared<Esri::GameEngine::Layers::ArcGISImageLayer>(TCHAR_TO_ANSI(*ServiceURL), "MyLayer_1", 1.0f, true, "");
	arcGISMap->GetLayers().Add(*imageLayer);

	imageLayer->SetLoadStatusChanged([imageLayer](Esri::ArcGISRuntime::LoadStatus loadStatus) {
		if (loadStatus == Esri::ArcGISRuntime::LoadStatus::FailedToLoad)
		{
			auto loadError = imageLayer->GetLoadError();

			UE_LOG(LogTemp, Error, TEXT("Failed to load the ArcGISImageLayer: %s"), ANSI_TO_TCHAR(loadError.what()));
		}
	});

	Esri::GameEngine::Location::ArcGISPosition position(0, 0, 0, Esri::ArcGISRuntime::Geometry::SpatialReference::WGS84());
	Esri::GameEngine::Location::ArcGISRotation rotation(0, 0, 0);

	// Create a camera
	FString cameraName("Camera");
	auto arcGISCamera = ::MakeShared<Esri::GameEngine::Camera::ArcGISCamera>(cameraName, position, rotation);

	// Create the renderer view options config struct
	Esri::GameEngine::View::ArcGISRendererViewOptions rendererViewOptions{false};

	// Create the renderer view and set it the camera and the map
	auto rendererView = ::MakeShared<Esri::GameEngine::View::ArcGISRendererView>(arcGISMap, arcGISCamera, rendererViewOptions);

	// Set the renderer view to the renderer component
	ArcGISRendererComponent->SetRendererView(rendererView);

	if (UArcGISCameraComponent* const arcGISDataCamera = Cast<UArcGISCameraComponent>(ArcGISDataCameraReference.GetComponent(GetOwner())))
	{
		// Set the renderer view to the camera component
		arcGISDataCamera->SetRendererView(rendererView);
	}
}
