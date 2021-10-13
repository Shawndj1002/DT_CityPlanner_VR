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
#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceRenderTarget.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
namespace GPUResources
{
FGPUResourceRenderTarget::FGPUResourceRenderTarget(uint32 Width, uint32 Height, ETextureRenderTargetFormat Format, bool bUseMipmaps)
{
	NativeRenderTarget = NewObject<UTextureRenderTarget2D>();

#if PLATFORM_ANDROID || PLATFORM_APPLE
	NativeRenderTarget->bAutoGenerateMips = false;
#else
	NativeRenderTarget->bAutoGenerateMips = bUseMipmaps;
#endif
	NativeRenderTarget->bNeedsTwoCopies = false;
	NativeRenderTarget->ClearColor = FLinearColor::White;

	NativeRenderTarget->RenderTargetFormat = Format;

	NativeRenderTarget->AddToRoot();
	NativeRenderTarget->InitAutoFormat(Width, Height);
	NativeRenderTarget->UpdateResourceImmediate(false);
}

UTextureRenderTarget2D* FGPUResourceRenderTarget::GetNativeRenderTarget() const
{
	return NativeRenderTarget;
}

FGPUResourceRenderTarget::~FGPUResourceRenderTarget()
{
	if (NativeRenderTarget && NativeRenderTarget->IsValidLowLevel())
	{
		NativeRenderTarget->RemoveFromRoot();
	}
}
} // namespace GPUResources
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
