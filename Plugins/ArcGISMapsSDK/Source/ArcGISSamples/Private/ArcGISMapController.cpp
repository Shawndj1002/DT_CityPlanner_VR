// Copyright 2021 Esri.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
//
#include "ArcGISMapController.h"

// Unreal

#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// ArcGISMapsSDK

#include "ArcGISMapsSDK/API/ArcGISRuntime/Geometry/SpatialReference.h"
#include "ArcGISMapsSDK/API/GameEngine/Camera/ArcGISCamera.h"
#include "ArcGISMapsSDK/API/GameEngine/Elevation/ArcGISImageElevationSource.h"
#include "ArcGISMapsSDK/API/GameEngine/Extent/ArcGISExtent.h"
#include "ArcGISMapsSDK/API/GameEngine/Extent/ArcGISExtentCircle.h"
#include "ArcGISMapsSDK/API/GameEngine/Extent/ArcGISExtentRectangle.h"
#include "ArcGISMapsSDK/API/GameEngine/Layers/ArcGIS3DModelLayer.h"
#include "ArcGISMapsSDK/API/GameEngine/Layers/ArcGISImageLayer.h"
#include "ArcGISMapsSDK/API/GameEngine/Layers/ArcGISIntegratedMeshLayer.h"
#include "ArcGISMapsSDK/API/GameEngine/Layers/Base/ArcGISLayer.h"
#include "ArcGISMapsSDK/API/GameEngine/Layers/Base/ArcGISLayerType.h"
#include "ArcGISMapsSDK/API/GameEngine/Location/ArcGISPosition.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISBasemap.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISMap.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISMapElevation.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISMapType.h"
#include "ArcGISMapsSDK/API/GameEngine/Security/ArcGISAuthenticationConfiguration.h"
#include "ArcGISMapsSDK/API/GameEngine/Security/ArcGISAuthenticationManager.h"
#include "ArcGISMapsSDK/API/GameEngine/Security/ArcGISOAuthAuthenticationConfiguration.h"
#include "ArcGISMapsSDK/API/GameEngine/View/ArcGISRendererView.h"
#include "ArcGISMapsSDK/API/GameEngine/View/Event/ArcGISElevationSourceViewStateChangedEvent.h"
#include "ArcGISMapsSDK/API/GameEngine/View/Event/ArcGISLayerViewStateChangedEvent.h"
#include "ArcGISMapsSDK/API/GameEngine/View/Event/ArcGISRendererViewStateChangedEvent.h"
#include "ArcGISMapsSDK/API/GameEngine/View/State/ArcGISElevationSourceViewState.h"
#include "ArcGISMapsSDK/API/GameEngine/View/State/ArcGISLayerViewState.h"
#include "ArcGISMapsSDK/API/Unreal/Collection.h"
#include "ArcGISMapsSDK/Actors/ArcGISMapView.h"
#include "ArcGISMapsSDK/Components/ArcGISCameraComponent.h"
#include "ArcGISMapsSDK/Components/ArcGISLocationComponent.h"
#include "ArcGISMapsSDK/Components/ArcGISRendererComponent.h"
#include "ArcGISMapsSDK/Security/AuthenticationChallengeManager.h"

// ArcGISSamples

#include "SampleDefaultPawn.h"

#if PLATFORM_ANDROID || PLATFORM_IOS
#include "Security/MobileOAuthAuthenticationChallengeHandler.h"
#else
#include "Security/DesktopOAuthAuthenticationChallengeHandler.h"
#endif

#define LOCTEXT_NAMESPACE "ArcGISMapController"

static const FString URLWorldElevation = TEXT("https://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer");
static const FString URLWorldImagery = TEXT("https://services.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer");
static const FString URLStreets = TEXT("https://services.arcgisonline.com/arcgis/rest/services/World_Street_Map/MapServer");
static const FString URLTopographic = TEXT("https://services.arcgisonline.com/arcgis/rest/services/World_Topo_Map/MapServer");
static const FString URLNationalGeographic = TEXT("https://services.arcgisonline.com/arcgis/rest/services/NatGeo_World_Map/MapServer");
static const FString URLOceans = TEXT("https://services.arcgisonline.com/arcgis/rest/services/Ocean/World_Ocean_Base/MapServer");
static const FString URLLightGrayCanvas = TEXT("https://services.arcgisonline.com/arcgis/rest/services/Canvas/World_Light_Gray_Base/MapServer");
static const FString URLDarkGreyCanvas = TEXT("https://services.arcgisonline.com/arcgis/rest/services/Canvas/World_Dark_Gray_Base/MapServer");

