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
#include "ArcGISMapsSDK/Renderer/GPUComputing/ImageComposer.h"

#include "Kismet/KismetRenderingLibrary.h"

#ifdef PLATFORM_APPLE
#include "CanvasItem.h"
#include "Engine/Canvas.h"

namespace
{
void Flush(UTextureRenderTarget2D* Target)
{
	FCanvasLineItem canvasLineItem;
	FCanvas Canvas(Target->GameThread_GetRenderTargetResource(), NULL, 0.0f, 0.0f, 0.0f, GMaxRHIFeatureLevel);
	Canvas.DrawItem(canvasLineItem, FVector2D(0, 0));
	Canvas.Flush_GameThread(true);
}
} // namespace
#endif

namespace
{
FName NumName(const FString& Name, int Number)
{
	return FName(*(Name + FString::FromInt(Number + 1)));
}
} // namespace

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
namespace GPUComputing
{
FImageComposer::FImageComposer(UObject* InOuter) : Outer(InOuter)
{
	UMaterial* Material =
		(UMaterial*)StaticLoadObject(UMaterial::StaticClass(), nullptr, TEXT("Material'/ArcGISMapsSDK/Materials/RT_BlendLayers.RT_BlendLayers'"));

	MaterialInstance = UMaterialInstanceDynamic::Create(Material, nullptr, TEXT("FImageComposer"));

	MaterialInstance->AddToRoot();

	BlendTempRenderTarget = ::MakeShared<GPUResources::FGPUResourceRenderTarget>(256, 256, RTF_RGBA8, false);
}

FImageComposer::~FImageComposer()
{
	MaterialInstance->RemoveFromRoot();
}

void FImageComposer::Compose(const std::vector<ComposableImage>& InputImages, const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& Output)
{
	if (InputImages.size() == 0)
	{
		SetMap(0, nullptr, 0, 1, FVector4());
		SetMap(1, nullptr, 0, 1, FVector4());
		Draw(Output);
	}
	else if (InputImages.size() == 1)
	{
		SetMap(0, InputImages[0].Image->GetNativeTexture(), InputImages[0].Opacity, 1, InputImages[0].Extent);
		SetMap(1, nullptr, 0, 1, FVector4());
		Draw(Output);
	}
	else
	{
		SetMap(0, InputImages[0].Image->GetNativeTexture(), InputImages[0].Opacity, 1, InputImages[0].Extent);
		SetMap(1, InputImages[1].Image->GetNativeTexture(), InputImages[1].Opacity, 1, InputImages[1].Extent);
		Draw(Output);

		if (InputImages.size() > 2)
		{
			for (int32 i = 2; i < InputImages.size(); ++i)
			{
				SetMap(0, (i & 1) ? BlendTempRenderTarget->GetNativeRenderTarget() : Output->GetNativeRenderTarget(), 1, 0, FVector4(0, 0, 1, 1));
				SetMap(1, InputImages[i].Image->GetNativeTexture(), InputImages[i].Opacity, 1, InputImages[i].Extent);

				Draw((i & 1) ? Output : BlendTempRenderTarget);
			}

			if (InputImages.size() & 1)
			{
				Copy(BlendTempRenderTarget, Output, FVector4(0, 0, 1, 1));
			}
		}
	}
#ifdef PLATFORM_APPLE
	Flush(Output->GetNativeRenderTarget());
#endif
}

void FImageComposer::Compose(const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& SourceA,
							 const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& SourceB,
							 float Alpha,
							 const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& Output,
							 const FVector4& Extent)
{
	SetMap(0, SourceA->GetNativeRenderTarget(), 1, 0, Extent);
	SetMap(1, SourceB->GetNativeRenderTarget(), Alpha, 0, Extent);

	Draw(Output);
}

void FImageComposer::Copy(const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& Source,
						  const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& Output,
						  const FVector4& Extent)
{
	SetMap(0, Source->GetNativeRenderTarget(), 1, 0, Extent);
	SetMap(1, nullptr, 0, 0, FVector4());

	Draw(Output);
}

void FImageComposer::Draw(const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& Output)
{
	UKismetRenderingLibrary::DrawMaterialToRenderTarget(Outer, Output->GetNativeRenderTarget(), MaterialInstance);
}

void FImageComposer::SetMap(int Channel, UTexture* Texture, float Opacity, float bUseTextureAlpha, const FVector4& Extent)
{
	static FString MapString = TEXT("Map");
	static FString TextureExtensionString = TEXT("TextureExtension");
	static FString OpacityString = TEXT("Opacity");
	static FString UseTextureAlphaString = TEXT("UseTextureAlpha");

	MaterialInstance->SetTextureParameterValue(NumName(MapString, Channel), Texture);
	MaterialInstance->SetVectorParameterValue(NumName(TextureExtensionString, Channel), FLinearColor(Extent));
	MaterialInstance->SetScalarParameterValue(NumName(OpacityString, Channel), Opacity);
	MaterialInstance->SetScalarParameterValue(NumName(UseTextureAlphaString, Channel), bUseTextureAlpha);
}
} // namespace GPUComputing
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
