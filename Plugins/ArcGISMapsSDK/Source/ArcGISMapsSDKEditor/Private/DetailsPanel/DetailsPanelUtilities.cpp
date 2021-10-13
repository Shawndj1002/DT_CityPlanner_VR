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
#include "DetailsPanelUtilities.h"

// Unreal

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Slate.h"

// ArcGISMapsSDKEditor

#include "MapControllerStyles.h"

FText DetailsPanelUtilities::ConvertFloatToFText(float Value)
{
	return FText::FromString(FString::SanitizeFloat(Value));
}

FText DetailsPanelUtilities::ConvertValueToFText(float Value)
{
	return ConvertFloatToFText(Value);
}

FText DetailsPanelUtilities::ConvertValueToFText(int32 Value)
{
	return FText::FromString(FString::FromInt(Value));
}

// SliderTextBox

DetailsPanelUtilities::SliderTextBox::SliderTextBox(TSharedPtr<AArcGISMapController> Controller)
{
	MapController = Controller;
}

void DetailsPanelUtilities::SliderTextBox::SetSlider(float Input)
{
	Slider.Get()->SetValue(Input);
}

void DetailsPanelUtilities::SliderTextBox::SetTextBox(float Input)
{
	auto Text = ConvertFloatToFText(Input);
	TextBox.Get()->SetText(Text);
}

void DetailsPanelUtilities::SliderTextBox::SetTextBox(int32 Input)
{
	auto Text = ConvertValueToFText(Input);
	TextBox.Get()->SetText(Text);
}

// Utility

void DetailsPanelUtilities::CreateCategorySpace(IDetailCategoryBuilder& Builder, const FString& Label)
{
	Builder.AddCustomRow(FText::FromString(*Label))
	.NameContent()
	[
		SNew(SSpacer).Size(FVector2D(0, 10))
	]
	.ValueContent()
	[
		SNew(SSpacer).Size(FVector2D(0, 10))
	];
}

void DetailsPanelUtilities::AddLinkedButtonRowToCategory(IDetailLayoutBuilder& Builder, FName Category, TSharedRef<SButton> Button1, TSharedRef<SButton> Button2)
{
	IDetailCategoryBuilder& CategoryBuilder = Builder.EditCategory(Category);
	CategoryBuilder.AddCustomRow(FText::FromName(Category))
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1)
		.Padding(0, 12, 8, 12)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			Button1
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1)
		.Padding(8, 12, 0, 12)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			Button2
		]
	];
}

void DetailsPanelUtilities::SwapButtonColors(TSharedPtr<SButton> Button1, TSharedPtr<SButton> Button2)
{
	Button1->SetBorderBackgroundColor(ColorDefaultBlue);
	Button2->SetBorderBackgroundColor(ColorDefaultGray);
}

void DetailsPanelUtilities::SetInputField(const FText& Text, ETextCommit::Type Type, SetMapControllerDouble SetValue, TSharedPtr<AArcGISMapController> MapController)
{
	double Input = FCString::Atof(*Text.ToString());
	(MapController.Get()->*SetValue)(Input);
	MapController->MarkPackageDirty();
}

// Panels Utils

void DetailsPanelUtilities::PanelUtils::CreateInputFieldCategoryRow(IDetailCategoryBuilder& Category, const char* CategoryName, const char* Label, float Input, SetMapControllerDouble SetValue, TSharedPtr<SEditableTextBox>& TextBox, TSharedPtr<AArcGISMapController> MapController, bool bUseIntegers)
{
	Category.AddCustomRow(FText::FromString(CategoryName))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString(Label))
	]
	.ValueContent()
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	[
		SAssignNew(TextBox, SEditableTextBox)
		.Text(bUseIntegers ? ConvertValueToFText(static_cast<int32>(Input)) : ConvertValueToFText(Input))
		.Padding(TextBoxPadding)
		.Style(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
		.OnKeyCharHandler(this, bUseIntegers ? &PanelUtils::SetIntegerInputKeyDown : &PanelUtils::SetInputKeyDown)
		.OnTextCommitted(this, &PanelUtils::SetInputField, SetValue, MapController)
	];
}

