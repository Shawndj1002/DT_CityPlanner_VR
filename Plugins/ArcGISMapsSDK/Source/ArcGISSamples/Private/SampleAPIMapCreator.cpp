// Copyright 2021 Esri.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
//
#include "SampleAPIMapCreator.h"

#include <bitset>
#include <sstream>

#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"

#include "ArcGISMapsSDK/API/ArcGISRuntime/Geometry/SpatialReference.h"
#include "ArcGISMapsSDK/API/GameEngine/Attributes/Attribute.h"
#include "ArcGISMapsSDK/API/GameEngine/Attributes/AttributeProcessor.h"
#include "ArcGISMapsSDK/API/GameEngine/Attributes/VisualizationAttribute.h"
#include "ArcGISMapsSDK/API/GameEngine/Attributes/VisualizationAttributeDescription.h"
#include "ArcGISMapsSDK/API/GameEngine/Attributes/VisualizationAttributeType.h"
#include "ArcGISMapsSDK/API/GameEngine/Camera/ArcGISCamera.h"
#include "ArcGISMapsSDK/API/GameEngine/Elevation/ArcGISImageElevationSource.h"
#include "ArcGISMapsSDK/API/GameEngine/Extent/ArcGISExtent.h"
#include "ArcGISMapsSDK/API/GameEngine/Extent/ArcGISExtentCircle.h"
#include "ArcGISMapsSDK/API/GameEngine/Layers/ArcGIS3DModelLayer.h"
#include "ArcGISMapsSDK/API/GameEngine/Layers/ArcGISImageLayer.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISBasemap.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISMap.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISMapElevation.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISMapType.h"
#include "ArcGISMapsSDK/API/GameEngine/View/ArcGISRendererView.h"
#include "ArcGISMapsSDK/API/GameEngine/View/ArcGISRendererViewOptions.h"
#include "ArcGISMapsSDK/API/GameEngine/View/Event/ArcGISElevationSourceViewStateChangedEvent.h"
#include "ArcGISMapsSDK/API/GameEngine/View/Event/ArcGISLayerViewStateChangedEvent.h"
#include "ArcGISMapsSDK/API/GameEngine/View/Event/ArcGISRendererViewStateChangedEvent.h"
#include "ArcGISMapsSDK/API/GameEngine/View/State/ArcGISElevationSourceViewState.h"
#include "ArcGISMapsSDK/API/GameEngine/View/State/ArcGISLayerViewState.h"
#include "ArcGISMapsSDK/API/Unreal/ArrayBuilder.h"
#include "ArcGISMapsSDK/API/Unreal/Collection.h"
#include "ArcGISMapsSDK/API/Unreal/ImmutableArray.h"
#include "ArcGISMapsSDK/Actors/ArcGISMapView.h"
#include "ArcGISMapsSDK/Components/ArcGISCameraComponent.h"

#include "SampleDefaultPawn.h"

ASampleAPIMapCreator::ASampleAPIMapCreator()
{
	PrimaryActorTick.bCanEverTick = false;
	ArcGISRendererComponent = CreateDefaultSubobject<UArcGISRendererComponent>(TEXT("ArcGISRendererComponent"));
	AddOwnedComponent(ArcGISRendererComponent);
}

void ASampleAPIMapCreator::BeginPlay()
{
	Super::BeginPlay();
	CreateArcGISMap();
}

