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
#include "ArcGISMapsSDK/API/GameEngine/View/State/ArcGISElevationSourceViewState.h"
#include "ArcGISMapsSDK/API/GameEngine/View/State/ArcGISRendererViewStatus.h"

namespace Esri
{
namespace GameEngine
{
namespace View
{
namespace State
{
ArcGISElevationSourceViewState::ArcGISElevationSourceViewState(ArcGISRuntime::MapView::ElevationSourceViewState state)
{
	State = std::move(state);
};

ArcGISElevationSourceViewStatus ArcGISElevationSourceViewState::GetStatus() const
{
	return static_cast<ArcGISElevationSourceViewStatus>(State.GetStatus());
};

Unreal::Exception ArcGISElevationSourceViewState::GetError() const
{
	return State.GetError();
};
} // namespace State
} // namespace View
} // namespace GameEngine
} // namespace Esri
