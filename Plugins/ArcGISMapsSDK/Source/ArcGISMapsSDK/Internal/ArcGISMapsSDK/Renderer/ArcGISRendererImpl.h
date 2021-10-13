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

#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/DecodedRenderCommandQueue.h"
#include "ArcGISMapsSDK/API/GameEngine/View/ArcGISRendererView.h"
#include "ArcGISMapsSDK/Renderer/ArcGISRenderer.h"
#include "ArcGISMapsSDK/Renderer/RenderCommandClient.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
class FArcGISRenderer::Impl
{
public:
	Impl(TSharedPtr<GameEngine::View::ArcGISRendererView> InRendererView, UObject* Outer);

	void Update();

private:
	TSharedPtr<FRenderCommandClient> RenderCommandClient;
	TSharedPtr<GameEngine::View::ArcGISRendererView> RendererView;
	TSharedPtr<GPUResources::FGPUResourceProvider> GPUResourceProvider;
	TSharedPtr<SceneComponents::FSceneComponentProvider> SceneComponentProvider;
	TSharedPtr<GPUComputing::FImageComposer> ImageComposer;
	TSharedPtr<GPUComputing::FNormalMapGenerator> NormalMapGenerator;
	Esri::GameEngine::RenderCommandQueue::DecodedRenderCommandQueue CurrentCommandQueue;
};
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
