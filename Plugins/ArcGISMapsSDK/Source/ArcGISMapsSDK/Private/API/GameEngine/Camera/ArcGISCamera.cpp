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

#include "ArcGISMapsSDK/API/ArcGISRuntime/Geometry/SpatialReference.h"
#include "ArcGISMapsSDK/API/GameEngine/Camera/ArcGISCamera.h"

namespace Esri
{
namespace GameEngine
{
namespace Camera
{
ArcGISCamera::ArcGISCamera(FString& name, Location::ArcGISPosition& position, Location::ArcGISRotation& orientation)
: Name(name),
  Orientation(orientation)
{
	Position = Location::ArcGISPosition(position.GetX(), position.GetY(), position.GetZ(), position.GetSpatialReference());
}

FString ArcGISCamera::GetName() const
{
	return Name;
}
void ArcGISCamera::SetName(const FString& name)
{
	Name = name;
}

Location::ArcGISRotation ArcGISCamera::GetOrientation() const
{
	return Orientation;
}

void ArcGISCamera::SetOrientation(const Location::ArcGISRotation& orientation)
{
	Orientation = orientation;
}

Location::ArcGISPosition ArcGISCamera::GetPosition() const
{
	return Location::ArcGISPosition(Position.GetX(), Position.GetY(), Position.GetZ(), Position.GetSpatialReference());
}

void ArcGISCamera::SetPosition(const Location::ArcGISPosition& position)
{
	Position = Location::ArcGISPosition(position.GetX(), position.GetY(), position.GetZ(), position.GetSpatialReference());
}
} // namespace Camera
} // namespace GameEngine
} // namespace Esri
