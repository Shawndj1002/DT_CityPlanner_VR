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
#include "ArcGISMapsSDK/Components/ArcGISMeshComponent.h"

#include "ArcGISMapsSDK/Components/ArcGISMeshSceneProxy.h"

UArcGISMeshComponent::UArcGISMeshComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bVisible = true;
}

void UArcGISMeshComponent::SetIgnoreCulling(bool bInIgnoreCulling)
{
	bIgnoreCulling = bInIgnoreCulling;
	if (GPUResourceMesh)
	{
		MarkRenderTransformDirty();
	}
}

FBoxSphereBounds UArcGISMeshComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBoxSphereBounds Ret = LocalBounds.TransformBy(LocalToWorld);

	float CullingScale = (bManualOcclusion || bIgnoreCulling) ? 1e16 : 1.0;

	Ret.BoxExtent *= BoundsScale * CullingScale;
	Ret.SphereRadius *= BoundsScale * CullingScale;

	return Ret;
}

FPrimitiveSceneProxy* UArcGISMeshComponent::CreateSceneProxy()
{
	if (!GPUResourceMesh)
	{
		return nullptr;
	}

	bool bUseManualOcclusion = (!bIgnoreCulling && bManualOcclusion);

	auto Proxy = new FArcGISMeshSceneProxy(this, GPUResourceMesh, bUseManualOcclusion, bVisible);

	GPUResourceMesh.Reset();

	return Proxy;
}

void UArcGISMeshComponent::SetInternalVisibility(bool bIsVisible)
{
	bVisible = bIsVisible;

	if (SceneProxy && !IsRenderStateDirty())
	{
		FArcGISMeshSceneProxy* Proxy = (FArcGISMeshSceneProxy*)SceneProxy;
		ENQUEUE_RENDER_COMMAND(FRenderMeshSetInternalVisibility)
		([Proxy, bIsVisible](FRHICommandListImmediate& RHICmdList) {
			Proxy->SetInternalVisibility_RenderThread(bIsVisible);
		});
	}
}

int32 UArcGISMeshComponent::GetNumMaterials() const
{
	return 1;
}

void UArcGISMeshComponent::UpdateLocalBounds(const TSharedPtr<const FGPUResourceMesh>& InGPUResourceMesh)
{
	FBox Box;

	Box.Init();

	for (int32 i = 0; i < InGPUResourceMesh->Positions.Num(); ++i)
	{
		Box += InGPUResourceMesh->Positions[i];
	}

	LocalBounds =
		Box.IsValid ? FBoxSphereBounds(Box) : FBoxSphereBounds(FVector(0, 0, 0), FVector(0, 0, 0), 0); // fallback to reset box sphere bounds

	UpdateBounds();
	MarkRenderTransformDirty();
}

void UArcGISMeshComponent::SetMesh(const TSharedPtr<const FGPUResourceMesh>& InGPUResourceMesh)
{
	// If we have a valid proxy and it is not pending recreation
	if (SceneProxy && !IsRenderStateDirty())
	{
		GPUResourceMesh.Reset();
		FArcGISMeshSceneProxy* Proxy = (FArcGISMeshSceneProxy*)SceneProxy;

		ENQUEUE_RENDER_COMMAND(FRenderMeshUpdate)
		([Proxy, InGPUResourceMesh](FRHICommandListImmediate& RHICmdList) {
			// Render thread
			Proxy->UpdateMesh_RenderThread(InGPUResourceMesh);
		});
	}
	else
	{
		GPUResourceMesh = InGPUResourceMesh;
	}

	UpdateLocalBounds(InGPUResourceMesh);
}

void UArcGISMeshComponent::UpdatePositions(TArrayView<FVector> Positions)
{
	if (SceneProxy && !IsRenderStateDirty())
	{
		FArcGISMeshSceneProxy* Proxy = (FArcGISMeshSceneProxy*)SceneProxy;

		ENQUEUE_RENDER_COMMAND(FRenderMeshUpdate)
		([Proxy, Positions = std::move(Positions)](FRHICommandListImmediate& RHICmdList) {
			// Render thread
			Proxy->UpdatePositions_RenderThread(Positions);
		});
	}
	else if (GPUResourceMesh)
	{
		auto MeshData = ::MakeShared<FGPUResourceMesh>();
		*MeshData = {GPUResourceMesh->Normals,
					 GPUResourceMesh->Tangents,
					 GPUResourceMesh->Triangles,
					 GPUResourceMesh->UVs,
					 GPUResourceMesh->Colors,
					 Positions,
					 GPUResourceMesh->bUseFullPrecisionUVs};
		SetMesh(GPUResourceMesh);
	}
}
