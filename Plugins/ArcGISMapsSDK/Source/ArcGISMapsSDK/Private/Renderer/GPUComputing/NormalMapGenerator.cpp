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
#include "ArcGISMapsSDK/Renderer/GPUComputing/NormalMapGenerator.h"

#include "Kismet/KismetRenderingLibrary.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
namespace GPUComputing
{
FNormalMapGenerator::FNormalMapGenerator(UObject* InOuter) : Outer(InOuter)
{
	auto Material = static_cast<UMaterial*>(
		StaticLoadObject(UMaterial::StaticClass(), nullptr, TEXT("Material'/ArcGISMapsSDK/Materials/CalculateNormals.CalculateNormals'")));

	MaterialInstance = UMaterialInstanceDynamic::Create(Material, nullptr, TEXT("FNormalMapGenerator"));

	MaterialInstance->AddToRoot();
}

FNormalMapGenerator::~FNormalMapGenerator()
{
	MaterialInstance->RemoveFromRoot();
}

void FNormalMapGenerator::Compute(const TSharedPtr<GPUResources::FGPUResourceTexture2D>& InputElevation,
								  FVector4 TileExtension,
								  FVector4 TextureExtension,
								  const TSharedPtr<const GPUResources::FGPUResourceRenderTarget>& Output)
{
	MaterialInstance->SetTextureParameterValue("Elevations", InputElevation->GetNativeTexture());

	MaterialInstance->SetVectorParameterValue(FName("TileExtension"), FLinearColor(TileExtension));
	MaterialInstance->SetVectorParameterValue(FName("TextureExtension"), FLinearColor(TextureExtension));

	UKismetRenderingLibrary::DrawMaterialToRenderTarget(Outer, Output->GetNativeRenderTarget(), MaterialInstance);
}
} // namespace GPUComputing
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
