// COPYRIGHT 1995-2021 ESRI
// TRADE SECRETS: ESRI PROPRIETARY AND CONFIDENTIAL
// Unpublished material - all rights reserved under the
// Copyright Laws of the United States and applicable international
// laws, treaties, and conventions.
//
// For additional information, contact:
// Attn: Contracts and Legal Department
// Environmental Systems Research Institute, Inc.
// 380 New York Street
// Redlands, California 92373
// USA
//
// email: legal@esri.com
#include "ArcGISMapsSDK/Components/ArcGISMapComponent.h"

#include "GeoReferencingSystem.h"

#include "ArcGISMapsSDK/API/ArcGISRuntime/Geometry/SpatialReference.h"
#include "ArcGISMapsSDK/API/GameEngine/Camera/ArcGISCamera.h"
#include "ArcGISMapsSDK/API/GameEngine/Elevation/ArcGISImageElevationSource.h"
#include "ArcGISMapsSDK/API/GameEngine/Layers/ArcGIS3DModelLayer.h"
#include "ArcGISMapsSDK/API/GameEngine/Layers/ArcGISImageLayer.h"
#include "ArcGISMapsSDK/API/GameEngine/Layers/ArcGISIntegratedMeshLayer.h"
#include "ArcGISMapsSDK/API/GameEngine/Location/ArcGISPosition.h"
#include "ArcGISMapsSDK/API/GameEngine/Location/ArcGISRotation.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISBasemap.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISMap.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISMapElevation.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISMapType.h"
#include "ArcGISMapsSDK/API/GameEngine/View/ArcGISRendererView.h"
#include "ArcGISMapsSDK/API/GameEngine/View/ArcGISRendererViewOptions.h"
#include "ArcGISMapsSDK/API/Unreal/Collection.h"
#include "ArcGISMapsSDK/Components/ArcGISCameraComponent.h"

UArcGISMapComponent::UArcGISMapComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
}

void UArcGISMapComponent::BeginPlay()
{
	Super::BeginPlay();

	auto GeoReferencingSystem = AGeoReferencingSystem::GetGeoReferencingSystem(this);
	check(GeoReferencingSystem);

	auto mapType = GeoReferencingSystem->PlanetShape == EPlanetShape::RoundPlanet ? Esri::GameEngine::Map::ArcGISMapType::Global :
																					Esri::GameEngine::Map::ArcGISMapType::Local;

	auto arcGISMap = ::MakeShared<Esri::GameEngine::Map::ArcGISMap>(mapType);

	if (Basemap != "")
	{
		auto arcGISBasemap = Esri::GameEngine::Map::ArcGISBasemap(Basemap, APIKey);

		// Set the Basemap
		arcGISMap->SetBasemap(arcGISBasemap);
	}

	if (Elevation != "")
	{
		auto elevationLayer = Esri::GameEngine::Elevation::ArcGISImageElevationSource(Elevation, "Elevation", APIKey);

		// Set the elevation
		arcGISMap->SetElevation(elevationLayer);
	}

	for (const auto& layerDefinition : Layers)
	{
		Esri::GameEngine::Layers::Base::ArcGISLayer* layer = nullptr;

		if (layerDefinition.Url != "")
		{
			if (layerDefinition.Type == EArcGISLayerType::ArcGIS3DModelLayer)
			{
				layer = new Esri::GameEngine::Layers::ArcGIS3DModelLayer(layerDefinition.Url, APIKey);
			}
			else if (layerDefinition.Type == EArcGISLayerType::ArcGISImageLayer)
			{
				layer = new Esri::GameEngine::Layers::ArcGISImageLayer(layerDefinition.Url, APIKey);
			}
			else if (layerDefinition.Type == EArcGISLayerType::ArcGISIntegratedMeshLayer)
			{
				layer = new Esri::GameEngine::Layers::ArcGISIntegratedMeshLayer(layerDefinition.Url, APIKey);
			}
		}

		if (layer != nullptr)
		{
			layer->SetOpacity(layerDefinition.Opacity);

			arcGISMap->GetLayers().Add(*layer);
		}
	}

	// Temporary position and rotation
	auto arcGISPosition = Esri::GameEngine::Location::ArcGISPosition(0, 0, 0, Esri::ArcGISRuntime::Geometry::SpatialReference::WGS84());
	auto arcGISRotation = Esri::GameEngine::Location::ArcGISRotation(0, 0, 0);

	// This position will be overwritten by the camera component
	FString cameraName("Camera");
	auto arcGISCamera = ::MakeShared<Esri::GameEngine::Camera::ArcGISCamera>(cameraName, arcGISPosition, arcGISRotation);

	auto arcGISRendererViewOptions = Esri::GameEngine::View::ArcGISRendererViewOptions();
	auto rendererView = ::MakeShared<Esri::GameEngine::View::ArcGISRendererView>(arcGISMap, arcGISCamera, arcGISRendererViewOptions);

	ArcGISRendererComponent->SetRendererView(rendererView);

	if (UArcGISCameraComponent* const arcGISDataCamera = Cast<UArcGISCameraComponent>(ArcGISDataCameraReference.GetComponent(GetOwner())))
	{
		// Set the renderer view to the camera component
		arcGISDataCamera->SetRendererView(rendererView);
	}
}

void UArcGISMapComponent::InitializeComponent()
{
	Super::InitializeComponent();

	ArcGISRendererComponent = GetOwner()->FindComponentByClass<UArcGISRendererComponent>();

	if (!ArcGISRendererComponent)
	{
		ArcGISRendererComponent = CreateDefaultSubobject<UArcGISRendererComponent>(TEXT("ArcGISRenderer"));
#if WITH_EDITOR
		ArcGISRendererComponent->bVisualizeComponent = true;
#endif
	}
}
