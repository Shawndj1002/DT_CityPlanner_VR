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
#include "ArcGISMapsSDK/Security/AuthenticationChallengeManager.h"

#include "Async/Async.h"

#include "ArcGISMapsSDK/API/GameEngine/Security/ArcGISAuthenticationManager.h"
#include "ArcGISMapsSDK/API/GameEngine/Security/ArcGISOAuthAuthenticationChallenge.h"
#include "ArcGISMapsSDK/Security/OAuthAuthenticationChallengeHandler.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Security
{
namespace
{
struct FChallenge
{
	Esri::GameEngine::Security::ArcGISOAuthAuthenticationChallenge AuthenticationChallenge;
};
} // namespace

void AuthenticationChallengeManager::SetOAuthChallengeHandler(const TSharedPtr<OAuthAuthenticationChallengeHandler>& OAuthChallengeHandler)
{
	Esri::GameEngine::Security::ArcGISAuthenticationManager::SetAuthenticationChallengeIssued(
		[OAuthChallengeHandler](Esri::GameEngine::Security::ArcGISAuthenticationChallenge& AuthChallenge) {
			auto& ArcGISOAuthAuthenticationChallenge = static_cast<Esri::GameEngine::Security::ArcGISOAuthAuthenticationChallenge&>(AuthChallenge);

			auto Challenge = ::MakeShared<FChallenge>();

			Challenge->AuthenticationChallenge = std::move(ArcGISOAuthAuthenticationChallenge);

			AsyncTask(ENamedThreads::GameThread, [Challenge, OAuthChallengeHandler]() {
				OAuthChallengeHandler->HandleChallenge(Challenge->AuthenticationChallenge.GetAuthorizeURI())
					.Then([Challenge](TFuture<TOptional<FString>> Continuation) {
						auto AuthorizationCode = Continuation.Get();

						if (AuthorizationCode.IsSet())
						{
							Challenge->AuthenticationChallenge.Respond(AuthorizationCode.GetValue());
						}
						else
						{
							Challenge->AuthenticationChallenge.Cancel();
						}
					});
			});
		});
}
} // namespace Security
} // namespace ArcGISMapsSDK
} // namespace Esri
