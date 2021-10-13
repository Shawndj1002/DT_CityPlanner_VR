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
#include "DetailsPanelLayers.h"

// Unreal

#include "DesktopPlatformModule.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "EditorDirectories.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "HAL/PlatformApplicationMisc.h"
#include "IDesktopPlatform.h"
#include "Slate.h"
#include "SlateCore.h"
#include "Widgets/Layout/SScaleBox.h"

// ArcGISMapsSDKEditor

#include "MapControllerStyles.h"

#define LOCTEXT_NAMESPACE "LayersText"

constexpr const char* LAYERS = "LAYERS";

static const FMargin LayerIconPadding(2, 0);
static const FMargin LeftHandPadding(2, 0, 0, 0);
static const float Width = 22;
static const float MaxDesiredRowHeight = 48;

TArray<TSharedPtr<FString>> LayerTypeOptions = TArray<TSharedPtr<FString>>
{
	LAYERTYPERASTERIMAGE,
	LAYERTYPE3DOBJECT,
	LAYERTYPEINTEGRATEDMESH,
};

FString PromptForFile()
{
	if (IDesktopPlatform* Platform = FDesktopPlatformModule::Get())
	{
		const void* ParentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);

		TArray<FString> OpenFilenames;
		FString OpenFilePath = FEditorDirectories::Get().GetLastDirectory(ELastDirectory::GENERIC_OPEN);

#if PLATFORM_WINDOWS
		auto FileTypeText = TEXT("All|*.tpk;*.tpkx;*.slpk|Scene Layer Packages|*.slpk|Tile Packages|*.tpk;*.tpkx");
#else
		auto FileTypeText = TEXT("All|*.tpk;*.tpkx;*.slpk|Scene Layer Packages (.slpk)|*.slpk|Tile Packages (.tpk,.tpkx)|*.tpk;*.tpkx");
#endif

		if (Platform->OpenFileDialog(ParentWindowWindowHandle, // ParentWindowHandle
									 *LOCTEXT("ImportMap_ChooseFile", "Choose map asset source file").ToString(), // DialogTitle
									 OpenFilePath, // DefaultPath
									 TEXT(""), // DefaultFile
									 FileTypeText, // FileTypes
									 EFileDialogFlags::None, // Flags
									 OpenFilenames)) // OutFilenames
		{
			return IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*(OpenFilenames[0]));
		}
	}

	return FString();
}

// Public

TArray<TSharedPtr<FString>> GetLayerTypeOptions()
{
	return LayerTypeOptions;
}

EArcGISLayerType GetLayerType(TSharedPtr<FString> Item)
{
	if (Item == LAYERTYPERASTERIMAGE)
	{
		return EArcGISLayerType::ArcGISImageLayer;
	}
	if (Item == LAYERTYPE3DOBJECT)
	{
		return EArcGISLayerType::ArcGIS3DModelLayer;
	}
	if (Item == LAYERTYPEINTEGRATEDMESH)
	{
		return EArcGISLayerType::ArcGISIntegratedMeshLayer;
	}

	return EArcGISLayerType::ArcGISImageLayer;
}

TSharedPtr<FString> ConvertLayerTypeToLayerTypeOption(EArcGISLayerType type)
{
	switch (type)
	{
		case EArcGISLayerType::ArcGISImageLayer:
			return LayerTypeOptions[0];
		case EArcGISLayerType::ArcGIS3DModelLayer:
			return LayerTypeOptions[1];
		case EArcGISLayerType::ArcGISIntegratedMeshLayer:
			return LayerTypeOptions[2];
		default:
			return LayerTypeOptions[0];
	}
}

