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

#include "Components/SceneComponent.h"
#include "CoreMinimal.h"

#include "ArcGISMapsSDK/API/GameEngine/Location/ArcGISPosition.h"
#include "ArcGISMapsSDK/API/GameEngine/Location/ArcGISRotation.h"
#include "ArcGISMapsSDK/BlueprintNodes/GameEngine/Location/ArcGISPosition.h"
#include "ArcGISMapsSDK/BlueprintNodes/GameEngine/Location/ArcGISRotation.h"

#include "ArcGISLocationComponent.generated.h"

USTRUCT()
struct FArcGISPositionInternal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Latitude;

	UPROPERTY(EditAnywhere)
	float Longitude;

	UPROPERTY(EditAnywhere)
	float Altitude;
};

USTRUCT()
struct FArcGISRotationInternal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Heading;

	UPROPERTY(EditAnywhere)
	float Pitch;

	UPROPERTY(EditAnywhere)
	float Roll;
};

UCLASS(ClassGroup = (ArcGISMapsSDK), meta = (BlueprintSpawnableComponent), Category = "ArcGISMapsSDK|ArcGISLocationComponent")
class ARCGISMAPSSDK_API UArcGISLocationComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UArcGISLocationComponent();

	UFUNCTION(BlueprintCallable, Category = "ArcGISMapsSDK|ArcGISLocationComponent")
	UArcGISPosition* GetPosition();
	UFUNCTION(BlueprintCallable, Category = "ArcGISMapsSDK|ArcGISLocationComponent")
	void SetPosition(UArcGISPosition* InPosition);

	UFUNCTION(BlueprintCallable, Category = "ArcGISMapsSDK|ArcGISLocationComponent")
	UArcGISRotation* GetRotation();
	UFUNCTION(BlueprintCallable, Category = "ArcGISMapsSDK|ArcGISLocationComponent")
	void SetRotation(UArcGISRotation* InRotation);

	void SetPosition(const Esri::GameEngine::Location::ArcGISPosition& position);
	void SetRotation(const Esri::GameEngine::Location::ArcGISRotation& rotation);

	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport = ETeleportType::None) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#if WITH_EDITORONLY_DATA
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	UPROPERTY(EditAnywhere, Category = "ArcGISLocationComponent")
	FArcGISPositionInternal Position;

	UPROPERTY(EditAnywhere, Category = "ArcGISLocationComponent")
	FArcGISRotationInternal Rotation;

	virtual void BeginPlay() override;

private:
	void UpdateTransform();

	bool bInternalHasChanged{false};
	bool bTransformHasChanged{false};
};
