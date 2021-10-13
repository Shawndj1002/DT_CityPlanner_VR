// Copyright 2021 Esri.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
//
#pragma once

#if PLATFORM_ANDROID || PLATFORM_IOS
#include "ArcGISMapsSDK/Security/OAuthAuthenticationChallengeHandler.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Security
{
class FMobileOAuthAuthenticationChallengeHandler : public OAuthAuthenticationChallengeHandler
{
public:
	// OAuthAuthenticationChallengeHandler
	TFuture<TOptional<FString>> HandleChallenge(const FString& authorizeURI) override;

private:
	struct FChallenge
	{
		TAtomic<bool> bResponded{false};

		TPromise<TOptional<FString>> Promise;
	};

	TSharedPtr<FChallenge> ActiveChallenge;
};
} // namespace Security
} // namespace ArcGISMapsSDK
} // namespace Esri
#endif
