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
#include "ArcGISMapsSDK/Renderer/ArcGISRenderer.h"

#include "ArcGISMapsSDK/Renderer/ArcGISRendererImpl.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
FArcGISRenderer::FArcGISRenderer(TSharedPtr<Esri::GameEngine::View::ArcGISRendererView> RendererView, UObject* Outer)
: PImpl(::MakeUnique<FArcGISRenderer::Impl>(std::move(RendererView), Outer))
{
}

FArcGISRenderer::~FArcGISRenderer() = default;

void FArcGISRenderer::Update()
{
	PImpl->Update();
}
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
