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

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class FArcGISToolBar
{
public:
	static void InitializeArcGISMapSetup();

private:
	static bool CheckForArcGISMapController(UWorld* World);
	static void InitializeArcGISMapController(UWorld* World);
	static void InitializeArcGISMapView(UWorld* World);
	static void InitializeArcGISPawn(UWorld* World);
	static void SelectActor(AActor* Actor);
};
