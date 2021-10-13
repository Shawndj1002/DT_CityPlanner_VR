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
#include "Materials/MaterialInstanceDynamic.h"

#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceRenderTarget.h"
#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceTexture2D.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
namespace GPUComputing
{
class FNormalMapGenerator
{
public:
	FNormalMapGenerator(UObject* InOuter);
	~FNormalMapGenerator();

	void Compute(const TSharedPtr<GPUResources::FGPUResourceTexture2D>& InputElevation,
				 FVector4 TileExtension,
				 FVector4 TextureExtension,
				 const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& Output);

private:
	UObject* Outer;
	UMaterialInstanceDynamic* MaterialInstance;
};
} // namespace GPUComputing
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
