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
#include "GeoReferencingSystem.h"

#include "ArcGISMapsSDK/BlueprintNodes/GameEngine/Map/ArcGISMapType.h"

#include "ArcGISMapView.generated.h"

UCLASS(HideCategories = (GeoReferencing), Category = "ArcGISMapsSDK|ArcGISMapView")
class ARCGISMAPSSDK_API AArcGISMapView : public AGeoReferencingSystem
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "GeoReferencing", meta = (WorldContext = "WorldContextObject"))
	static AArcGISMapView* GetArcGISMapView(UObject* WorldContextObject);

	UFUNCTION(BlueprintSetter)
	void SetMapType(EArcGISMapType InMapType);

	UFUNCTION(BlueprintGetter)
	EArcGISMapType GetMapType();

	UFUNCTION(BlueprintSetter)
	void SetOriginLatitude(int32 InLatitude);

	UFUNCTION(BlueprintGetter)
	int32 GetOriginLatitude();

	UFUNCTION(BlueprintSetter)
	void SetOriginLongitude(int32 InLongitude);

	UFUNCTION(BlueprintGetter)
	int32 GetOriginLongitude();

	UFUNCTION(BlueprintSetter)
	void SetOriginAltitude(int32 InAltitude);

	UFUNCTION(BlueprintGetter)
	int32 GetOriginAltitude();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetMapType, BlueprintGetter = GetMapType, Category = "View Mode")
	EArcGISMapType MapType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetOriginLatitude, BlueprintGetter = GetOriginLatitude, Category = "Origin Location")
	int32 Latitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetOriginLongitude, BlueprintGetter = GetOriginLongitude, Category = "Origin Location")
	int32 Longitude;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetOriginAltitude, BlueprintGetter = GetOriginAltitude, Category = "Origin Location")
	int32 Altitude;

	virtual void PostLoad() override;

	virtual void PostActorCreated() override;

protected:
	void BeginPlay() override;

	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Destroyed() override;

private:
	void Initialize();
};
