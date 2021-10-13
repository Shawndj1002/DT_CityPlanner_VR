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

#include "ArcGISToolBarStyle.h"
#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"

class FArcGISToolBarCommands : public TCommands<FArcGISToolBarCommands>
{
public:
	FArcGISToolBarCommands();

	// TCommands<FArcGISToolBarCommands>
	virtual void RegisterCommands() override;

	TSharedPtr<const FUICommandInfo> GetPluginAction() const;

private:
	TSharedPtr<FUICommandInfo> PluginAction;
};
