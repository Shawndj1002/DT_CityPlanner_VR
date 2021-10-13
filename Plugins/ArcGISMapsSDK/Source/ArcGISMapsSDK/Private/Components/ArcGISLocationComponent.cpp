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
#include "ArcGISMapsSDK/Components/ArcGISLocationComponent.h"

#include "ArcGISMapsSDK/Actors/ArcGISMapView.h"

// Tangent and Camera coordinate systems have a difference of +90 degrees in the Z axis.
auto CameraToTangent = FRotator(0, -90, 0).Quaternion();

// Cameras in UE and RTC have a difference of +90 degrees in the internal Y axis. We have to add +90 degrees to the pitch to make the camera "look down" in RTC.
auto CameraInternalRotation = FRotator(-90, 0, 0).Quaternion();

UArcGISLocationComponent::UArcGISLocationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bWantsOnUpdateTransform = true;
}

void UArcGISLocationComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdateTransform();
}

UArcGISPosition* UArcGISLocationComponent::GetPosition()
{
	return UArcGISPosition::CreateArcGISPositionWithXYZ(Position.Longitude, Position.Latitude, Position.Altitude);
}

UArcGISRotation* UArcGISLocationComponent::GetRotation()
{
	return UArcGISRotation::CreateArcGISRotation(Rotation.Pitch, Rotation.Roll, Rotation.Heading);
}

void UArcGISLocationComponent::SetPosition(const Esri::GameEngine::Location::ArcGISPosition& position)
{
	Position.Longitude = position.GetX();
	Position.Latitude = position.GetY();
	Position.Altitude = position.GetZ();

	bInternalHasChanged = true;
}

void UArcGISLocationComponent::SetRotation(const Esri::GameEngine::Location::ArcGISRotation& rotation)
{
	Rotation.Pitch = rotation.GetPitch();
	Rotation.Heading = rotation.GetHeading();
	Rotation.Roll = rotation.GetRoll();

	bInternalHasChanged = true;
}

void UArcGISLocationComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport)
{
	Super::OnUpdateTransform(UpdateTransformFlags, Teleport);

	auto ArcGISMapView = AArcGISMapView::GetArcGISMapView(this);

	if (ArcGISMapView && !bTransformHasChanged)
	{
		FCartesianCoordinates ProjectedCoordinates;
		ArcGISMapView->EngineToProjected(GetComponentLocation(), ProjectedCoordinates);

		FGeographicCoordinates GeographicCoordinates;
		ArcGISMapView->ProjectedToGeographic(ProjectedCoordinates, GeographicCoordinates);

		Position.Longitude = GeographicCoordinates.Longitude;
		Position.Latitude = GeographicCoordinates.Latitude;
		Position.Altitude = GeographicCoordinates.Altitude;

		auto TangentTransform = ArcGISMapView->GetTangentTransformAtGeographicLocation(GeographicCoordinates);

		// Convert from UE rotation coordinates to geographic camera coordinates (relative to the tangent).
		const auto GeographicQuaternion = TangentTransform.GetRotation().Inverse() * CameraToTangent.Inverse() * GetComponentRotation().Quaternion() *
			CameraInternalRotation.Inverse();

		const auto GeographicRotator = GeographicQuaternion.Rotator();

		Rotation.Pitch = GeographicRotator.Pitch;
		Rotation.Heading = GeographicRotator.Yaw;
		Rotation.Roll = GeographicRotator.Roll;
	}

	bTransformHasChanged = false;
}

#if WITH_EDITORONLY_DATA
void UArcGISLocationComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	UpdateTransform();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void UArcGISLocationComponent::SetPosition(UArcGISPosition* InPosition)
{
	Position.Longitude = InPosition->GetX();
	Position.Latitude = InPosition->GetY();
	Position.Altitude = InPosition->GetZ();

	bInternalHasChanged = true;
}

void UArcGISLocationComponent::SetRotation(UArcGISRotation* InRotation)
{
	Rotation.Pitch = InRotation->GetPitch();
	Rotation.Heading = InRotation->GetHeading();
	Rotation.Roll = InRotation->GetRoll();

	bInternalHasChanged = true;
}

void UArcGISLocationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bInternalHasChanged)
	{
		UpdateTransform();
	}
}

void UArcGISLocationComponent::UpdateTransform()
{
	auto ArcGISMapView = AArcGISMapView::GetArcGISMapView(this);

	if (ArcGISMapView)
	{
		FGeographicCoordinates GeographicCoordinates(Position.Longitude, Position.Latitude, Position.Altitude);

		FCartesianCoordinates ECEFCoordinates;
		ArcGISMapView->GeographicToECEF(GeographicCoordinates, ECEFCoordinates);

		FVector EngineLocation;
		ArcGISMapView->ECEFToEngine(ECEFCoordinates, EngineLocation);

		auto TangentTransform = ArcGISMapView->GetTangentTransformAtGeographicLocation(GeographicCoordinates);

		const auto GeographicRotator = FRotator(Rotation.Pitch, Rotation.Heading, Rotation.Roll);

		// Convert from geographic camera coordinates (relative to the tangent) to UE rotation coordinates.
		const auto NewCameraRotation = TangentTransform.GetRotation() * CameraToTangent * GeographicRotator.Quaternion() * CameraInternalRotation;

		bInternalHasChanged = false;
		bTransformHasChanged = true;

		GetOwner()->SetActorLocationAndRotation(EngineLocation, NewCameraRotation.Rotator(), false, nullptr, ETeleportType::ResetPhysics);
	}
}
