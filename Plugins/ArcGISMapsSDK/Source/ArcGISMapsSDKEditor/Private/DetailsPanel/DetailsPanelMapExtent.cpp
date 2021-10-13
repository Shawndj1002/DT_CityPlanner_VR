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
#include "DetailsPanelMapExtent.h"

// Unreal

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Slate.h"

// ArcGISMapsSDKEditor

#include "MapControllerStyles.h"

constexpr const char* MAP_EXTENT = "MAP EXTENT";

// Public

void DetailsPanelMapExtent::CreateStyle(IDetailLayoutBuilder& Builder, TSharedPtr<AArcGISMapController> Controller)
{
	MapController = Controller;

	FText CatName = FText::FromString(MAP_EXTENT);
	IDetailCategoryBuilder& MapExtentCat = Builder.EditCategory(MAP_EXTENT);

	ExtentFields = new MapExtentFields();
	ExtentFields->AltitudeSliderTextBox = MakeShareable(new SliderTextBox(MapController));
	
	DetailsPanelUtilities::CreateCategorySpace(MapExtentCat, MAP_EXTENT);
	
	Utils = MakeShareable(new PanelUtils());

	Utils->CreateInputFieldCategoryRow(MapExtentCat, MAP_EXTENT, "Latitude", MapController->GetExtentLatitude(), &AArcGISMapController::SetExtentLatitude, ExtentFields->LatitudeTextBox, MapController);
	
	Utils->CreateInputFieldCategoryRow(MapExtentCat, MAP_EXTENT, "Longitude", MapController->GetExtentLongitude(), &AArcGISMapController::SetExtentLongitude, ExtentFields->LongitudeTextBox, MapController);
	
	Utils->CreateSliderBarRow(MapExtentCat, MAP_EXTENT, "Altitude", "Altitude is in meters", MapController->GetExtentAltitude(), 50000,
							  &AArcGISMapController::SetExtentAltitude, ExtentFields->AltitudeSliderTextBox.Get());

	MapExtentCat.AddCustomRow(CatName)
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Shape"))
	]
	.ValueContent()
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	[
		SAssignNew(ExtentFields->ShapeComboBox, STextComboBox)
		.OptionsSource(&ExtentShapeOptions)
		.InitiallySelectedItem(GetExtentShape())
		.OnSelectionChanged(this, &DetailsPanelMapExtent::SetExtentShape)
		.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("ComboBoxButton"))
		.ColorAndOpacity(FSlateColor(ColorFontInner))
	];

	MapExtentCat.AddCustomRow(CatName)
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Shape Dimensions"))
		.ToolTipText(FText::FromString("Shape Dimensions are in meters"))
	]
	.ValueContent()
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	[
		SNew(SHorizontalBox) 
		+ SHorizontalBox::Slot()
		[
			SAssignNew(ExtentFields->WidthTextBox, SEditableTextBox)
			.Text(ConvertFloatToFText(MapController->GetExtentWidth()))
			.Padding(TextBoxPadding)
			.Style(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
			.OnTextCommitted(this, &DetailsPanelMapExtent::SetInputField, &AArcGISMapController::SetExtentWidth)
		]
		+ SHorizontalBox::Slot()
		.Padding(4, 0, 0, 0)
		[
			SAssignNew(ExtentFields->LengthTextBox, SEditableTextBox)
			.Text(ConvertFloatToFText(MapController->GetExtentLength()))
			.Padding(TextBoxPadding)
			.Style(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
			.OnTextCommitted(this, &DetailsPanelMapExtent::SetInputField, &AArcGISMapController::SetExtentLength)
		]
	];

	MapExtentCat.AddCustomRow(CatName)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			SNew(SSpacer)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(0.325)
		.Padding(0, 12)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			SNew(SButton)
			.Content()
			[
				SNew(STextBlock)
				.Text(FText::FromString("Reset Extent"))
				.Justification(ETextJustify::Center)
				.Margin(FMargin(0, 3))
				.ColorAndOpacity(ColorFontInner)
			]
			.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("Button"))
			.ButtonColorAndOpacity(FSlateColor(ColorDefaultGray))
			.OnClicked(this, &DetailsPanelMapExtent::ResetExtent, ExtentFields)
		]
	];

	MapExtentCategory = &MapExtentCat;
	MapExtentCategory->SetCategoryVisibility(false);
	SetExtentShape(GetExtentShape(), ESelectInfo::Type::Direct);
}