// We create our map with this method
void ASampleAPIMapCreator::CreateArcGISMap()
{
	// API Key
	constexpr auto apiKey = "";

	// Get planet shape
	auto ArcGISMapView = AArcGISMapView::GetArcGISMapView(this);
	check(ArcGISMapView);
	auto IsGlobal = ArcGISMapView->GetMapType() == EArcGISMapType::Global;

	// Create the map document
	auto mapType = IsGlobal ? Esri::GameEngine::Map::ArcGISMapType::Global : Esri::GameEngine::Map::ArcGISMapType::Local;
	auto arcGISMap = ::MakeShared<Esri::GameEngine::Map::ArcGISMap>(mapType);

	// Add a basemap
	auto arcGISBasemap =
		Esri::GameEngine::Map::ArcGISBasemap("https://www.arcgis.com/sharing/rest/content/items/716b600dbbac433faa4bec9220c76b3a/data", apiKey);

	// Set the basemap
	arcGISMap->SetBasemap(arcGISBasemap);

	// Set the elevation
	auto elevationLayer = Esri::GameEngine::Elevation::ArcGISImageElevationSource(
		"https://elevation3d.arcgis.com/arcgis/rest/services/WorldElevation3D/Terrain3D/ImageServer", "Elevation", apiKey);
	arcGISMap->SetElevation(elevationLayer);

	// Create layers
	auto layer_1 = Esri::GameEngine::Layers::ArcGISImageLayer(
		"https://tiles.arcgis.com/tiles/nGt4QxSblgDfeJn9/arcgis/rest/services/UrbanObservatory_NYC_TransitFrequency/MapServer",
		"NYTransitFrequencyTiles", 1.0f, true, apiKey);
	arcGISMap->GetLayers().Add(layer_1);

	auto layer_2 = Esri::GameEngine::Layers::ArcGISImageLayer(
		"https://tiles.arcgis.com/tiles/nGt4QxSblgDfeJn9/arcgis/rest/services/New_York_Industrial/MapServer", "NYIndustrialTiles", 1.0f, true,
		apiKey);
	arcGISMap->GetLayers().Add(layer_2);

	auto layer_3 = Esri::GameEngine::Layers::ArcGIS3DModelLayer(
		"https://tiles.arcgis.com/tiles/P3ePLMYs2RVChkJx/arcgis/rest/services/Buildings_NewYork_17/SceneServer", "NYScene", 1.0f, true, apiKey);
	arcGISMap->GetLayers().Add(layer_3);

	// To switch the Attribute method open the ArcGISMapCreator actor and switch the attribute type method
	if (AttributeType == AttributeVisualizationType::BuildingName)
	{
		Setup3DAttributesOtherType(layer_3);
	}
	else if (AttributeType == AttributeVisualizationType::ConstructionYear)
	{
		Setup3DAttributesFloatAndIntegerType(layer_3);
	}

	auto layer_4 = Esri::GameEngine::Layers::ArcGISImageLayer(
		"https://tiles.arcgis.com/tiles/4yjifSiIG17X0gW4/arcgis/rest/services/NewYorkCity_PopDensity/MapServer", "NYPopDensityTiles", 1.0f, true,
		apiKey);
	arcGISMap->GetLayers().Add(layer_4);

	// Remove a layer
	auto index = arcGISMap->GetLayers().IndexOf(layer_4);
	arcGISMap->GetLayers().Remove(index);

	// Update properties
	layer_1.SetOpacity(0.9f);
	layer_2.SetOpacity(0.6f);
	layer_4.SetOpacity(1.0f);

	// Create extent
	if (mapType == Esri::GameEngine::Map::ArcGISMapType::Local)
	{
		// Extent centered near the camera
		auto extentCenter =
			Esri::GameEngine::Location::ArcGISPosition(-74.054921, 40.691242, 3000, Esri::ArcGISRuntime::Geometry::SpatialReference::WGS84());
		auto extent = Esri::GameEngine::Extent::ArcGISExtentCircle(extentCenter, 100000);
		arcGISMap->SetClippingArea(extent);
	}

	// The position of the camera is a placeholder and will be internally overwritten
	Esri::GameEngine::Location::ArcGISPosition position(0, 0, 0, Esri::ArcGISRuntime::Geometry::SpatialReference::WGS84());
	Esri::GameEngine::Location::ArcGISRotation rotation(0, 0, 0);

	// Create a camera
	FString cameraName("Camera");
	auto arcGISCamera = ::MakeShared<Esri::GameEngine::Camera::ArcGISCamera>(cameraName, position, rotation);

	// Create the renderer view options config struct
	Esri::GameEngine::View::ArcGISRendererViewOptions rendererViewOptions{true};

	// Create the renderer view with camera, map and options
	auto rendererView = ::MakeShared<Esri::GameEngine::View::ArcGISRendererView>(arcGISMap, arcGISCamera, rendererViewOptions);

	// Attach logging handlers for view state change events
	AttachViewStateEventHandlers(*rendererView);

	// Set the renderer view to the renderer component
	ArcGISRendererComponent->SetRendererView(rendererView);

	if (UArcGISCameraComponent* const arcGISDataCamera = Cast<UArcGISCameraComponent>(ArcGISDataCameraReference.GetComponent(GetOwner())))
	{
		// Set the renderer view to the camera component
		arcGISDataCamera->SetRendererView(rendererView);
	}
}

template <typename T>
FString StatusFlags(T value)
{
	std::bitset<6> bits(static_cast<int>(value));
	std::stringstream ss;
	ss << bits;
	return ANSI_TO_TCHAR(ss.str().c_str());
}

