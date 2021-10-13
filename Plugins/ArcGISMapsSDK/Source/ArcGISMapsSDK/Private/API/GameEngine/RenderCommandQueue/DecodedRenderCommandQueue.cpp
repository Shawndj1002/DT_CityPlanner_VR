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
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/DecodedRenderCommandQueue.h"

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
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/NullCommandParameters.h"
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
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/RenderCommandType.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Vector4.h"
#include "ArcGISMapsSDK/API/Unreal/DataBuffer.h"

namespace Esri
{
namespace GameEngine
{
namespace RenderCommandQueue
{
DecodedRenderCommandQueue::DecodedRenderCommandQueue(Unreal::DataBuffer<uint8_t> InRawRenderCommands)
{
	RawRenderCommands = std::move(InRawRenderCommands);
}

#define DECODE_COMMAND_WITH_PARAMETERS(EnumValue, ParameterName)                 \
	case RenderCommandType::EnumValue:                                           \
	{                                                                            \
		return GetRenderCommand<Parameters::ParameterName##CommandParameters>(); \
	}

#define DECODE_COMMAND(EnumValue) DECODE_COMMAND_WITH_PARAMETERS(EnumValue, EnumValue)

RenderCommand* DecodedRenderCommandQueue::GetNextCommand()
{
	if (RawRenderCommands && CurrentOffset < RawRenderCommands.GetSizeBytes())
	{
		auto CommandType = GetCommandType();
		switch (CommandType)
		{
			DECODE_COMMAND(CreateMaterial);
			DECODE_COMMAND(CreateRenderTarget);
			DECODE_COMMAND(CreateTexture);
			DECODE_COMMAND(CreateSceneComponent);
			DECODE_COMMAND(DestroyMaterial);
			DECODE_COMMAND(DestroyRenderTarget);
			DECODE_COMMAND(DestroyTexture);
			DECODE_COMMAND(DestroySceneComponent);
			DECODE_COMMAND(MultipleCompose);
			DECODE_COMMAND(Compose);
			DECODE_COMMAND(Copy);
			DECODE_COMMAND(GenerateNormalTexture);
			DECODE_COMMAND(SetTexturePixelData);
			DECODE_COMMAND(SetMaterialScalarProperty);
			DECODE_COMMAND(SetMaterialVectorProperty);
			DECODE_COMMAND(SetMaterialRenderTargetProperty);
			DECODE_COMMAND(SetMaterialTextureProperty);
			DECODE_COMMAND(GenerateMipMaps);
			DECODE_COMMAND(SetVisible);
			DECODE_COMMAND(SetMaterial);
			DECODE_COMMAND(SetMesh);
			DECODE_COMMAND(SetMeshBuffer);
			DECODE_COMMAND(SetSceneComponentPivot);
			DECODE_COMMAND(SetMaterialNamedTextureProperty);
			DECODE_COMMAND_WITH_PARAMETERS(CommandGroupBegin, Null);
			DECODE_COMMAND_WITH_PARAMETERS(CommandGroupEnd, Null);
			default:
			{
				// debug ignore unknown commands
				throw "Unknown render command type!";
			}
		}
	}
	return nullptr;
}

RenderCommandType DecodedRenderCommandQueue::GetCommandType()
{
	return *reinterpret_cast<RenderCommandType*>(RawRenderCommands.GetData<uint8>() + CurrentOffset);
}

template <typename T>
RenderCommand* DecodedRenderCommandQueue::GetRenderCommand()
{
	auto Address = RawRenderCommands.GetData<uint8>() + CurrentOffset;
	CurrentOffset += sizeof(RenderCommandType) + sizeof(T);
	return reinterpret_cast<RenderCommand*>(Address);
}
} // namespace RenderCommandQueue
} // namespace GameEngine
} // namespace Esri
