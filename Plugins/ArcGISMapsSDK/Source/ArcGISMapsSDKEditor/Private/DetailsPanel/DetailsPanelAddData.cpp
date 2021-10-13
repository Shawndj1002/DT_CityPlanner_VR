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
#include "DetailsPanelAddData.h"

// Unreal

#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Slate.h"
#include "SlateCore.h"

// ArcGISMapsSDKEditor

#include "MapControllerStyles.h"

#define LOCTEXT_NAMESPACE "AddDataText"

constexpr const char* ADD_DATA = "ADD DATA";

void DetailsPanelAddData::CreateStyle(IDetailLayoutBuilder& Builder, TSharedPtr<AArcGISMapController> Controller, TSharedPtr<DetailsPanelLayers> PanelLayers)
{
	MapController = Controller;
	Layers = PanelLayers;

	LayerTypeOptions = GetLayerTypeOptions();

	AddURLButton = CreateButton(FText::FromString("Add From URL"), &DetailsPanelAddData::SetSourceModeToUrl);
	AddFileButton = CreateButton(FText::FromString("Add From File"), &DetailsPanelAddData::SetSourceModeToFile);
	AddLinkedButtonRowToCategory(Builder, ADD_DATA, AddURLButton.ToSharedRef(), AddFileButton.ToSharedRef());

	IDetailCategoryBuilder& InputFields = Builder.EditCategory(ADD_DATA);

	InputFields.AddCustomRow(FText::FromString(ADD_DATA))
	.NameContent()[SNew(STextBlock).Text(FText::FromString("Layer Name"))]
	.ValueContent()
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	[
		SAssignNew(LayerNameTextBox, SEditableTextBox)
		.HintText(FText::FromString("Enter Layer Name"))
		.Padding(TextBoxPadding)
		.Style(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
	];

	InputFields.AddCustomRow(FText::FromString(ADD_DATA))
	.NameContent()
	[
		SAssignNew(PathLabel, STextBlock)
	]
	.ValueContent()
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	[
		SNew(SHorizontalBox) 
		+ SHorizontalBox::Slot()
		.FillWidth(1)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			SAssignNew(PathTextBox, SEditableTextBox)
			.Padding(TextBoxPadding)
			.Style(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FEditableTextBoxStyle>("NormalEditableTextBox"))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(0.6)
		.Padding(4, 0, 0, 0)
		[
			SAssignNew(SelectFileButton, SButton)
			.Content()
			[
				SNew(STextBlock)
				.Text(FText::FromString("Select File"))
				.Justification(ETextJustify::Center)
				.Margin(FMargin(0, 1))
				.ColorAndOpacity(ColorFontInner)
			]
			.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("Button"))
			.ButtonColorAndOpacity(FSlateColor(ColorDefaultGray))
			.OnClicked(this, &DetailsPanelAddData::SelectFile)
		]
	];

	InputFields.AddCustomRow(FText::FromString(ADD_DATA))
	.NameContent()[SNew(STextBlock).Text(FText::FromString("Layer Type"))]
	.ValueContent()
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	[
		SAssignNew(LayerTypeComboBox, STextComboBox)
		.OptionsSource(&LayerTypeOptions)
		.InitiallySelectedItem(LayerTypeOptions[0])
		.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("ComboBoxButton"))
		.ContentPadding(FMargin(0, 0))
		.ColorAndOpacity(FSlateColor(ColorFontInner))
	];

	InputFields.AddCustomRow(FText::FromString(ADD_DATA))
	[
		SNew(SHorizontalBox) 
		+ SHorizontalBox::Slot()
		.FillWidth(1)
		.Padding(0, 12, 8, 12)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			SNew(SSpacer)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1)
		.Padding(8, 12, 0, 12)
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.Padding(0, 12, 2, 12)
			[
				SNew(SButton)
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Clear"))
					.Justification(ETextJustify::Center)
					.Margin(FMargin(0, 3))
					.ColorAndOpacity(ColorFontInner)
				]
				.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("Button"))
				.ButtonColorAndOpacity(FSlateColor(ColorDefaultGray))
				.OnClicked(this, &DetailsPanelAddData::ClearSourceUIFields)
			]
			+ SHorizontalBox::Slot()
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.Padding(2, 12, 0, 12)
			[
				SNew(SButton)
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString("Add Layer"))
					.Justification(ETextJustify::Center)
					.Margin(FMargin(0, 3))
					.ColorAndOpacity(ColorFontInner)
				]
				.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("Button"))
				.ButtonColorAndOpacity(FSlateColor(ColorDefaultGray))
				.OnClicked(this, &DetailsPanelAddData::AddLayerFromSourceUIFields)
			]
		]
	];

	SetSourceModeToUrl();
}

