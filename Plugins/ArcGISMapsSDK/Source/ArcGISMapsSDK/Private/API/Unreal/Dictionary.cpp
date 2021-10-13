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

#include "ArcGISMapsSDK/API/GameEngine/Security/ArcGISAuthenticationConfiguration.h"
#include "ArcGISMapsSDK/API/Standard/ElementTraits.h"
#include "ArcGISMapsSDK/API/Standard/IntermediateDictionary.h"
#include "ArcGISMapsSDK/API/Unreal/Dictionary.h" // IWYU pragma: associated

namespace Esri
{
namespace Unreal
{
// This is a walkaround for macOS where the destructor symbol is not being exported as expected
void DestroyDictionary(void* object)
{
    auto intermediateDictionary = reinterpret_cast<Standard::IntermediateDictionary*>(object);

    intermediateDictionary->~IntermediateDictionary();
}

template <>
Dictionary<FString, GameEngine::Security::ArcGISAuthenticationConfiguration>::Dictionary()
{
    new (this) Standard::IntermediateDictionary
    (
        Standard::ElementTraits<FString>::type,
        Standard::ElementTraits<GameEngine::Security::ArcGISAuthenticationConfiguration>::type
    );
}

template <>
void Dictionary<FString, GameEngine::Security::ArcGISAuthenticationConfiguration>::Insert(const FString& key, const GameEngine::Security::ArcGISAuthenticationConfiguration& value)
{
    auto intermediateDictionary = reinterpret_cast<Standard::IntermediateDictionary*>(this);

    Standard::Element keyElement = Standard::ElementTraits<FString>::create(key);
    Standard::Element valueElement = Standard::ElementTraits<GameEngine::Security::ArcGISAuthenticationConfiguration>::create(value);

    intermediateDictionary->Insert(keyElement, valueElement);
}

template <>
void Dictionary<FString, GameEngine::Security::ArcGISAuthenticationConfiguration>::RemoveAll()
{
    auto intermediateDictionary = reinterpret_cast<Standard::IntermediateDictionary*>(this);

    intermediateDictionary->RemoveAll();
}
} // namespace Unreal
} // namespace Esri
