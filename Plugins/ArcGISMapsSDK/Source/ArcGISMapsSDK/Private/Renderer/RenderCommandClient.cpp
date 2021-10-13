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
#include "ArcGISMapsSDK/Renderer/RenderCommandClient.h"

#include "Misc/EngineVersionComparison.h"

#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/DataBufferView.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/ComposeCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/ComposedTextureElement.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/CopyCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/CreateMaterialCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/CreateRenderTargetCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/CreateSceneComponentCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/CreateTextureCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/DestroyMaterialCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/DestroyRenderTargetCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/DestroySceneComponentCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/DestroyTextureCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/GenerateMipmapsCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/GenerateNormalTextureCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/MaterialType.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/MeshBufferChangeType.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/MultipleComposeCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/SetMaterialCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/SetMaterialNamedTexturePropertyCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/SetMaterialRenderTargetPropertyCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/SetMaterialScalarPropertyCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/SetMaterialTexturePropertyCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/SetMaterialVectorPropertyCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/SetMeshBufferCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/SetMeshCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/SetSceneComponentPivotCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/SetTexturePixelDataCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/SetVisibleCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/TextureFormat.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/RenderCommandType.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Vector4.h"
#include "ArcGISMapsSDK/API/Unreal/DataBuffer.h"
#include "ArcGISMapsSDK/Actors/ArcGISMapView.h"
#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceMaterial.h"

namespace
{
template <typename T>
TArrayView<T> ToArrayView(const Esri::GameEngine::RenderCommandQueue::DataBufferView& View)
{
	return TArrayView<T>(reinterpret_cast<T*>(View.Data), View.Size / sizeof(T));
}

template <typename To, typename From>
To TransformStruct(const From& from)
{
	To to;

	static_assert(sizeof(From) == sizeof(To), "The input struct size should be the same as the output one");
	static_assert(std::is_trivially_copyable<From>::value, "The input struct should be trivially copyable");
	static_assert(std::is_trivially_copyable<To>::value, "The output struct should be trivially copyable");

	memcpy(&to, &from, sizeof(To));

	return to;
}
} // namespace

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
FRenderCommandClient::FRenderCommandClient(UObject* InOuter,
										   TSharedPtr<GPUResources::FGPUResourceProvider> InGPUResourceProvider,
										   TSharedPtr<SceneComponents::FSceneComponentProvider> InSceneComponentProvider,
										   TSharedPtr<GPUComputing::FImageComposer> InImageComposer,
										   TSharedPtr<GPUComputing::FNormalMapGenerator> InNormalMapGenerator)
: GPUResourceProvider(InGPUResourceProvider),
  SceneComponentProvider(InSceneComponentProvider),
  ImageComposer(InImageComposer),
  NormalMapGenerator(InNormalMapGenerator)
{
	ArcGISMapView = AArcGISMapView::GetArcGISMapView(InOuter);
	check(ArcGISMapView != NULL);
}

