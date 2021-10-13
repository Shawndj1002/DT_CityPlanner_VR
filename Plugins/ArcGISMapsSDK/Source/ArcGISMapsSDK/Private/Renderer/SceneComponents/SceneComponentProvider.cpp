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
#include "ArcGISMapsSDK/Renderer/SceneComponents/SceneComponentProvider.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
namespace SceneComponents
{
FSceneComponentProvider::FSceneComponentProvider(UObject* InOuter) : Outer(InOuter)
{
}

TSharedPtr<FSceneComponent> FSceneComponentProvider::CreateSceneComponent(uint32 Id)
{
	auto NewSceneComponent = ::MakeShared<FSceneComponent>(Outer);

	SceneComponents.emplace(Id, NewSceneComponent);

	return NewSceneComponent;
}

TSharedPtr<FSceneComponent> FSceneComponentProvider::GetSceneComponent(uint32 Id)
{
	return SceneComponents[Id];
}

void FSceneComponentProvider::DestroySceneComponent(uint32 Id)
{
	SceneComponents.erase(Id);
}
} // namespace SceneComponents
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
