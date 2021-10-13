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
#include "DetailsPanel/ArcGISMapsSDKArray.h"

// Unreal

#include "PropertyCustomizationHelpers.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Views/SListView.h"

void SArcGISMapsSDKArray::Construct(const FArguments& arguments, TSharedRef<IPropertyHandle> propertyHandle)
{
	OnElementAdded = arguments._OnElementAdded;
	OnElementsRemoved = arguments._OnElementsRemoved;
	OnGenerateWidget = arguments._OnGenerateWidget;
	PropertyHandle = propertyHandle;
	PropertyHandleArray = propertyHandle->AsArray();
	Title = arguments._Title;

	check(PropertyHandleArray.IsValid());

	if (!Title.IsSet())
	{
		Title = PropertyHandle->GetPropertyDisplayName();
	}

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBorder)
			.BorderImage(this, &SArcGISMapsSDKArray::GetBackgroundImage)
			.Padding(FMargin(0.0f, 0.0f, 16.0f, 0.0f))
			.BorderBackgroundColor(FLinearColor(0.6, 0.6, 0.6, 1.0f))
			.OnMouseButtonDown_Lambda([this](const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
			{
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
				{
					ToggleExpansion();

					return FReply::Handled();
				}
				else
				{
					return FReply::Unhandled();
				}
			})
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.Padding(2.0f, 10.0f, 2.0f, 8.0f)
				.AutoWidth()
				[
					SAssignNew(ExpanderArrow, SButton)
					.ButtonStyle(FCoreStyle::Get(), "NoBorder")
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					.ClickMethod(EButtonClickMethod::MouseDown)
					.OnClicked(this, &SArcGISMapsSDKArray::OnArrowClicked)
					.ContentPadding(0.f)
					.ForegroundColor(FSlateColor::UseForeground())
					.IsFocusable(false)
					[
						SNew(SImage)
						.Image(this, &SArcGISMapsSDKArray::GetExpanderImage)
						.ColorAndOpacity(FSlateColor::UseForeground())
					]
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(Title)
					.Font(FEditorStyle::GetFontStyle("DetailsView.CategoryFontStyle"))
					.ShadowOffset(FVector2D(1.0f, 1.0f))
				]
			]
		]
		+ SVerticalBox::Slot()
		[
			SNew(SBorder)
			.Visibility_Lambda([this]()
			{
				return bIsItemExpanded ? EVisibility::Visible : EVisibility::Collapsed;
			})
			.Padding(FMargin(10.0f))
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
					.Visibility_Lambda([this]()
					{
						uint32 numElements;

						PropertyHandleArray->GetNumElements(numElements);

						return !numElements ? EVisibility::Visible : EVisibility::Collapsed;
					})
					.Text(FText::FromString(TEXT("Click on Add to add a new item")))
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SAssignNew(ListView, SListView<FListViewEntry>)
					.Visibility_Lambda([this]()
					{
						uint32 numElements;

						PropertyHandleArray->GetNumElements(numElements);

						return numElements ? EVisibility::Visible : EVisibility::Collapsed;
					})
					.ListItemsSource(&ListViewItems)
					.OnGenerateRow(this, &SArcGISMapsSDKArray::OnGenerateRowForListView)
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Right)
				.Padding(FMargin(0.0f, 10.0f, 0.0f, 0.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Padding(2.0f, 1.0f)
					[
						PropertyCustomizationHelpers::MakeAddButton(FSimpleDelegate::CreateLambda([this]
						{
							PropertyHandleArray->AddItem();

							if (OnElementAdded.IsBound())
							{
								uint32 numElements;

								PropertyHandleArray->GetNumElements(numElements);

								auto elementProperty = PropertyHandleArray->GetElement(numElements - 1);

								OnElementAdded.Execute(elementProperty);
							}

							RefreshListViewItems();
						}))
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Padding(2.0f, 1.0f)
					[
						PropertyCustomizationHelpers::MakeRemoveButton(FSimpleDelegate::CreateLambda([this]
						{
							auto selectedItems = ListView->GetSelectedItems();

							if (selectedItems.Num() > 0)
							{
								for (const auto& item : selectedItems)
								{
									PropertyHandleArray->DeleteItem(*item);
								}

								if (OnElementsRemoved.IsBound())
								{
									OnElementsRemoved.Execute();
								}

								RefreshListViewItems();
							}
							
						}), FText(), TAttribute<bool>::Create([this]() -> bool
						{
							return ListView->GetNumItemsSelected() > 0;
						}))
					]
				]
			]
		]
	];

	RefreshListViewItems();
}

const FSlateBrush* SArcGISMapsSDKArray::GetBackgroundImage() const
{
	if (IsHovered())
	{
		return bIsItemExpanded ? FEditorStyle::GetBrush("DetailsView.CategoryTop_Hovered") : FEditorStyle::GetBrush("DetailsView.CollapsedCategory_Hovered");
	}
	else
	{
		return bIsItemExpanded ? FEditorStyle::GetBrush("DetailsView.CategoryTop") : FEditorStyle::GetBrush("DetailsView.CollapsedCategory");
	}
}

const FSlateBrush* SArcGISMapsSDKArray::GetExpanderImage() const
{
	FName ResourceName;

	if (bIsItemExpanded)
	{
		if (ExpanderArrow->IsHovered())
		{
			static FName ExpandedHoveredName = "TreeArrow_Expanded_Hovered";
			ResourceName = ExpandedHoveredName;
		}
		else
		{
			static FName ExpandedName = "TreeArrow_Expanded";
			ResourceName = ExpandedName;
		}
	}
	else
	{
		if (ExpanderArrow->IsHovered())
		{
			static FName CollapsedHoveredName = "TreeArrow_Collapsed_Hovered";
			ResourceName = CollapsedHoveredName;
		}
		else
		{
			static FName CollapsedName = "TreeArrow_Collapsed";
			ResourceName = CollapsedName;
		}
	}

	return FCoreStyle::Get().GetBrush(ResourceName);
}

FReply SArcGISMapsSDKArray::OnArrowClicked()
{
	ToggleExpansion();

	return FReply::Handled();
}

TSharedRef<ITableRow> SArcGISMapsSDKArray::OnGenerateRowForListView(FListViewEntry Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	check(OnGenerateWidget.IsBound());

	auto elementProperty = PropertyHandleArray->GetElement(*Item);

	return SNew(STableRow<FListViewEntry>, OwnerTable)
		.Padding(FMargin(2, 2))
		[
			OnGenerateWidget.Execute(elementProperty)
		];
}

void SArcGISMapsSDKArray::RefreshListViewItems()
{
	ListViewItems.Empty();

	uint32 numElements;

	PropertyHandleArray->GetNumElements(numElements);

	for (uint32 i = 0; i < numElements; ++i)
	{
		TSharedPtr<uint32> newEntry = MakeShareable(new uint32);

		*newEntry = ListViewItems.Num();

		ListViewItems.Add(newEntry);
	}

	ListView->RequestListRefresh();
}

void SArcGISMapsSDKArray::ToggleExpansion()
{
	bIsItemExpanded = !bIsItemExpanded;
}
