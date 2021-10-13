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
#include "ArcGISMapsSDK/Components/ArcGISRendererComponent.h"

#include "Engine/World.h"
#include "EngineUtils.h"
#include "GenericPlatform/GenericPlatformMisc.h"

#if PLATFORM_ANDROID
#include "GenericPlatform/GenericPlatformCrashContext.h"
#endif
#if PLATFORM_IOS
#include "IOS/IOSPlatformMisc.h"
#endif

#include "ArcGISMapsSDK/API/GameEngine/View/ArcGISRendererView.h"
#include "ArcGISMapsSDK/BlueprintNodes/GameEngine/View/ArcGISRendererView.h"
#include "ArcGISMapsSDK/Renderer/ArcGISRenderer.h"

// Static members
FCriticalSection UArcGISRendererComponent::StaticStateLock;
int64_t UArcGISRendererComponent::TotalSystemMemory{-1};
int64_t UArcGISRendererComponent::InUseSystemMemory{-1};
UArcGISRendererComponent* UArcGISRendererComponent::Instance{nullptr};

void LowMemoryStaticCallback(const FGenericMemoryWarningContext&)
{
	UArcGISRendererComponent::HandleLowMemoryWarning();
}

UArcGISRendererComponent* UArcGISRendererComponent::GetInstance()
{
	FScopeLock ScopeLock(&StaticStateLock);
	ensureMsgf(UArcGISRendererComponent::Instance, TEXT("UArcGISRendererComponent::Instance is unset in GetInstance"));
	return UArcGISRendererComponent::Instance;
}

void UArcGISRendererComponent::SetInstance(UArcGISRendererComponent* value)
{
	FScopeLock ScopeLock(&StaticStateLock);
	if (value)
	{
		ensureMsgf(!UArcGISRendererComponent::Instance, TEXT("UArcGISRendererComponent::Instance is already set in SetInstance"));
	}
	else
	{
		ensureMsgf(UArcGISRendererComponent::Instance, TEXT("UArcGISRendererComponent::Instance is not set in SetInstance(nullptr)"));
	}
	UArcGISRendererComponent::Instance = value;
}

UArcGISRendererComponent::UArcGISRendererComponent() : Super()
{
	PrimaryComponentTick.bCanEverTick = true;

	// The renderer has to be updated at the beginning of the frame, before any other component, so the reposition is done at the beginning too.
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void UArcGISRendererComponent::BeginPlay()
{
	Super::BeginPlay();

	SetInstance(this);
	FPlatformMisc::SetMemoryWarningHandler(LowMemoryStaticCallback);
}

void UArcGISRendererComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ArcGISRenderer.Reset();
	ArcGISRendererView.Reset();

	FGenericPlatformMisc::SetMemoryWarningHandler(nullptr);
	SetInstance(nullptr);

	Super::EndPlay(EndPlayReason);
}

void UArcGISRendererComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ArcGISRenderer)
	{
		ArcGISRenderer->Update();
	}
}

void UArcGISRendererComponent::SetRendererView(UArcGISRendererView* InRendererView)
{
	SetRendererView(InRendererView->APIObject);
}

void UArcGISRendererComponent::SetRendererView(TSharedPtr<Esri::GameEngine::View::ArcGISRendererView> InRendererView)
{
	FScopeLock ScopeLock(&StaticStateLock);

	ArcGISRendererView = std::move(InRendererView);

	PropagateMemoryAvailabilityToRendererView();

	ArcGISRenderer = ::MakeShared<Esri::ArcGISMapsSDK::Renderer::FArcGISRenderer>(ArcGISRendererView, this);
}

void UArcGISRendererComponent::PropagateMemoryAvailabilityToRendererView()
{
	FScopeLock ScopeLock(&StaticStateLock);
	if (ArcGISRendererView)
	{
		ArcGISRendererView->UpdateMemoryAvailability(UArcGISRendererComponent::TotalSystemMemory, UArcGISRendererComponent::InUseSystemMemory);
	}
}

/*static*/ void UArcGISRendererComponent::UpdateMemoryAvailability(long Total, long InUse)
{
	FScopeLock ScopeLock(&StaticStateLock);

	UArcGISRendererComponent::TotalSystemMemory = Total;
	UArcGISRendererComponent::InUseSystemMemory = InUse;

	auto SafeInstance = GetInstance();
	if (SafeInstance)
	{
		SafeInstance->PropagateMemoryAvailabilityToRendererView();
	}
}

/*static*/ void UArcGISRendererComponent::HandleLowMemoryWarning()
{
	FScopeLock ScopeLock(&StaticStateLock);
	//NB: if low memory warning arrives before there's a UArcGISRendererComponent, it will be ignored.
	auto SafeInstance = GetInstance();
	if (SafeInstance && SafeInstance->ArcGISRendererView)
	{
		SafeInstance->ArcGISRendererView->HandleLowMemoryWarning();
	}
}
