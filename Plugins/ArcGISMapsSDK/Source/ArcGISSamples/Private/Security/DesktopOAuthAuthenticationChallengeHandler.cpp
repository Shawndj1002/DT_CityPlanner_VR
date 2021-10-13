// Copyright 2021 Esri.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
//
#include "Security/DesktopOAuthAuthenticationChallengeHandler.h"

#include "Framework/Application/SlateApplication.h"
#include "Internationalization/Regex.h"
#include "WebBrowserModule.h"
#include "Widgets/Layout/SBorder.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Security
{
TSharedPtr<FDesktopOAuthAuthenticationChallengeHandler::FChallenge> FDesktopOAuthAuthenticationChallengeHandler::CreateWebBrowserWindow()
{
	auto challenge = ::MakeShared<FChallenge>();

	challenge->Window = SNew(SWindow).ClientSize(FVector2D(900, 700));

	FCreateBrowserWindowSettings WindowSettings;

	WindowSettings.bUseTransparency = true;
	WindowSettings.bShowErrorMessage = false;

	challenge->WebBrowserWindow = IWebBrowserModule::Get().GetSingleton()->CreateBrowserWindow(WindowSettings);

	challenge->WebBrowserWindow->OnTitleChanged().AddLambda([challenge](FString NewTitle) {
		if (NewTitle.StartsWith("SUCCESS code="))
		{
			if (!challenge->bResponded.Exchange(true))
			{
				FSlateApplicationBase::Get().RequestDestroyWindow(challenge->Window.ToSharedRef());

				NewTitle.ReplaceInline(TEXT("SUCCESS code="), TEXT(""));

				challenge->Promise.SetValue(NewTitle);
			}
		}
	});

	challenge->Window->GetOnWindowClosedEvent().AddLambda([challenge](const TSharedRef<SWindow>&) {
		if (!challenge->bResponded.Exchange(true))
		{
			challenge->Promise.SetValue({});
		}
	});

	// Setup browser widget.
	TSharedPtr<SWebBrowser> BrowserWidget;
	challenge->Window->SetContent(
		SNew(SBorder)
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			.Padding(0)[SAssignNew(BrowserWidget, SWebBrowser, challenge->WebBrowserWindow).ShowControls(false).ShowAddressBar(false)]);

	FSlateApplication::Get().AddWindow(challenge->Window.ToSharedRef(), true);
	challenge->Window->BringToFront();
	FSlateApplication::Get().SetKeyboardFocus(BrowserWidget, EFocusCause::SetDirectly);

	return challenge;
}

TFuture<TOptional<FString>> FDesktopOAuthAuthenticationChallengeHandler::HandleChallenge(const FString& authorizeURI)
{
	FRegexPattern RegexPattern("redirect_uri=([^&]*)");
	FRegexMatcher RegexMatcher(RegexPattern, authorizeURI);

	if (RegexMatcher.FindNext())
	{
		auto redirectURI = RegexMatcher.GetCaptureGroup(1);

		if (redirectURI == "urn:ietf:wg:oauth:2.0:oob")
		{
			auto challenge = CreateWebBrowserWindow();

			challenge->WebBrowserWindow->LoadURL(authorizeURI);

			return challenge->Promise.GetFuture();
		}

		UE_LOG(LogTemp, Error, TEXT("\"urn:ietf:wg:oauth:2.0:oob\" is the only supported redirect URI"));
	}

	return MakeFulfilledPromise<TOptional<FString>>().GetFuture();
}
} // namespace Security
} // namespace ArcGISMapsSDK
} // namespace Esri
