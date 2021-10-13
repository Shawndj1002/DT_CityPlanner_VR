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

#include "CoreMinimal.h"
#include "DetailCategoryBuilder.h"
#include "IDetailCustomization.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SSlider.h"

// ArcGISSamples

#include "ArcGISMapController.h"

namespace DetailsPanelUtilities
{
	typedef void (AArcGISMapController::*SetMapControllerDouble)(double i);
	
	typedef struct SliderTextBox
	{
	public:
		SliderTextBox(TSharedPtr<AArcGISMapController> Controller);

		TSharedPtr<AArcGISMapController> MapController;
		TSharedPtr<SSlider> Slider;
		TSharedPtr<SEditableTextBox> TextBox;

		void SetSlider(float Input);
		void SetTextBox(float Input);
		void SetTextBox(int32 Input);
	} SliderTextBox;
	
	// Utility
	
	FText ConvertFloatToFText(float Value);
	FText ConvertValueToFText(float Value);
	FText ConvertValueToFText(int32 Value);
	
	
	void CreateCategorySpace(IDetailCategoryBuilder& Builder, const FString& Label);
	void SwapButtonColors(TSharedPtr<SButton> Button1, TSharedPtr<SButton> Button2);
	void AddLinkedButtonRowToCategory(IDetailLayoutBuilder& Builder, FName Category, TSharedRef<SButton> Button1, TSharedRef<SButton> Button2);
	void SetInputField(const FText& Text, ETextCommit::Type Type, SetMapControllerDouble SetValue, TSharedPtr<AArcGISMapController> MapController);

	// Panel Utils
	
	class ARCGISMAPSSDKEDITOR_API PanelUtils : public TSharedFromThis<PanelUtils>
	{
	public:
		void CreateInputFieldCategoryRow(IDetailCategoryBuilder& Category,
										 const char* CategoryName,
										 const char* Label,
										 float Input,
										 SetMapControllerDouble SetValue,
										 TSharedPtr<SEditableTextBox>& TextBox,
										 TSharedPtr<AArcGISMapController> MapController,
										 bool bUseIntegers = false);

		void CreateSliderBarRow(IDetailCategoryBuilder& Category,
									   const char* CategoryName,
									   const char* Label,
									   const char* ToolTipText,
									   float Input,
									   float MaxSliderValue,
									   SetMapControllerDouble SetValue,
									   SliderTextBox* ComboBox,
									   bool bUseIntegers = false);
		void SetInputField(const FText& Text, ETextCommit::Type Type, SetMapControllerDouble SetValue, TSharedPtr<AArcGISMapController> MapController);
		void SetComboSlider(float Input, SetMapControllerDouble SetValue, SliderTextBox* ComboBox, bool bUseIntegers);
		void SetComboInput(const FText& Text, ETextCommit::Type Type, SetMapControllerDouble SetValue, SliderTextBox* ComboBox);
		FReply SetInputKeyDown(const FGeometry& MyGeometry, const FCharacterEvent& InKeyEvent);
		FReply SetIntegerInputKeyDown(const FGeometry& MyGeometry, const FCharacterEvent& InKeyEvent);
	};
}
