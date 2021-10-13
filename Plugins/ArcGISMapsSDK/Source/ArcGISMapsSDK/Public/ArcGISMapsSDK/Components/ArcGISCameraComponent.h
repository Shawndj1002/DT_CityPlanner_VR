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

#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"

#include "ArcGISMapsSDK/API/GameEngine/View/ArcGISRendererView.h"

#include "ArcGISCameraComponent.generated.h"

UCLASS(ClassGroup = (ArcGISMapsSDK), meta = (BlueprintSpawnableComponent), Category = "ArcGISMapsSDK|ArcGISCameraComponent")
class ARCGISMAPSSDK_API UArcGISCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UArcGISCameraComponent();

	// UActorComponent
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "ArcGISMapsSDK|ArcGISCameraComponent")
	void SetRendererView(UArcGISRendererView* InRendererView);

	void SetRendererView(TSharedPtr<Esri::GameEngine::View::ArcGISRendererView> InRendererView);

protected:
	TSharedPtr<Esri::GameEngine::Camera::ArcGISCamera> ArcGISCamera;
	TSharedPtr<Esri::GameEngine::View::ArcGISRendererView> ArcGISRendererView;

private:
	void PushPosition();
	void UpdateViewport();

	float Fov = 0;
	int32 ViewportSizeX = 0;
	int32 ViewportSizeY = 0;
};
