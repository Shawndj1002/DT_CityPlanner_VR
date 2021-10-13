// Copyright 2021 Esri.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
// You may obtain a copy of the License at: http://www.apache.org/licenses/LICENSE-2.0
//
#pragma once

// Unreal

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// ArcGISMapsSDK

#include "ArcGISMapsSDK/BlueprintNodes/GameEngine/Layers/Base/ArcGISLayerType.h"
#include "ArcGISSamples/OAuthAuthenticationConfigurationMapping.h"

#include "ArcGISMapController.generated.h"

namespace Esri
{
namespace GameEngine
{
namespace Extent
{
class ArcGISExtent;
} // namespace Extent

namespace Layers
{
namespace Base
{
class ArcGISLayer;
} // namespace Base
} // namespace Layers

namespace Map
{
class ArcGISMap;
} // namespace Map
} // namespace GameEngine
} // namespace Esri

class UArcGISRendererComponent;

#define MapControllerCategory

UENUM()
enum EViewMode
{
	Global = 0,
	Local = 1
};

UENUM()
enum EExtentShapes
{
	Square = 0,
	Rectangle = 1,
	Circle = 2
};

UENUM()
enum EBaseMaps
{
	WorldImagery = 0,
	Streets = 1,
	Topographic = 2,
	NationalGeographic = 3,
	Oceans = 4,
	LightGrayCanvas = 5,
	DarkGrayCanvas = 6,
};

USTRUCT()
struct FViewLayer
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	bool bIsVisible;

	UPROPERTY(EditAnywhere)
	float LayerOpacity;

	UPROPERTY(EditAnywhere)
	FString LayerName;

	UPROPERTY(EditAnywhere)
	FString LayerSource;

	UPROPERTY(EditAnywhere)
	EArcGISLayerType LayerType;
};

UCLASS()
class ARCGISSAMPLES_API AArcGISMapController : public AActor
{
	GENERATED_BODY()

public:
	AArcGISMapController();

	// View Mode

	EViewMode GetViewMode();
	void SetViewMode(EViewMode Mode);

	// Origin Location

	double GetOriginLatitude();
	double GetOriginLongitude();
	double GetOriginAltitude();

	void SetOriginLatitude(double Lat);
	void SetOriginLongitude(double Lon);
	void SetOriginAltitude(double Alt);

	// Camera Locations

	double GetCamLatitude();
	double GetCamLongitude();
	double GetCamAltitude();
	double GetCamHeading();
	double GetCamPitch();
	double GetCamRoll();

	void SetCamLatitude(double Lat);
	void SetCamLongitude(double Lon);
	void SetCamAltitude(double Alt);
	void SetCamHeading(double Heading);
	void SetCamPitch(double Pitch);
	void SetCamRoll(double Roll);

	// Map Extent

	double GetExtentLatitude();
	double GetExtentLongitude();
	double GetExtentAltitude();
	double GetExtentWidth();
	double GetExtentLength();
	EExtentShapes GetExtentShape();

	void SetExtentLatitude(double Lat);
	void SetExtentLongitude(double Lon);
	void SetExtentAltitude(double Alt);
	void SetExtentWidth(double Width);
	void SetExtentLength(double Length);
	void SetExtentShape(EExtentShapes Shape);

	// Elevation

	bool GetElevation();
	void SetElevation(bool Toggle);

	// Base Map

	EBaseMaps GetBaseMap();
	void SetBaseMap(EBaseMaps Map);

	// Layers

	TArray<FViewLayer>& GetLayers();

protected:
	// Camera Locations

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	double CamLatitude;

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	double CamLongitude;

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	double CamAltitude;

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	double CamHeading;

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	double CamPitch;

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	double CamRoll;

	// Map Extent

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	double ExtentLatitude;

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	double ExtentLongitude;

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	double ExtentAltitude;

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	double ExtentWidth;

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	double ExtentLength;

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	TEnumAsByte<EExtentShapes> ExtentShape;

	// Base Map

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	TEnumAsByte<EBaseMaps> BaseMap;

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	bool TerrainElevation;

	// Layers

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	TArray<FViewLayer> ViewLayers;

public:
	// Authentication

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	FString APIKey;

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	TArray<UOAuthAuthenticationConfiguration*> OAuthAuthenticationConfigurations;

	UPROPERTY(EditAnywhere, Category = MapControllerCategory)
	TArray<FOAuthAuthenticationConfigurationMapping> OAuthAuthenticationConfigurationMappings;

	// Init

	virtual void BeginPlay() override;

private:
	UArcGISRendererComponent* ArcGISRendererComponent;

	void CreateArcGISMap();
	const FString& GetBaseMapURL();
	void GenerateArcGISLayers(Esri::GameEngine::Map::ArcGISMap& Map, const char* ApiKey);
	bool IsExtentValid();
	Esri::GameEngine::Extent::ArcGISExtent GenerateExtent();
	TSharedPtr<Esri::GameEngine::Layers::Base::ArcGISLayer> CreateArcGISLayerFromViewLayer(const FViewLayer& Layer, const char* ApiKey);
};

#undef MapControllerCategory
