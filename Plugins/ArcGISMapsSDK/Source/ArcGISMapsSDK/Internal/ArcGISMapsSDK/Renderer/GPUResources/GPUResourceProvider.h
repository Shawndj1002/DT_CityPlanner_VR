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

#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceMaterial.h"
#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceRenderTarget.h"
#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceTexture2D.h"

namespace Esri
{
namespace GameEngine
{
namespace RenderCommandQueue
{
namespace Parameters
{
enum class MaterialType;
enum class TextureFormat;
} // namespace Parameters
} // namespace RenderCommandQueue
} // namespace GameEngine

namespace ArcGISMapsSDK
{
namespace Renderer
{
namespace GPUResources
{
class FGPUResourceProvider
{
public:
	FGPUResourceProvider(UObject* InOuter);

	TSharedPtr<FGPUResourceMaterial> CreateMaterial(uint32 MaterialId,
													GameEngine::RenderCommandQueue::Parameters::MaterialType MaterialType,
													void* CustomMaterial);

	TSharedPtr<FGPUResourceRenderTarget> CreateRenderTarget(uint32 Id,
															uint32 Width,
															uint32 Height,
															GameEngine::RenderCommandQueue::Parameters::TextureFormat Format,
															bool bUseMipMaps);

	TSharedPtr<FGPUResourceTexture2D> CreateTexture(uint32 Id,
													uint32 Width,
													uint32 Height,
													GameEngine::RenderCommandQueue::Parameters::TextureFormat Format,
													uint8 IsSRGB);

	TSharedPtr<FGPUResourceMaterial> GetMaterial(uint32 Id);
	TSharedPtr<FGPUResourceRenderTarget> GetRenderTarget(uint32 Id);
	TSharedPtr<FGPUResourceTexture2D> GetTexture(uint32 Id);

	void DestroyMaterial(uint32 Id);
	void DestroyRenderTarget(uint32 Id);
	void DestroyTexture(uint32 Id);

private:
	UObject* Outer;

	std::unordered_map<uint32, TSharedPtr<FGPUResourceMaterial>> Materials;
	std::unordered_map<uint32, TSharedPtr<FGPUResourceTexture2D>> Textures;
	std::unordered_map<uint32, TSharedPtr<FGPUResourceRenderTarget>> RenderTargets;
};
} // namespace GPUResources
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
