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

#include "ArcGISMapsSDK/API/GameEngine/Elevation/Base/ArcGISElevationSource.h"
#include "ArcGISMapsSDK/API/GameEngine/View/State/ArcGISRendererViewState.h"

namespace Esri
{
namespace GameEngine
{
namespace View
{
namespace Event
{
using ArcGISRendererViewStateChangedEvent = std::function<void(Esri::GameEngine::View::State::ArcGISRendererViewState& arcGISRendererViewState)>;
} // namespace Event
} // namespace View
} // namespace GameEngine
} // namespace Esri
