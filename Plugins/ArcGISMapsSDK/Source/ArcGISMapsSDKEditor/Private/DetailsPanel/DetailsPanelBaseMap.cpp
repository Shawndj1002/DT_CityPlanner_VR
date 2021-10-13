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
#include "DetailsPanelBaseMap.h"

// Unreal

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Slate.h"
#include "SlateCore.h"
#include "Widgets/Layout/SScaleBox.h"

// ArcGISMapsSDKEditor

#include "MapControllerStyles.h"

constexpr const char* BASE_MAP = "BASE MAP";

static const TSharedPtr<FString> WORLDIMAGERY = MakeShareable(new FString("World Imagery"));
static const TSharedPtr<FString> STREETS = MakeShareable(new FString("Streets"));
static const TSharedPtr<FString> TOPOGRAPHIC = MakeShareable(new FString("Topographic"));
static const TSharedPtr<FString> NATGEO = MakeShareable(new FString("National Geographic"));
static const TSharedPtr<FString> OCEANS = MakeShareable(new FString("Oceans"));
static const TSharedPtr<FString> LIGHTGRAYCANVAS = MakeShareable(new FString("Light Gray Canvas"));
static const TSharedPtr<FString> DARKGRAYCANVAS = MakeShareable(new FString("Dark Gray Canvas")); 

// Public

void DetailsPanelBaseMap::CreateStyle(IDetailLayoutBuilder& Builder, TSharedPtr<AArcGISMapController> Controller)
{
	MapController = Controller;
	IDetailCategoryBuilder& BaseMapCategory = Builder.EditCategory(BASE_MAP);

	DetailsPanelUtilities::CreateCategorySpace(BaseMapCategory, BASE_MAP);
		
	BaseMapCategory.AddCustomRow(FText::FromString(BASE_MAP))
	.NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Select Base Map"))
	]
	.ValueContent()
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	[
		SNew(STextComboBox)
		.OptionsSource(&BaseMapOptions)
		.InitiallySelectedItem(GetBaseMap())
		.OnSelectionChanged(this, &DetailsPanelBaseMap::SetBaseMap)
		.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("ComboBoxButton"))
		.ContentPadding(FMargin(0, 0))
		.ColorAndOpacity(FSlateColor(ColorFontInner))
	];
	
	BaseMapCategory.AddCustomRow(FText::FromString(BASE_MAP))
	[
		SNew(SSpacer)
		.Size(FVector2D(0, 10))
	];

	BaseMapCategory.AddCustomRow(FText::FromString(BASE_MAP))
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(EVerticalAlignment::VAlign_Top)
		.HAlign(EHorizontalAlignment::HAlign_Left)
		[
			SNew(SBox)
			.HAlign(EHorizontalAlignment::HAlign_Left)
			.WidthOverride(22)
			[
				SNew(SCheckBox)
				.IsChecked(MapController->GetElevation())
				.OnCheckStateChanged(this, &DetailsPanelBaseMap::SetElevation)
				.Style(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FCheckBoxStyle>("CheckBox"))
			]
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			SNew(STextBlock)
			.Justification(ETextJustify::Left)
			.Margin(FMargin(12, 4))
			.Text(FText::FromString("Terrain Elevation"))
		]
		+ SHorizontalBox::Slot()
		.Padding(0, 0, 0, 14)
		.HAlign(EHorizontalAlignment::HAlign_Right)
		[
			SNew(SBox)
			.HAlign(EHorizontalAlignment::HAlign_Right)
			[
				SNew(SScaleBox)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					.VAlign(EVerticalAlignment::VAlign_Fill)
					[
						SNew(SImage)
						.Image(new FSlateBrush())
						.ColorAndOpacity(FSlateColor(ColorDefaultBlue))
					]
					+ SOverlay::Slot()
					.Padding(1.5)
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					.VAlign(EVerticalAlignment::VAlign_Fill)
					[
						SAssignNew(BaseMapImage, SImage)
						.Image(GetBaseMapImage())
					]
				]
			]
		]
	];
}

