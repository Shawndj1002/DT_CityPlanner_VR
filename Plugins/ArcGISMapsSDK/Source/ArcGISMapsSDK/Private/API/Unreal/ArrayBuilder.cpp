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

#include "ArcGISMapsSDK/API/GameEngine/Attributes/VisualizationAttributeDescription.h"
#include "ArcGISMapsSDK/API/Standard/Element.h"
#include "ArcGISMapsSDK/API/Standard/IntermediateImmutableArrayBuilder.h"
#include "ArcGISMapsSDK/API/Unreal/ArrayBuilder.h" // IWYU pragma: associated
#include "ArcGISMapsSDK/API/Unreal/ImmutableArray.h"

namespace Esri
{
namespace Unreal
{
template <>
void ArrayBuilder<FString>::Add(const FString& value)
{
	auto intermediateImmutableArrayBuilder = reinterpret_cast<Standard::IntermediateImmutableArrayBuilder*>(this);

	intermediateImmutableArrayBuilder->Add(Standard::Element::FromString(value));
}

template <>
void ArrayBuilder<GameEngine::Attributes::VisualizationAttributeDescription>::Add(const GameEngine::Attributes::VisualizationAttributeDescription& value)
{
	auto intermediateImmutableArrayBuilder = reinterpret_cast<Standard::IntermediateImmutableArrayBuilder*>(this);

	intermediateImmutableArrayBuilder->Add(Standard::Element::FromVisualizationAttributeDescription(value));
}

template <>
Unreal::ImmutableArray<FString> ArrayBuilder<FString>::MoveToArray()
{
	auto intermediateImmutableArrayBuilder = reinterpret_cast<Standard::IntermediateImmutableArrayBuilder*>(this);

	auto immutableList = intermediateImmutableArrayBuilder->MoveToArray();

	auto handle = immutableList.GetHandle();

	immutableList.SetHandle(nullptr);

	return Unreal::ImmutableArray<FString>(handle);
}

template <>
Unreal::ImmutableArray<GameEngine::Attributes::VisualizationAttributeDescription> ArrayBuilder<GameEngine::Attributes::VisualizationAttributeDescription>::MoveToArray()
{
	auto intermediateImmutableArrayBuilder = reinterpret_cast<Standard::IntermediateImmutableArrayBuilder*>(this);

	auto immutableList = intermediateImmutableArrayBuilder->MoveToArray();

	auto handle = immutableList.GetHandle();

	immutableList.SetHandle(nullptr);

	return Unreal::ImmutableArray<GameEngine::Attributes::VisualizationAttributeDescription>(handle);
}
} // namespace Unreal
} // namespace Esri
