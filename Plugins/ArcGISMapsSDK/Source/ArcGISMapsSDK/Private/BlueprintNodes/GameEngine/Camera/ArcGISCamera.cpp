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

#include "ArcGISMapsSDK/BlueprintNodes/GameEngine/Camera/ArcGISCamera.h" // IWYU pragma: associated
#include "ArcGISMapsSDK/API/GameEngine/Camera/ArcGISCamera.h"

UArcGISCamera* UArcGISCamera::CreateArcGISCamera(FString name, UArcGISPosition* position, UArcGISRotation* orientation)
{
	auto result = NewObject<UArcGISCamera>();

	result->APIObject = ::MakeShared<Esri::GameEngine::Camera::ArcGISCamera>(name, *position->APIObject, *orientation->APIObject);

	return result;
}
