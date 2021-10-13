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
#include "ArcGISMapsSDK/Components/ArcGISMeshSceneProxy.h"

#include "Engine.h"
#include "Materials/Material.h"
#include "Misc/EngineVersionComparison.h"

FArcGISMeshSceneProxy::FArcGISMeshSceneProxy(const UMeshComponent* InComponent,
											 const TSharedPtr<const FGPUResourceMesh>& GPUResourceMesh,
											 bool bInManualOcclusion,
											 bool bIsVisible)
: FPrimitiveSceneProxy(InComponent),
  MaterialRelevance(InComponent->GetMaterialRelevance(GetScene().GetFeatureLevel()))
{
	bManualOcclusion = bInManualOcclusion;
	bUseDynamicPath = true;
	bVisible = bIsVisible;

	RenderMeshBuffers = MakeUnique<FArcGISVertexBuffer>(GetScene().GetFeatureLevel());

	RenderMeshBuffers->Create(GPUResourceMesh);

	MaterialInterface = InComponent->GetMaterial(0);

	if (MaterialInterface == nullptr)
	{
		MaterialInterface = UMaterial::GetDefaultMaterial(MD_Surface);
	}
}

bool FArcGISMeshSceneProxy::CanBeOccluded() const
{
	return !MaterialRelevance.bDisableDepthTest;
}

void FArcGISMeshSceneProxy::DrawStaticElements(FStaticPrimitiveDrawInterface* PDI)
{
	if (!bUseDynamicPath)
	{
		FMeshBatch MeshBatch;

		SetupMeshBatch(MeshBatch, MaterialInterface->GetRenderProxy(), false);

		PDI->DrawMesh(MeshBatch, FLT_MAX);
	}
}

uint32 FArcGISMeshSceneProxy::GetAllocatedSize(void) const
{
	return (FPrimitiveSceneProxy::GetAllocatedSize());
}

void FArcGISMeshSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views,
												   const FSceneViewFamily& ViewFamily,
												   uint32 VisibilityMap,
												   FMeshElementCollector& Collector) const
{
	// Set up wireframe material (if needed)
	const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;

	FColoredMaterialRenderProxy* WireframeMaterialInstance = NULL;

	if (bWireframe)
	{
		WireframeMaterialInstance = new FColoredMaterialRenderProxy(GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy() : NULL,
																	FLinearColor(0, 0.5f, 1.f));
		Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);
	}

	if (!bVisible)
	{
		return;
	}

	FMaterialRenderProxy* MaterialProxy = bWireframe ? WireframeMaterialInstance : MaterialInterface->GetRenderProxy();

	for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
	{
		if (VisibilityMap & (1 << ViewIndex))
		{
#if UE_VERSION_OLDER_THAN(4, 26, 0)
			bool bForceDynamicPath = IsRichView(*Views[ViewIndex]->Family) || Views[ViewIndex]->Family->EngineShowFlags.Wireframe || IsSelected() ||
									 !IsStaticPathAvailable();
#else
			bool bForceDynamicPath = IsRichView(*Views[ViewIndex]->Family) || Views[ViewIndex]->Family->EngineShowFlags.Wireframe || IsSelected();
#endif

			if (bForceDynamicPath || bUseDynamicPath)
			{
				const FSceneView* View = Views[ViewIndex];

				if (IsInsideFrustum(View))
				{
					FMeshBatch& MeshBatch = Collector.AllocateMesh();

					SetupMeshBatch(MeshBatch, MaterialProxy, bWireframe);
					SetupMeshBatchElement(MeshBatch.Elements[0], Collector);

					Collector.AddMesh(ViewIndex, MeshBatch);
				}
			}
		}
	}
}

uint32 FArcGISMeshSceneProxy::GetMemoryFootprint(void) const
{
	return (sizeof(*this) + GetAllocatedSize());
}

SIZE_T FArcGISMeshSceneProxy::GetTypeHash() const
{
	static size_t UniquePointer;

	return reinterpret_cast<size_t>(&UniquePointer);
}

