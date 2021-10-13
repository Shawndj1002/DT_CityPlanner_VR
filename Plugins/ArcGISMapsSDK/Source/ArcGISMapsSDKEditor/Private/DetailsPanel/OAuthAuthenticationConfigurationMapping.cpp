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
#include "DetailsPanel/OAuthAuthenticationConfigurationMapping.h"

// Unreal

#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/SBoxPanel.h"

// ArcGISMapsSDKEditor

#include "MapControllerStyles.h"

void SOAuthAuthenticationConfigurationMapping::Construct(const FArguments& InArgs, FOAuthAuthenticationConfigurationMapping* oauthAuthenticationConfigurationMapping, TArray<UOAuthAuthenticationConfiguration*>* oauthAuthenticationConfigurations)
{
	constexpr float Padding = 4;

	OAuthAuthenticationConfigurationMapping = oauthAuthenticationConfigurationMapping;
	OAuthAuthenticationConfigurations = oauthAuthenticationConfigurations;

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
						return FText::FromString(OAuthAuthenticationConfigurationMapping->Name);
					})
					.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
					{
						OAuthAuthenticationConfigurationMapping->Name = NewText.ToString().TrimStartAndEnd();
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
					.Text(FText::FromString(TEXT("URI")))
				]
				+SHorizontalBox::Slot()
				.FillWidth(2.0f)
				[
					SNew(SEditableTextBox)
					.HintText(FText::FromString("Enter URI"))
					.Text_Lambda([this]()
					{
						return FText::FromString(OAuthAuthenticationConfigurationMapping->URI);
					})
					.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
					{
						OAuthAuthenticationConfigurationMapping->URI = NewText.ToString().TrimStartAndEnd();
					})
					.Padding(TextBoxPadding)
					.Style(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
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
					.Text(FText::FromString(TEXT("Configuration")))
				]
				+SHorizontalBox::Slot()
				.FillWidth(2.0f)
				[
					SNew(SComboBox<UOAuthAuthenticationConfiguration*>)
					.OptionsSource(OAuthAuthenticationConfigurations)
					.OnGenerateWidget_Lambda([](UOAuthAuthenticationConfiguration* item)
					{
						return SNew(STextBlock)
							.Text_Lambda([item]()
							{
								return FText::FromString(item->Name);
							});
					})
					.OnSelectionChanged_Lambda([this](UOAuthAuthenticationConfiguration* selectedItem, ESelectInfo::Type)
					{
						OAuthAuthenticationConfigurationMapping->OAuthAuthenticationConfiguration = selectedItem;
					})
					.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("ComboBoxButton"))
					[
						SNew(STextBlock)
						.ColorAndOpacity(FSlateColor(ColorFontInner))
						.Text_Lambda([this]()
						{
							if (OAuthAuthenticationConfigurationMapping->OAuthAuthenticationConfiguration.IsValid())
							{
								return FText::FromString(OAuthAuthenticationConfigurationMapping->OAuthAuthenticationConfiguration->Name);
							}

							return FText::FromString("None");
						})
					]
				]
			]
		]
	);
}
