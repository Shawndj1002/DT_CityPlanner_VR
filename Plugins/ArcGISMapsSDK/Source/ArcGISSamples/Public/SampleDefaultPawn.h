// Copyright 2021 Esri.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
//
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"

#include "ArcGISMapsSDK/Components/ArcGISCameraComponent.h"
#include "ArcGISMapsSDK/Components/ArcGISLocationComponent.h"

#include "SampleDefaultPawn.generated.h"

UCLASS(BlueprintType, Blueprintable)
class ARCGISSAMPLES_API ASampleDefaultPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:
	ASampleDefaultPawn();

	UPROPERTY(VisibleAnywhere)
	UArcGISCameraComponent* ArcGISCamera;

	UPROPERTY(VisibleAnywhere)
	UArcGISLocationComponent* ArcGISLocation;

protected:
	virtual void Tick(float DeltaTime) override;

private:
	void MoveUp_World(float Value) override;
	void MoveForward(float Value) override;
	void MoveRight(float Value) override;
	void AddControllerYawInput(float Value) override;
	void AddControllerPitchInput(float Value) override;
	void TurnAtRate(float Value) override;
	void LookUpAtRate(float Value) override;

	double GetCameraSpeed();
	FVector GetUpVector();
	void TurnCameraH(float Value);
	void TurnCameraV(float Value);

	void ApplyEarthCurvatureToMovement();

	FTransform LastTangentTransform;
	float PawnSpeed = 1.0f;
	bool bInitLastCoordinateSystem = false;
};
