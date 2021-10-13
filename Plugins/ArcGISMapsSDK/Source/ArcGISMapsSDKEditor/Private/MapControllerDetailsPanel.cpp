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
#include "MapControllerDetailsPanel.h"

// Unreal

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Slate.h"
#include "SlateCore.h"
#include "Widgets/Layout/SScaleBox.h"

// ArcGISMapsSDKEditor

#include "MapControllerStyles.h"

// Macros

#define LOCTEXT_NAMESPACE "MapControllerDetailsPanelCpp"

// Const

constexpr const char* LOGO = "ARCGIS MAPS SDK";

// Init

TSharedRef<IDetailCustomization> MapControllerDetailsPanel::MakeInstance()
{
	return MakeShared<MapControllerDetailsPanel>();
}

bool MapControllerDetailsPanel::InitializeMapController(const IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> SelectedObjects;
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);
	if (SelectedObjects.Num() != 1)
	{
		return false;
	}
	MapController = MakeShareable(Cast<AArcGISMapController>(SelectedObjects[0].Get()));
	return true;
}

void MapControllerDetailsPanel::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	if (!InitializeMapController(DetailBuilder))
	{
		return;
	}

	// Default Actor Categories
	DetailBuilder.HideCategory("Cooking");
	DetailBuilder.HideCategory("LOD");
	DetailBuilder.HideCategory("Collision");
	DetailBuilder.HideCategory("Rendering");
	DetailBuilder.HideCategory("Replication");
	DetailBuilder.HideCategory("Input");
	DetailBuilder.HideCategory("Actor");

	// Map Controller Category
	DetailBuilder.HideCategory("MapControllerCategory");

	// Styles
	StyleHeaderLogo(DetailBuilder);
	
	AddData = MakeShareable(new DetailsPanelAddData());
	Authentication = MakeShareable(new FDetailsPanelAuthentication());
	BaseMap = MakeShareable(new DetailsPanelBaseMap());
	CamView = MakeShareable(new DetailsPanelCameraView());
	Layers = MakeShareable(new DetailsPanelLayers());
	MapExtent = MakeShareable(new DetailsPanelMapExtent());
	
	CamView->CreateStyle(DetailBuilder, MapController, MapExtent);
	MapExtent->CreateStyle(DetailBuilder, MapController);
	BaseMap->CreateStyle(DetailBuilder, MapController);
	AddData->CreateStyle(DetailBuilder, MapController, Layers);
	Layers->CreateStyle(DetailBuilder, MapController);
	Authentication->CreateStyle(DetailBuilder, MapController);

	CamView->GetViewMode();
}

void MapControllerDetailsPanel::StyleHeaderLogo(IDetailLayoutBuilder& Builder)
{
	IDetailCategoryBuilder& ApiSettingsCategory = Builder.EditCategory(LOGO);
	ApiSettingsCategory.AddCustomRow(FText::FromString(LOGO))
	[
		SNew(SScaleBox)
		[
			SNew(SImage)
			.Image(MapControllerStyles::GetCommonStyleSet()->GetBrush("Logo"))
		]
	];
}

#undef LOCTEXT_NAMESPACE
