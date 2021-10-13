// Copyright 2021 Esri.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
//
#include "SampleDefaultPawn.h"

// Unreal

#include "Engine/World.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PlayerController.h"

// ArcGISMapsSDK

#include "ArcGISMapsSDK/Actors/ArcGISMapView.h"

ASampleDefaultPawn::ASampleDefaultPawn() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	ArcGISCamera = CreateDefaultSubobject<UArcGISCameraComponent>(TEXT("ArcGISCamera"));
	ArcGISLocation = CreateDefaultSubobject<UArcGISLocationComponent>(TEXT("ArcGISLocation"));

#if WITH_EDITOR
	ArcGISCamera->bVisualizeComponent = true;
#endif

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

FVector ASampleDefaultPawn::GetUpVector()
{
	auto ArcGISMapView = AArcGISMapView::GetArcGISMapView(this);
	if (ArcGISMapView)
	{
		auto TangentTransform = ArcGISMapView->GetTangentTransformAtEngineLocation(GetActorLocation());
		return TangentTransform.GetUnitAxis(EAxis::Z);
	}
	return FVector(0, 0, 1);
}

void ASampleDefaultPawn::AddControllerPitchInput(float Value)
{
	if (Controller && Value != 0.f)
	{
		auto controller = (APlayerController*)Controller;

		if (controller->IsInputKeyDown(EKeys::LeftMouseButton))
		{
			auto cameraRightVector = GetActorRotation().RotateVector(FVector::RightVector);

			// The forward vector is tangent to the earth, so the camera moves parallel to the Earth surface
			// in the direction of the camera. The effect is similar to rotating the earth.
			auto forwardVector = (cameraRightVector ^ GetUpVector());

			AddMovementInput(forwardVector.GetSafeNormal(), Value * PI * GetCameraSpeed());
		}
		else if (controller->IsInputKeyDown(EKeys::RightMouseButton))
		{
			TurnCameraV(Value);
		}
	}
}

void ASampleDefaultPawn::AddControllerYawInput(float Value)
{
	if (Controller && Value != 0.f)
	{
		auto controller = (APlayerController*)Controller;

		if (controller->IsInputKeyDown(EKeys::LeftMouseButton))
		{
			auto cameraRightVector = GetActorRotation().RotateVector(FVector::RightVector);

			auto upVector = GetUpVector();

			// The right vector is tangent to the earth, so the camera moves parallel to the Earth surface
			// perpendicular to the direction of the camera. The effect is similar to rotating the earth.
			auto rightVector = (cameraRightVector ^ upVector) ^ upVector;

			AddMovementInput(rightVector.GetSafeNormal(), Value * PI * GetCameraSpeed());
		}
		else if (controller->IsInputKeyDown(EKeys::RightMouseButton))
		{
			TurnCameraH(Value);
		}
	}
}

void ASampleDefaultPawn::TurnAtRate(float Value)
{
	if (Controller && Value != 0.f)
	{
		TurnCameraH(Value);
	}
}

void ASampleDefaultPawn::LookUpAtRate(float Value)
{
	if (Controller && Value != 0.f)
	{
		TurnCameraV(Value);
	}
}

void ASampleDefaultPawn::TurnCameraH(float Value)
{
	auto controller = (APlayerController*)Controller;

	FQuat axisRot(GetUpVector(), Value * FMath::DegreesToRadians(controller->InputYawScale));
	SetActorRotation((axisRot * GetActorRotation().Quaternion()).Rotator());
}

void ASampleDefaultPawn::TurnCameraV(float Value)
{
	auto controller = (APlayerController*)Controller;

	FQuat axisRot(FVector::RightVector, -Value * FMath::DegreesToRadians(controller->InputPitchScale));
	SetActorRotation((GetActorRotation().Quaternion() * axisRot).Rotator());
}

double ASampleDefaultPawn::GetCameraSpeed()
{
	UFloatingPawnMovement* movement = (UFloatingPawnMovement*)MovementComponent;
	return movement->MaxSpeed / 100.0;
}

void ASampleDefaultPawn::MoveForward(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(FRotationMatrix(GetActorRotation()).GetScaledAxis(EAxis::X), Value * GetCameraSpeed());
	}
}

void ASampleDefaultPawn::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(FRotationMatrix(GetActorRotation()).GetScaledAxis(EAxis::Y), Value * GetCameraSpeed());
	}
}

void ASampleDefaultPawn::MoveUp_World(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(FRotationMatrix(GetActorRotation()).GetScaledAxis(EAxis::Z), Value * GetCameraSpeed());
	}
}

void ASampleDefaultPawn::Tick(float DeltaTime)
{
	ApplyEarthCurvatureToMovement();

	Super::Tick(DeltaTime);
}

void ASampleDefaultPawn::ApplyEarthCurvatureToMovement()
{
	auto ArcGISMapView = AArcGISMapView::GetArcGISMapView(this);
	if (ArcGISMapView)
	{
		auto TangentTransform = ArcGISMapView->GetTangentTransformAtEngineLocation(GetActorLocation());

		if (!bInitLastCoordinateSystem)
		{
			LastTangentTransform = TangentTransform;
			bInitLastCoordinateSystem = true;
		}

		// Adjust rotation to follow the Earth, while preventing spinning at the poles
		FQuat NewRotation = GetActorRotation().Quaternion();
		auto movement = static_cast<UFloatingPawnMovement*>(GetMovementComponent());

		if (FVector::DotProduct(TangentTransform.GetRotation().Vector(), LastTangentTransform.GetRotation().Vector()) > 0.8)
		{
			auto DeltaRotation = TangentTransform.GetRotation() * LastTangentTransform.GetRotation().Inverse();
			NewRotation = DeltaRotation * NewRotation;
			movement->Velocity = DeltaRotation * movement->Velocity;
		}

		SetActorRotation(NewRotation, ETeleportType::TeleportPhysics);

		FCartesianCoordinates ECEFCoordinates;
		ArcGISMapView->EngineToECEF(GetActorLocation(), ECEFCoordinates);

		FGeographicCoordinates GeographicCoordinates;
		ArcGISMapView->ECEFToGeographic(ECEFCoordinates, GeographicCoordinates);
		auto GroundDistance = GeographicCoordinates.Altitude * 100.0;

		movement->MaxSpeed = PawnSpeed * GroundDistance;
		movement->Acceleration = movement->MaxSpeed * 3.0;
		movement->Deceleration = movement->Acceleration * 2.0;

		LastTangentTransform = TangentTransform;
	}
}
