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

#include "CoreMinimal.h"

#include "ArcGISMapsSDK/API/GameEngine/GameEngineView.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/RenderCommand.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/RenderCommandServer.h"
#include "ArcGISMapsSDK/Renderer/GPUComputing/ImageComposer.h"
#include "ArcGISMapsSDK/Renderer/GPUComputing/NormalMapGenerator.h"
#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceProvider.h"
#include "ArcGISMapsSDK/Renderer/SceneComponents/SceneComponentProvider.h"

class AArcGISMapView;

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
class FRenderCommandClient
{
public:
	FRenderCommandClient(UObject* InOuter,
						 TSharedPtr<GPUResources::FGPUResourceProvider> InGPUResourceProvider,
						 TSharedPtr<SceneComponents::FSceneComponentProvider> InSceneComponentProvider,
						 TSharedPtr<GPUComputing::FImageComposer> InImageComposer,
						 TSharedPtr<GPUComputing::FNormalMapGenerator> InNormalMapGenerator);
	void ExecuteRenderCommand(const GameEngine::RenderCommandQueue::RenderCommand& renderCommand);

private:
	AArcGISMapView* ArcGISMapView;
	GameEngine::RenderCommandQueue::RenderCommandServer RenderCommandServer;
	TSharedPtr<GPUResources::FGPUResourceProvider> GPUResourceProvider;
	TSharedPtr<SceneComponents::FSceneComponentProvider> SceneComponentProvider;
	TSharedPtr<GPUComputing::FImageComposer> ImageComposer;
	TSharedPtr<GPUComputing::FNormalMapGenerator> NormalMapGenerator;
};
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
