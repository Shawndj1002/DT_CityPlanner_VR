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
#include "MapControllerStyles.h"

// Unreal

#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IPluginManager.h"
#include "Slate/SlateGameResources.h"
#include "Styling/SlateStyleRegistry.h"

// ArcGISMapsSDKEditor

#include "ArcGISMapsSDKEditorModule.h"

#define JPGImageBrush(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".jpg")), __VA_ARGS__)
#define PNGBorderBrush(RelativePath, ...) FSlateBorderBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define PNGImageBrush(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

const FVector2D BaseMapMargin(164, 108);
const FVector2D LogoMargin(370, 114);
const FVector2D SmallIconMargin(15, 15);
const FMargin TextBoxMargin(1.0f / 16.0f);

const FVector2D CheckBoxSize(22.0f, 22.0f);
const FVector2D BlankImageSize(16.0f, 16.0f);

TSharedPtr<FSlateStyleSet> MapControllerStyles::BaseMapStyleSet = nullptr;
TSharedPtr<FSlateStyleSet> MapControllerStyles::CommonStyleSet = nullptr;

namespace
{
void CreateButtonStyles(TSharedPtr<FSlateStyleSet> StyleSet)
{
	const FButtonStyle ButtonStyle = FButtonStyle();
	{
		StyleSet->Set("Button", ButtonStyle);
	}
}

void CreateCheckBoxStyles(TSharedPtr<FSlateStyleSet> StyleSet)
{
	auto CheckBoxBlue = PNGImageBrush(TEXT("BLANK_IMAGE"), CheckBoxSize, ColorDefaultBlue);
	auto CheckBoxGray = PNGImageBrush(TEXT("BLANK_IMAGE"), CheckBoxSize, ColorDefaultGray);

	const FCheckBoxStyle CheckBoxStyle = FCheckBoxStyle()
		.SetCheckedImage(CheckBoxBlue)
		.SetCheckedHoveredImage(CheckBoxBlue)
		.SetCheckedPressedImage(CheckBoxBlue)
		.SetUncheckedImage(CheckBoxGray)
		.SetUncheckedHoveredImage(CheckBoxGray)
		.SetUncheckedPressedImage(CheckBoxGray);
	{
		StyleSet->Set("CheckBox", CheckBoxStyle);
	}
}

void CreateComboBoxStyles(TSharedPtr<FSlateStyleSet> StyleSet)
{
	auto BlueFill = PNGImageBrush(TEXT("BLANK_IMAGE"), CheckBoxSize);
	{
		BlueFill.TintColor = FSlateColor(ColorDefaultBlue);
	}

	auto ComboBorderBrush = PNGBorderBrush(TEXT("BLANK_IMAGE"), TextBoxMargin);
	{
		ComboBorderBrush.TintColor = FSlateColor(ColorDefaultBlue);
	}

	const FButtonStyle ComboBoxButtonStyle = FButtonStyle()
		.SetNormalPadding(TextBoxPadding)
		.SetPressedPadding(TextBoxPadding)
		.SetNormal(ComboBorderBrush)
		.SetHovered(BlueFill)
		.SetPressed(BlueFill);
	{
		StyleSet->Set("ComboBoxButton", ComboBoxButtonStyle);
	}
}

void CreateEditableTextBoxStyles(TSharedPtr<FSlateStyleSet> StyleSet)
{
	const FEditableTextBoxStyle SolidEditableTextBoxStyle = FEditableTextBoxStyle()
		.SetBackgroundColor(ColorDefaultBlue)
		.SetForegroundColor(ColorFontInner);
	{
		StyleSet->Set("SolidEditableTextBox", SolidEditableTextBoxStyle);
	}

	const FEditableTextBoxStyle NormalEditableTextBoxStyle = FEditableTextBoxStyle(SolidEditableTextBoxStyle)
		.SetBackgroundImageNormal(PNGBorderBrush(TEXT("BLANK_IMAGE"), TextBoxMargin));
	{
		StyleSet->Set("NormalEditableTextBox", NormalEditableTextBoxStyle);
	}
}
} // namespace

