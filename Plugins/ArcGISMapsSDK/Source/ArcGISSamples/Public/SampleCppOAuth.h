// Copyright 2021 Esri.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
//
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SampleCppOAuth.generated.h"

class UArcGISRendererComponent;

UCLASS()
class ASampleCppOAuth : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASampleCppOAuth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly,
			  meta = (DisplayName = "ArcGISDataCamera", UseComponentPicker, AllowAnyActor, AllowedClasses = "ArcGISCameraComponent"))
	FComponentReference ArcGISDataCameraReference;

	UPROPERTY(EditAnywhere)
	FString ClientID;

	UPROPERTY(EditAnywhere)
	FString RedirectURI;

	UPROPERTY(EditAnywhere)
	FString ServiceURL;

private:
	UArcGISRendererComponent* ArcGISRendererComponent;
};
