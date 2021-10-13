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

// ArcGISMapsSDKEditor

#include "DetailsPanelUtilities.h"

using namespace DetailsPanelUtilities;

class ARCGISMAPSSDKEDITOR_API DetailsPanelMapExtent : public TSharedFromThis<DetailsPanelMapExtent> 
{
public:
	void CreateStyle(IDetailLayoutBuilder& Builder, TSharedPtr<AArcGISMapController> Controller);
	void SetCategoryVisibility(bool IsVisible);

private:
	struct MapExtentFields
	{
	public:
		TSharedPtr<SEditableTextBox> LatitudeTextBox;
		TSharedPtr<SEditableTextBox> LongitudeTextBox;
		TSharedPtr<SliderTextBox> AltitudeSliderTextBox;
		TSharedPtr<SEditableTextBox> WidthTextBox;
		TSharedPtr<SEditableTextBox> LengthTextBox;
		TSharedPtr<STextComboBox> ShapeComboBox;
	};

	TSharedPtr<AArcGISMapController> MapController;
	
	void SetInputField(const FText& Text, ETextCommit::Type Type, SetMapControllerDouble SetValue);

	// Map Extent

	static TArray<TSharedPtr<FString>> ExtentShapeOptions;
	IDetailCategoryBuilder* MapExtentCategory;
	MapExtentFields* ExtentFields;
	TSharedPtr<PanelUtils> Utils;

	TSharedPtr<FString> GetExtentShape();
	void SetExtentShape(TSharedPtr<FString> Selection, ESelectInfo::Type Type);
	FReply ResetExtent(MapExtentFields* Fields);
};