TSharedRef<FSlateStyleSet> MapControllerStyles::CreateBaseMapStyleSet()
{
	TSharedRef<FSlateStyleSet> StyleSet = MakeShareable(new FSlateStyleSet("BaseMapStyles"));

	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin("ArcGISMapsSDK")->GetBaseDir() / TEXT("Resources/BaseMaps"));

	StyleSet->Set("DarkGrayCanvas", new JPGImageBrush(TEXT("DARK_GRAY_CANVAS"), BaseMapMargin));
	StyleSet->Set("LightGrayCanvas", new JPGImageBrush(TEXT("LIGHT_GRAY_CANVAS"), BaseMapMargin));
	StyleSet->Set("Imagery", new JPGImageBrush(TEXT("IMAGERY"), BaseMapMargin));
	StyleSet->Set("NatGeo", new JPGImageBrush(TEXT("NATGEO"), BaseMapMargin));
	StyleSet->Set("Oceans", new JPGImageBrush(TEXT("OCEANS"), BaseMapMargin));
	StyleSet->Set("Streets", new JPGImageBrush(TEXT("STREETS"), BaseMapMargin));
	StyleSet->Set("Topographic", new JPGImageBrush(TEXT("TOPOGRAPHIC"), BaseMapMargin));

	return StyleSet;
}

TSharedRef<FSlateStyleSet> MapControllerStyles::CreateCommonStyleSet()
{
	TSharedRef<FSlateStyleSet> StyleSet = MakeShareable(new FSlateStyleSet("MapControllerStyles"));

	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin("ArcGISMapsSDK")->GetBaseDir() / TEXT("Resources"));

	StyleSet->Set("Logo", new PNGImageBrush(TEXT("ESRI_LOGO"), LogoMargin));

	CreateButtonStyles(StyleSet);

	CreateComboBoxStyles(StyleSet);

	CreateCheckBoxStyles(StyleSet);

	CreateEditableTextBoxStyles(StyleSet);

	StyleSet->Set("Visible", new PNGImageBrush(TEXT("LayerIcons/ICON_VISIBILITY"), SmallIconMargin));
	StyleSet->Set("Delete", new PNGImageBrush(TEXT("LayerIcons/ICON_DELETE"), SmallIconMargin));
	StyleSet->Set("MoveUp", new PNGImageBrush(TEXT("LayerIcons/ICON_MOVE_UP"), SmallIconMargin));
	StyleSet->Set("MoveDown", new PNGImageBrush(TEXT("LayerIcons/ICON_MOVE_DOWN"), SmallIconMargin));
	StyleSet->Set("Clipboard", new PNGImageBrush(TEXT("LayerIcons/ICON_CLIPBOARD"), SmallIconMargin));
	StyleSet->Set("ArrowRight", new PNGImageBrush(TEXT("LayerIcons/ICON_ARROW_RIGHT"), SmallIconMargin));
	StyleSet->Set("ArrowDown", new PNGImageBrush(TEXT("LayerIcons/ICON_ARROW_DOWN"), SmallIconMargin));
	StyleSet->Set("Ellipse", new PNGImageBrush(TEXT("LayerIcons/ICON_ELLIPSE"), SmallIconMargin));

	return StyleSet;
}

const FSlateStyleSet* MapControllerStyles::GetCommonStyleSet()
{
	return CommonStyleSet.Get();
}

const FSlateStyleSet* MapControllerStyles::GetBaseMapStyleSet()
{
	return BaseMapStyleSet.Get();
}

void MapControllerStyles::Initialize()
{
	if (!BaseMapStyleSet.IsValid())
	{
		BaseMapStyleSet = CreateBaseMapStyleSet();
		FSlateStyleRegistry::RegisterSlateStyle(*BaseMapStyleSet);
	}

	if (!CommonStyleSet.IsValid())
	{
		CommonStyleSet = CreateCommonStyleSet();
		FSlateStyleRegistry::RegisterSlateStyle(*CommonStyleSet);
	}

	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

void MapControllerStyles::Shutdown()
{
	UnregisterStyle(CommonStyleSet);
	UnregisterStyle(BaseMapStyleSet);
}

void MapControllerStyles::UnregisterStyle(TSharedPtr<FSlateStyleSet> Style)
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*Style);
	Style.Reset();
}

#undef PNGImageBrush
#undef PNGBorderBrush
#undef JPGImageBrush