void DetailsPanelLayers::CreateStyle(IDetailLayoutBuilder& Builder, TSharedPtr<AArcGISMapController> Controller)
{
	MapController = Controller;
	IDetailCategoryBuilder& LayerCategory = Builder.EditCategory(LAYERS);

	LayerCategory.AddCustomRow(FText::FromString(LAYERS))
	[
		SNew(SBox)
		.Padding(FMargin(0, 10))
		[
			SNew(SScrollBox)
			.NavigationScrollPadding(10)
			+ SScrollBox::Slot()
			[
				SAssignNew(LayerList, SListView<TSharedPtr<FViewLayer>>)
				.IsFocusable(false)
				.ItemHeight(0.2)
				.ListItemsSource(GetLayers().Get())
				.OnGenerateRow(this, &DetailsPanelLayers::GenerateLayerWidget)
			]
		]
	];
}

void DetailsPanelLayers::Add(FViewLayer Layer)
{
	Layers->Add(MakeShareable(new FViewLayer(Layer)));
}

void DetailsPanelLayers::RefreshLayerList()
{
	LayerList->RequestListRefresh();
}

// Utility

const FLinearColor DetailsPanelLayers::GetButtonColor(bool IsActive)
{
	if (IsActive)
	{
		return ColorDefaultBlue; 
	}
	return ColorDefaultGray;
}

// Layers

TSharedPtr<TArray<TSharedPtr<FViewLayer>>> DetailsPanelLayers::GetLayers()
{
	if (!Layers.IsValid())
	{
		Layers = MakeShareable(new TArray<TSharedPtr<FViewLayer>>); 
		auto ViewLayers = MapController->GetLayers();

		for (const FViewLayer& Layer : ViewLayers)
		{
			TSharedPtr<FViewLayer> RowLayer = MakeShareable(new FViewLayer(Layer));
			Layers->Add(RowLayer);
		}
	}
	
	return Layers;
}

TSharedRef<SBox> DetailsPanelLayers::CreateLayerHeaderIcon(const FString& BrushName)
{
	return SNew(SBox)
		.WidthOverride(Width)
		.MaxDesiredWidth(Width)
		.MaxAspectRatio(1)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(new FSlateBrush())
			]
			+ SOverlay::Slot()
			[
				SNew(SScaleBox)
				[
					SNew(SImage)
					.Image(MapControllerStyles::GetCommonStyleSet()->GetBrush(*BrushName))
				]
			]
		];
}

