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

#include "DetailsPanelLayers.h"
#include "DetailsPanelUtilities.h"

using namespace DetailsPanelUtilities;

class ARCGISMAPSSDKEDITOR_API DetailsPanelAddData : public TSharedFromThis<DetailsPanelAddData>
{
public:
	void CreateStyle(IDetailLayoutBuilder& Builder, TSharedPtr<AArcGISMapController> Controller, TSharedPtr<DetailsPanelLayers> PanelLayers);

private:
	TSharedPtr<AArcGISMapController> MapController;
	TSharedPtr<DetailsPanelLayers> Layers;

	TArray<TSharedPtr<FString>> LayerTypeOptions;

	TSharedPtr<SButton> AddFileButton;
	TSharedPtr<SButton> AddURLButton;
	TSharedPtr<SButton> SelectFileButton;
	TSharedPtr<SEditableTextBox> LayerNameTextBox;
	TSharedPtr<SEditableTextBox> PathTextBox;
	TSharedPtr<STextBlock> PathLabel;
	TSharedPtr<STextComboBox> LayerTypeComboBox;

	TSharedRef<SButton> CreateButton(FText ButtonText, FOnClicked::TSPMethodDelegate_Const<DetailsPanelAddData>::FMethodPtr ButtonFunc);
	FReply AddLayerFromSourceUIFields() const;
	FReply ClearSourceUIFields() const;
	FReply SelectFile() const;
	FReply SetSourceModeToFile() const;
	FReply SetSourceModeToUrl() const;
};