void ASampleAPIMapCreator::AttachViewStateEventHandlers(Esri::GameEngine::View::ArcGISRendererView& rendererView)
{
	// Adding callbacks to get state changes from view
	Esri::GameEngine::View::Event::ArcGISRendererViewStateChangedEvent arcGISRendererViewChanged =
		[](Esri::GameEngine::View::State::ArcGISRendererViewState& state) {
			auto status = StatusFlags(state.GetStatus());
			UE_LOG(LogTemp, Display, TEXT("ArcGISRendererView status=%s"), *status);
		};

	// Adding callbacks to get state changes from view of ArcGISVisualLayer
	Esri::GameEngine::View::Event::ArcGISLayerViewStateChangedEvent arcGISLayerViewChanged =
		[](Esri::GameEngine::Layers::Base::ArcGISLayer& layer, Esri::GameEngine::View::State::ArcGISLayerViewState& state) {
			auto status = StatusFlags(state.GetStatus());

			auto additionalMessage = state.GetError().GetAdditionalMessage();
			auto message = state.GetError().GetMessage();

			if (message.IsEmpty())
			{
				UE_LOG(LogTemp, Display, TEXT("ArcGISLayer status=%s name='%s'."), *status, *layer.GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ArcGISLayer status=%s name='%s' message='%s' additional-message='%s'"), *status, *layer.GetName(),
					   *message, *additionalMessage);
			}
		};

	// Adding callbacks to get state changes from view of ArcGISElevationSource
	Esri::GameEngine::View::Event::ArcGISElevationSourceViewStateChangedEvent arcGISElevationSourceViewChanged =
		[](Esri::GameEngine::Elevation::Base::ArcGISElevationSource& elevation,
		   Esri::GameEngine::View::State::ArcGISElevationSourceViewState& state) {
			auto status = StatusFlags(state.GetStatus());

			auto additionalMessage = state.GetError().GetAdditionalMessage();
			auto message = state.GetError().GetMessage();

			if (message.IsEmpty())
			{
				UE_LOG(LogTemp, Display, TEXT("ArcGISElevationSource status=%s name='%s'"), *status, *elevation.GetName());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("ArcGISElevationSource status=%s name='%s' message='%s' additional-message='%s'"), *status,
					   *elevation.GetName(), *message, *additionalMessage);
			}
		};

	// Set event listeners
	rendererView.SetArcGISRendererViewStateChanged(std::move(arcGISRendererViewChanged));
	rendererView.SetArcGISLayerViewStateChanged(std::move(arcGISLayerViewChanged));
	rendererView.SetArcGISElevationSourceViewStateChanged(std::move(arcGISElevationSourceViewChanged));
}

// This function is an example of how to use attributes WITHOUT the attribute processor
void ASampleAPIMapCreator::Setup3DAttributesFloatAndIntegerType(Esri::GameEngine::Layers::ArcGIS3DModelLayer& LayerToVisualize)
{
	// Setup an array with the attributes we want forwarded to the material below
	// Because CNSTRCT_YR is of type esriFieldTypeInteger the values can be passed directly to the material as a float.
	// See Setup3DAttributesOtherType below for an example on how to pass other types to the material.
	// esriFieldTypeSingle, esriFieldTypeSmallInteger, esriFieldTypeInteger and esriFieldTypeDouble can be passed
	// directly to the material without processing. Although esriFieldTypeDouble & esriFieldTypeInteger are
	// converted to a float so it is a lossy conversion.
	auto layerAttributes = Esri::Unreal::ImmutableArray<FString>::CreateBuilder();
	layerAttributes.Add("CNSTRCT_YR");
	LayerToVisualize.SetAttributesToVisualize(layerAttributes.MoveToArray());

	// Set the material we will use to visualize this layer on the layer. In Unreal Engine open this material to see exactly how that is accomplished.
	// As a note you can use this function alone to change the material used to render all of the buildings.
	LayerToVisualize.SetMaterialReference(
		LoadObject<UMaterial>(this, TEXT("Material'/ArcGISMapsSDK/Samples/AttributeMaterials/ConstructionYearRenderer.ConstructionYearRenderer'")));
}