// Init

AArcGISMapController::AArcGISMapController()
{
	PrimaryActorTick.bCanEverTick = false;
	ArcGISRendererComponent = CreateDefaultSubobject<UArcGISRendererComponent>(TEXT("ArcGISRendererComponent"));
	AddOwnedComponent(ArcGISRendererComponent);
}

void AArcGISMapController::BeginPlay()
{
	Super::BeginPlay();
	CreateArcGISMap();
}

// View Mode

EViewMode AArcGISMapController::GetViewMode()
{
	auto ArcGISMapView = AArcGISMapView::GetArcGISMapView(this);
	check(ArcGISMapView);
	return ArcGISMapView->GetMapType() == EArcGISMapType::Global ? EViewMode::Global : EViewMode::Local;
}

void AArcGISMapController::SetViewMode(EViewMode Mode)
{
	auto ArcGISMapView = AArcGISMapView::GetArcGISMapView(this);
	if (ArcGISMapView)
	{
		ArcGISMapView->SetMapType(Mode == EViewMode::Global ? EArcGISMapType::Global : EArcGISMapType::Local);
	}
}

// Origin Location

double AArcGISMapController::GetOriginLatitude()
{
	auto ArcGISMapView = AArcGISMapView::GetArcGISMapView(this);
	check(ArcGISMapView);
	return ArcGISMapView->GetOriginLatitude();
}

double AArcGISMapController::GetOriginLongitude()
{
	auto ArcGISMapView = AArcGISMapView::GetArcGISMapView(this);
	check(ArcGISMapView);
	return ArcGISMapView->GetOriginLongitude();
}

double AArcGISMapController::GetOriginAltitude()
{
	auto ArcGISMapView = AArcGISMapView::GetArcGISMapView(this);
	check(ArcGISMapView);
	return ArcGISMapView->GetOriginAltitude();
}

void AArcGISMapController::SetOriginLatitude(double Lat)
{
	auto ArcGISMapView = AArcGISMapView::GetArcGISMapView(this);
	if (ArcGISMapView)
	{
		ArcGISMapView->SetOriginLatitude(static_cast<int32>(Lat));
	}
}

void AArcGISMapController::SetOriginLongitude(double Lon)
{
	auto ArcGISMapView = AArcGISMapView::GetArcGISMapView(this);
	if (ArcGISMapView)
	{
		ArcGISMapView->SetOriginLongitude(static_cast<int32>(Lon));
	}
}

void AArcGISMapController::SetOriginAltitude(double Alt)
{
	auto ArcGISMapView = AArcGISMapView::GetArcGISMapView(this);
	if (ArcGISMapView)
	{
		ArcGISMapView->SetOriginAltitude(static_cast<int32>(Alt));
	}
}

// Camera Locations

double AArcGISMapController::GetCamLatitude()
{
	return CamLatitude;
}

double AArcGISMapController::GetCamLongitude()
{
	return CamLongitude;
}

double AArcGISMapController::GetCamAltitude()
{
	return CamAltitude;
}

double AArcGISMapController::GetCamHeading()
{
	return CamHeading;
}

double AArcGISMapController::GetCamPitch()
{
	return CamPitch;
}

double AArcGISMapController::GetCamRoll()
{
	return CamRoll;
}

void AArcGISMapController::SetCamLatitude(double Lat)
{
	CamLatitude = Lat;
}

void AArcGISMapController::SetCamLongitude(double Lon)
{
	CamLongitude = Lon;
}

void AArcGISMapController::SetCamAltitude(double Alt)
{
	CamAltitude = Alt;
}

void AArcGISMapController::SetCamHeading(double Heading)
{
	CamHeading = Heading;
}

void AArcGISMapController::SetCamPitch(double Pitch)
{
	CamPitch = Pitch;
}

void AArcGISMapController::SetCamRoll(double Roll)
{
	CamRoll = Roll;
}

// Map Extent

double AArcGISMapController::GetExtentLatitude()
{
	return ExtentLatitude;
}

