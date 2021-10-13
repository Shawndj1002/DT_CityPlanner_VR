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
#include "ArcGISToolbar.h"

// Unreal

#include "Editor.h"
#include "Engine/Selection.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// ArcGISMapsSDK

#include "ArcGISMapsSDK/Actors/ArcGISMapView.h"

// ArcGISSamples

#include "ArcGISMapController.h"
#include "SampleDefaultPawn.h"

void FArcGISToolBar::InitializeArcGISMapSetup()
{
	auto World = GEditor -> GetEditorWorldContext().World();

	if (CheckForArcGISMapController(World))
	{
		return;
	}

	if (!AArcGISMapView::GetArcGISMapView(World))
	{
		InitializeArcGISMapView(World);
	}

	if (!UGameplayStatics::GetActorOfClass(World, ASampleDefaultPawn::StaticClass()))
	{
		InitializeArcGISPawn(World);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("An ArcGISPawn currently exists in the world outliner"));
	}

	if (!UGameplayStatics::GetActorOfClass(World, AArcGISMapController::StaticClass()))
	{
		InitializeArcGISMapController(World);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("An ArcGISMapController currently exists in the world outliner. Remove it to initialize an ArcGISMapController"));
		CheckForArcGISMapController(World);
	}
}

bool FArcGISToolBar::CheckForArcGISMapController(UWorld* World)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, AArcGISMapController::StaticClass(), FoundActors);

	if (FoundActors.Num() == 1)
	{
		if (!AArcGISMapView::GetArcGISMapView(FoundActors[0]))
		{
			return false;
		}

		SelectActor(FoundActors[0]);
		return true;
	}
	else if (FoundActors.Num() > 1)
	{
		UE_LOG(LogTemp, Error, TEXT("Multiple ArcGISMapController were found in the world outliner"));
	}

	return false;
}

void FArcGISToolBar::InitializeArcGISMapView(UWorld* World)
{
	auto* MapView = static_cast<AArcGISMapView*>(GEditor->AddActor(World->GetCurrentLevel(), AArcGISMapView::StaticClass(), FTransform()));
	MapView->SetOriginLatitude(41);
	MapView->SetOriginLongitude(-74);
	MapView->SetOriginAltitude(0);
	MapView->SetActorLabel("ArcGISMapView");
}

void FArcGISToolBar::InitializeArcGISPawn(UWorld* World)
{
	auto* Pawn = static_cast<ASampleDefaultPawn*>(GEditor->AddActor(World->GetCurrentLevel(), ASampleDefaultPawn::StaticClass(), FTransform()));
	Pawn->SetActorLabel(FString("ArcGISCamera"));

	if (Pawn == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("PAWN IS NULL"));
		return;
	}

	auto arcGISLocationComponent = Pawn->ArcGISLocation;

	arcGISLocationComponent->SetPosition(Esri::GameEngine::Location::ArcGISPosition(74.054924, 40.691242, 3000.0));
	arcGISLocationComponent->SetRotation(Esri::GameEngine::Location::ArcGISRotation(68, 0, 65));
}

void FArcGISToolBar::InitializeArcGISMapController(UWorld* World)
{
	auto* MapControllerActor = GEditor->AddActor(World->GetCurrentLevel(), AArcGISMapController::StaticClass(), FTransform());
	MapControllerActor->SetActorLabel(FString("ArcGISMap"));

	auto MapController = static_cast<AArcGISMapController*>(MapControllerActor);
	MapController->SetCamLongitude(-74.054921);
	MapController->SetCamLatitude(40.691242);
	MapController->SetCamAltitude(3000);
	MapController->SetCamHeading(65);
	MapController->SetCamPitch(68);

	SelectActor(MapControllerActor);
}

void FArcGISToolBar::SelectActor(AActor* Actor)
{
	auto* EditorSelection = GEditor->GetSelectedActors();
	EditorSelection->DeselectAll();
	GEditor->SelectActor(Actor, true, true);
}
