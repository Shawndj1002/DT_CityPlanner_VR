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
struct ComposableImage
{
	FVector4 Extent;
	TSharedPtr<const GPUResources::FGPUResourceTexture2D> Image;
	float Opacity;
};

class FImageComposer
{
public:
	FImageComposer(UObject* InOuter);
	~FImageComposer();

	void Compose(const std::vector<ComposableImage>& InputImages, const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& Output);

	// Used by tile blending
	void Compose(const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& SourceA,
				 const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& SourceB,
				 float Alpha,
				 const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& Output,
				 const FVector4& Extent);

	// Used by tile blending
	void Copy(const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& Source,
			  const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& Output,
			  const FVector4& Extent);

private:
	void Draw(const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& Output);

	void SetMap(int Channel, UTexture* Texture, float Opacity, float bUseTextureAlpha, const FVector4& Extent);

	TSharedPtr<GPUResources::FGPUResourceRenderTarget> BlendTempRenderTarget;
	UMaterialInstanceDynamic* MaterialInstance;
	UObject* Outer;
};
} // namespace GPUComputing
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
