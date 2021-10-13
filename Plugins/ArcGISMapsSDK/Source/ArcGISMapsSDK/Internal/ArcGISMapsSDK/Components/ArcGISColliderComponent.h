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

#include "Components/ActorComponent.h"
#include "Components/MeshComponent.h"
#include "CoreMinimal.h"
#include "Interfaces/Interface_CollisionDataProvider.h"

#include "ArcGISColliderComponent.generated.h"

UCLASS(ClassGroup = (ArcGISMapsSDK), meta = (BlueprintSpawnableComponent), Category = "ArcGISMapsSDK|ArcGISColliderComponent")
class UArcGISColliderComponent : public UMeshComponent, public IInterface_CollisionDataProvider
{
	GENERATED_BODY()

public:
	UArcGISColliderComponent(const FObjectInitializer& ObjectInitializer);

	void CreateCollider(TArray<FVector> InCollisionPositions, TArray<int32> InCollisionTriangles);
	void UpdateColliderPositions(TArray<FVector> InCollisionPositions);
	void SetEnabled(bool bEnabled);

private:
	void FinishPhysicsAsyncCook(bool bSuccess, UBodySetup* FinishedBodySetup);

	virtual bool GetPhysicsTriMeshData(struct FTriMeshCollisionData* CollisionData, bool bInUseAllTriData) override;
	virtual bool ContainsPhysicsTriMeshData(bool bInUseAllTriData) const override;
	virtual bool WantsNegXTriMesh() override
	{
		return false;
	}
	virtual class UBodySetup* GetBodySetup() override;
	virtual UMaterialInterface* GetMaterialFromCollisionFaceIndex(int32 FaceIndex, int32& SectionIndex) const override;

	bool bDirtyCollisionPositions;

	UBodySetup* BodySetup;
	TArray<FVector> CollisionPositions;
	TArray<int32> CollisionTriangles;
};
