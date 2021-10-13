// COPYRIGHT 1995-2021 ESRI
// TRADE SECRETS: ESRI PROPRIETARY AND CONFIDENTIAL
// Unpublished material - all rights reserved under the
// Copyright Laws of the United States and applicable international
// laws, treaties, and conventions.
//
// For additional information, contact:
// Environmental Systems Research Institute, Inc.
// Attn: Contracts and Legal Services Department
// 380 New York Street
// Redlands, California, 92373
// USA
//
// email: contracts@esri.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ArcGISMapsSDK/BlueprintNodes/GameEngine/View/ArcGISRendererView.h"
#include "ArcGISMapsSDK/Components/ArcGISRendererComponent.h"

#include "ArcGISRenderer.generated.h"

UCLASS()
class ARCGISMAPSSDK_API AArcGISRenderer : public AActor
{
	GENERATED_BODY()

public:
	AArcGISRenderer();

	UFUNCTION(BlueprintCallable, Category = "ArcGISMapsSDK|ArcGISRenderer")
	void SetRendererView(UArcGISRendererView* InRendererView);

protected:
	UArcGISRendererComponent* ArcGISRendererComponent;
};
