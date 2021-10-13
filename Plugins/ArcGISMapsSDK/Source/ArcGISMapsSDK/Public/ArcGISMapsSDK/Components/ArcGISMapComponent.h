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
#pragma once

#include "Components/SceneComponent.h"
#include "CoreMinimal.h"

#include "ArcGISMapsSDK/BlueprintNodes/GameEngine/Layers/Base/ArcGISLayerType.h"
#include "ArcGISMapsSDK/Components/ArcGISRendererComponent.h"

#include "ArcGISMapComponent.generated.h"

USTRUCT()
struct FArcGISMapLayer
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	EArcGISLayerType Type;

	UPROPERTY(EditAnywhere)
	FString Url;

	UPROPERTY(EditAnywhere)
	double Opacity;
};

UCLASS(ClassGroup = (ArcGISMapsSDK), meta = (BlueprintSpawnableComponent), Category = "ArcGISMapsSDK|ArcGISMapComponent")
class ARCGISMAPSSDK_API UArcGISMapComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UArcGISMapComponent();

	UPROPERTY(EditAnywhere, Category = "ArcGISMapsSDK")
	FString APIKey;

	UPROPERTY(EditAnywhere, Category = "ArcGISMapsSDK")
	FString Basemap;

	UPROPERTY(EditAnywhere, Category = "ArcGISMapsSDK")
	FString Elevation;

	UPROPERTY(EditAnywhere, Category = "ArcGISMapsSDK")
	TArray<FArcGISMapLayer> Layers;

	UPROPERTY(EditInstanceOnly, Category = "ArcGISMapsSDK", meta = (DisplayName = "ArcGISDataCamera", UseComponentPicker, AllowAnyActor, AllowedClasses = "ArcGISCameraComponent"))
	FComponentReference ArcGISDataCameraReference;

protected:
	void BeginPlay() override;
	void InitializeComponent() override;

	UArcGISRendererComponent* ArcGISRendererComponent;
};
