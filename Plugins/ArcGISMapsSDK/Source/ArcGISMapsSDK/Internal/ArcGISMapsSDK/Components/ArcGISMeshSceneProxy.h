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
#include "Materials/MaterialInterface.h"
#include "PrimitiveSceneProxy.h"

#include "ArcGISMapsSDK/Components/ArcGISVertexBuffer.h"
#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceMesh.h"

using namespace Esri::ArcGISMapsSDK::Renderer::GPUResources;

class FArcGISMeshSceneProxy final : public FPrimitiveSceneProxy
{
public:
	FArcGISMeshSceneProxy(const UMeshComponent* InComponent,
						  const TSharedPtr<const FGPUResourceMesh>& GPUResourceMesh,
						  bool bInManualOcclusion,
						  bool bIsVisible);

	uint32 GetAllocatedSize(void) const;
	void SetInternalVisibility_RenderThread(bool bVisible);
	void UpdateMesh_RenderThread(const TSharedPtr<const FGPUResourceMesh>& GPUResourceMesh);
	void UpdatePositions_RenderThread(const TArrayView<FVector>& Positions);

	// FPrimitiveSceneProxy
	bool CanBeOccluded() const override;
	void DrawStaticElements(FStaticPrimitiveDrawInterface* PDI) override;
	void GetDynamicMeshElements(const TArray<const FSceneView*>& Views,
								const FSceneViewFamily& ViewFamily,
								uint32 VisibilityMap,
								FMeshElementCollector& Collector) const override;
	uint32 GetMemoryFootprint(void) const override;
	virtual SIZE_T GetTypeHash() const override;
	FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;

private:
	bool IsInsideFrustum(const FSceneView* View) const;
	void SetupMeshBatch(FMeshBatch& MeshBatch, FMaterialRenderProxy* Material, bool bWireframe) const;
	void SetupMeshBatchElement(FMeshBatchElement& BatchElement, FMeshElementCollector& Collector) const;

	bool bManualOcclusion;
	bool bUseDynamicPath;
	bool bVisible;

	UMaterialInterface* MaterialInterface;
	FMaterialRelevance MaterialRelevance;
	TUniquePtr<FArcGISVertexBuffer> RenderMeshBuffers;
};
