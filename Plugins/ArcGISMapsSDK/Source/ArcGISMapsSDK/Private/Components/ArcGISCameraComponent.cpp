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
#include "ArcGISMapsSDK/Components/ArcGISCameraComponent.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#include "ArcGISMapsSDK/API/ArcGISRuntime/Geometry/SpatialReference.h"
#include "ArcGISMapsSDK/Actors/ArcGISMapView.h"
#include "ArcGISMapsSDK/BlueprintNodes/GameEngine/View/ArcGISRendererView.h"

UArcGISCameraComponent::UArcGISCameraComponent() : Super()
{
	PrimaryComponentTick.bCanEverTick = true;

	// The update of the camera has to be done at the end of the frame, when all the actors and pawn have their position defined.
	// If we changed the camera during the update, some actors could use the new transformation and some the old one, resulting in objects poping in/out one frame.
	PrimaryComponentTick.TickGroup = TG_LastDemotable;
}

void UArcGISCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdateViewport();
}

void UArcGISCameraComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ArcGISRendererView.Reset();

	Super::EndPlay(EndPlayReason);
}

void UArcGISCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateViewport();

	PushPosition();
}

void UArcGISCameraComponent::SetRendererView(UArcGISRendererView* InRendererView)
{
	ArcGISRendererView = InRendererView->APIObject;
}

void UArcGISCameraComponent::SetRendererView(TSharedPtr<Esri::GameEngine::View::ArcGISRendererView> InRendererView)
{
	ArcGISRendererView = std::move(InRendererView);
}

void UArcGISCameraComponent::PushPosition()
{
	if (!ArcGISRendererView)
	{
		return;
	}

	auto ArcGISMapView = AArcGISMapView::GetArcGISMapView(this);
	if (ArcGISMapView)
	{
		if (!ArcGISCamera)
		{
			FString Name("ArcGISRendererComponent_Camera");
			Esri::GameEngine::Location::ArcGISRotation Orientation(0, 0, 0);
			Esri::GameEngine::Location::ArcGISPosition Position(0, 0, 0);

			ArcGISCamera = ::MakeShared<Esri::GameEngine::Camera::ArcGISCamera>(Name, Position, Orientation);
		}

		auto CameraPosition = GetComponentLocation();
		if (ArcGISMapView->GetMapType() == EArcGISMapType::Local)
		{
			//Calculate SR bounds in ECEF coordinates once (not affected by rebasing).
			static const auto UpperLeftECEF = [ArcGISMapView]() {
				FCartesianCoordinates ECEF;
				ArcGISMapView->GeographicToECEF({-179.0, 88.0, 0.0}, ECEF);
				return ECEF;
			}();
			static const auto LowerRightECEF = [ArcGISMapView]() {
				FCartesianCoordinates ECEF;
				ArcGISMapView->GeographicToECEF({179.0, -88.0, 0.0}, ECEF);
				return ECEF;
			}();

			//ECEFToEngine is affected by rebasing and needs to be recalculated each frame.
			FVector MinCameraBound;
			ArcGISMapView->ECEFToEngine(UpperLeftECEF, MinCameraBound);
			FVector MaxCameraBound;
			ArcGISMapView->ECEFToEngine(LowerRightECEF, MaxCameraBound);

			//Camera's geo-position should not leave the bounds that are supported by the SR.
			const FBox2D CameraBounds = {FVector2D{MinCameraBound}, FVector2D{MaxCameraBound}};
			const auto ClosestPosition = CameraBounds.GetClosestPointTo(FVector2D{CameraPosition});
			CameraPosition = {ClosestPosition.X, ClosestPosition.Y, CameraPosition.Z};
		}

		FCartesianCoordinates ECEFCoordinates;
		ArcGISMapView->EngineToECEF(CameraPosition, ECEFCoordinates);

		FGeographicCoordinates GeographicCoordinates;
		ArcGISMapView->ECEFToGeographic(ECEFCoordinates, GeographicCoordinates);

		ArcGISCamera->SetPosition({GeographicCoordinates.Latitude, GeographicCoordinates.Longitude, GeographicCoordinates.Altitude,
								   Esri::ArcGISRuntime::Geometry::SpatialReference::WGS84()});

		// Tangent and Camera coordinate systems have a difference of +90 degrees in the Z axis.
		const auto CameraToTangent = FRotator(0, -90, 0).Quaternion();

		const auto CameraRotation = GetComponentRotation();

		const auto TangentTransform = ArcGISMapView->GetTangentTransformAtEngineLocation(CameraPosition);

		// Convert from UE rotation coordinates to geographic camera coordinates (relative to the tangent).
		const auto NewRotation = CameraToTangent.Inverse() * TangentTransform.GetRotation().Inverse() * CameraRotation.Quaternion();

		// Cameras in UE and RTC have a difference of +90 degrees in the internal Y axis. We have to add +90 degrees to the pitch to make the camera "look down" in RTC.
		const auto CameraInternalRotation = FRotator(90, 0, 0);

		// Note that ENU and Unreal cameras are both left handed, which permits this direct calculation of the rotator without having to transform axis.
		const auto Rotator = NewRotation.Rotator() + CameraInternalRotation;

		ArcGISCamera->SetOrientation({Rotator.Pitch, Rotator.Roll, Rotator.Yaw});

		ArcGISRendererView->SetCamera(ArcGISCamera);
	}
}

void UArcGISCameraComponent::UpdateViewport()
{
	if (!ArcGISRendererView)
	{
		return;
	}

	auto Controller = GetWorld()->GetFirstPlayerController();

	int32 SizeX;
	int32 SizeY;
	Controller->GetViewportSize(SizeX, SizeY);

	float FovAngle = Controller->PlayerCameraManager->GetFOVAngle();

	if (Fov != FovAngle || SizeX != ViewportSizeX || SizeY != ViewportSizeY)
	{
		Fov = FovAngle;
		ViewportSizeX = SizeX;
		ViewportSizeY = SizeY;

		if (SizeX != 0)
		{
			ArcGISRendererView->SetViewportProperties(SizeX, SizeY, FovAngle, FovAngle * SizeY / SizeX, 1);
		}
	}
}
