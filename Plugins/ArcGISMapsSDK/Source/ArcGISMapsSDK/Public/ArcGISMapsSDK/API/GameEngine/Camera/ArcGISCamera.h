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

#include "ArcGISMapsSDK/API/GameEngine/Location/ArcGISPosition.h"
#include "ArcGISMapsSDK/API/GameEngine/Location/ArcGISRotation.h"

namespace Esri
{
namespace GameEngine
{
namespace Camera
{
class ARCGISMAPSSDK_API ArcGISCamera
{
public:
#pragma region Constructors
	/// Create a new ArcGIS Camera.
	///
	/// - Remark: Creates a camera.
	/// - Parameters:
	///   - name: Specifies the camera name.
	///   - position: Specifies the position.
	///   - orientation: Specifies the orientation with pitch, roll and heading of camera with ArcGISRotation.
	ArcGISCamera(FString& name, Location::ArcGISPosition& position, Location::ArcGISRotation& orientation);
#pragma endregion Constructors

#pragma region Properties
	/// Camera name
	///
	FString GetName() const;
	void SetName(const FString& name);

	/// The camera orientation ArcGISRotation
	///
	Location::ArcGISRotation GetOrientation() const;
	void SetOrientation(const Location::ArcGISRotation& orientation);

	/// The camera position ArcGISPosition
	///
	Location::ArcGISPosition GetPosition() const;
	void SetPosition(const Location::ArcGISPosition& position);
#pragma endregion Properties

private:
	FString Name;
	Location::ArcGISRotation Orientation;
	Location::ArcGISPosition Position;
};
} // namespace Camera
} // namespace GameEngine
} // namespace Esri