FPrimitiveViewRelevance FArcGISMeshSceneProxy::GetViewRelevance(const FSceneView* View) const
{
	FPrimitiveViewRelevance Result;
	Result.bDrawRelevance = IsShown(View);
	Result.bShadowRelevance = IsShadowCast(View);

#if UE_VERSION_OLDER_THAN(4, 26, 0)
	bool bForceDynamicPath = !IsStaticPathAvailable() || IsRichView(*View->Family) || IsSelected() || View->Family->EngineShowFlags.Wireframe;
#else
	bool bForceDynamicPath = IsRichView(*View->Family) || IsSelected() || View->Family->EngineShowFlags.Wireframe;
#endif
	Result.bStaticRelevance = !bForceDynamicPath && !bUseDynamicPath;
	Result.bDynamicRelevance = bForceDynamicPath || bUseDynamicPath;

	Result.bRenderInMainPass = ShouldRenderInMainPass();
	Result.bUsesLightingChannels = GetLightingChannelMask() != GetDefaultLightingChannelMask();
	Result.bRenderCustomDepth = ShouldRenderCustomDepth();
	Result.bTranslucentSelfShadow = bCastVolumetricTranslucentShadow;
	MaterialRelevance.SetPrimitiveViewRelevance(Result);

	// Important: we're repositioning, not moving with velocity!
	// Result.bVelocityRelevance = IsMovable() && Result.bOpaqueRelevance&& Result.bRenderInMainPass;
	Result.bVelocityRelevance = 0;
	return Result;
}

bool FArcGISMeshSceneProxy::IsInsideFrustum(const FSceneView* View) const
{
	if (!bManualOcclusion)
	{
		return true;
	}
	auto Box = GetBounds();
	float Radius = Box.SphereRadius * 1.000001 / 1e16;

	auto ShadowFrustum = View->GetDynamicMeshElementsShadowCullFrustum();

	if (ShadowFrustum)
	{
		return ShadowFrustum->IntersectSphere(Box.Origin, Radius);
	}
	else
	{
		return View->ViewFrustum.IntersectSphere(Box.Origin, Radius);
	}
}

void FArcGISMeshSceneProxy::SetInternalVisibility_RenderThread(bool bIsVisible)
{
	check(IsInRenderingThread());

	bVisible = bIsVisible;
}

void FArcGISMeshSceneProxy::SetupMeshBatch(FMeshBatch& MeshBatch, FMaterialRenderProxy* Material, bool bWireframe) const
{
	RenderMeshBuffers->SetupMeshBatch(MeshBatch);

	MeshBatch.bCanApplyViewModeOverrides = true;
	MeshBatch.bWireframe = bWireframe;

	MeshBatch.CastShadow = true;
	MeshBatch.DepthPriorityGroup = SDPG_World;
	MeshBatch.MaterialRenderProxy = Material;
	MeshBatch.ReverseCulling = IsLocalToWorldDeterminantNegative();
	MeshBatch.Type = PT_TriangleList;
}

void FArcGISMeshSceneProxy::SetupMeshBatchElement(FMeshBatchElement& BatchElement, FMeshElementCollector& Collector) const
{
	bool bHasPrecomputedVolumetricLightmap;
	FMatrix PreviousLocalToWorld;
	int32 SingleCaptureIndex;
	bool bOutputVelocity;
	GetScene().GetPrimitiveUniformShaderParameters_RenderThread(GetPrimitiveSceneInfo(), bHasPrecomputedVolumetricLightmap, PreviousLocalToWorld,
																SingleCaptureIndex, bOutputVelocity);

	FDynamicPrimitiveUniformBuffer& DynamicPrimitiveUniformBuffer = Collector.AllocateOneFrameResource<FDynamicPrimitiveUniformBuffer>();

	DynamicPrimitiveUniformBuffer.Set(GetLocalToWorld(), PreviousLocalToWorld, GetBounds(), GetLocalBounds(), true, bHasPrecomputedVolumetricLightmap,
									  DrawsVelocity(), bOutputVelocity);
	BatchElement.PrimitiveUniformBufferResource = &DynamicPrimitiveUniformBuffer.UniformBuffer;
}

void FArcGISMeshSceneProxy::UpdateMesh_RenderThread(const TSharedPtr<const FGPUResourceMesh>& GPUResourceMesh)
{
	check(IsInRenderingThread());

	if (GPUResourceMesh)
	{
		RenderMeshBuffers->Update_RenderThread(GPUResourceMesh);
	}
}

void FArcGISMeshSceneProxy::UpdatePositions_RenderThread(const TArrayView<FVector>& Positions)
{
	check(IsInRenderingThread());
	RenderMeshBuffers->UpdatePositions_RenderThread(Positions);
}
