// COPYRIGHT 1995-2021 ESRI
// TRADE SECRETS: ESRI PROPRIETARY AND CONFIDENTIAL
// Unpublished material - all rights reserved under the
// Copyright Laws of the United States and applicable international
// laws, treaties, and conventions.
//
// For additional information, contact:
// Environmental Systems Research Institute, Inc.
// Attn: Contracts and Legal Services Department
// 380 New York Street
// Redlands, California, 92373
// USA
//
// email: contracts@esri.com

#include "ArcGISMapsSDK/API/GameEngine/Attributes/Attribute.h"
#include "ArcGISMapsSDK/API/GameEngine/Attributes/VisualizationAttribute.h"
#include "ArcGISMapsSDK/API/GameEngine/Attributes/VisualizationAttributeDescription.h"
#include "ArcGISMapsSDK/API/Standard/Element.h"
#include "ArcGISMapsSDK/API/Standard/ElementType.h"
#include "ArcGISMapsSDK/API/Standard/IntermediateImmutableArray.h"
#include "ArcGISMapsSDK/API/Standard/IntermediateImmutableArrayBuilder.h"
#include "ArcGISMapsSDK/API/Unreal/ArrayBuilder.h"
#include "ArcGISMapsSDK/API/Unreal/ImmutableArray.h" // IWYU pragma: associated

namespace Esri
{
namespace Unreal
{
template <>
ARCGISMAPSSDK_API FString ImmutableArray<FString>::At(size_t position) const
{
	auto intermediateImmutableArray = reinterpret_cast<const Standard::IntermediateImmutableArray*>(this);

	auto element = intermediateImmutableArray->At(position);

	return element.GetValueAsString();
}

template <>
ARCGISMAPSSDK_API GameEngine::Attributes::Attribute
ImmutableArray<GameEngine::Attributes::Attribute>::At(size_t position) const
{
	auto intermediateImmutableArray = reinterpret_cast<const Standard::IntermediateImmutableArray*>(this);

	auto element = intermediateImmutableArray->At(position);

	return element.GetValueAsAttribute();
}

template <>
ARCGISMAPSSDK_API GameEngine::Attributes::VisualizationAttribute
ImmutableArray<GameEngine::Attributes::VisualizationAttribute>::At(size_t position) const
{
	auto intermediateImmutableArray = reinterpret_cast<const Standard::IntermediateImmutableArray*>(this);

	auto element = intermediateImmutableArray->At(position);

	return element.GetValueAsVisualizationAttribute();
}

template <>
ARCGISMAPSSDK_API GameEngine::Attributes::VisualizationAttributeDescription
ImmutableArray<GameEngine::Attributes::VisualizationAttributeDescription>::At(size_t position) const
{
	auto intermediateImmutableArray = reinterpret_cast<const Standard::IntermediateImmutableArray*>(this);

	auto element = intermediateImmutableArray->At(position);

	return element.GetValueAsVisualizationAttributeDescription();
}

template <>
ArrayBuilder<FString> ImmutableArray<FString>::CreateBuilder()
{
	auto intermediateArrayBuilder = Standard::IntermediateImmutableArray::CreateBuilder(Standard::ElementType::String);

	auto handle = intermediateArrayBuilder.GetHandle();

	intermediateArrayBuilder.SetHandle(nullptr);

	return ArrayBuilder<FString>(handle);
}

template <>
ArrayBuilder<GameEngine::Attributes::VisualizationAttributeDescription>
ImmutableArray<GameEngine::Attributes::VisualizationAttributeDescription>::CreateBuilder()
{
	auto intermediateArrayBuilder = Standard::IntermediateImmutableArray::CreateBuilder(Standard::ElementType::VisualizationAttributeDescription);

	auto handle = intermediateArrayBuilder.GetHandle();

	intermediateArrayBuilder.SetHandle(nullptr);

	return ArrayBuilder<GameEngine::Attributes::VisualizationAttributeDescription>(handle);
}
} // namespace Unreal
} // namespace Esri
