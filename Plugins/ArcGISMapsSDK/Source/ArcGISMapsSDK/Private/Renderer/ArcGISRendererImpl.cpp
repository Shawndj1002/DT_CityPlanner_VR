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
#include "ArcGISMapsSDK/Renderer/ArcGISRendererImpl.h"
#include "ArcGISMapsSDK/Renderer/RenderCommandThrottle.h"

#include "ArcGISMapsSDK/API/GameEngine/GameEngineView.h"
#include "ArcGISMapsSDK/API/GameEngine/View/ArcGISRendererViewImpl.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
FArcGISRenderer::Impl::Impl(TSharedPtr<Esri::GameEngine::View::ArcGISRendererView> InRendererView, UObject* Outer)
{
	ImageComposer = ::MakeShared<GPUComputing::FImageComposer>(Outer);
	NormalMapGenerator = ::MakeShared<GPUComputing::FNormalMapGenerator>(Outer);
	GPUResourceProvider = ::MakeShared<GPUResources::FGPUResourceProvider>(Outer);
	SceneComponentProvider = ::MakeShared<SceneComponents::FSceneComponentProvider>(Outer);

	RenderCommandClient = ::MakeShared<FRenderCommandClient>(Outer, GPUResourceProvider, SceneComponentProvider, ImageComposer, NormalMapGenerator);

	RendererView = std::move(InRendererView);
}

void FArcGISRenderer::Impl::Update()
{
	if /*constexpr*/ (!FRenderCommandThrottle::bEnableThrottle)
	{
		// Process one full command queue at a time
		CurrentCommandQueue = RendererView->GetCurrentDecodedRenderCommandQueue();
		auto RenderCommand = CurrentCommandQueue.GetNextCommand();
		while (RenderCommand != nullptr)
		{
			RenderCommandClient->ExecuteRenderCommand(*RenderCommand);
			RenderCommand = CurrentCommandQueue.GetNextCommand();
		}
	}
	else
	{
		FRenderCommandThrottle RenderCommandThrottle;

		// Pick up next command from previous queue, if any
		auto RenderCommand = CurrentCommandQueue.GetNextCommand();
		if (RenderCommand == nullptr)
		{
			CurrentCommandQueue = RendererView->GetCurrentDecodedRenderCommandQueue();
		}
		while (RenderCommand != nullptr)
		{
			RenderCommandClient->ExecuteRenderCommand(*RenderCommand);
			if (RenderCommandThrottle.DoThrottle(*RenderCommand))
			{
				// Break and defer processing of the remaining commands to next Update
				break;
			}
			RenderCommand = CurrentCommandQueue.GetNextCommand();
		}
	}
}
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
