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

// ArcGISSamples

#include "ArcGISMapController.h"

using namespace DetailsPanelUtilities;

class ARCGISMAPSSDKEDITOR_API DetailsPanelBaseMap : public TSharedFromThis<DetailsPanelBaseMap>
{
public:
	void CreateStyle(IDetailLayoutBuilder& Builder, TSharedPtr<AArcGISMapController> Controller);

private:
	TSharedPtr<AArcGISMapController> MapController;
	
	// Base Map

	static TArray<TSharedPtr<FString>> BaseMapOptions;
	TSharedPtr<SImage> BaseMapImage;

	const FSlateBrush* GetBaseMapImage();
	TSharedPtr<FString> GetBaseMap();
	void SetBaseMap(TSharedPtr<FString> Item, ESelectInfo::Type Type);

	// Elevation

	void SetElevation(ECheckBoxState State) const;
};
