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
#include "ArcGISMapsSDK/Components/ArcGISVertexBuffer.h"

namespace
{
inline void InitOrUpdateResource(FRenderResource* Resource)
{
	if (!Resource->IsInitialized())
	{
		Resource->InitResource();
	}
	else
	{
		Resource->UpdateRHI();
	}
}
} // namespace

FArcGISVertexBuffer::FArcGISVertexBuffer(ERHIFeatureLevel::Type InFeatureLevel) : VertexFactory(InFeatureLevel, "FProcMeshProxySection")
{
}

FArcGISVertexBuffer::~FArcGISVertexBuffer()
{
	PositionVertexBuffer.ReleaseResource();
	StaticMeshVertexBuffer.ReleaseResource();
	ColorVertexBuffer.ReleaseResource();
	IndexBuffer.ReleaseResource();
	VertexFactory.ReleaseResource();
}

void FArcGISVertexBuffer::CopyGPUBufferLocked(FRHIVertexBuffer* RHIVertexBuffer, void* Src, SIZE_T Size)
{
	void* VertexBufferData = RHILockVertexBuffer(RHIVertexBuffer, 0, Size, RLM_WriteOnly);
	FMemory::Memcpy(VertexBufferData, Src, Size);
	RHIUnlockVertexBuffer(RHIVertexBuffer);
}

void FArcGISVertexBuffer::Create(const TSharedPtr<const FGPUResourceMesh>& GPUResourceMesh)
{
	InitFromMeshData(GPUResourceMesh);

	// Enqueue initialization of render resource
	BeginInitResource(&PositionVertexBuffer);
	BeginInitResource(&StaticMeshVertexBuffer);
	BeginInitResource(&ColorVertexBuffer);
	BeginInitResource(&IndexBuffer);
	BeginInitResource(&VertexFactory);
}

void FArcGISVertexBuffer::FetchBuffersFromMeshData(const TSharedPtr<const FGPUResourceMesh>& GPUResourceMesh)
{
	static FVector4 XAxisTangent(1, 0, 0, 0);

	auto NumVertices = GPUResourceMesh->Positions.Num();

	for (int32 i = 0; i < NumVertices; ++i)
	{
		PositionVertexBuffer.VertexPosition(i) = GPUResourceMesh->Positions[i];
	}

	for (int32 i = 0; i < FMath::Min(NumVertices, GPUResourceMesh->Normals.Num()); ++i)
	{
		const auto& TangentZ = GPUResourceMesh->Normals[i];
		const FVector4* TangentX = &XAxisTangent;

		if (i < GPUResourceMesh->Tangents.Num())
		{
			TangentX = &GPUResourceMesh->Tangents[i];
		}

		const auto TangentY = FVector::CrossProduct(*TangentX, TangentZ) * ((*TangentX).W < 0 ? -1 : 1);
		StaticMeshVertexBuffer.SetVertexTangents(i, *TangentX, TangentY, TangentZ);
	}

	for (int32 i = 0; i < FMath::Min(NumVertices, GPUResourceMesh->Colors.Num()); ++i)
	{
		ColorVertexBuffer.VertexColor(i) = FColor(GPUResourceMesh->Colors[i]);
	}

	int32 NumTexCoords = FMath::Min(StaticMeshVertexBuffer.GetNumTexCoords(), (uint32)GPUResourceMesh->UVs.Num());

	for (int32 j = 0; j < NumTexCoords; ++j)
	{
		const auto& UVs = GPUResourceMesh->UVs[j];
		if (UVs.Dimensions == 1 || UVs.Dimensions == 2)
		{
			auto NumUVs = FMath::Min(NumVertices, static_cast<int32>(UVs.Elements.Num() / UVs.Dimensions));
			for (int32 i = 0; i < NumUVs; ++i)
			{
				if (UVs.Dimensions == 1)
				{
					StaticMeshVertexBuffer.SetVertexUV(i, j, {UVs.Elements[i], 0});
				}
				else
				{
					StaticMeshVertexBuffer.SetVertexUV(i, j, {UVs.Elements[i * 2], UVs.Elements[i * 2 + 1]});
				}
			}
		}
	}
}

