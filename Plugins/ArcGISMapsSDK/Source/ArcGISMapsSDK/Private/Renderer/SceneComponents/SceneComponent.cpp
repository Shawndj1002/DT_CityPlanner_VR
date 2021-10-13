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
#include "ArcGISMapsSDK/Renderer/SceneComponents/SceneComponent.h"

#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceMaterial.h"
#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceMesh.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
namespace SceneComponents
{
FSceneComponent::FSceneComponent(UObject* InOuter) : Outer(InOuter)
{
	CreateRenderMesh();

	if (bPhysics)
	{
		CreateCollider();
	}
}

FSceneComponent::~FSceneComponent()
{
	if (ArcGISMeshComponent && ArcGISMeshComponent->IsValidLowLevel())
	{
		ArcGISMeshComponent->ConditionalBeginDestroy();
	}

	if (ArcGISColliderComponent && ArcGISColliderComponent->IsValidLowLevel())
	{
		ArcGISColliderComponent->ConditionalBeginDestroy();
	}
}

void FSceneComponent::CreateCollider()
{
	if (!ArcGISColliderComponent)
	{
		ArcGISColliderComponent = NewObject<UArcGISColliderComponent>(Outer);

		ArcGISColliderComponent->RegisterComponent();
		ArcGISColliderComponent->SetEnabled(false);
	}
}

void FSceneComponent::CreateRenderMesh()
{
	if (!ArcGISMeshComponent)
	{
		ArcGISMeshComponent = NewObject<UArcGISMeshComponent>(Outer);

		ArcGISMeshComponent->CastShadow = bShadows ? 1 : 0;

		ArcGISMeshComponent->RegisterComponent();
	}

	ArcGISMeshComponent->SetInternalVisibility(false);

	bVisible = false;
}

void FSceneComponent::SetPhysics(bool bUsePhysics)
{
	if (bPhysics != bUsePhysics)
	{
		bPhysics = bUsePhysics;
		if (bPhysics)
		{
			CreateCollider();
		}
	}
}

void FSceneComponent::SetShadows(bool bUseShadows)
{
	if (bShadows != bUseShadows)
	{
		bShadows = bUseShadows;
		ArcGISMeshComponent->CastShadow = bShadows ? 1 : 0;
	}
}

void FSceneComponent::SetVisible(bool bIsVisible)
{
	if (bVisible != bIsVisible)
	{
		bVisible = bIsVisible;

		ArcGISMeshComponent->SetInternalVisibility(bIsVisible);

		if (ArcGISColliderComponent)
		{
			ArcGISColliderComponent->SetEnabled(bIsVisible);
		}
	}
}

void FSceneComponent::SetMaterial(const TSharedPtr<const GPUResources::FGPUResourceMaterial>& GPUResourceMaterial)
{
	if (ArcGISMeshComponent)
	{
		ArcGISMeshComponent->SetMaterial(0, GPUResourceMaterial->GetInstance());
	}
}

void FSceneComponent::SetMesh(const TSharedPtr<const GPUResources::FGPUResourceMesh>& GPUResourceMesh)
{
	ArcGISMeshComponent->SetMesh(GPUResourceMesh);

	UpdateCollider(GPUResourceMesh);

	bMeshCreated = true;
}

void FSceneComponent::UpdatePositions(TArrayView<FVector> Positions)
{
	if (bMeshCreated)
	{
		if (ArcGISColliderComponent)
		{
			TArray<FVector> ColliderVertices(Positions.GetData(), Positions.Num());
			ArcGISColliderComponent->UpdateColliderPositions(std::move(ColliderVertices));
		}
		ArcGISMeshComponent->UpdatePositions(std::move(Positions));
	}
}
void FSceneComponent::UpdateCollider(const TSharedPtr<const GPUResources::FGPUResourceMesh>& GPUResourceMesh)
{
	if (ArcGISColliderComponent)
	{
		TArray<FVector> ColliderVertices(GPUResourceMesh->Positions.GetData(), GPUResourceMesh->Positions.Num());
		if (bMeshCreated)
		{
			ArcGISColliderComponent->UpdateColliderPositions(std::move(ColliderVertices));
		}
		else
		{
			TArray<int32> ColliderTriangles(GPUResourceMesh->Triangles.GetData(), GPUResourceMesh->Triangles.Num());

			ArcGISColliderComponent->CreateCollider(std::move(ColliderVertices), std::move(ColliderTriangles));
		}
	}
}

void FSceneComponent::SetLocation(const FVector& NewLocation)
{
	ArcGISMeshComponent->SetWorldLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

	if (ArcGISColliderComponent)
	{
		ArcGISColliderComponent->SetWorldTransform(ArcGISMeshComponent->GetComponentTransform(), false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void FSceneComponent::SetScale(const FVector& NewScale)
{
	ArcGISMeshComponent->SetWorldScale3D(NewScale);

	if (ArcGISColliderComponent)
	{
		ArcGISColliderComponent->SetWorldTransform(ArcGISMeshComponent->GetComponentTransform(), false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void FSceneComponent::SetRotation(const FQuat& NewRotation)
{
	ArcGISMeshComponent->SetWorldRotation(NewRotation, false, nullptr, ETeleportType::TeleportPhysics);

	if (ArcGISColliderComponent)
	{
		ArcGISColliderComponent->SetWorldTransform(ArcGISMeshComponent->GetComponentTransform(), false, nullptr, ETeleportType::TeleportPhysics);
	}
}
} // namespace SceneComponents
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