// This function is an example of how to use attributes WITH the attribute processor
void ASampleAPIMapCreator::Setup3DAttributesOtherType(Esri::GameEngine::Layers::ArcGIS3DModelLayer& LayerToVisualize)
{
	// Setup an array with the attributes we want forwarded to the material below
	// Because NAME is of type esriFieldTypeString/AttributeType.string we will need to configure the AttributeProcessor to pass
	// usable and meaningful values to the material.
	auto layerAttributes = Esri::Unreal::ImmutableArray<FString>::CreateBuilder();
	layerAttributes.Add("NAME");

	// The attribute description is the buffer that is output to the material. Think of "NAME" in the layer attributes as an input to the
	// attribute processor while "IsBuildingOfInterest" is how we are choosing to convert "NAME" into a type usable in the material with
	// values we can use to render the models in a way we see fit.
	// In this case we are using "IsBuildingOfInterest" to output either a 0 or a 1 depending on if the buildings "NAME" is a name of interest.
	auto attributeDescriptions = Esri::Unreal::ImmutableArray<Esri::GameEngine::Attributes::VisualizationAttributeDescription>::CreateBuilder();
	attributeDescriptions.Add(Esri::GameEngine::Attributes::VisualizationAttributeDescription(
		"IsBuildingOfInterest", Esri::GameEngine::Attributes::VisualizationAttributeType::Int32));

	// The attribute processor is what does the work on the CPU of converting the attribute into a value that can be used in the material which is executed on the GPU.
	// Integers and floats can be processed in the same way as other types although it is not normally necessary.
	AttributeProcessor = ::MakeShared<Esri::GameEngine::Attributes::AttributeProcessor>();

	AttributeProcessor->SetProcessEvent(
		[this](const Esri::Unreal::ImmutableArray<Esri::GameEngine::Attributes::Attribute>& inputAttributes,
			   const Esri::Unreal::ImmutableArray<Esri::GameEngine::Attributes::VisualizationAttribute>& outputVisualizationAttributes) {
			// Buffers will be provided in the same order they appear in the layer metadata. So if layerAttributes had an additional element it would be at inputAttributes.At(1)
			const auto nameAttribute = inputAttributes.At(0);
			// The outputVisualizationAttributes array expects that its data is indexed the same way as the attributeDescriptions above.
			const auto isBuildingOfInterestAttribute = outputVisualizationAttributes.At(0);

			const auto isBuildingOfInterestBuffer = isBuildingOfInterestAttribute.GetData();
			const auto isBuildingOfInterestData =
				TArrayView<int32>(reinterpret_cast<int32*>(isBuildingOfInterestBuffer.GetData()), isBuildingOfInterestBuffer.Num() / sizeof(int32));

			// Go over each attribute and if its name is one of the four buildings of interest set its "isBuildingOfInterest" value to 1, otherwise set it to 0.
			ForEachString(nameAttribute, [this, &isBuildingOfInterestData](const FAnsiStringView& element, int32 index) {
				isBuildingOfInterestData[index] = IsBuildingOfInterest(element);
			});
		});

	// Pass the layer attributes, attribute descriptions and the attribute processor to the layer
	LayerToVisualize.SetAttributesToVisualize(layerAttributes.MoveToArray(), attributeDescriptions.MoveToArray(), *AttributeProcessor);

	// Check out this material in Unreal Engine to learn what work is needed in the material to enable this work
	LayerToVisualize.SetMaterialReference(
		LoadObject<UMaterial>(this, TEXT("Material'/ArcGISMapsSDK/Samples/AttributeMaterials/BuildingNameRenderer.BuildingNameRenderer'")));
}

// ForEachString  takes care of converting the attribute buffer into a readable string value
void ASampleAPIMapCreator::ForEachString(const Esri::GameEngine::Attributes::Attribute& attribute,
										 std::function<void(const FAnsiStringView&, int32)> predicate)
{
	const auto buffer = attribute.GetData();
	const auto metadata = reinterpret_cast<const int*>(buffer.GetData());

	const auto count = metadata[0];
	assert(count);

	const char* string = reinterpret_cast<const char*>(buffer.GetData() + (2 + count) * sizeof(int));

	for (int32 i = 0; i < count; ++i)
	{
		auto element = FAnsiStringView();

		if (metadata[2 + i] > 0)
		{
			// If the length of the string element is 0, it means the element is null
			element = FAnsiStringView(string, metadata[2 + i] - 1);
		}

		predicate(element, i);

		string += metadata[2 + i];
	}
}

int32 ASampleAPIMapCreator::IsBuildingOfInterest(const FAnsiStringView& buildingName)
{
	if (buildingName.Equals("Empire State Building") || buildingName.Equals("Chrysler Building") || buildingName.Equals("Tower 1 World Trade Ctr") ||
		buildingName.Equals("One Chase Manhattan Plaza"))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