void FRenderCommandClient::ExecuteRenderCommand(const GameEngine::RenderCommandQueue::RenderCommand& RenderCommand)
{
	using namespace GameEngine::RenderCommandQueue;
	switch (RenderCommand.GetType())
	{
		case RenderCommandType::CreateMaterial:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::CreateMaterialCommandParameters>();

			GPUResourceProvider->CreateMaterial(Params.MaterialId, Params.MaterialType, Params.Material);

			break;
		}

		case RenderCommandType::CreateRenderTarget:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::CreateRenderTargetCommandParameters>();

			GPUResourceProvider->CreateRenderTarget(Params.RenderTargetId, Params.Width, Params.Height, Params.TextureFormat, Params.HasMipMaps);

			break;
		}

		case RenderCommandType::CreateTexture:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::CreateTextureCommandParameters>();

			GPUResourceProvider->CreateTexture(Params.TextureId, Params.Width, Params.Height, Params.TextureFormat, Params.IsSRGB);

			break;
		}

		case RenderCommandType::CreateSceneComponent:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::CreateSceneComponentCommandParameters>();

			SceneComponentProvider->CreateSceneComponent(Params.SceneComponentId);

			break;
		}

		case RenderCommandType::DestroyMaterial:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::DestroyMaterialCommandParameters>();

			GPUResourceProvider->DestroyMaterial(Params.MaterialId);
			break;
		}

		case RenderCommandType::DestroyRenderTarget:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::DestroyRenderTargetCommandParameters>();

			GPUResourceProvider->DestroyRenderTarget(Params.RenderTargetId);
			break;
		}

		case RenderCommandType::DestroyTexture:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::DestroyTextureCommandParameters>();

			GPUResourceProvider->DestroyTexture(Params.TextureId);
			break;
		}

		case RenderCommandType::DestroySceneComponent:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::DestroySceneComponentCommandParameters>();

			SceneComponentProvider->DestroySceneComponent(Params.SceneComponentId);

			break;
		}

		case RenderCommandType::MultipleCompose:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::MultipleComposeCommandParameters>();

			const auto& Target = GPUResourceProvider->GetRenderTarget(Params.TargetId);

			std::vector<GPUComputing::ComposableImage> Images;

			auto Texture = reinterpret_cast<Parameters::ComposedTextureElement*>(Params.ComposedTextures.Data);
			auto Texture_end = reinterpret_cast<Parameters::ComposedTextureElement*>(Params.ComposedTextures.Data + Params.ComposedTextures.Size);

			while (Texture < Texture_end)
			{
				auto TargetTexture = GPUResourceProvider->GetTexture(Texture->TextureId);
				const auto& Region = Texture->Region;
				Images.emplace_back(GPUComputing::ComposableImage{FVector4(Region.X, Region.Y, Region.X + Region.Z, Region.Y + Region.W),
																  TargetTexture, Texture->Opacity});
				Texture++;
			}

			ImageComposer->Compose(Images, Target);
			break;
		}

		case RenderCommandType::Compose:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::ComposeCommandParameters>();

			const auto& Source1 = GPUResourceProvider->GetRenderTarget(Params.SourceId1);
			const auto& Source2 = GPUResourceProvider->GetRenderTarget(Params.SourceId2);
			const auto& Target = GPUResourceProvider->GetRenderTarget(Params.TargetId);

			ImageComposer->Compose(Source1, Source2, Params.Alpha, Target, reinterpret_cast<const FVector4&>(Params.Region));
			break;
		}

		case RenderCommandType::Copy:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::CopyCommandParameters>();

			const auto& Source = GPUResourceProvider->GetRenderTarget(Params.SourceId);
			const auto& Target = GPUResourceProvider->GetRenderTarget(Params.TargetId);

			ImageComposer->Copy(Source, Target, reinterpret_cast<const FVector4&>(Params.Region));
			break;
		}

		case RenderCommandType::GenerateNormalTexture:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::GenerateNormalTextureCommandParameters>();

			const auto& Target = GPUResourceProvider->GetRenderTarget(Params.TargetId);
			const auto& Elevation = GPUResourceProvider->GetTexture(Params.ElevationId);
			auto TileExtension = reinterpret_cast<const FVector4&>(Params.TileExtension);
			auto Region = reinterpret_cast<const FVector4&>(Params.TextureExtension);

			NormalMapGenerator->Compute(Elevation, TileExtension, FVector4(Region.X, Region.Y, Region.X + Region.Z, Region.Y + Region.W), Target);
			break;
		}

		case RenderCommandType::SetTexturePixelData:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::SetTexturePixelDataCommandParameters>();
			const auto& Texture = GPUResourceProvider->GetTexture(Params.TextureId);

			Texture->SetPixelData(Params.Pixels.Data);

			break;
		}

		case RenderCommandType::SetMaterialScalarProperty:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::SetMaterialScalarPropertyCommandParameters>();
			static auto Names = std::array<FString, 5>{"ClippingMode", "UseUVIndex", "BlendFactor", "PositionsBlendFactor", "Opacity"};

			const auto& Material = GPUResourceProvider->GetMaterial(Params.MaterialId);
			const auto& Name = Names[static_cast<int>(Params.MaterialScalarProperty)];

			Material->SetFloat(Name, Params.Value);
			break;
		}

		case RenderCommandType::SetMaterialVectorProperty:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::SetMaterialVectorPropertyCommandParameters>();

			static auto Names = std::array<FString, 2>{"MapAreaMin", "MapAreaMax"};

			const auto& Material = GPUResourceProvider->GetMaterial(Params.MaterialId);
			const auto& Name = Names[static_cast<int>(Params.MaterialVectorProperty)];
			const auto Value = TransformStruct<FVector4>(Params.Value);

			Material->SetVector(Name, Value);
			break;
		}

		case RenderCommandType::SetMaterialRenderTargetProperty:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::SetMaterialRenderTargetPropertyCommandParameters>();

			static auto Names = std::array<FString, 4>{"ImageryMap0", "ImageryMap1", "NormalMap0", "NormalMap1"};

			const auto& Material = GPUResourceProvider->GetMaterial(Params.MaterialId);
			const auto& RenderTarget = GPUResourceProvider->GetRenderTarget(Params.Value);
			const auto& Name = Names[static_cast<int>(Params.MaterialRenderTargetProperty)];

			Material->SetTexture(Name, RenderTarget);
			break;
		}

		case RenderCommandType::SetMaterialTextureProperty:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::SetMaterialTexturePropertyCommandParameters>();

			static auto Names = std::array<FString, 5>{"BaseMap", "UVRegionLUT", "PositionsMap0", "PositionsMap1", "FeatureIds"};

			const auto& Material = GPUResourceProvider->GetMaterial(Params.MaterialId);
			const auto& Texture = GPUResourceProvider->GetTexture(Params.Value);
			const auto& Name = Names[static_cast<int>(Params.MaterialTextureProperty)];

			Material->SetTexture(Name, Texture);
			break;
		}

		case RenderCommandType::SetMaterialNamedTextureProperty:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::SetMaterialNamedTexturePropertyCommandParameters>();

			const auto& Material = GPUResourceProvider->GetMaterial(Params.MaterialId);
			const auto& Texture = GPUResourceProvider->GetTexture(Params.Value);
			const FString Name{static_cast<int32>(Params.TextureName.Size), reinterpret_cast<const char*>(Params.TextureName.Data)};

			Material->SetTexture(Name, Texture);
			break;
		}
		case RenderCommandType::GenerateMipMaps:
		{
			RenderCommand.GetParameters<Parameters::GenerateMipMapsCommandParameters>();
			break;
		}

		case RenderCommandType::SetVisible:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::SetVisibleCommandParameters>();

			const auto& SceneComponent = SceneComponentProvider->GetSceneComponent(Params.SceneComponentId);

			SceneComponent->SetVisible(Params.IsVisible);
			break;
		}

		case RenderCommandType::SetMaterial:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::SetMaterialCommandParameters>();

			const auto& SceneComponent = SceneComponentProvider->GetSceneComponent(Params.SceneComponentId);
			const auto& Material = GPUResourceProvider->GetMaterial(Params.MaterialId);

			SceneComponent->SetMaterial(Material);
			break;
		}

		case RenderCommandType::SetMesh:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::SetMeshCommandParameters>();
			const auto& SceneComponent = SceneComponentProvider->GetSceneComponent(Params.SceneComponentId);

			auto SafeMeshData = ::MakeShared<FGPUResourceMesh>();

			// Assure the vertex attributes are always packed into the same UV index
			static constexpr auto RealUVIndex = 0;
			static constexpr auto FeatureIndicesUvIndex = 1;
			static constexpr auto UvRegionIdsUvIndex = 2;

			bool HasFeatureIndices = Params.FeatureIndices.Size > 0u;
			bool HasUvRegionIds = Params.UvRegionIds.Size > 0u;

			auto NumUVs = 1 + std::max(HasFeatureIndices * FeatureIndicesUvIndex, HasUvRegionIds * UvRegionIdsUvIndex);

			SafeMeshData->UVs.SetNum(NumUVs);

			if (HasFeatureIndices)
			{
				SafeMeshData->bUseFullPrecisionUVs = true;
				SafeMeshData->UVs[FeatureIndicesUvIndex] = {ToArrayView<float>(Params.FeatureIndices), 2};
			}

			if (HasUvRegionIds)
			{
				SafeMeshData->UVs[UvRegionIdsUvIndex] = {ToArrayView<float>(Params.UvRegionIds), 1};
			}

			SafeMeshData->Normals = ToArrayView<FVector>(Params.Normals);
			SafeMeshData->Tangents = ToArrayView<FVector4>(Params.Tangents);
			SafeMeshData->UVs[RealUVIndex] = {ToArrayView<float>(Params.Uvs), 2};
			SafeMeshData->Triangles = ToArrayView<int32>(Params.Triangles);
			SafeMeshData->Colors = ToArrayView<uint32>(Params.Colors);
			SafeMeshData->Positions = ToArrayView<FVector>(Params.Positions);

			SceneComponent->SetMesh(SafeMeshData);
			break;
		}

		case RenderCommandType::SetMeshBuffer:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::SetMeshBufferCommandParameters>();
			const auto& SceneComponent = SceneComponentProvider->GetSceneComponent(Params.SceneComponentId);

			SceneComponent->UpdatePositions(ToArrayView<FVector>(Params.Buffer));
			break;
		}

		case RenderCommandType::SetSceneComponentPivot:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::SetSceneComponentPivotCommandParameters>();

			const auto& SceneComponent = SceneComponentProvider->GetSceneComponent(Params.SceneComponentId);

			SceneComponent->SetScale(FVector(100.0));

			auto Transform = ArcGISMapView->GetPlanetCenterTransform();
			SceneComponent->SetRotation(Transform.GetRotation());

			// Pivots and vertices are in UE coordinates.
			auto UEFrameCoordinates = Params;

			// To convert from UE into ECEF/Projected, we have to invert the Y coordinate because of left-handed UE Frame.
			FCartesianCoordinates Coordinates(UEFrameCoordinates.X, -UEFrameCoordinates.Y, UEFrameCoordinates.Z);

			FVector NewLocation;
			if (ArcGISMapView->PlanetShape == EPlanetShape::RoundPlanet)
			{
				ArcGISMapView->ECEFToEngine(Coordinates, NewLocation);
			}
			else
			{
				ArcGISMapView->ProjectedToEngine(Coordinates, NewLocation);
			}
			SceneComponent->SetLocation(NewLocation);

			break;
		}

		case RenderCommandType::CommandGroupBegin:
		case RenderCommandType::CommandGroupEnd:
			break;

		default:
			throw "Unknown render command type!";
	}
}
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
