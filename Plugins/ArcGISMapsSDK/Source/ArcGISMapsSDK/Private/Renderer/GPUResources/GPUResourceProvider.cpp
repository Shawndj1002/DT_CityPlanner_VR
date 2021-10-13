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
#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceProvider.h"

#include "Engine/Texture2D.h"
#include "Engine/TextureRenderTarget2D.h"

#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/MaterialType.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/TextureFormat.h"

namespace
{
ETextureRenderTargetFormat ConvertToETextureRenderTargetFormat(Esri::GameEngine::RenderCommandQueue::Parameters::TextureFormat Format)
{
	using namespace Esri::GameEngine::RenderCommandQueue::Parameters;
	switch (Format)
	{
		case TextureFormat::RGBA8UNorm:
			return RTF_RGBA8;
		default:
			throw "Unkown RenderTargetFormat";
	}
}

EPixelFormat ConvertToEPixelFormat(Esri::GameEngine::RenderCommandQueue::Parameters::TextureFormat Format)
{
	using namespace Esri::GameEngine::RenderCommandQueue::Parameters;
	switch (Format)
	{
		case TextureFormat::RGBA16UNorm:
			return PF_A16B16G16R16;
		case TextureFormat::R32Norm:
#ifdef PLATFORM_APPLE
			return PF_R32_FLOAT;
#else
			return PF_R32_SINT;
#endif
		case TextureFormat::RGBA32Float:
			return PF_A32B32G32R32F;
		case TextureFormat::RGBA8UNorm:
			return PF_B8G8R8A8;
		case TextureFormat::R32Float:
			return PF_R32_FLOAT;
		case TextureFormat::DXT1:
			return PF_DXT1;
		case TextureFormat::DXT3:
			return PF_DXT3;
		case TextureFormat::DXT5:
			return PF_DXT5;
		case TextureFormat::ETC2RGB8:
		case TextureFormat::ETC2SRGB8:
		case TextureFormat::ETC2EacSRGB8:
			return PF_ETC2_RGB;
		case TextureFormat::ETC2RGB8PunchthroughAlpha1:
		case TextureFormat::ETC2SRGB8PunchthroughAlpha1:
		case TextureFormat::ETC2EacRGBA8:
			return PF_ETC2_RGBA;
		default:
			throw "Unkown PixelFormat";
	}
}
} // namespace

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
namespace GPUResources
{
FGPUResourceProvider::FGPUResourceProvider(UObject* InOuter) : Outer(InOuter)
{
}

TSharedPtr<FGPUResourceMaterial> FGPUResourceProvider::CreateMaterial(uint32 Id,
																	  GameEngine::RenderCommandQueue::Parameters::MaterialType MaterialType,
																	  void* CustomMaterial)
{
	TSharedPtr<FGPUResourceMaterial> NewMaterial;

	if (CustomMaterial)
	{
		NewMaterial = ::MakeShared<FGPUResourceMaterial>(static_cast<UMaterialInterface*>(reinterpret_cast<FWeakObjectPtr*>(CustomMaterial)->Get()));
	}
	else
	{
		auto Name = MaterialType == GameEngine::RenderCommandQueue::Parameters::MaterialType::SceneNode ?
						  "Material'/ArcGISMapsSDK/Materials/SceneMesh.SceneMesh'" :
						  "Material'/ArcGISMapsSDK/Materials/Ground.Ground'";

		NewMaterial = ::MakeShared<FGPUResourceMaterial>(LoadObject<UMaterial>(Outer, ANSI_TO_TCHAR(Name)));
	}

	Materials.emplace(Id, NewMaterial);

	return NewMaterial;
}

TSharedPtr<FGPUResourceRenderTarget> FGPUResourceProvider::CreateRenderTarget(uint32 Id,
																			  uint32 Width,
																			  uint32 Height,
																			  GameEngine::RenderCommandQueue::Parameters::TextureFormat Format,
																			  bool bUseMipmaps)
{
	auto NewRenderTarget = ::MakeShared<FGPUResourceRenderTarget>(Width, Height, ConvertToETextureRenderTargetFormat(Format), bUseMipmaps);

	RenderTargets.emplace(Id, NewRenderTarget);

	return NewRenderTarget;
}

TSharedPtr<FGPUResourceTexture2D> FGPUResourceProvider::CreateTexture(uint32 Id,
																	  uint32 Width,
																	  uint32 Height,
																	  GameEngine::RenderCommandQueue::Parameters::TextureFormat Format,
																	  uint8 IsSRGB)
{
	auto NewTexture = ::MakeShared<FGPUResourceTexture2D>(Width, Height, ConvertToEPixelFormat(Format), IsSRGB);

	Textures.emplace(Id, NewTexture);

	return NewTexture;
}

TSharedPtr<FGPUResourceMaterial> FGPUResourceProvider::GetMaterial(uint32 Id)
{
	return Materials[Id];
}

TSharedPtr<FGPUResourceRenderTarget> FGPUResourceProvider::GetRenderTarget(uint32 Id)
{
	return RenderTargets[Id];
}

TSharedPtr<FGPUResourceTexture2D> FGPUResourceProvider::GetTexture(uint32 Id)
{
	return Textures[Id];
}

void FGPUResourceProvider::DestroyMaterial(uint32 Id)
{
	Materials.erase(Id);
}

void FGPUResourceProvider::DestroyRenderTarget(uint32 Id)
{
	RenderTargets.erase(Id);
}

void FGPUResourceProvider::DestroyTexture(uint32 Id)
{
	Textures.erase(Id);
}
} // namespace GPUResources
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
