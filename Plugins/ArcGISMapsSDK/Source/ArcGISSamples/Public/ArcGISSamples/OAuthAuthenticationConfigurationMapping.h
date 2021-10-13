// Copyright 2021 Esri.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
//
#pragma once

// Unreal

#include "CoreMinimal.h"

// ArcGISSamples

#include "ArcGISSamples/OAuthAuthenticationConfiguration.h"

#include "OAuthAuthenticationConfigurationMapping.generated.h"

USTRUCT()
struct FOAuthAuthenticationConfigurationMapping
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	FString URI;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UOAuthAuthenticationConfiguration> OAuthAuthenticationConfiguration;
};
