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

#include "ArcGISMapsSDK/Components/ArcGISColliderComponent.h"
#include "ArcGISMapsSDK/Components/ArcGISMeshComponent.h"
#include "ArcGISMapsSDK/Renderer/GPUResources/GPUResourceMaterial.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
namespace SceneComponents
{
class FSceneComponent final
{
public:
	FSceneComponent(UObject* outer);
	~FSceneComponent();

	void SetLocation(const FVector& NewLocation);
	void SetRotation(const FQuat& NewRotation);
	void SetScale(const FVector& NewScale);
	void SetMaterial(const TSharedPtr<const GPUResources::FGPUResourceMaterial>& GPUResourceMaterial);
	void SetMesh(const TSharedPtr<const GPUResources::FGPUResourceMesh>& GPUResourceMesh);
	void UpdatePositions(TArrayView<FVector> Positions);
	void SetPhysics(bool bUsePhysics);
	void SetShadows(bool bUseShadows);
	void SetVisible(bool bIsVisible);

private:
	void CreateCollider();
	void CreateRenderMesh();
	void UpdateCollider(const TSharedPtr<const GPUResources::FGPUResourceMesh>& GPUResourceMesh);

	bool bMeshCreated = false;
	bool bPhysics = false;
	bool bShadows = true;
	bool bVisible = false;

	UArcGISMeshComponent* ArcGISMeshComponent = nullptr;
	UArcGISColliderComponent* ArcGISColliderComponent = nullptr;
	UObject* Outer;
};
} // namespace SceneComponents
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
