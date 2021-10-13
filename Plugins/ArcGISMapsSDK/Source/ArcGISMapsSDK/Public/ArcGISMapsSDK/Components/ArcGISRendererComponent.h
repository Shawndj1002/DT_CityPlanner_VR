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

#include "ArcGISRendererComponent.generated.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
class FArcGISRenderer;
}
} // namespace ArcGISMapsSDK

namespace GameEngine
{
namespace View
{
class ArcGISRendererView;
} // namespace View
} // namespace GameEngine
} // namespace Esri

UCLASS(ClassGroup = (ArcGISMapsSDK), meta = (BlueprintSpawnableComponent), Category = "ArcGISMapsSDK|ArcGISRendererComponent")
class ARCGISMAPSSDK_API UArcGISRendererComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UArcGISRendererComponent();

	// UActorComponent
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Get the instance of this component (memory-related callbacks rely on there being a single instance)
	static UArcGISRendererComponent* GetInstance();

	static void HandleLowMemoryWarning();
	static void UpdateMemoryAvailability(long Lotal, long InUse);

	UFUNCTION(BlueprintCallable, Category = "ArcGISMapsSDK|ArcGISRendererComponent")
	void SetRendererView(UArcGISRendererView* InRendererView);

	void SetRendererView(TSharedPtr<Esri::GameEngine::View::ArcGISRendererView> InRendererView);

private:
	static void SetInstance(UArcGISRendererComponent*);
	void PropagateMemoryAvailabilityToRendererView();

	TSharedPtr<Esri::ArcGISMapsSDK::Renderer::FArcGISRenderer> ArcGISRenderer;
	TSharedPtr<Esri::GameEngine::View::ArcGISRendererView> ArcGISRendererView;

	// Used to guard any access to static state that's used to pass memory availability
	static FCriticalSection StaticStateLock;

	// The ArcGISRendererComponent instance, self-registered/unregistered in BeginPlay/EndPlay
	static UArcGISRendererComponent* Instance;

	// Used to pass memory availability metrics when they are provided before ArcGISRendererView is available
	static int64_t TotalSystemMemory;
	static int64_t InUseSystemMemory;
};
