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
#include "ArcGISMapsSDK/API/GameEngine/View/ArcGISRendererViewImpl.h"

#include "ArcGISMapsSDK/API/ArcGISRuntime/Geometry/Point.h"
#include "ArcGISMapsSDK/API/ArcGISRuntime/Geometry/SpatialReference.h"
#include "ArcGISMapsSDK/API/ArcGISRuntime/MapView/Camera.h"
#include "ArcGISMapsSDK/API/ArcGISRuntime/MapView/GlobeModel.h"
#include "ArcGISMapsSDK/API/ArcGISRuntime/MapView/LayerViewState.h"
#include "ArcGISMapsSDK/API/ArcGISRuntime/MapView/TransformationMatrix.h"
#include "ArcGISMapsSDK/API/GameEngine/GameEngineType.h"
#include "ArcGISMapsSDK/API/GameEngine/GameEngineView.h"
#include "ArcGISMapsSDK/API/GameEngine/GameEngineViewViewState.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/DecodedRenderCommandQueue.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/RenderCommandServer.h"
#include "ArcGISMapsSDK/API/GameEngine/View/Event/ArcGISLayerViewStateChangedEvent.h"
#include "ArcGISMapsSDK/API/GameEngine/View/Event/ArcGISRendererViewStateChangedEvent.h"
#include "ArcGISMapsSDK/API/GameEngine/View/State/ArcGISLayerViewState.h"
#include "ArcGISMapsSDK/API/GameEngine/View/State/ArcGISRendererViewState.h"
#include "ArcGISMapsSDK/API/GameEngine/View/State/ArcGISRendererViewStatus.h"
#include "ArcGISMapsSDK/Utils/Debug.h"
namespace Esri
{
namespace GameEngine
{
namespace View
{
ArcGISRendererView::Impl::Impl(TSharedPtr<Esri::GameEngine::Map::ArcGISMap> ArcGISMap,
							   const TSharedPtr<const Esri::GameEngine::Camera::ArcGISCamera>& ArcGISCamera,
							   ArcGISRendererViewOptions Options)
{
	GameEngineView = ::MakeShared<Esri::GameEngine::GameEngineView>(Esri::GameEngine::GameEngineType::UnrealEngine,
																	Esri::ArcGISRuntime::MapView::GlobeModel::Ellipsoid);

	// no-op call to set memory availability. TODO: provide useful values
	GameEngineView->SetMemoryAvailability(-1, -1, -1, -1);

	SetOptions(Options);
	SetMap(std::move(ArcGISMap));

	SetCamera(ArcGISCamera);
}

void ArcGISRendererView::Impl::SetCamera(const TSharedPtr<const Esri::GameEngine::Camera::ArcGISCamera>& ArcGISCamera)
{
	const auto Position = ArcGISCamera->GetPosition();
	const auto Point =
		Esri::ArcGISRuntime::Geometry::Point(Position.GetY(), Position.GetX(), Position.GetZ(),
											 Esri::ArcGISRuntime::Geometry::SpatialReference::WGS84()); //  Position.GetSpatialReference());

	const auto Orientation = ArcGISCamera->GetOrientation();

	const auto RtcCamera = Esri::ArcGISRuntime::MapView::Camera(Point, Orientation.GetHeading(), Orientation.GetPitch(), Orientation.GetRoll());

	GameEngineView->SetCamera(RtcCamera);
}

void ArcGISRendererView::Impl::SetMap(TSharedPtr<Esri::GameEngine::Map::ArcGISMap> ArcGISMap)
{
	Map = std::move(ArcGISMap);

	try
	{
		GameEngineView->SetMap(*Map);
	}
	catch (Esri::Unreal::Exception& Ex)
	{
		Esri::ArcGISMapsSDK::Utils::Debug::WriteLine("ArcGISRendererView::Impl::SetMap: %s", Ex.what());
	}
}

void ArcGISRendererView::Impl::SetOptions(ArcGISRendererViewOptions Options)
{
	GameEngineView->SetViewOptions(Options);
}

void ArcGISRendererView::Impl::UpdateMemoryAvailability(long Total, long InUse)
{
	GameEngineView->SetMemoryAvailability(Total, InUse, -1, -1);
}

RenderCommandQueue::DecodedRenderCommandQueue ArcGISRendererView::Impl::GetCurrentDecodedRenderCommandQueue()
{
	return RenderCommandQueue::DecodedRenderCommandQueue(GameEngineView->GetRenderCommandServer().GetRenderCommands());
}

TSharedPtr<const Esri::GameEngine::GameEngineView> ArcGISRendererView::Impl::GetGameEngineView() const
{
	return GameEngineView;
}

void ArcGISRendererView::Impl::SetViewportProperties(uint32 ViewportWidthPixels,
													 uint32 ViewportHeightPixels,
													 float HorizontalFieldOfViewDegrees,
													 float VerticalFieldOfViewDegrees,
													 float VerticalDistortionFactor)
{
	GameEngineView->SetViewportProperties(ViewportWidthPixels, ViewportHeightPixels, HorizontalFieldOfViewDegrees, VerticalFieldOfViewDegrees,
										  VerticalDistortionFactor);
}

void ArcGISRendererView::Impl::HandleLowMemoryWarning()
{
	if (GameEngineView)
	{
		GameEngineView->HandleLowMemoryWarning();
	}
}

void ArcGISRendererView::Impl::SetViewStateChanged(Esri::GameEngine::View::Event::ArcGISRendererViewStateChangedEvent Callback)
{
	if (GameEngineView)
	{
		GameEngineView->SetViewViewStateChanged([this, Callback](GameEngineViewViewState& State) {
			if (this->GameEngineView)
			{
				auto GameEngineViewViewState = std::move(State);
				auto ArcGISRendererViewState = Esri::GameEngine::View::State::ArcGISRendererViewState(
					std::move(*reinterpret_cast<State::ArcGISRendererViewState*>(&GameEngineViewViewState)));

				Callback(ArcGISRendererViewState);
			}
		});
	}
}

void ArcGISRendererView::Impl::SetLayerStateChanged(Esri::GameEngine::View::Event::ArcGISLayerViewStateChangedEvent Callback)
{
	if (GameEngineView)
	{
		GameEngineView->SetLayerViewStateChanged([this, Callback](Layers::Base::ArcGISLayer& Layer, ArcGISRuntime::MapView::LayerViewState& State) {
			if (this->GameEngineView)
			{
				auto ArcGISLayerViewState = Esri::GameEngine::View::State::ArcGISLayerViewState(std::move(State));
				Callback(Layer, ArcGISLayerViewState);
			}
		});
	}
}

void ArcGISRendererView::Impl::SetElevationSourceViewStateChanged(Esri::GameEngine::View::Event::ArcGISElevationSourceViewStateChangedEvent Callback)
{
	if (GameEngineView)
	{
		GameEngineView->SetElevationSourceViewStateChanged(
			[this, Callback](Elevation::Base::ArcGISElevationSource& Elevation, ArcGISRuntime::MapView::ElevationSourceViewState& State) {
				if (this->GameEngineView)
				{
					auto ArcGISElevationSourceViewState = Esri::GameEngine::View::State::ArcGISElevationSourceViewState(std::move(State));
					Callback(Elevation, ArcGISElevationSourceViewState);
				}
			});
	}
}

ArcGISRendererView::ArcGISRendererView(TSharedPtr<Esri::GameEngine::Map::ArcGISMap> ArcGISMap,
									   const TSharedPtr<const Esri::GameEngine::Camera::ArcGISCamera>& ArcGISCamera,
									   ArcGISRendererViewOptions Options)
: PImpl(new ArcGISRendererView::Impl(std::move(ArcGISMap), ArcGISCamera, Options))
{
}

ArcGISRendererView::~ArcGISRendererView()
{
	delete PImpl;
}

void ArcGISRendererView::SetCamera(const TSharedPtr<const Esri::GameEngine::Camera::ArcGISCamera>& ArcGISCamera)
{
	PImpl->SetCamera(ArcGISCamera);
}

void ArcGISRendererView::SetMap(TSharedPtr<Esri::GameEngine::Map::ArcGISMap> ArcGISMap)
{
	PImpl->SetMap(std::move(ArcGISMap));
}

void ArcGISRendererView::SetOptions(ArcGISRendererViewOptions Options)
{
	PImpl->SetOptions(Options);
}

void ArcGISRendererView::UpdateMemoryAvailability(long Total, long InUse)
{
	PImpl->UpdateMemoryAvailability(Total, InUse);
}

RenderCommandQueue::DecodedRenderCommandQueue ArcGISRendererView::GetCurrentDecodedRenderCommandQueue()
{
	return PImpl->GetCurrentDecodedRenderCommandQueue();
}

void ArcGISRendererView::HandleLowMemoryWarning()
{
	PImpl->HandleLowMemoryWarning();
}

void ArcGISRendererView::SetViewportProperties(uint32 ViewportWidthPixels,
											   uint32 ViewportHeightPixels,
											   float HorizontalFieldOfViewDegrees,
											   float VerticalFieldOfViewDegrees,
											   float VerticalDistortionFactor)
{
	PImpl->SetViewportProperties(ViewportWidthPixels, ViewportHeightPixels, HorizontalFieldOfViewDegrees, VerticalFieldOfViewDegrees,
								 VerticalDistortionFactor);
}

void ArcGISRendererView::SetArcGISRendererViewStateChanged(Esri::GameEngine::View::Event::ArcGISRendererViewStateChangedEvent Callback)
{
	PImpl->SetViewStateChanged(std::move(Callback));
}

void ArcGISRendererView::SetArcGISLayerViewStateChanged(Esri::GameEngine::View::Event::ArcGISLayerViewStateChangedEvent Callback)
{
	PImpl->SetLayerStateChanged(std::move(Callback));
}

void ArcGISRendererView::SetArcGISElevationSourceViewStateChanged(Esri::GameEngine::View::Event::ArcGISElevationSourceViewStateChangedEvent Callback)
{
	PImpl->SetElevationSourceViewStateChanged(std::move(Callback));
}
} // namespace View
} // namespace GameEngine
} // namespace Esri