double AArcGISMapController::GetExtentLongitude()
{
	return ExtentLongitude;
}

double AArcGISMapController::GetExtentAltitude()
{
	return ExtentAltitude;
}

double AArcGISMapController::GetExtentWidth()
{
	return ExtentWidth;
}

double AArcGISMapController::GetExtentLength()
{
	return ExtentLength;
}

EExtentShapes AArcGISMapController::GetExtentShape()
{
	return ExtentShape;
}

void AArcGISMapController::SetExtentLatitude(double Lat)
{
	ExtentLatitude = Lat;
}

void AArcGISMapController::SetExtentLongitude(double Lon)
{
	ExtentLongitude = Lon;
}

void AArcGISMapController::SetExtentAltitude(double Alt)
{
	ExtentAltitude = Alt;
}

void AArcGISMapController::SetExtentWidth(double Width)
{
	ExtentWidth = Width;
}

void AArcGISMapController::SetExtentLength(double Length)
{
	ExtentLength = Length;
}

void AArcGISMapController::SetExtentShape(EExtentShapes Shape)
{
	ExtentShape = Shape;
}

// Elevation

bool AArcGISMapController::GetElevation()
{
	return TerrainElevation;
}

void AArcGISMapController::SetElevation(bool Toggle)
{
	TerrainElevation = Toggle;
}

// Base map

EBaseMaps AArcGISMapController::GetBaseMap()
{
	return BaseMap;
}

void AArcGISMapController::SetBaseMap(EBaseMaps Map)
{
	BaseMap = Map;
}

// Layers

TArray<FViewLayer>& AArcGISMapController::GetLayers()
{
	return ViewLayers;
}

// Private

void AArcGISMapController::CreateArcGISMap()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	auto arcGISOAuthAuthenticationChallengeHandler = ::MakeShared<Esri::ArcGISMapsSDK::Security::FMobileOAuthAuthenticationChallengeHandler>();
#else
	auto arcGISOAuthAuthenticationChallengeHandler = ::MakeShared<Esri::ArcGISMapsSDK::Security::FDesktopOAuthAuthenticationChallengeHandler>();
