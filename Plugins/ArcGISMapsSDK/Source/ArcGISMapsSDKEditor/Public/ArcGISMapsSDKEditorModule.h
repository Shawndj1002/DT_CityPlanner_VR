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

#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class FUICommandList;

class FArcGISMapsSDKEditorModule : public IModuleInterface
{
public:
	// IModuleInterface
	virtual void ShutdownModule() override;
	virtual void StartupModule() override;

	void PluginButtonClicked();

private:
	TSharedPtr<FUICommandList> PluginCommands;
	void RegisterMenus();
};
