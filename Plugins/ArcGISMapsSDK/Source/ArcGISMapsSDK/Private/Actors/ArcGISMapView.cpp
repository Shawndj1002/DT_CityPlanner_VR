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
#include "ArcGISMapsSDK/Actors/ArcGISMapView.h"

#include "Engine.h"

namespace Esri
{
AArcGISMapView* GArcGISMapView{nullptr};
UWorld* GWorld{nullptr};
} // namespace Esri

void RegisterArcGISMapView(AArcGISMapView* ArcGISMapView, UWorld* World)
{
	if (Esri::GWorld == nullptr || Esri::GWorld != World)
	{
		Esri::GArcGISMapView = ArcGISMapView;
		Esri::GWorld = World;
	}
	else
	{
		if (Esri::GArcGISMapView != nullptr && Esri::GArcGISMapView != ArcGISMapView)
		{
			UE_LOG(LogTemp, Error, TEXT("Only one ArcGISMapView actor should exist in the world."));
		}
		else
		{
			Esri::GArcGISMapView = ArcGISMapView;
			Esri::GWorld = World;
		}
	}
}

void UnregisterArcGISMapView(AArcGISMapView* ArcGISMapView, UWorld* World)
{
	check(Esri::GWorld == World);

	if (Esri::GArcGISMapView != ArcGISMapView)
	{
		UE_LOG(LogTemp, Error, TEXT("Only one ArcGISMapView actor should exist in the world."));
	}
	else
	{
		Esri::GArcGISMapView = nullptr;
		Esri::GWorld = nullptr;
	}
}

void AArcGISMapView::BeginPlay()
{
	Super::BeginPlay();

	RegisterArcGISMapView(this, GetWorld());
}

void AArcGISMapView::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterArcGISMapView(this, GetWorld());

	Super::EndPlay(EndPlayReason);
}

void AArcGISMapView::Destroyed()
{
	UnregisterArcGISMapView(this, GetWorld());
	Super::Destroyed();
}

void AArcGISMapView::PostLoad()
{
	Initialize();
	Super::PostLoad();
}

void AArcGISMapView::PostActorCreated()
{
	Initialize();
	Super::PostActorCreated();
}

void AArcGISMapView::Initialize()
{
	GeographicCRS = "EPSG:4326";
	ProjectedCRS = "EPSG:3857";
	bOriginAtPlanetCenter = false;
	bOriginLocationInProjectedCRS = false;
	OriginLongitude = Longitude;
	OriginLatitude = Latitude;
	OriginAltitude = Altitude;
	PlanetShape = (MapType == EArcGISMapType::Global ? EPlanetShape::RoundPlanet : EPlanetShape::FlatPlanet);
}

AArcGISMapView* AArcGISMapView::GetArcGISMapView(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	if (Esri::GArcGISMapView == nullptr || Esri::GWorld != World)
	{
		if (World)
		{
			TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsOfClass(World, AArcGISMapView::StaticClass(), Actors);
			int NbActors = Actors.Num();
			if (NbActors == 1)
			{
				Esri::GArcGISMapView = Cast<AArcGISMapView>(Actors[0]);
				Esri::GWorld = World;
			}
			else
			{
				Esri::GArcGISMapView = nullptr;
				Esri::GWorld = nullptr;

				if (NbActors == 0)
				{
					UE_LOG(LogTemp, Error, TEXT("ArcGISMapView actor not found. Please add one to your world."));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Multiple ArcGISMapView actors found. Only one actor should be present in the world."));
				}
			}
		}
	}

	return Esri::GArcGISMapView;
}

void AArcGISMapView::SetMapType(EArcGISMapType InMapType)
{
	if (InMapType == EArcGISMapType::Global)
	{
		PlanetShape = EPlanetShape::RoundPlanet;
	}
	else
	{
		PlanetShape = EPlanetShape::FlatPlanet;
	}
	MapType = InMapType;
}

EArcGISMapType AArcGISMapView::GetMapType()
{
	if (PlanetShape == EPlanetShape::RoundPlanet)
	{
		return EArcGISMapType::Global;
	}
	else
	{
		return EArcGISMapType::Local;
	}
}

void AArcGISMapView::SetOriginLongitude(int32 InLongitude)
{
	OriginLongitude = Longitude = InLongitude;
}

int32 AArcGISMapView::GetOriginLongitude()
{
	return OriginLongitude;
}

void AArcGISMapView::SetOriginLatitude(int32 InLatitude)
{
	OriginLatitude = Latitude = InLatitude;
}

int32 AArcGISMapView::GetOriginLatitude()
{
	return OriginLatitude;
}

void AArcGISMapView::SetOriginAltitude(int32 InAltitude)
{
	OriginAltitude = Altitude = InAltitude;
}

int32 AArcGISMapView::GetOriginAltitude()
{
	return OriginAltitude;
}