#endif

	Esri::ArcGISMapsSDK::Security::AuthenticationChallengeManager::SetOAuthChallengeHandler(arcGISOAuthAuthenticationChallengeHandler);

	Esri::GameEngine::Security::ArcGISAuthenticationManager::GetAuthenticationConfigurations().RemoveAll();

	for (const auto* oauthAuthenticationConfiguration : OAuthAuthenticationConfigurations)
	{
		auto trimmedClientID = oauthAuthenticationConfiguration->ClientID.TrimStartAndEnd();
		auto trimmedRedirectURI = oauthAuthenticationConfiguration->RedirectURI.TrimStartAndEnd();

		Esri::GameEngine::Security::ArcGISOAuthAuthenticationConfiguration authenticationConfiguration =
			Esri::GameEngine::Security::ArcGISOAuthAuthenticationConfiguration(TCHAR_TO_ANSI(*trimmedClientID), "",
																			   TCHAR_TO_ANSI(*trimmedRedirectURI));

		for (const auto& oauthAuthenticationConfigurationMapping : OAuthAuthenticationConfigurationMappings)
		{
			if (oauthAuthenticationConfigurationMapping.OAuthAuthenticationConfiguration == oauthAuthenticationConfiguration)
			{
				Esri::GameEngine::Security::ArcGISAuthenticationManager::GetAuthenticationConfigurations().Insert(
					TCHAR_TO_ANSI(*oauthAuthenticationConfigurationMapping.URI), authenticationConfiguration);
			}
		}
	}

	// Create the map document
	auto arcGISMap =
		::MakeShared<Esri::GameEngine::Map::ArcGISMap>(static_cast<Esri::GameEngine::Map::ArcGISMapType>(static_cast<uint8>(GetViewMode())));

	auto arcGISBasemap = Esri::GameEngine::Map::ArcGISBasemap(
		TCHAR_TO_UTF8(*GetBaseMapURL()), Esri::GameEngine::Layers::Base::ArcGISLayerType::ArcGISImageLayer, TCHAR_TO_UTF8(*APIKey));

	arcGISMap->SetBasemap(arcGISBasemap);

	if (TerrainElevation)
	{
		// Set the elevation
		auto elevationLayer =
			Esri::GameEngine::Elevation::ArcGISImageElevationSource(TCHAR_TO_UTF8(*URLWorldElevation), "Elevation", TCHAR_TO_UTF8(*APIKey));
		arcGISMap->SetElevation(elevationLayer);
	}

	GenerateArcGISLayers(*arcGISMap, TCHAR_TO_UTF8(*APIKey));

	if (IsExtentValid())
	{
		auto extent = GenerateExtent();
		arcGISMap->SetClippingArea(extent);
	}

	Esri::GameEngine::Location::ArcGISPosition position(CamLatitude, CamLongitude, CamAltitude,
														Esri::ArcGISRuntime::Geometry::SpatialReference::WGS84());
	Esri::GameEngine::Location::ArcGISRotation rotation(CamPitch, CamRoll, CamHeading);

	// Create a camera
	FString cameraName("Camera");
	auto arcGISCamera = ::MakeShared<Esri::GameEngine::Camera::ArcGISCamera>(cameraName, position, rotation);

	// Create the renderer view options config struct

	Esri::GameEngine::View::ArcGISRendererViewOptions rendererViewOptions{true};

	// Create the renderer view with camera, map and options

	auto rendererView = ::MakeShared<Esri::GameEngine::View::ArcGISRendererView>(arcGISMap, arcGISCamera, rendererViewOptions);

	// Adding callbacks to get state changes from view
	Esri::GameEngine::View::Event::ArcGISRendererViewStateChangedEvent arcGISRendererViewChanged =
		[](Esri::GameEngine::View::State::ArcGISRendererViewState& state) {
			UE_LOG(LogTemp, Warning, TEXT("ArcGISRendererView status changed : %d"), (int)state.GetStatus());
		};

	// Adding callbacks to get state changes from view of ArcGISVisualLayer
	Esri::GameEngine::View::Event::ArcGISLayerViewStateChangedEvent arcGISLayerViewChanged =
		[](Esri::GameEngine::Layers::Base::ArcGISLayer& layer, Esri::GameEngine::View::State::ArcGISLayerViewState& state) {
			UE_LOG(LogTemp, Warning, TEXT("ArcGISLayer status changed : %s %d"), *layer.GetName(), (int)state.GetStatus());
		};

	// Adding callbacks to get state changes from view of ArcGISElevationSource
	Esri::GameEngine::View::Event::ArcGISElevationSourceViewStateChangedEvent arcGISElevationSourceViewChanged =
		[](Esri::GameEngine::Elevation::Base::ArcGISElevationSource& elevation,
		   Esri::GameEngine::View::State::ArcGISElevationSourceViewState& state) {
			UE_LOG(LogTemp, Warning, TEXT("ArcGISElevationSource status changed : %s %d"), *elevation.GetName(), (int)state.GetStatus());
		};

	// Set event listeners
	rendererView->SetArcGISRendererViewStateChanged(std::move(arcGISRendererViewChanged));
	rendererView->SetArcGISLayerViewStateChanged(std::move(arcGISLayerViewChanged));
	rendererView->SetArcGISElevationSourceViewStateChanged(std::move(arcGISElevationSourceViewChanged));

	// Set the renderer view to the renderer component
	ArcGISRendererComponent->SetRendererView(rendererView);

	// Get the sample pawn
	ASampleDefaultPawn* arcGISPawn =
		static_cast<ASampleDefaultPawn*>(UGameplayStatics::GetActorOfClass(GetWorld(), ASampleDefaultPawn::StaticClass()));

	// Set the location of the pawn
	auto pawnLocation = arcGISPawn->ArcGISLocation;
	pawnLocation->GetPosition();
	pawnLocation->GetRotation();
	pawnLocation->SetPosition(
		Esri::GameEngine::Location::ArcGISPosition(CamLongitude, CamLatitude, CamAltitude, Esri::ArcGISRuntime::Geometry::SpatialReference::WGS84()));
	pawnLocation->SetRotation(Esri::GameEngine::Location::ArcGISRotation(CamPitch, CamRoll, CamHeading));

	auto arcGISDataCamera = arcGISPawn->ArcGISCamera;

	// Set the renderer view to the camera component
	arcGISDataCamera->SetRendererView(rendererView);
}

