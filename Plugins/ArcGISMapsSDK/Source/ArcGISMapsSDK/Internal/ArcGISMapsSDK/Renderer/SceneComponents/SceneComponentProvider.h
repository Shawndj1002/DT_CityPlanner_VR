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

#include "ArcGISMapsSDK/Renderer/SceneComponents/SceneComponent.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
namespace SceneComponents
{
class FSceneComponentProvider
{
public:
	FSceneComponentProvider(UObject* InOuter);

	TSharedPtr<FSceneComponent> CreateSceneComponent(uint32 Id);

	TSharedPtr<FSceneComponent> GetSceneComponent(uint32 Id);

	void DestroySceneComponent(uint32 Id);

protected:
	UObject* Outer;

	std::unordered_map<uint32, TSharedPtr<FSceneComponent>> SceneComponents;
};
} // namespace SceneComponents
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
