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
#include "ArcGISToolBarStyle.h"

// Unreal
#include "Framework/Application/SlateApplication.h"
#include "Interfaces/IPluginManager.h"
#include "Slate/SlateGameResources.h"
#include "Styling/SlateStyleRegistry.h"

// ArcGISMapsSDKEditor
#include "ArcGISMapsSDKEditorModule.h"

#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(Style->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)

const FVector2D Icon40x40(40.0f, 40.0f);

TSharedPtr<FSlateStyleSet> FArcGISToolBarStyle::StyleInstance = nullptr;

void FArcGISToolBarStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}

	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

void FArcGISToolBarStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

const ISlateStyle& FArcGISToolBarStyle::Get()
{
	return *StyleInstance;
}

FName FArcGISToolBarStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ArcGISToolBarStyle"));
	return StyleSetName;
}

TSharedRef<FSlateStyleSet> FArcGISToolBarStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("ArcGISToolBarStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("ArcGISMapsSDK")->GetBaseDir() / TEXT("Resources"));
	Style->Set("ArcGISToolBar.PluginAction", new IMAGE_BRUSH(TEXT("Icon128"), Icon40x40));
	return Style;
}

#undef IMAGE_BRUSH
