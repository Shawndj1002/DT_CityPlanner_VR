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
#pragma once

// Unreal

#include "Widgets/Layout/SBox.h"

// ArcGISSamples

#include "ArcGISSamples/OAuthAuthenticationConfigurationMapping.h"

class SOAuthAuthenticationConfigurationMapping : public SBox
{
public:
	void Construct(const FArguments& InArgs, FOAuthAuthenticationConfigurationMapping* oauthAuthenticationConfigurationMapping, TArray<UOAuthAuthenticationConfiguration*>* oauthAuthenticationConfigurations);

private:
	FOAuthAuthenticationConfigurationMapping* OAuthAuthenticationConfigurationMapping{nullptr};
	TArray<UOAuthAuthenticationConfiguration*>* OAuthAuthenticationConfigurations{nullptr};
};
