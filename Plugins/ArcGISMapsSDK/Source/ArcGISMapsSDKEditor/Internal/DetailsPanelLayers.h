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
#include "IDetailCustomization.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Views/SListView.h"

// ArcGISSamples

#include "ArcGISMapController.h"

const TSharedPtr<FString> LAYERTYPERASTERIMAGE = MakeShareable(new FString("Image"));
const TSharedPtr<FString> LAYERTYPE3DOBJECT = MakeShareable(new FString("3D model"));
const TSharedPtr<FString> LAYERTYPEINTEGRATEDMESH = MakeShareable(new FString("Integrated mesh"));

TArray<TSharedPtr<FString>> GetLayerTypeOptions();
EArcGISLayerType GetLayerType(TSharedPtr<FString> Item);
FString PromptForFile();

class ARCGISMAPSSDKEDITOR_API DetailsPanelLayers : public TSharedFromThis<DetailsPanelLayers>
{
public:
	void CreateStyle(IDetailLayoutBuilder& Builder, TSharedPtr<AArcGISMapController> Controller);
	void Add(FViewLayer Layer);
	void RefreshLayerList();

private:
	struct LayerRowElements
	{
	public:
		TSharedPtr<SBox> LayerBox;
		TSharedPtr<SImage> DropdownArrow;
		TSharedPtr<SButton> VisibleButton;
		TSharedPtr<SEditableTextBox> OpacityTextBox;
		TSharedPtr<SEditableTextBox> SourceTextBox;
		TSharedPtr<SHorizontalBox> AdditionalLayerInfo;
	};

	TSharedPtr<AArcGISMapController> MapController;

	// Utility

	static const FLinearColor GetButtonColor(bool IsActive);
	
	// Layers

	TSharedPtr<TArray<TSharedPtr<FViewLayer>>> Layers;
	TSharedPtr<SListView<TSharedPtr<FViewLayer>>> LayerList;

	TSharedPtr<TArray<TSharedPtr<FViewLayer>>> GetLayers();
	TSharedRef<SBox> CreateLayerHeaderIcon(const FString& BrushName);
	TSharedRef<ITableRow> GenerateLayerWidget(TSharedPtr<FViewLayer> Layer, const TSharedRef<STableViewBase>& OwnerTable);

	// Layer Widget

	FReply ToggleAdditionalLayerInfo(LayerRowElements* Row);
	FReply SelectFile(LayerRowElements* Row, TSharedPtr<FViewLayer> Layer);
	FReply SetLayerVisible(LayerRowElements* Row, TSharedPtr<FViewLayer> Layer);
	FReply MoveLayerUp(TSharedPtr<FViewLayer> Layer);
	FReply MoveLayerDown(TSharedPtr<FViewLayer> Layer);
	FReply CopyLayerURL(TSharedPtr<FViewLayer> Layer);
	FReply RemoveLayer(TSharedPtr<FViewLayer> Layer);
	void EditLayerName(const FText& Text, ETextCommit::Type Type, TSharedPtr<FViewLayer> Layer);
	void EditLayerSource(const FText& Text, ETextCommit::Type Type, TSharedPtr<FViewLayer> Layer);
	void EditLayerOpacity(const FText& Text, ETextCommit::Type Type, TSharedPtr<FViewLayer> Layer, LayerRowElements* Row);
	void SetLayerType(TSharedPtr<FString> Item, ESelectInfo::Type Type, TSharedPtr<FViewLayer> Layer);
};
