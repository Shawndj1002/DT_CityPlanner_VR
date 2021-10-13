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

#include "Framework/SlateDelegates.h"
#include "PropertyHandle.h"
#include "Widgets/SCompoundWidget.h"

class SButton;
template <typename ItemType> class SListView;

class SArcGISMapsSDKArray : public SCompoundWidget
{
public:
	using FOnGenerateWidget = typename TSlateDelegates<TSharedRef<IPropertyHandle>>::FOnGenerateWidget;

	DECLARE_DELEGATE_OneParam(FOnElementAdded, TSharedRef<IPropertyHandle>);
	DECLARE_DELEGATE(FOnElementsRemoved);

	SLATE_BEGIN_ARGS(SArcGISMapsSDKArray)
		: _OnElementAdded()
		, _OnElementsRemoved()
		, _OnGenerateWidget()
		, _Title()
		{
		}

		SLATE_EVENT(FOnElementAdded, OnElementAdded)
		SLATE_EVENT(FOnElementsRemoved, OnElementsRemoved)
		SLATE_EVENT(FOnGenerateWidget, OnGenerateWidget)

		SLATE_ATTRIBUTE(FText, Title)
	SLATE_END_ARGS()

	void Construct(const FArguments& arguments, TSharedRef<IPropertyHandle> propertyHandle);

protected:
	FOnElementAdded OnElementAdded;
	FOnElementsRemoved OnElementsRemoved;
	FOnGenerateWidget OnGenerateWidget;
	TAttribute<FText> Title;

private:
	typedef TSharedPtr<uint32> FListViewEntry;

	const FSlateBrush* GetBackgroundImage() const;
	const FSlateBrush* GetExpanderImage() const;
	FReply OnArrowClicked();
	TSharedRef<ITableRow> OnGenerateRowForListView(FListViewEntry Item, const TSharedRef<STableViewBase>& OwnerTable);
	void RefreshListViewItems();
	void ToggleExpansion();

	bool bIsItemExpanded = true;

	TSharedPtr<SButton> ExpanderArrow;
	TSharedPtr<SListView<FListViewEntry>> ListView;
	TArray<FListViewEntry> ListViewItems;
	TSharedPtr<IPropertyHandle> PropertyHandle;
	TSharedPtr<IPropertyHandleArray> PropertyHandleArray;
};
