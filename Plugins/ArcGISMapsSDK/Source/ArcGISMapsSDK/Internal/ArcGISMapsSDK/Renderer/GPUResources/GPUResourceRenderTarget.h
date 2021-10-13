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
#include "Engine/TextureRenderTarget2D.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
namespace GPUResources
{
class FGPUResourceRenderTarget final
{
public:
	FGPUResourceRenderTarget(uint32 Width, uint32 Height, ETextureRenderTargetFormat Format, bool bUseMipMaps);
	~FGPUResourceRenderTarget();

	UTextureRenderTarget2D* GetNativeRenderTarget() const;

private:
	UTextureRenderTarget2D* NativeRenderTarget = nullptr;
};
} // namespace GPUResources
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