TSharedRef<ITableRow> DetailsPanelLayers::GenerateLayerWidget(TSharedPtr<FViewLayer> Layer, const TSharedRef<STableViewBase>& OwnerTable)
{
	LayerRowElements* Row = new LayerRowElements();

	auto table = SNew(STableRow<TSharedPtr<FViewLayer>>, OwnerTable)
		.Padding(FMargin(0, 2))
		[
			SAssignNew(Row->LayerBox, SBox)
			.MaxDesiredHeight(MaxDesiredRowHeight)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(EHorizontalAlignment::HAlign_Left)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(0, 0, 2, 0)
						.MaxWidth(Width)
						.HAlign(EHorizontalAlignment::HAlign_Left)
						[
							SNew(SButton)
							.ButtonColorAndOpacity(ColorTransparent)
							.OnClicked(this, &DetailsPanelLayers::ToggleAdditionalLayerInfo, Row)
							.Content()
							[
								SNew(SScaleBox)
								.Stretch(EStretch::ScaleToFit)
								.Content()
								[
									SAssignNew(Row->DropdownArrow, SImage)
									.Image(MapControllerStyles::GetCommonStyleSet()->GetBrush("ArrowRight"))
								]
							]
						]
						+ SHorizontalBox::Slot()
						.Padding(LayerIconPadding)
						.MaxWidth(Width)
						.HAlign(EHorizontalAlignment::HAlign_Left)
						[
							SAssignNew(Row->VisibleButton, SButton)
							.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("Button"))
							.ButtonColorAndOpacity(GetButtonColor(Layer->bIsVisible))
							.OnClicked(this, &DetailsPanelLayers::SetLayerVisible, Row, Layer)
							.Content()
							[
								SNew(SScaleBox)
								.Stretch(EStretch::ScaleToFit)
								.Content()
								[
									SNew(SImage)
									.Image(MapControllerStyles::GetCommonStyleSet()->GetBrush("Visible"))
								]
							]
						]
						+ SHorizontalBox::Slot()
						.Padding(LayerIconPadding)
						.HAlign(EHorizontalAlignment::HAlign_Left)
						.MaxWidth(Width)
						[
							SNew(SButton)
							.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("Button"))
							.ButtonColorAndOpacity(ColorDefaultGray)
							.OnClicked(this, &DetailsPanelLayers::MoveLayerUp, Layer)
							.Content()
							[
								SNew(SScaleBox)
								.Stretch(EStretch::ScaleToFit)
								.Content()
								[
									SNew(SImage)
									.Image(MapControllerStyles::GetCommonStyleSet()->GetBrush("MoveUp"))
								]
							]
						]
						+ SHorizontalBox::Slot()
						.Padding(LayerIconPadding)
						.HAlign(EHorizontalAlignment::HAlign_Left)
						.MaxWidth(Width)
						[
							SNew(SButton)
							.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("Button"))
							.ButtonColorAndOpacity(ColorDefaultGray)
							.OnClicked(this, &DetailsPanelLayers::MoveLayerDown, Layer)
							.Content()
							[
								SNew(SScaleBox)
								.Stretch(EStretch::ScaleToFit)
								.Content()
								[
									SNew(SImage)
									.Image(MapControllerStyles::GetCommonStyleSet()->GetBrush("MoveDown"))
								]
							]
						]
					]
					+ SHorizontalBox::Slot()
					.Padding(LayerIconPadding)
					[
						SNew(SEditableTextBox)
						.Text(FText::FromString(Layer->LayerName))
						.OnTextCommitted(this, &DetailsPanelLayers::EditLayerName, Layer)
						.Padding(TextBoxPadding)
						.HintText(FText::FromString("Enter Layer Name"))
						.Style(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
					]
					+ SHorizontalBox::Slot()
					.Padding(LeftHandPadding)
					.FillWidth(0.6)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.Padding(0, 0, 2, 0)
						[
							SAssignNew(Row->OpacityTextBox, SEditableTextBox)
							.Text(FText::FromString(FString::FromInt(Layer->LayerOpacity * 100.0) + "%"))
							.OnTextCommitted(this, &DetailsPanelLayers::EditLayerOpacity, Layer, Row)
							.Padding(TextBoxPadding)
							.Style(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
							.Justification(ETextJustify::Center)
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.HAlign(EHorizontalAlignment::HAlign_Right)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.Padding(LeftHandPadding)
							.MaxWidth(Width)
							.HAlign(EHorizontalAlignment::HAlign_Right)
							[
								SNew(SButton)
								.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("Button"))
								.ButtonColorAndOpacity(ColorDefaultGray)
								.OnClicked(this, &DetailsPanelLayers::RemoveLayer, Layer)
								.Content()
								[
									SNew(SScaleBox)
									.Stretch(EStretch::ScaleToFit)
									.Content()
									[
										SNew(SImage)
										.Image(MapControllerStyles::GetCommonStyleSet()->GetBrush("Delete"))
									]
								]
							]
						]
					]
				]
				+ SVerticalBox::Slot()
				.Padding(FMargin(0, 4, 0, 0))
				[
					SAssignNew(Row->AdditionalLayerInfo, SHorizontalBox)
					+ SHorizontalBox::Slot()
					.Padding(Width * 3.5 + 1, 0, 2, 0)
					.AutoWidth()
					.HAlign(EHorizontalAlignment::HAlign_Left)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.MaxWidth(Width)
						[
							SNew(SButton)
							.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("Button"))
							.ButtonColorAndOpacity(ColorDefaultGray)
							.OnClicked(this, &DetailsPanelLayers::CopyLayerURL, Layer)
							.Content()
							[
								SNew(SScaleBox)
								.Stretch(EStretch::ScaleToFit)
								.Content()
								[
									SNew(SImage)
									.Image(MapControllerStyles::GetCommonStyleSet()->GetBrush("Clipboard"))
								]
							]
						]
						
					]
					+ SHorizontalBox::Slot()
					.Padding(LayerIconPadding)
					[
						SNew(SOverlay)
						+ SOverlay::Slot()
						[
							SAssignNew(Row->SourceTextBox, SEditableTextBox)
							.Text(FText::FromString(Layer->LayerSource))
							.OnTextCommitted(this, &DetailsPanelLayers::EditLayerSource, Layer)
							.Padding(FMargin(12, 3, 30, 3))
							.HintText(FText::FromString("Enter Layer Source"))
							.Style(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
						]
						+ SOverlay::Slot()
						.HAlign(EHorizontalAlignment::HAlign_Right)
						[
							SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.MaxWidth(Width)
							[
								SNew(SButton)
								.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("Button"))
								.ButtonColorAndOpacity(ColorDefaultGray)
								.OnClicked(this, &DetailsPanelLayers::SelectFile, Row, Layer)
								.Content()
								[
									SNew(SScaleBox)
									.Stretch(EStretch::ScaleToFit)
									.Content()
									[
										SNew(SImage)
										.Image(MapControllerStyles::GetCommonStyleSet()->GetBrush("Ellipse"))
									]
								]
							]
						]
					]
					+ SHorizontalBox::Slot()
					.Padding(LeftHandPadding)
					.FillWidth(0.6)
					.HAlign(EHorizontalAlignment::HAlign_Fill)
					[
						SNew(STextComboBox)
						.OptionsSource(&LayerTypeOptions)
						.InitiallySelectedItem(ConvertLayerTypeToLayerTypeOption(Layer->LayerType))
						.OnSelectionChanged(this, &DetailsPanelLayers::SetLayerType, Layer) 
						.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("ComboBoxButton"))
						.ContentPadding(FMargin(0, 0))
						.ColorAndOpacity(FSlateColor(ColorFontInner))
					]
				]
			]
		];

	ToggleAdditionalLayerInfo(Row);
	return table;
}

