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
#include "DetailsPanel/OAuthAuthenticationConfiguration.h"

// Unreal

#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

// ArcGISMapsSDKEditor

#include "MapControllerStyles.h"

void SOAuthAuthenticationConfiguration::Construct(const FArguments& InArgs, UOAuthAuthenticationConfiguration* oauthAuthenticationConfiguration)
{
	constexpr float Padding = 4;

	OAuthAuthenticationConfiguration = oauthAuthenticationConfiguration;

	SBox::Construct(SBox::FArguments());

	SetContent(
		SNew(SBox)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SEditableTextBox)
					.Padding(TextBoxPadding)
					.Style(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FEditableTextBoxStyle>("SolidEditableTextBox"))
					.Justification(ETextJustify::Right)
					.HintText(FText::FromString("Enter Name"))
					.Text_Lambda([this]()
					{
						return FText::FromString(OAuthAuthenticationConfiguration->Name);
					})
					.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
					{
						OAuthAuthenticationConfiguration->Name = NewText.ToString().TrimStartAndEnd();
					})
				]
			]
			+ SVerticalBox::Slot()
			.Padding(0, Padding, 0, 0)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("Client ID")))
				]
				+SHorizontalBox::Slot()
				.FillWidth(2.0f)
				[
					SNew(SEditableTextBox)
					.Padding(TextBoxPadding)
					.Style(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
					.HintText(FText::FromString("Enter Client ID"))
					.Text_Lambda([this]()
					{
						return FText::FromString(OAuthAuthenticationConfiguration->ClientID);
					})
					.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
					{
						OAuthAuthenticationConfiguration->ClientID = NewText.ToString().TrimStartAndEnd();
					})
				]
			]
			+ SVerticalBox::Slot()
			.Padding(0, Padding, 0, 0)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("Redirect URI")))
				]
				+SHorizontalBox::Slot()
				.FillWidth(2.0f)
				[
					SNew(SEditableTextBox)
					.Padding(TextBoxPadding)
					.Style(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
					.HintText(FText::FromString("Enter Redirect URI"))
					.Text_Lambda([this]()
					{
						return FText::FromString(OAuthAuthenticationConfiguration->RedirectURI);
					})
					.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
					{
						OAuthAuthenticationConfiguration->RedirectURI = NewText.ToString().TrimStartAndEnd();
					})
				]
			]

		]
	);
}