// Base Map

TArray<TSharedPtr<FString>> DetailsPanelBaseMap::BaseMapOptions = TArray<TSharedPtr<FString>>
{
	WORLDIMAGERY,
	STREETS,
	TOPOGRAPHIC,
	NATGEO,
	OCEANS,
	LIGHTGRAYCANVAS,
	DARKGRAYCANVAS
};

const FSlateBrush* DetailsPanelBaseMap::GetBaseMapImage()
{
	auto BaseMap = MapController->GetBaseMap();
	
	switch (BaseMap)
	{
		case WorldImagery:
			return MapControllerStyles::GetBaseMapStyleSet()->GetBrush("Imagery");
		case Streets:
			return MapControllerStyles::GetBaseMapStyleSet()->GetBrush("Streets");
		case Topographic:
			return MapControllerStyles::GetBaseMapStyleSet()->GetBrush("Topographic");
		case NationalGeographic:
			return MapControllerStyles::GetBaseMapStyleSet()->GetBrush("NatGeo");
		case Oceans:
			return MapControllerStyles::GetBaseMapStyleSet()->GetBrush("Oceans");
		case LightGrayCanvas:
			return MapControllerStyles::GetBaseMapStyleSet()->GetBrush("LightGrayCanvas");
		case DarkGrayCanvas:
			return MapControllerStyles::GetBaseMapStyleSet()->GetBrush("DarkGrayCanvas");
		default:
			return nullptr;
	}
}

TSharedPtr<FString> DetailsPanelBaseMap::GetBaseMap()
{
	auto BaseMap = MapController->GetBaseMap();
	
	switch (BaseMap)
	{
		case WorldImagery:
			return WORLDIMAGERY;
		case Streets:
			return STREETS;
		case Topographic:
			return TOPOGRAPHIC;
		case NationalGeographic:
			return NATGEO;
		case Oceans:
			return OCEANS;
		case LightGrayCanvas:
			return LIGHTGRAYCANVAS;
		case DarkGrayCanvas:
			return DARKGRAYCANVAS;
		default:
			return WORLDIMAGERY;
	}
}

void DetailsPanelBaseMap::SetBaseMap(TSharedPtr<FString> Item, ESelectInfo::Type Type)
{
	if (Item == WORLDIMAGERY)
	{
		MapController->SetBaseMap(EBaseMaps::WorldImagery);
	}
	else if (Item == STREETS)
	{
		MapController->SetBaseMap(EBaseMaps::Streets);
	}
	else if (Item == TOPOGRAPHIC)
	{
		MapController->SetBaseMap(EBaseMaps::Topographic);
	}
	else if (Item == NATGEO)
	{
		MapController->SetBaseMap(EBaseMaps::NationalGeographic);
	}
	else if (Item == OCEANS)
	{
		MapController->SetBaseMap(EBaseMaps::Oceans);
	}
	else if (Item == LIGHTGRAYCANVAS)
	{
		MapController->SetBaseMap(EBaseMaps::LightGrayCanvas);
	}
	else if (Item == DARKGRAYCANVAS)
	{
		MapController->SetBaseMap(EBaseMaps::DarkGrayCanvas);
	}

	BaseMapImage.Get()->SetImage(GetBaseMapImage());
	MapController->MarkPackageDirty();
}

// Elevation

void DetailsPanelBaseMap::SetElevation(ECheckBoxState State) const
{
	switch (State)
	{
		case ECheckBoxState::Unchecked:
			MapController->SetElevation(false);
			break;
		case ECheckBoxState::Checked:
			MapController->SetElevation(true);
			break;
		case ECheckBoxState::Undetermined:
			MapController->SetElevation(false);
			break;
		default:
			MapController->SetElevation(false);
			break;
	}
	MapController->MarkPackageDirty();
}
