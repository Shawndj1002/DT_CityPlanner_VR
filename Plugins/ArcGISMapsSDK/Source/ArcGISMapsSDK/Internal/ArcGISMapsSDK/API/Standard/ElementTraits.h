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

#pragma once

#include "ArcGISMapsSDK/API/GameEngine/Security/ArcGISAuthenticationConfiguration.h"
#include "ArcGISMapsSDK/API/Standard/Element.h"
#include "ArcGISMapsSDK/API/Standard/ElementType.h"

namespace Esri
{
namespace Standard
{
template<typename T>
struct ElementTraits;

template<>
struct ElementTraits<FString>
{
    static constexpr decltype(&Element::FromString) create{&Element::FromString};
    static constexpr ElementType type{ElementType::String};
};

template<>
struct ElementTraits<GameEngine::Security::ArcGISAuthenticationConfiguration>
{
    static constexpr decltype(&Element::FromArcGISAuthenticationConfiguration) create{&Element::FromArcGISAuthenticationConfiguration};
    static constexpr ElementType type{ElementType::ArcGISAuthenticationConfiguration};
};
} // namespace Standard
} // namespace Esri
