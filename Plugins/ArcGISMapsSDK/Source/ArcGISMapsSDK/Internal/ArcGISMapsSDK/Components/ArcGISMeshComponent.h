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

#include "Components/MeshComponent.h"
#include "CoreMinimal.h"

#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceMesh.h"

#include "ArcGISMeshComponent.generated.h"

using namespace Esri::ArcGISMapsSDK::Renderer::GPUResources;

UCLASS()
class UArcGISMeshComponent : public UMeshComponent
{
	GENERATED_BODY()

public:
	UArcGISMeshComponent(const FObjectInitializer& ObjectInitializer);

	void SetMesh(const TSharedPtr<const FGPUResourceMesh>& InGPUResourceMesh);
	void UpdatePositions(TArrayView<FVector> Positions);

	void SetInternalVisibility(bool bIsVisible);
	void SetIgnoreCulling(bool bInIgnoreCulling);

private:
	// USceneComponent
	FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	// UPrimitiveComponent
	FPrimitiveSceneProxy* CreateSceneProxy() override;
	int32 GetNumMaterials() const override;

	void UpdateLocalBounds(const TSharedPtr<const FGPUResourceMesh>& InGPUResourceMesh);

	bool bIgnoreCulling = true;
	bool bManualOcclusion = true;
	bool bVisible;

	FBoxSphereBounds LocalBounds;
	TSharedPtr<const FGPUResourceMesh> GPUResourceMesh;
};
