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

#include "ArcGISMapsSDK/API/Unreal/Exception.h"
#include "ArcGISMapsSDK/API/ArcGISRuntime/MapView/ElevationSourceViewState.h"

namespace Esri
{
namespace GameEngine
{
namespace View
{
namespace State
{
enum class ArcGISElevationSourceViewStatus;

class ARCGISMAPSSDK_API ArcGISElevationSourceViewState final
{
public:
	ArcGISElevationSourceViewState(ArcGISRuntime::MapView::ElevationSourceViewState status);

	Unreal::Exception GetError() const;
	ArcGISElevationSourceViewStatus GetStatus() const;

private:
	ArcGISRuntime::MapView::ElevationSourceViewState State;
};
} // namespace State
} // namespace View
} // namespace GameEngine
} // namespace Esri