void DetailsPanelMapExtent::SetCategoryVisibility(bool IsVisible)
{
	MapExtentCategory->SetCategoryVisibility(IsVisible);
}

void DetailsPanelMapExtent::SetInputField(const FText& Text, ETextCommit::Type Type, SetMapControllerDouble SetValue)
{
	DetailsPanelUtilities::SetInputField(Text, Type, SetValue, MapController);
}

// Map Extent

TArray<TSharedPtr<FString>> DetailsPanelMapExtent::ExtentShapeOptions = TArray<TSharedPtr<FString>>
{
	TSharedPtr<FString>(new FString("Square")),
	TSharedPtr<FString>(new FString("Rectangle")),
	TSharedPtr<FString>(new FString("Circle")),
};

TSharedPtr<FString> DetailsPanelMapExtent::GetExtentShape()
{
	auto Shape = MapController->GetExtentShape();
	auto Square = ExtentShapeOptions[0];
	auto Rectangle = ExtentShapeOptions[1];
	auto Circle = ExtentShapeOptions[2];

	switch (Shape)
	{
		case EExtentShapes::Square:
			return Square; 
		case EExtentShapes::Rectangle:
			return Rectangle;
		case EExtentShapes::Circle:
			return Circle;
		default:
			return nullptr;
	}
}

void DetailsPanelMapExtent::SetExtentShape(TSharedPtr<FString> Selection, ESelectInfo::Type Type)
{
	EExtentShapes Shape = EExtentShapes::Square;
	auto Square = ExtentShapeOptions[0];
	auto Rectangle = ExtentShapeOptions[1];
	auto Circle = ExtentShapeOptions[2];

	if (Selection == Square)
	{
		Shape = EExtentShapes::Square;
		ExtentFields->LengthTextBox->SetVisibility(EVisibility::Collapsed);
	}
	else if (Selection == Rectangle)
	{
		Shape = EExtentShapes::Rectangle;
		ExtentFields->LengthTextBox->SetVisibility(EVisibility::Visible);
	}
	else if (Selection == Circle)
	{
		Shape = EExtentShapes::Circle;
		ExtentFields->LengthTextBox->SetVisibility(EVisibility::Collapsed);
	}
	
	MapController->SetExtentShape(Shape);
	MapController->MarkPackageDirty();
}

FReply DetailsPanelMapExtent::ResetExtent(MapExtentFields* Fields)
{
	Fields->LatitudeTextBox->SetText(ConvertFloatToFText(0));
	Fields->LongitudeTextBox->SetText(ConvertFloatToFText(0));
	Fields->WidthTextBox->SetText(ConvertFloatToFText(0));
	Fields->LengthTextBox->SetText(ConvertFloatToFText(0));
	Fields->AltitudeSliderTextBox->SetSlider(0);
	Fields->AltitudeSliderTextBox->SetTextBox(0);
	Fields->ShapeComboBox->SetSelectedItem(ExtentShapeOptions[0]);

	MapController->SetExtentLatitude(0);
	MapController->SetExtentLongitude(0);
	MapController->SetExtentAltitude(0);
	MapController->SetExtentShape(EExtentShapes::Square);
	MapController->SetExtentWidth(0);
	MapController->SetExtentLength(0);

	MapController->MarkPackageDirty();
	return FReply::Handled();
}
