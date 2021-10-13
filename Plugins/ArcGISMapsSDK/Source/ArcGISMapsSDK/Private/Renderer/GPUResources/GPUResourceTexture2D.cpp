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
#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceTexture2D.h"

#include "RenderUtils.h"

namespace
{
void UpdatePlatformData(UTexture2D* Texture, const uint8* Data)
{
	auto PlatformData = Texture->PlatformData;
	uint8* MipData = (uint8*)PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);

	auto Format = PlatformData->PixelFormat;

	int32 numBlocksX = PlatformData->SizeX / GPixelFormats[Format].BlockSizeX;
	int32 numBlocksY = PlatformData->SizeY / GPixelFormats[Format].BlockSizeY;

	FMemory::Memcpy(MipData, (void*)Data, numBlocksX * numBlocksY * GPixelFormats[Format].BlockBytes);

	PlatformData->Mips[0].BulkData.Unlock();
}

uint32 getPixelSize(EPixelFormat Format)
{
	switch (Format)
	{
		case PF_A32B32G32R32F:
			return 16;
		case PF_A16B16G16R16:
			return 8;
		case PF_R32_FLOAT:
		case PF_B8G8R8A8:
		default:
			return 4;
	}
}

#define ALIGN_BLOCK(x, y) (((x) + y - 1) / y) * y
uint32 CalculatePitch(FRHITexture2D* Texture)
{
	const auto format = Texture->GetFormat();
	const auto width = Texture->GetSizeX();

	switch (format)
	{
		// See https://docs.microsoft.com/en-us/windows/win32/direct3ddds/dx-graphics-dds-pguide for DXT formats
		case PF_DXT1:
			return std::max(1u, (width + 3) / 4) * 8;
		case PF_DXT3:
		case PF_DXT5:
			return std::max(1u, (width + 3) / 4) * 16;
		default:
			return ALIGN_BLOCK(width * getPixelSize(format), 4);
	}
}

void UpdateRHITexture(UTexture2D* Texture, const uint8* Data)
{
	ENQUEUE_RENDER_COMMAND(UpdateTextureRegionsData)
	([Texture, Data](FRHICommandListImmediate& RHICmdList) {
		auto texture2DRHI = static_cast<FTextureResource*>(Texture->Resource)->GetTexture2DRHI();

		auto height = FMath::DivideAndRoundUp(texture2DRHI->GetSizeY(), (uint32)GPixelFormats[texture2DRHI->GetFormat()].BlockSizeY);

		uint32 destStride;
		auto mipData = RHILockTexture2D(texture2DRHI, 0, RLM_WriteOnly, destStride, false);
		FMemory::Memcpy(mipData, Data, height * CalculatePitch(texture2DRHI));
		RHIUnlockTexture2D(texture2DRHI, 0, false);
	});
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
FGPUResourceTexture2D::FGPUResourceTexture2D(uint32 Width, uint32 Height, EPixelFormat Format, uint8 IsSRGB)
{
	auto Texture = UTexture2D::CreateTransient(Width, Height, Format);

	Texture->AddressX = TA_Clamp;
	Texture->AddressY = TA_Clamp;
	Texture->SRGB = IsSRGB;
	Texture->AddToRoot();
	Texture->UpdateResource();

	NativeTexture = Texture;
}

FGPUResourceTexture2D ::~FGPUResourceTexture2D()
{
	auto Texture = NativeTexture;

	if (Texture && Texture->IsValidLowLevel())
	{
		Texture->RemoveFromRoot();
	}
}

UTexture2D* FGPUResourceTexture2D::GetNativeTexture() const
{
	return NativeTexture;
}

void FGPUResourceTexture2D::SetPixelData(const uint8* Data)
{
#ifdef PLATFORM_APPLE
	UpdatePlatformData(NativeTexture, Data);
#endif
	UpdateRHITexture(NativeTexture, Data);
}
} // namespace GPUResources
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
