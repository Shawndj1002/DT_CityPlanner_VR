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
#include "DetailsPanelAuthentication.h"

// Unreal

#include "Containers/UnrealString.h"
#include "DetailWidgetRow.h"
#include "Widgets/Text/STextBlock.h"

// ArcGISMapsSDKEditor

#include "DetailsPanel/ArcGISMapsSDKArray.h"
#include "DetailsPanel/OAuthAuthenticationConfiguration.h"
#include "DetailsPanel/OAuthAuthenticationConfigurationMapping.h"
#include "DetailsPanelUtilities.h"
#include "MapControllerStyles.h"

namespace
{
FString FindNextConfigurationName(TSharedPtr<AArcGISMapController> arcGISMapController)
{
	uint32 index = 1;

	FNumberFormattingOptions NumberFormattingOptions;
	
	NumberFormattingOptions.MinimumIntegralDigits = 2;

	while(arcGISMapController->OAuthAuthenticationConfigurations.FindByPredicate([&](UOAuthAuthenticationConfiguration* oauthAuthenticationConfiguration)
	{
		return oauthAuthenticationConfiguration && oauthAuthenticationConfiguration->Name == FString::Format(TEXT("Configuration {0}"), { FText::AsNumber(index, &NumberFormattingOptions).ToString() });
	}))
	{
		index++;
	}

	return FString::Format(TEXT("Configuration {0}"), { FText::AsNumber(index, &NumberFormattingOptions).ToString() });
}

FString FindNextMappingName(TSharedPtr<AArcGISMapController> arcGISMapController)
{
	uint32 index = 1;

	FNumberFormattingOptions NumberFormattingOptions;

	NumberFormattingOptions.MinimumIntegralDigits = 2;

	while (arcGISMapController->OAuthAuthenticationConfigurationMappings.FindByPredicate([&](FOAuthAuthenticationConfigurationMapping& oauthAuthenticationConfigurationMappings)
	{
		return oauthAuthenticationConfigurationMappings.Name == FString::Format(TEXT("Mapping {0}"), { FText::AsNumber(index, &NumberFormattingOptions).ToString() });
	}))
	{
		index++;
	}

	return FString::Format(TEXT("Mapping {0}"), { FText::AsNumber(index, &NumberFormattingOptions).ToString() });
}

void UpdateMappings(TSharedPtr<AArcGISMapController> arcGISMapController)
{
	for (auto& oauthAuthenticationConfigurationMapping : arcGISMapController->OAuthAuthenticationConfigurationMappings)
	{
		if (!arcGISMapController->OAuthAuthenticationConfigurations.Contains(oauthAuthenticationConfigurationMapping.OAuthAuthenticationConfiguration.Get()))
		{
			oauthAuthenticationConfigurationMapping.OAuthAuthenticationConfiguration.Reset();
		}
	}
}
} // namespace

void FDetailsPanelAuthentication::CreateStyle(IDetailLayoutBuilder& Builder, TSharedPtr<AArcGISMapController> arcGISMapController)
{
	constexpr const char* AUTHENTICATION = "AUTHENTICATION";

	ArcGISMapController = arcGISMapController;

	auto& categoryBuilder = Builder.EditCategory(AUTHENTICATION);

	categoryBuilder.InitiallyCollapsed(true);

	auto OAuthAuthenticationConfigurationsProperty =
			Builder.GetProperty(GET_MEMBER_NAME_CHECKED(AArcGISMapController, OAuthAuthenticationConfigurations));

	auto OAuthAuthenticationConfigurationMappingsProperty =
			Builder.GetProperty(GET_MEMBER_NAME_CHECKED(AArcGISMapController, OAuthAuthenticationConfigurationMappings));

	DetailsPanelUtilities::CreateCategorySpace(categoryBuilder, AUTHENTICATION);
	{
		categoryBuilder.AddCustomRow(FText::FromString(AUTHENTICATION))
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.FillWidth(1.0f)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("API Key")))
			]
			+SHorizontalBox::Slot()
			.FillWidth(2.0f)
			[
				SNew(SEditableTextBox)
				.HintText(FText::FromString("Enter API Key"))
				.Text_Lambda([this]()
				{
					return FText::FromString(ArcGISMapController->APIKey);
				})
				.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type CommitType)
				{
					ArcGISMapController->APIKey = NewText.ToString().TrimStartAndEnd();
				})
				.Padding(TextBoxPadding)
				.Style(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
			]
		];
	}
	DetailsPanelUtilities::CreateCategorySpace(categoryBuilder, AUTHENTICATION);
	{
		categoryBuilder.AddCustomRow(FText::FromString(AUTHENTICATION))
		[
			SNew(SArcGISMapsSDKArray, OAuthAuthenticationConfigurationsProperty)
			.Title(FText::FromString(TEXT("OAuth Configurations")))
			.OnElementAdded_Lambda([this](TSharedRef<IPropertyHandle> elementPropertyHandle)
			{
				auto object = NewObject<UOAuthAuthenticationConfiguration>(ArcGISMapController.Get(), UOAuthAuthenticationConfiguration::StaticClass(), NAME_None, RF_Public | RF_Transactional);

				object->Name = FindNextConfigurationName(ArcGISMapController);

				check(elementPropertyHandle->SetValue(object) == FPropertyAccess::Success);
			})
			.OnElementsRemoved_Lambda([this]()
			{
				UpdateMappings(ArcGISMapController);
			})
			.OnGenerateWidget_Lambda([](TSharedRef<IPropertyHandle> elementPropertyHandle) -> TSharedRef<SWidget>
			{
				UObject* data;

				check(elementPropertyHandle->GetValue(data) == FPropertyAccess::Success);

				if (data)
				{
					auto entity = Cast<UOAuthAuthenticationConfiguration>(data);

					return SNew(SOAuthAuthenticationConfiguration, entity);
				}
				else
				{
					return SNew(STextBlock).Text(FText::FromString("Invalid"));
				}
			})
		];
	}
	DetailsPanelUtilities::CreateCategorySpace(categoryBuilder, AUTHENTICATION);
	{
		categoryBuilder.AddCustomRow(FText::FromString(AUTHENTICATION))
		[
			SNew(SArcGISMapsSDKArray, OAuthAuthenticationConfigurationMappingsProperty)
			.Title(FText::FromString(TEXT("Configuration Mappings")))
			.OnElementAdded_Lambda([this](TSharedRef<IPropertyHandle> elementPropertyHandle)
			{
				void* data;

				check(elementPropertyHandle->GetValueData(data) == FPropertyAccess::Success);

				auto entity = reinterpret_cast<FOAuthAuthenticationConfigurationMapping*>(data);

				entity->Name = FindNextMappingName(ArcGISMapController);
			})
			.OnGenerateWidget_Lambda([this](TSharedRef<IPropertyHandle> elementPropertyHandle)
			{
				void* data;

				check(elementPropertyHandle->GetValueData(data) == FPropertyAccess::Success);

				auto entity = reinterpret_cast<FOAuthAuthenticationConfigurationMapping*>(data);

				return SNew(SOAuthAuthenticationConfigurationMapping, entity, &ArcGISMapController->OAuthAuthenticationConfigurations);
			})
		];
	}
	DetailsPanelUtilities::CreateCategorySpace(categoryBuilder, AUTHENTICATION);
}