TSharedRef<SButton> DetailsPanelAddData::CreateButton(FText ButtonText, FOnClicked::TSPMethodDelegate_Const<DetailsPanelAddData>::FMethodPtr ButtonFunc)
{
	return SNew(SButton)
		.ButtonStyle(&MapControllerStyles::GetCommonStyleSet()->GetWidgetStyle<FButtonStyle>("Button"))
		.ButtonColorAndOpacity(FSlateColor(ColorDefaultGray))
		.OnClicked(this, ButtonFunc)
		.ContentPadding(TextPadding)
		[
			SNew(STextBlock)
			.Text(ButtonText)
			.Justification(ETextJustify::Center)
			.Margin(TextPadding)
			.ColorAndOpacity(FSlateColor(FLinearColor::White))
		];
}

FReply DetailsPanelAddData::SetSourceModeToUrl() const
{
	SwapButtonColors(AddURLButton, AddFileButton);
	PathLabel->SetText(FText::FromString("URL"));
	PathTextBox->SetHintText(FText::FromString("Enter URL"));
	SelectFileButton->SetVisibility(EVisibility::Collapsed);
	return FReply::Handled();
}

FReply DetailsPanelAddData::SetSourceModeToFile() const
{
	SwapButtonColors(AddFileButton, AddURLButton);
	PathLabel->SetText(FText::FromString("File Path"));
	PathTextBox->SetHintText(FText::FromString("Enter File Path"));
	SelectFileButton->SetVisibility(EVisibility::Visible);
	return FReply::Handled();
}

FReply DetailsPanelAddData::AddLayerFromSourceUIFields() const
{
	if (LayerNameTextBox->GetText().ToString() == "" || PathTextBox->GetText().ToString() == "")
	{
		return FReply::Handled();
	}

	FViewLayer Layer = FViewLayer();
	Layer.LayerName = LayerNameTextBox->GetText().ToString().TrimStartAndEnd();
	Layer.bIsVisible = true;
	Layer.LayerOpacity = 1.0;
	Layer.LayerSource = PathTextBox->GetText().ToString().TrimStartAndEnd();
	Layer.LayerType = GetLayerType(LayerTypeComboBox.Get()->GetSelectedItem());

	Layers->Add(Layer);
	Layers->RefreshLayerList();
	MapController->GetLayers().Add(Layer);
	LayerNameTextBox->SetText(FText::FromString(""));
	PathTextBox->SetText(FText::FromString(""));
	LayerTypeComboBox.Get()->SetSelectedItem(LayerTypeOptions[0]);
	MapController->MarkPackageDirty();

	return FReply::Handled();
}

FReply DetailsPanelAddData::ClearSourceUIFields() const
{
	PathTextBox->SetText(FText::FromString(""));
	LayerNameTextBox->SetText(FText::FromString(""));
	return FReply::Handled();
}

FReply DetailsPanelAddData::SelectFile() const
{
	FString FileName = PromptForFile();

	if (FileName.IsEmpty())
	{
		return FReply::Handled();
	}

	PathTextBox->SetText(FText::FromString(FileName));
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
