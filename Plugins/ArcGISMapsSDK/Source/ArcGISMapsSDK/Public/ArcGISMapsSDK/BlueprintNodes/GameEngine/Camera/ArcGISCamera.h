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

#include "ArcGISMapsSDK/BlueprintNodes/GameEngine/Location/ArcGISPosition.h"
#include "ArcGISMapsSDK/BlueprintNodes/GameEngine/Location/ArcGISRotation.h"

#include "ArcGISCamera.generated.h"

namespace Esri
{
namespace GameEngine
{
namespace Camera
{
class ArcGISCamera;
} // namespace Camera
} // namespace GameEngine
} // namespace Esri

UCLASS(BlueprintType, Blueprintable)
class ARCGISMAPSSDK_API UArcGISCamera : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ArcGISMapsSDK|ArcGISCamera")
	static UArcGISCamera* CreateArcGISCamera(FString name, UArcGISPosition* position, UArcGISRotation* orientation);

	TSharedPtr<Esri::GameEngine::Camera::ArcGISCamera> APIObject;
};
