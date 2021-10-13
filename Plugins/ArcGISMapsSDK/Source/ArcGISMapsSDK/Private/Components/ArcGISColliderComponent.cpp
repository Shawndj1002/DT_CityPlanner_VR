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
#include "ArcGISMapsSDK/Components/ArcGISColliderComponent.h"

#include "PhysicsEngine/BodySetup.h"
#include "PhysicsEngine/PhysicsSettings.h"

UArcGISColliderComponent::UArcGISColliderComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	BodySetup = nullptr;
	bDirtyCollisionPositions = false;
}

bool UArcGISColliderComponent::ContainsPhysicsTriMeshData(bool bInUseAllTriData) const
{
	return BodySetup ? true : false;
}

UBodySetup* UArcGISColliderComponent::GetBodySetup()
{
	return BodySetup;
}

UMaterialInterface* UArcGISColliderComponent::GetMaterialFromCollisionFaceIndex(int32 FaceIndex, int32& SectionIndex) const
{
	return nullptr;
}

void UArcGISColliderComponent::UpdateColliderPositions(TArray<FVector> InCollisionPositions)
{
	CollisionPositions = std::move(InCollisionPositions);
	bDirtyCollisionPositions = true;
	BodyInstance.UpdateTriMeshVertices(InCollisionPositions);
}

void UArcGISColliderComponent::CreateCollider(TArray<FVector> InCollisionPositions, TArray<int32> InCollisionTriangles)
{
	UWorld* World = GetWorld();
	const bool bUseAsyncCook = World && World->IsGameWorld();

	if (BodySetup)
	{
		BodySetup->AbortPhysicsMeshAsyncCreation();
		BodySetup = nullptr;
	}

	// The body setup in a template needs to be public since the property is Tnstanced and thus is the archetype of the instance meaning there is a direct reference
	BodySetup = NewObject<UBodySetup>(this, NAME_None, (IsTemplate() ? RF_Public : RF_NoFlags));
	BodySetup->BodySetupGuid = FGuid::NewGuid();
	BodySetup->bGenerateMirroredCollision = false;
	BodySetup->bDoubleSidedGeometry = true;
	BodySetup->CollisionTraceFlag = CTF_UseComplexAsSimple;

	CollisionPositions = std::move(InCollisionPositions);
	CollisionTriangles = std::move(InCollisionTriangles);

	BodySetup->CreatePhysicsMeshesAsync(
		FOnAsyncPhysicsCookFinished::CreateUObject(this, &UArcGISColliderComponent::FinishPhysicsAsyncCook, BodySetup));
}

void UArcGISColliderComponent::FinishPhysicsAsyncCook(bool bSuccess, UBodySetup* FinishedBodySetup)
{
	if (bSuccess)
	{
		RecreatePhysicsState();
	}
}

//#define USE_COLLISION_UV
bool UArcGISColliderComponent::GetPhysicsTriMeshData(struct FTriMeshCollisionData* CollisionData, bool bInUseAllTriData)
{
	int32 VertexBase = 0; // Base vertex index for current section

	CollisionData->Vertices.SetNum(CollisionPositions.Num());

	for (int i = 0; i < CollisionPositions.Num(); ++i)
	{
		CollisionData->Vertices[i] = CollisionPositions[i];
	}

	int ntriangles = CollisionTriangles.Num() / 3;

	CollisionData->Indices.SetNum(ntriangles);
	CollisionData->MaterialIndices.SetNumZeroed(ntriangles);

	for (int i = 0; i < CollisionTriangles.Num(); i += 3)
	{
		FTriIndices& triangle = CollisionData->Indices[i / 3];
		triangle.v0 = CollisionTriangles[i];
		triangle.v1 = CollisionTriangles[i + 1];
		triangle.v2 = CollisionTriangles[i + 2];
	}

#ifdef USE_COLLISION_UV
	bool bCopyUVs = UPhysicsSettings::Get()->bSupportUVFromHitResults;

	if (bCopyUVs)
	{
		CollisionData->UVs.AddZeroed(1);
		CollisionData->UVs[0].SetNum(CollisionPositions.Num());
	}
#endif

	CollisionData->bFlipNormals = true;
	CollisionData->bDeformableMesh = true;
	CollisionData->bFastCook = true;

	return true;
}

void UArcGISColliderComponent::SetEnabled(bool bEnabled)
{
	ECollisionEnabled::Type NewType = bEnabled ? (ECollisionEnabled::QueryAndPhysics) : (ECollisionEnabled::NoCollision);

	if (NewType != BodyInstance.GetCollisionEnabled())
	{
		SetCollisionEnabled(NewType);

		if (bEnabled && bDirtyCollisionPositions)
		{
			BodyInstance.UpdateTriMeshVertices(CollisionPositions);
		}
	}
}
