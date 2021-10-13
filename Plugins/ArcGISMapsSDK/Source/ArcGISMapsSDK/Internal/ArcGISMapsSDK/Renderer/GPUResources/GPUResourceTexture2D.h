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
#include "Engine/Texture2D.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
namespace GPUResources
{
class FGPUResourceTexture2D final
{
public:
	FGPUResourceTexture2D(uint32 Width, uint32 Height, EPixelFormat Format, uint8 IsSRGB);
	~FGPUResourceTexture2D();

	void SetPixelData(const uint8* Data);

	UTexture2D* GetNativeTexture() const;

private:
	UTexture2D* NativeTexture = nullptr;
};
} // namespace GPUResources
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