// Layer Widget

FReply DetailsPanelLayers::ToggleAdditionalLayerInfo(LayerRowElements* Row)
{
	EVisibility visible = Row->AdditionalLayerInfo->GetVisibility();

	if (visible.IsVisible())
	{
		Row->AdditionalLayerInfo->SetVisibility(EVisibility::Collapsed);
		Row->LayerBox->SetMaxDesiredHeight(Width);
		Row->DropdownArrow->SetImage(MapControllerStyles::GetCommonStyleSet()->GetBrush("ArrowRight"));
	}
	else
	{
		Row->AdditionalLayerInfo->SetVisibility(EVisibility::Visible);
		Row->LayerBox->SetMaxDesiredHeight(MaxDesiredRowHeight);
		Row->DropdownArrow->SetImage(MapControllerStyles::GetCommonStyleSet()->GetBrush("ArrowDown"));
	}

	return FReply::Handled();
}

FReply DetailsPanelLayers::SetLayerVisible(LayerRowElements* Row, TSharedPtr<FViewLayer> Layer)
{
	Layer->bIsVisible = !Layer->bIsVisible;
	Row->VisibleButton->SetBorderBackgroundColor(GetButtonColor(Layer->bIsVisible));
	int LayerIndex = Layers->Find(Layer);
	MapController->GetLayers()[LayerIndex].bIsVisible = Layer->bIsVisible;
	MapController->MarkPackageDirty();
	return FReply::Handled();
}

FReply DetailsPanelLayers::MoveLayerUp(TSharedPtr<FViewLayer> Layer)
{
	auto LayerIndex = Layers->Find(Layer);

	if (LayerIndex <= 0)
	{
		return FReply::Handled();
	}
	
	Layers->Swap(LayerIndex, LayerIndex - 1);
	MapController->GetLayers().Swap(LayerIndex, LayerIndex - 1);
	MapController->MarkPackageDirty();
	LayerList->RequestListRefresh();
	return FReply::Handled();
}

