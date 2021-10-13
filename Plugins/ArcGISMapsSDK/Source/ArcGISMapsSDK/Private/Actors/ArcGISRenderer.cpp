// COPYRIGHT 1995-2021 ESRI
// TRADE SECRETS: ESRI PROPRIETARY AND CONFIDENTIAL
// Unpublished material - all rights reserved under the
// Copyright Laws of the United States and applicable international
// laws, treaties, and conventions.
//
// For additional information, contact:
// Environmental Systems Research Institute, Inc.
// Attn: Contracts and Legal Services Department
// 380 New York Street
// Redlands, California, 92373
// USA
//
// email: contracts@esri.com

#include "ArcGISMapsSDK/Actors/ArcGISRenderer.h" // IWYU pragma: associated

AArcGISRenderer::AArcGISRenderer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ArcGISRendererComponent = CreateDefaultSubobject<UArcGISRendererComponent>(TEXT("ArcGISRenderer"));
#if WITH_EDITOR
	ArcGISRendererComponent->bVisualizeComponent = true;
#endif

	SetRootComponent(ArcGISRendererComponent);
}

void AArcGISRenderer::SetRendererView(UArcGISRendererView* InRendererView)
{
	ArcGISRendererComponent->SetRendererView(InRendererView);
}