const FString& AArcGISMapController::GetBaseMapURL()
{
	switch (BaseMap)
	{
		case EBaseMaps::DarkGrayCanvas:
			return URLDarkGreyCanvas;

		case EBaseMaps::LightGrayCanvas:
			return URLLightGrayCanvas;

		case EBaseMaps::NationalGeographic:
			return URLNationalGeographic;

		case EBaseMaps::Oceans:
			return URLOceans;

		case EBaseMaps::Streets:
			return URLStreets;

		case EBaseMaps::Topographic:
			return URLTopographic;

		case EBaseMaps::WorldImagery:
			return URLWorldImagery;

		default:
			return URLWorldImagery;
	}
}

void AArcGISMapController::GenerateArcGISLayers(Esri::GameEngine::Map::ArcGISMap& Map, const char* ApiKey)
{
	for (const auto& ViewLayer : ViewLayers)
	{
		auto layer = CreateArcGISLayerFromViewLayer(ViewLayer, ApiKey);

		layer->SetLoadStatusChanged([layer](Esri::ArcGISRuntime::LoadStatus loadStatus) {
			if (loadStatus == Esri::ArcGISRuntime::LoadStatus::FailedToLoad)
			{
				auto loadError = layer->GetLoadError();

				UE_LOG(LogTemp, Error, TEXT("Failed to load ArcGISLayer: %s"), ANSI_TO_TCHAR(loadError.what()));
			}
		});

		Map.GetLayers().Add(*layer);
	}
}

TSharedPtr<Esri::GameEngine::Layers::Base::ArcGISLayer> AArcGISMapController::CreateArcGISLayerFromViewLayer(const FViewLayer& Layer,
																											 const char* ApiKey)
{
	TSharedPtr<Esri::GameEngine::Layers::Base::ArcGISLayer> layer;

	switch (Layer.LayerType)
	{
		case EArcGISLayerType::ArcGISImageLayer:
			layer = MakeShared<Esri::GameEngine::Layers::ArcGISImageLayer>(TCHAR_TO_UTF8(*Layer.LayerSource), TCHAR_TO_UTF8(*Layer.LayerName),
																		   Layer.LayerOpacity, Layer.bIsVisible, ApiKey);
			break;
		case EArcGISLayerType::ArcGISIntegratedMeshLayer:
			layer = MakeShared<Esri::GameEngine::Layers::ArcGISIntegratedMeshLayer>(
				TCHAR_TO_UTF8(*Layer.LayerSource), TCHAR_TO_UTF8(*Layer.LayerName), Layer.LayerOpacity, Layer.bIsVisible, ApiKey);
			break;
		case EArcGISLayerType::ArcGIS3DModelLayer:
			layer = MakeShared<Esri::GameEngine::Layers::ArcGIS3DModelLayer>(TCHAR_TO_UTF8(*Layer.LayerSource), TCHAR_TO_UTF8(*Layer.LayerName),
																			 Layer.LayerOpacity, Layer.bIsVisible, ApiKey);
			break;
	}

	return layer;
}

bool AArcGISMapController::IsExtentValid()
{
	if (GetViewMode() != EViewMode::Local)
	{
		return false;
	}

	if (ExtentLatitude == 0 && ExtentLongitude == 0 && ExtentAltitude == 0 && ExtentWidth == 0 && ExtentLength == 0)
	{
		return false;
	}

	return true;
}

Esri::GameEngine::Extent::ArcGISExtent AArcGISMapController::GenerateExtent()
{
	Esri::GameEngine::Location::ArcGISPosition center(ExtentLongitude, ExtentLatitude, ExtentAltitude,
													  Esri::ArcGISRuntime::Geometry::SpatialReference::WGS84());
	Esri::GameEngine::Extent::ArcGISExtent extent;

	switch (ExtentShape)
	{
		case EExtentShapes::Rectangle:
			extent = Esri::GameEngine::Extent::ArcGISExtentRectangle(center, ExtentWidth, ExtentLength);
			break;
		case EExtentShapes::Square:
			extent = Esri::GameEngine::Extent::ArcGISExtentRectangle(center, ExtentWidth, ExtentWidth);
			break;
		case EExtentShapes::Circle:
			extent = Esri::GameEngine::Extent::ArcGISExtentCircle(center, ExtentWidth);
			break;
	}

	return extent;
}

#undef LOCTEXT_NAMESPACE
