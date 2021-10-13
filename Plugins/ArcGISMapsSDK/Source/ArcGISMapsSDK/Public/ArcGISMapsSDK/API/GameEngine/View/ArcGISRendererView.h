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

#include "ArcGISMapsSDK/API/GameEngine/Camera/ArcGISCamera.h"
#include "ArcGISMapsSDK/API/GameEngine/Map/ArcGISMap.h"
#include "ArcGISMapsSDK/API/GameEngine/View/ArcGISRendererViewOptions.h"
#include "ArcGISMapsSDK/API/GameEngine/View/Event/ArcGISElevationSourceViewStateChangedEvent.h"
#include "ArcGISMapsSDK/API/GameEngine/View/Event/ArcGISLayerViewStateChangedEvent.h"
#include "ArcGISMapsSDK/API/GameEngine/View/Event/ArcGISRendererViewStateChangedEvent.h"

namespace Esri
{
namespace GameEngine
{
namespace RenderCommandQueue
{
class DecodedRenderCommandQueue;
} // namespace RenderCommandQueue
namespace View
{
class ARCGISMAPSSDK_API ArcGISRendererView final
{
public:
	class Impl;

public:
	ArcGISRendererView(TSharedPtr<Esri::GameEngine::Map::ArcGISMap> ArcGISMap,
					   const TSharedPtr<const Esri::GameEngine::Camera::ArcGISCamera>& ArcGISCamera,
					   ArcGISRendererViewOptions Options);
	~ArcGISRendererView();

	void SetViewportProperties(uint32 ViewportWidthPixels,
							   uint32 ViewportHeightPixels,
							   float HorizontalFieldOfViewDegrees,
							   float VerticalFieldOfViewDegrees,
							   float VerticalDistortionFactor);

	void SetCamera(const TSharedPtr<const Esri::GameEngine::Camera::ArcGISCamera>& ArcGISCamera);
	void SetMap(TSharedPtr<Esri::GameEngine::Map::ArcGISMap> ArcGISMap);
	void SetOptions(ArcGISRendererViewOptions Options);
	void SetArcGISRendererViewStateChanged(Esri::GameEngine::View::Event::ArcGISRendererViewStateChangedEvent Callback);
	void SetArcGISLayerViewStateChanged(Esri::GameEngine::View::Event::ArcGISLayerViewStateChangedEvent Callback);
	void SetArcGISElevationSourceViewStateChanged(Esri::GameEngine::View::Event::ArcGISElevationSourceViewStateChangedEvent Callback);
	void UpdateMemoryAvailability(long Total, long InUse);
	RenderCommandQueue::DecodedRenderCommandQueue GetCurrentDecodedRenderCommandQueue();

	void HandleLowMemoryWarning();

private:
	Impl* PImpl;
};
} // namespace View
} // namespace GameEngine
} // namespace Esri