void FArcGISVertexBuffer::InitFromMeshData(const TSharedPtr<const FGPUResourceMesh>& GPUResourceMesh, uint32 LightMapIndex)
{
	IndexBuffer.Indices.SetNum(GPUResourceMesh->Triangles.Num());
	FMemory::Memcpy(IndexBuffer.Indices.GetData(), GPUResourceMesh->Triangles.GetData(), GPUResourceMesh->Triangles.Num() * sizeof(int32));

	int32 NumVertices = GPUResourceMesh->Positions.Num();
	int32 NumUVs = GPUResourceMesh->UVs.Num();

	PositionVertexBuffer.Init(NumVertices);
	StaticMeshVertexBuffer.SetUseHighPrecisionTangentBasis(true);
	StaticMeshVertexBuffer.SetUseFullPrecisionUVs(GPUResourceMesh->bUseFullPrecisionUVs);
	StaticMeshVertexBuffer.Init(NumVertices, NumUVs);
	ColorVertexBuffer.Init(NumVertices);

	for (int32 i = 0; i < NumVertices; ++i)
	{
		StaticMeshVertexBuffer.SetVertexTangents(i, FVector(0, 1, 0), FVector(1, 0, 0), FVector(0, 0, 1));

		for (int32 j = 0; j < NumUVs; ++j)
		{
			StaticMeshVertexBuffer.SetVertexUV(i, j, FVector2D(0, 0));
		}

		ColorVertexBuffer.VertexColor(i) = FColor(0);
	}

	FetchBuffersFromMeshData(GPUResourceMesh);

	ENQUEUE_RENDER_COMMAND(StaticMeshVertexBuffersLegacyInit)
	([&, LightMapIndex](FRHICommandListImmediate& RHICmdList) {
		InitOrUpdateResource(&PositionVertexBuffer);
		InitOrUpdateResource(&StaticMeshVertexBuffer);
		InitOrUpdateResource(&ColorVertexBuffer);

		FLocalVertexFactory::FDataType Data;

		PositionVertexBuffer.BindPositionVertexBuffer(&VertexFactory, Data);
		StaticMeshVertexBuffer.BindTangentVertexBuffer(&VertexFactory, Data);
		StaticMeshVertexBuffer.BindPackedTexCoordVertexBuffer(&VertexFactory, Data);
		StaticMeshVertexBuffer.BindLightMapVertexBuffer(&VertexFactory, Data, LightMapIndex);
		ColorVertexBuffer.BindColorVertexBuffer(&VertexFactory, Data);

		VertexFactory.SetData(Data);

		InitOrUpdateResource(&VertexFactory);
	});
}

void FArcGISVertexBuffer::SetupMeshBatch(FMeshBatch& MeshBatch) const
{
	MeshBatch.VertexFactory = &VertexFactory;

	FMeshBatchElement& BatchElement = MeshBatch.Elements[0];

	BatchElement.FirstIndex = 0;
	BatchElement.IndexBuffer = &IndexBuffer;
	BatchElement.MaxVertexIndex = PositionVertexBuffer.GetNumVertices() - 1;
	BatchElement.MinVertexIndex = 0;
	BatchElement.NumPrimitives = IndexBuffer.Indices.Num() / 3;
}

void FArcGISVertexBuffer::Update_RenderThread(const TSharedPtr<const FGPUResourceMesh>& GPUResourceMesh)
{
	FetchBuffersFromMeshData(GPUResourceMesh);

	CopyGPUBufferLocked(PositionVertexBuffer.VertexBufferRHI, PositionVertexBuffer.GetVertexData(),
						PositionVertexBuffer.GetNumVertices() * PositionVertexBuffer.GetStride());
	CopyGPUBufferLocked(ColorVertexBuffer.VertexBufferRHI, ColorVertexBuffer.GetVertexData(),
						ColorVertexBuffer.GetNumVertices() * ColorVertexBuffer.GetStride());
	CopyGPUBufferLocked(StaticMeshVertexBuffer.TangentsVertexBuffer.VertexBufferRHI, StaticMeshVertexBuffer.GetTangentData(),
						StaticMeshVertexBuffer.GetTangentSize());
	CopyGPUBufferLocked(StaticMeshVertexBuffer.TexCoordVertexBuffer.VertexBufferRHI, StaticMeshVertexBuffer.GetTexCoordData(),
						StaticMeshVertexBuffer.GetTexCoordSize());
}

void FArcGISVertexBuffer::UpdatePositions_RenderThread(const TArrayView<FVector>& Positions)
{
	auto NumVertices = Positions.Num();
	for (int32 i = 0; i < NumVertices; ++i)
	{
		PositionVertexBuffer.VertexPosition(i) = Positions[i];
	}
	CopyGPUBufferLocked(PositionVertexBuffer.VertexBufferRHI, PositionVertexBuffer.GetVertexData(),
						PositionVertexBuffer.GetNumVertices() * PositionVertexBuffer.GetStride());
}
