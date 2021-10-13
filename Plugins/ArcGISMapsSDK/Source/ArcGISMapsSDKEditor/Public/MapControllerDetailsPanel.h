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

// ArcGISMapsSDKEditor

#include "DetailsPanelAddData.h"
#include "DetailsPanelAuthentication.h"
#include "DetailsPanelBaseMap.h"
#include "DetailsPanelCameraView.h"
#include "DetailsPanelLayers.h"
#include "DetailsPanelMapExtent.h"
#include "DetailsPanelUtilities.h"

// ArcGISSamples

#include "ArcGISMapController.h"

class MapControllerDetailsPanel : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	TSharedPtr<AArcGISMapController> MapController;

	TSharedPtr<DetailsPanelAddData> AddData;
	TSharedPtr<FDetailsPanelAuthentication> Authentication;
	TSharedPtr<DetailsPanelBaseMap> BaseMap;
	TSharedPtr<DetailsPanelCameraView> CamView;
	TSharedPtr<DetailsPanelLayers> Layers;
	TSharedPtr<DetailsPanelMapExtent> MapExtent;

	bool InitializeMapController(const IDetailLayoutBuilder& Builder);
	void StyleHeaderLogo(IDetailLayoutBuilder& Builder);
};
