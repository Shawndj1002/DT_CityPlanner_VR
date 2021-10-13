// Copyright 2021 Esri.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
//
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ArcGISMapsSDK/API/GameEngine/Layers/ArcGIS3DModelLayer.h"
#include "ArcGISMapsSDK/Components/ArcGISRendererComponent.h"

#include "SampleAPIMapCreator.generated.h"

namespace Esri
{
namespace GameEngine
{
namespace Attributes
{
class Attribute;
class AttributeProcessor;
} // namespace Attributes

namespace View
{
class ArcGISRendererView;
}
} // namespace GameEngine
} // namespace Esri

UENUM()
enum class AttributeVisualizationType : uint8
{
	None = 0,
	ConstructionYear = 1,
	BuildingName = 2
};

// The declaration of our Actor class
UCLASS()
class ARCGISSAMPLES_API ASampleAPIMapCreator : public AActor
{
	GENERATED_BODY()

public:
	ASampleAPIMapCreator();

	UPROPERTY(EditInstanceOnly,
			  meta = (DisplayName = "ArcGISDataCamera", UseComponentPicker, AllowAnyActor, AllowedClasses = "ArcGISCameraComponent"))
	FComponentReference ArcGISDataCameraReference;

	UPROPERTY(EditAnywhere)
	AttributeVisualizationType AttributeType;

protected:
	virtual void BeginPlay() override;

private:
	UArcGISRendererComponent* ArcGISRendererComponent;
	TSharedPtr<Esri::GameEngine::Attributes::AttributeProcessor> AttributeProcessor;

	void CreateArcGISMap();

	void AttachViewStateEventHandlers(Esri::GameEngine::View::ArcGISRendererView& rendererView);

	void Setup3DAttributesFloatAndIntegerType(Esri::GameEngine::Layers::ArcGIS3DModelLayer& LayerToVisualize);
	void Setup3DAttributesOtherType(Esri::GameEngine::Layers::ArcGIS3DModelLayer& LayerToVisualize);

	int32 IsBuildingOfInterest(const FAnsiStringView& buildingName);
	void ForEachString(const Esri::GameEngine::Attributes::Attribute& attribute, std::function<void(const FAnsiStringView&, int32)> predicate);
};
