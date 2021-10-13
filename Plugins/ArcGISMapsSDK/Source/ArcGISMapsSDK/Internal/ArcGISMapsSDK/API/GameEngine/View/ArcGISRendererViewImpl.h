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

#include "ArcGISMapsSDK/API/GameEngine/View/ArcGISRendererView.h"

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
class GameEngineView;

namespace View
{
class ArcGISRendererView::Impl
{
public:
	Impl(TSharedPtr<Esri::GameEngine::Map::ArcGISMap> arcGISMap,
		 const TSharedPtr<const Esri::GameEngine::Camera::ArcGISCamera>& ArcGISCamera,
		 ArcGISRendererViewOptions options);

	static Impl* Get(ArcGISRendererView* arcGISRendererView)
	{
		return *reinterpret_cast<Impl**>(arcGISRendererView);
	}

	void SetViewportProperties(uint32 viewportWidthPixels,
							   uint32 viewportHeightPixels,
							   float horizontalFieldOfViewDegrees,
							   float verticalFieldOfViewDegrees,
							   float verticalDistortionFactor);

	void SetCamera(const TSharedPtr<const Esri::GameEngine::Camera::ArcGISCamera>& ArcGISCamera);
	void SetMap(TSharedPtr<Esri::GameEngine::Map::ArcGISMap> arcGISMap);
	void SetOptions(ArcGISRendererViewOptions options);
	void SetViewStateChanged(Esri::GameEngine::View::Event::ArcGISRendererViewStateChangedEvent callback);
	void SetLayerStateChanged(Esri::GameEngine::View::Event::ArcGISLayerViewStateChangedEvent callback);
	void SetElevationSourceViewStateChanged(Esri::GameEngine::View::Event::ArcGISElevationSourceViewStateChangedEvent callback);
	void UpdateMemoryAvailability(long total, long inUse);
	Esri::GameEngine::RenderCommandQueue::DecodedRenderCommandQueue GetCurrentDecodedRenderCommandQueue();

	TSharedPtr<const Esri::GameEngine::GameEngineView> GetGameEngineView() const;

	void HandleLowMemoryWarning();

private:
	TSharedPtr<Esri::GameEngine::Map::ArcGISMap> Map;
	TSharedPtr<Esri::GameEngine::GameEngineView> GameEngineView;
};
} // namespace View
} // namespace GameEngine
} // namespace Esri
