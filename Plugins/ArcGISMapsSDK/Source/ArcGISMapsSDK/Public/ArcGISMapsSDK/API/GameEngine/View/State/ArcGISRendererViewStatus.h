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

namespace Esri
{
namespace GameEngine
{
namespace View
{
namespace State
{
enum class ArcGISRendererViewStatus
{
	Active = 1,
	MapNotReady = 2,
	NoViewport = 4,
	Loading = 8,
	Error = 16,
	Warning = 32
};

enum class ArcGISElevationSourceViewStatus
{
	Active = 1,
	NotEnabled = 2,
	OutOfScale = 4,
	Loading = 8,
	Error = 16,
	Warning = 32
};

enum class ArcGISLayerViewStatus
{
	Active = 1,
	NotVisible = 2,
	OutOfScale = 4,
	Loading = 8,
	Error = 16,
	Warning = 32
};

} //namespace State
} //namespace View
} //namespace GameEngine
} //namespace Esri
