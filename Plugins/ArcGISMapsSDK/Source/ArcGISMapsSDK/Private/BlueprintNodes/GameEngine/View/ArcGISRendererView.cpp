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

#include "ArcGISMapsSDK/API/GameEngine/View/ArcGISRendererView.h"
#include "ArcGISMapsSDK/API/GameEngine/View/ArcGISRendererViewOptions.h"
#include "ArcGISMapsSDK/BlueprintNodes/GameEngine/Camera/ArcGISCamera.h"
#include "ArcGISMapsSDK/BlueprintNodes/GameEngine/Map/ArcGISMap.h"
#include "ArcGISMapsSDK/BlueprintNodes/GameEngine/View/ArcGISRendererView.h" // IWYU pragma: associated

UArcGISRendererView* UArcGISRendererView::CreateArcGISRendererView(UArcGISMap* arcGISMap,
																   UArcGISCamera* arcGISCamera,
																   FArcGISRendererViewOptions arcGISRendererViewOptions)
{
	Esri::GameEngine::View::ArcGISRendererViewOptions apiArcGISRendererViewOptions{arcGISRendererViewOptions.LoadDataFromInvisibleLayers};

	auto result = NewObject<UArcGISRendererView>();

	result->ArcGISCamera = arcGISCamera;
	result->ArcGISMap = arcGISMap;
	result->ArcGISRendererViewOptions = arcGISRendererViewOptions;

	result->APIObject =
		::MakeShared<Esri::GameEngine::View::ArcGISRendererView>(arcGISMap->APIObject, arcGISCamera->APIObject, apiArcGISRendererViewOptions);

	return result;
}

UArcGISMap* UArcGISRendererView::GetArcGISMap()
{
	return ArcGISMap;
}
void UArcGISRendererView::SetArcGISMap(UArcGISMap* arcGISMap)
{
	APIObject->SetMap(arcGISMap->APIObject);
	ArcGISMap = arcGISMap;
}

UArcGISCamera* UArcGISRendererView::GetArcGISCamera()
{
	return ArcGISCamera;
}
void UArcGISRendererView::SetArcGISCamera(UArcGISCamera* arcGISCamera)
{
	APIObject->SetCamera(arcGISCamera->APIObject);
	ArcGISCamera = arcGISCamera;
}

FArcGISRendererViewOptions UArcGISRendererView::GetOptions()
{
	return ArcGISRendererViewOptions;
}
void UArcGISRendererView::SetOptions(FArcGISRendererViewOptions arcGISRendererViewOptions)
{
	auto viewOptions = Esri::GameEngine::View::ArcGISRendererViewOptions();
	viewOptions.LoadDataFromInvisibleLayers = arcGISRendererViewOptions.LoadDataFromInvisibleLayers;
	APIObject->SetOptions(viewOptions);
	ArcGISRendererViewOptions = arcGISRendererViewOptions;
}
