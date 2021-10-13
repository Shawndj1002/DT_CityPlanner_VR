// Copyright 2021 Esri.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
//
#include "Security/MobileOAuthAuthenticationChallengeHandler.h"

#if PLATFORM_ANDROID || PLATFORM_IOS
#include "Internationalization/Regex.h"
#include "WebAuth.h"
#include "WebAuthModule.h"

static Esri::ArcGISMapsSDK::Security::FMobileOAuthAuthenticationChallengeHandler* mobileOAuthAuthenticationChallengeHandler = nullptr;

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Security
{
TFuture<TOptional<FString>> FMobileOAuthAuthenticationChallengeHandler::HandleChallenge(const FString& authorizeURI)
{
	FRegexPattern RegexPattern("redirect_uri=([^&]*)");
	FRegexMatcher RegexMatcher(RegexPattern, authorizeURI);

	if (RegexMatcher.FindNext())
	{
		auto redirectURI = RegexMatcher.GetCaptureGroup(1);

		if (redirectURI != "urn:ietf:wg:oauth:2.0:oob")
		{
			auto challenge = ::MakeShared<FChallenge>();

			auto& webAuth = FWebAuthModule::Get().GetWebAuth();

			webAuth.AuthSessionWithURL(
				authorizeURI, redirectURI,
				FWebAuthSessionCompleteDelegate::CreateLambda([challenge, redirectURI](FString RedirectURL, bool bHasResponse) {
					auto pattern = redirectURI + "?code=";

					if (RedirectURL.StartsWith(pattern))
					{
						if (!challenge->bResponded.Exchange(true))
						{
							RedirectURL.ReplaceInline(*pattern, TEXT(""));

							challenge->Promise.SetValue(RedirectURL);
						}
					}
					else
					{
						if (!RedirectURL.IsEmpty())
						{
							UE_LOG(LogTemp, Error, TEXT("Invalid redirect URI"));
						}

						challenge->Promise.SetValue({});
					}
				}));

			return challenge->Promise.GetFuture();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("\"urn:ietf:wg:oauth:2.0:oob\" is not a supported redirect URI"));
		}
	}

	return MakeFulfilledPromise<TOptional<FString>>().GetFuture();
}
} // namespace Security
} // namespace ArcGISMapsSDK
} // namespace Esri
#endif
