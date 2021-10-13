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
#include "Styling/SlateStyle.h"
#include "Styling/SlateTypes.h"

const FLinearColor ColorTransparent = FLinearColor(0, 0, 0, 0);
const FLinearColor ColorDefaultBlue = FLinearColor(0.0f, 0.162029f, 0.467784f);
const FLinearColor ColorDefaultGray = FLinearColor(0.171441f, 0.168269f, 0.181164f);
const FLinearColor ColorFontInner = FLinearColor(0.9f, 0.9f, 0.9f);

const FMargin TextBoxPadding(12, 3);
const FMargin ButtonPadding(0.0f, 12.0f);
const FMargin TextPadding(10.0f, 5.0f);
const FMargin RowPadding(0, 2);

class MapControllerStyles
{
public:
	static void Initialize();
	static void Shutdown();

	static const FSlateStyleSet* GetBaseMapStyleSet();
	static const FSlateStyleSet* GetCommonStyleSet();

private:
	static TSharedPtr<FSlateStyleSet> BaseMapStyleSet;
	static TSharedPtr<FSlateStyleSet> CommonStyleSet;
	
	static void UnregisterStyle(TSharedPtr<FSlateStyleSet> Style);

	static TSharedRef<FSlateStyleSet> CreateBaseMapStyleSet();
	static TSharedRef<FSlateStyleSet> CreateCommonStyleSet();
};
