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
#include "DetailsPanelCameraView.h"

// Unreal

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Editor.h"
#include "Slate.h"
#include "SlateCore.h"

// ArcGISMapsSDKEditor

#include "MapControllerStyles.h"

constexpr const char* VIEW_MODE = "VIEW MODE";
constexpr const char* CAM_LOCATION = "CAMERA LOCATION";
constexpr const char* ORIGIN_LOCATION = "ORIGIN LOCATION";

void DetailsPanelCameraView::CreateStyle(IDetailLayoutBuilder& Builder,
										 TSharedPtr<AArcGISMapController> Controller,
										 TSharedPtr<DetailsPanelMapExtent> Extent)
{
	MapController = Controller;
	MapExtent = Extent;
	Utils = MakeShareable(new PanelUtils());

	CreateViewModeSection(Builder);
	CreateOriginLocationSection(Builder);
	CreateCameraLocationSection(Builder);
}

void DetailsPanelCameraView::CreateViewModeSection(IDetailLayoutBuilder& Builder)
{
	GlobalButton = CreateButton(FText::FromString("Global Scene"), &DetailsPanelCameraView::SetViewModeToGlobal);
	LocalButton = CreateButton(FText::FromString("Local Scene"), &DetailsPanelCameraView::SetViewModeToLocal);
	AddLinkedButtonRowToCategory(Builder, VIEW_MODE, GlobalButton.ToSharedRef(), LocalButton.ToSharedRef());
}

void DetailsPanelCameraView::CreateOriginLocationSection(IDetailLayoutBuilder& Builder)
{
	IDetailCategoryBuilder& OriginCategory = Builder.EditCategory(ORIGIN_LOCATION);
	TSharedPtr<SEditableTextBox> OriginLatTextBox;
	TSharedPtr<SEditableTextBox> OriginLonTextBox;

	DetailsPanelUtilities::CreateCategorySpace(OriginCategory, ORIGIN_LOCATION);

	Utils->CreateInputFieldCategoryRow(OriginCategory, ORIGIN_LOCATION, "Latitude", MapController->GetOriginLatitude(),
									   &AArcGISMapController::SetOriginLatitude, OriginLatTextBox, MapController, true);

	Utils->CreateInputFieldCategoryRow(OriginCategory, ORIGIN_LOCATION, "Longitude", MapController->GetOriginLongitude(),
									   &AArcGISMapController::SetOriginLongitude, OriginLonTextBox, MapController, true);

	Utils->CreateSliderBarRow(OriginCategory, ORIGIN_LOCATION, "Altitude", "Altitude is in meters", MapController->GetOriginAltitude(), 50000,
							  &AArcGISMapController::SetOriginAltitude, new SliderTextBox(MapController), true);

	DetailsPanelUtilities::CreateCategorySpace(OriginCategory, ORIGIN_LOCATION);
}

void DetailsPanelCameraView::CreateCameraLocationSection(IDetailLayoutBuilder& Builder)
{
	IDetailCategoryBuilder& CamCategory = Builder.EditCategory(CAM_LOCATION);
	TSharedPtr<SEditableTextBox> LatTextBox;
	TSharedPtr<SEditableTextBox> LonTextBox;

	DetailsPanelUtilities::CreateCategorySpace(CamCategory, CAM_LOCATION);

	Utils->CreateInputFieldCategoryRow(CamCategory, CAM_LOCATION, "Latitude", MapController->GetCamLatitude(), &AArcGISMapController::SetCamLatitude,
									   LatTextBox, MapController);

	Utils->CreateInputFieldCategoryRow(CamCategory, CAM_LOCATION, "Longitude", MapController->GetCamLongitude(), &AArcGISMapController::SetCamLongitude,
									   LonTextBox, MapController);

	Utils->CreateSliderBarRow(CamCategory, CAM_LOCATION, "Altitude", "Altitude is in meters", MapController->GetCamAltitude(), 50000,
							  &AArcGISMapController::SetCamAltitude, new SliderTextBox(MapController));

	Utils->CreateSliderBarRow(CamCategory, CAM_LOCATION, "Heading", "", MapController->GetCamHeading(), 360, &AArcGISMapController::SetCamHeading,
							  new SliderTextBox(MapController));

	Utils->CreateSliderBarRow(CamCategory, CAM_LOCATION, "Pitch", "", MapController->GetCamPitch(), 360, &AArcGISMapController::SetCamPitch,
							  new SliderTextBox(MapController));

	Utils->CreateSliderBarRow(CamCategory, CAM_LOCATION, "Roll", "", MapController->GetCamRoll(), 360, &AArcGISMapController::SetCamRoll,
							  new SliderTextBox(MapController));

	DetailsPanelUtilities::CreateCategorySpace(CamCategory, CAM_LOCATION);
}

void DetailsPanelCameraView::GetViewMode() const
{
	if (MapController->GetViewMode() == EViewMode::Global)
	{
		SwapButtonColors(GlobalButton, LocalButton);
		MapExtent->SetCategoryVisibility(false);
	}
	else if (MapController->GetViewMode() == EViewMode::Local)
	{
		SwapButtonColors(LocalButton, GlobalButton);
		MapExtent->SetCategoryVisibility(true);
	}
}

TSharedRef<SButton> DetailsPanelCameraView::CreateButton(FText ButtonText,
														 FOnClicked::TSPMethodDelegate_Const<DetailsPanelCameraView>::FMethodPtr ButtonFunc)
{
	return SNew(SButton)
		.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("Button"))
		.ButtonColorAndOpacity(FSlateColor(ColorDefaultGray))
		.OnClicked(this, ButtonFunc)
		.ContentPadding(TextPadding)[SNew(STextBlock)
										 .Text(ButtonText)
										 .Justification(ETextJustify::Center)
										 .Margin(TextPadding)
										 .ColorAndOpacity(FSlateColor(FLinearColor::White))];
}

FReply DetailsPanelCameraView::SetViewModeToGlobal() const
{
	if (GEditor->IsPlayingSessionInEditor())
	{
		return FReply::Handled();
	}
	MapController->SetViewMode(EViewMode::Global);
	SwapButtonColors(GlobalButton, LocalButton);
	MapExtent->SetCategoryVisibility(false);
	MapController->MarkPackageDirty();
	return FReply::Handled();
}

FReply DetailsPanelCameraView::SetViewModeToLocal() const
{
	if (GEditor->IsPlayingSessionInEditor())
	{
		return FReply::Handled();
	}
	MapController->SetViewMode(EViewMode::Local);
	SwapButtonColors(LocalButton, GlobalButton);
	MapExtent->SetCategoryVisibility(true);
	MapController->MarkPackageDirty();
	return FReply::Handled();
}
