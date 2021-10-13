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
#include "UObject/Object.h"

#include "ArcGISMapsSDK/BlueprintNodes/GameEngine/View/ArcGISRendererViewOptions.h"

#include "ArcGISRendererView.generated.h"

namespace Esri
{
namespace GameEngine
{
namespace View
{
class ArcGISRendererView;
} // namespace View
} // namespace GameEngine
} // namespace Esri

UCLASS(BlueprintType, Blueprintable)
class ARCGISMAPSSDK_API UArcGISRendererView : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ArcGISMapsSDK|ArcGISRendererView")
	static UArcGISRendererView* CreateArcGISRendererView(UArcGISMap* arcGISMap, UArcGISCamera* arcGISCamera, FArcGISRendererViewOptions options);

	UFUNCTION(BlueprintCallable, Category = "ArcGISMapsSDK|ArcGISRendererView")
	UArcGISMap* GetArcGISMap();
	UFUNCTION(BlueprintCallable, Category = "ArcGISMapsSDK|ArcGISRendererView")
	void SetArcGISMap(UArcGISMap* arcGISMap);

	UFUNCTION(BlueprintCallable, Category = "ArcGISMapsSDK|ArcGISRendererView")
	UArcGISCamera* GetArcGISCamera();
	UFUNCTION(BlueprintCallable, Category = "ArcGISMapsSDK|ArcGISRendererView")
	void SetArcGISCamera(UArcGISCamera* arcGISCamera);

	UFUNCTION(BlueprintCallable, Category = "ArcGISMapsSDK|ArcGISRendererView")
	FArcGISRendererViewOptions GetOptions();
	UFUNCTION(BlueprintCallable, Category = "ArcGISMapsSDK|ArcGISRendererView")
	void SetOptions(FArcGISRendererViewOptions arcGISRendererViewOptions);

	TSharedPtr<Esri::GameEngine::View::ArcGISRendererView> APIObject;

private:
	UArcGISMap* ArcGISMap;
	UArcGISCamera* ArcGISCamera;
	FArcGISRendererViewOptions ArcGISRendererViewOptions;
};
