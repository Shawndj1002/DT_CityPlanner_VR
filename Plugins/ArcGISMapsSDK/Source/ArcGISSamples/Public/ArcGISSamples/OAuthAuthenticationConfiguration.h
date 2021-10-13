// Copyright 2021 Esri.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
//
#pragma once

// Unreal

#include "CoreMinimal.h"

#include "OAuthAuthenticationConfiguration.generated.h"

UCLASS()
class ARCGISSAMPLES_API UOAuthAuthenticationConfiguration : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	FString ClientID;

	UPROPERTY(EditAnywhere)
	FString RedirectURI;
};
