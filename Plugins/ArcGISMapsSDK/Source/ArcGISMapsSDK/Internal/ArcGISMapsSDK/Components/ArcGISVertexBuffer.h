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

#include "CoreMinimal.h"
#include "DynamicMeshBuilder.h"
#include "Rendering/ColorVertexBuffer.h"
#include "Rendering/PositionVertexBuffer.h"
#include "Rendering/StaticMeshVertexBuffer.h"

#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceMesh.h"

using namespace Esri::ArcGISMapsSDK::Renderer::GPUResources;

class FArcGISVertexBuffer
{
public:
	FArcGISVertexBuffer(ERHIFeatureLevel::Type InFeatureLevel);
	~FArcGISVertexBuffer();

	void Create(const TSharedPtr<const FGPUResourceMesh>& GPUResourceMesh);
	void SetupMeshBatch(FMeshBatch& MeshBatch) const;
	void Update_RenderThread(const TSharedPtr<const FGPUResourceMesh>& GPUResourceMesh);
	void UpdatePositions_RenderThread(const TArrayView<FVector>& Positions);

private:
	void CopyGPUBufferLocked(FRHIVertexBuffer* RHIVertexBuffer, void* Src, SIZE_T Size);
	void FetchBuffersFromMeshData(const TSharedPtr<const FGPUResourceMesh>& GPUResourceMesh);
	void InitFromMeshData(const TSharedPtr<const FGPUResourceMesh>& GPUResourceMesh, uint32 LightMapIndex = 0);

	FColorVertexBuffer ColorVertexBuffer;
	FDynamicMeshIndexBuffer32 IndexBuffer;
	FPositionVertexBuffer PositionVertexBuffer;
	FStaticMeshVertexBuffer StaticMeshVertexBuffer;
	FLocalVertexFactory VertexFactory;
};
