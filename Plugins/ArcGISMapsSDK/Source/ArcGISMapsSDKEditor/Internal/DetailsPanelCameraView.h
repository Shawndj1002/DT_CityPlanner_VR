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

#include "DetailsPanelMapExtent.h"
#include "DetailsPanelUtilities.h"

using namespace DetailsPanelUtilities;

class ARCGISMAPSSDKEDITOR_API DetailsPanelCameraView : public TSharedFromThis<DetailsPanelCameraView>
{
public:
	void CreateStyle(IDetailLayoutBuilder& Builder, TSharedPtr<AArcGISMapController> Controller, TSharedPtr<DetailsPanelMapExtent> Extent);
	void GetViewMode() const;

private:
	TSharedPtr<AArcGISMapController> MapController;

	TSharedPtr<DetailsPanelMapExtent> MapExtent;
	TSharedPtr<PanelUtils> Utils;

	TSharedPtr<SButton> GlobalButton;
	TSharedPtr<SButton> LocalButton;

	TSharedRef<SButton> CreateButton(FText ButtonText, FOnClicked::TSPMethodDelegate_Const<DetailsPanelCameraView>::FMethodPtr ButtonFunc);
	FReply SetViewModeToGlobal() const;
	FReply SetViewModeToLocal() const;

	void CreateViewModeSection(IDetailLayoutBuilder& Builder);
	void CreateCameraLocationSection(IDetailLayoutBuilder& Builder);
	void CreateOriginLocationSection(IDetailLayoutBuilder& Builder);
};