void DetailsPanelUtilities::PanelUtils::CreateSliderBarRow(IDetailCategoryBuilder& Category, const char* CategoryName, const char* Label, const char* ToolTipText, float Input, float MaxSliderValue, SetMapControllerDouble SetValue, SliderTextBox* ComboBox, bool bUseIntegers)
{
	Category.AddCustomRow(FText::FromString(CategoryName))
	.NameContent()
	[
		SNew(STextBlock).Text(FText::FromString(Label))
		.ToolTipText(FText::FromString(ToolTipText))
	]
	.ValueContent()
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	[
		SNew(SHorizontalBox) 
		+ SHorizontalBox::Slot()
		.Padding(0, 0, 1, 0)
		[
			SAssignNew(ComboBox->Slider, SSlider)
			.MaxValue(MaxSliderValue)
			.Value(Input)
			.SliderBarColor(ColorDefaultBlue)
			.OnValueChanged(this, &PanelUtils::SetComboSlider, SetValue, ComboBox, bUseIntegers)
		]
		+ SHorizontalBox::Slot()
		[
			SAssignNew(ComboBox->TextBox, SEditableTextBox)
			.Text(bUseIntegers ? ConvertValueToFText(static_cast<int32>(Input)) : ConvertValueToFText(Input))
			.Padding(TextBoxPadding)
			.Style(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
			.OnKeyCharHandler(this, bUseIntegers ? &PanelUtils::SetIntegerInputKeyDown : &PanelUtils::SetInputKeyDown)
			.OnTextCommitted(this, &PanelUtils::SetComboInput, SetValue, ComboBox)
		]
	];
}

void DetailsPanelUtilities::PanelUtils::SetInputField(const FText& Text, ETextCommit::Type Type, SetMapControllerDouble SetValue, TSharedPtr<AArcGISMapController> MapController)
{
	DetailsPanelUtilities::SetInputField(Text, Type, SetValue, MapController);
}

void DetailsPanelUtilities::PanelUtils::SetComboSlider(float Input, SetMapControllerDouble SetValue, SliderTextBox* ComboBox, bool bUseIntegers)
{
	if (bUseIntegers)
	{
		ComboBox->SetTextBox(static_cast<int32>(Input));
	}
	else
	{
		ComboBox->SetTextBox(Input);
	}
	(ComboBox->MapController.Get()->*SetValue)(Input);
	ComboBox->MapController->MarkPackageDirty();
}

void DetailsPanelUtilities::PanelUtils::SetComboInput(const FText& Text, ETextCommit::Type Type, SetMapControllerDouble SetValue, SliderTextBox* ComboBox)
{
	double Input = FCString::Atof(*Text.ToString());
	ComboBox->SetSlider(Input);
	(ComboBox->MapController.Get()->*SetValue)(Input);
	ComboBox->MapController->MarkPackageDirty();
}

FReply DetailsPanelUtilities::PanelUtils::SetIntegerInputKeyDown(const FGeometry& MyGeometry, const FCharacterEvent& InKeyEvent)
{
	if (InKeyEvent.GetCharacter() == '.')
	{
		return FReply::Handled();
	}
	return SetInputKeyDown(MyGeometry, InKeyEvent);
}

FReply DetailsPanelUtilities::PanelUtils::SetInputKeyDown(const FGeometry& MyGeometry, const FCharacterEvent& InKeyEvent)
{
	if (InKeyEvent.GetCharacter() >= '0' && InKeyEvent.GetCharacter() <= '9' || InKeyEvent.GetCharacter() == '-' || InKeyEvent.GetCharacter() == '.')
	{
		return FReply::Unhandled();
	}
	else if (InKeyEvent.GetCharacter() < ' ')
	{
		return FReply::Unhandled();
	}
	else
	{
		return FReply::Handled();
	}
}