FReply DetailsPanelLayers::MoveLayerDown(TSharedPtr<FViewLayer> Layer)
{
	int LayerIndex = Layers->Find(Layer);

	if (LayerIndex >= Layers->Num() - 1)
	{
		return FReply::Handled();
	}
	
	Layers->Swap(LayerIndex, LayerIndex + 1);
	MapController->GetLayers().Swap(LayerIndex, LayerIndex + 1);
	MapController->MarkPackageDirty();
	LayerList->RequestListRefresh();
	return FReply::Handled();
}

FReply DetailsPanelLayers::CopyLayerURL(TSharedPtr<FViewLayer> Layer)
{
	FString Source = Layer->LayerSource;
	UE_LOG(LogTemp, Warning, TEXT("Layer URL: %s"), *Source);	
	FPlatformApplicationMisc::ClipboardCopy(*Source);
	return FReply::Handled();
}

FReply DetailsPanelLayers::RemoveLayer(TSharedPtr<FViewLayer> Layer)
{
	auto LayerIndex = Layers->Find(Layer);
	Layers->RemoveAt(LayerIndex);
	MapController->GetLayers().RemoveAt(LayerIndex);	
	MapController->MarkPackageDirty();
	LayerList->RequestListRefresh();
	return FReply::Handled();
}

void DetailsPanelLayers::EditLayerName(const FText& Text, ETextCommit::Type Type, TSharedPtr<FViewLayer> Layer)
{
	Layer->LayerName = Text.ToString();
	auto LayerIndex = Layers->Find(Layer);
	MapController->GetLayers()[LayerIndex].LayerName = Layer->LayerName;
	MapController->MarkPackageDirty();
}

void DetailsPanelLayers::EditLayerSource(const FText& Text, ETextCommit::Type Type, TSharedPtr<FViewLayer> Layer)
{
	Layer->LayerSource = Text.ToString();
	auto LayerIndex = Layers->Find(Layer);
	MapController->GetLayers()[LayerIndex].LayerSource = Layer->LayerSource;
	MapController->MarkPackageDirty();
}

void DetailsPanelLayers::EditLayerOpacity(const FText& Text, ETextCommit::Type Type, TSharedPtr<FViewLayer> Layer, LayerRowElements* Row)
{
	double Opacity = FCString::Atof(*Text.ToString());

	if (Opacity >= 100)
	{
		Opacity = 100;
	}
	else if (Opacity <= 0)
	{
		Opacity = 0;
	}
	
	Layer->LayerOpacity = Opacity * 0.01;
	auto LayerIndex = Layers->Find(Layer);
	MapController->GetLayers()[LayerIndex].LayerOpacity = Layer->LayerOpacity;
	MapController->MarkPackageDirty();
	Row->OpacityTextBox->SetText(FText::FromString(FString::FromInt(Opacity) + "%"));
}

void DetailsPanelLayers::SetLayerType(TSharedPtr<FString> Item, ESelectInfo::Type Type, TSharedPtr<FViewLayer> Layer)
{
	Layer->LayerType = GetLayerType(Item);
	auto LayerIndex = Layers->Find(Layer);
	MapController->GetLayers()[LayerIndex].LayerType = Layer->LayerType;
	MapController->MarkPackageDirty();
}

FReply DetailsPanelLayers::SelectFile(LayerRowElements* Row, TSharedPtr<FViewLayer> Layer)
{
	FString FileName = PromptForFile();

	if (FileName.IsEmpty())
	{
		return FReply::Handled();
	}

	FText FileNameFText = FText::FromString(FileName);
	Row->SourceTextBox->SetText(FileNameFText);
	EditLayerSource(FileNameFText, ETextCommit::Type::Default, Layer);
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
