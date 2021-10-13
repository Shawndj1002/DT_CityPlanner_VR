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
#include "ArcGISMapsSDK/API/GameEngine/View/State/ArcGISRendererViewState.h"

#include "ArcGISMapsSDK/API/GameEngine/GameEngineViewViewState.h"
#include "ArcGISMapsSDK/API/GameEngine/View/State/ArcGISRendererViewStatus.h"
#include "ArcGISMapsSDK/API/Unreal/Exception.h"

namespace Esri
{
namespace GameEngine
{
namespace View
{
namespace State
{
Unreal::Exception ArcGISRendererViewState::GetError() const
{
	auto state = reinterpret_cast<const GameEngineViewViewState*>(this);

	return state->GetError();
}

ArcGISRendererViewStatus ArcGISRendererViewState::GetStatus() const
{
	auto state = reinterpret_cast<const GameEngineViewViewState*>(this);

	return static_cast<ArcGISRendererViewStatus>(state->GetStatus());
}

ArcGISRendererViewState::ArcGISRendererViewState(void* handle) :
	m_handle{ handle }
{
}

ArcGISRendererViewState::ArcGISRendererViewState(ArcGISRendererViewState&& other) noexcept :
	m_handle{ other.m_handle }
{
	other.m_handle = nullptr;
}

ArcGISRendererViewState::~ArcGISRendererViewState()
{
	if (this->m_handle)
	{
		auto state = reinterpret_cast<GameEngineViewViewState*>(this);

		state->~GameEngineViewViewState();
	}
}

ArcGISRendererViewState& ArcGISRendererViewState::operator=(ArcGISRendererViewState&& other) noexcept
{
	if (this != &other)
	{
		using std::swap;

		swap(m_handle, other.m_handle);
	}

	return *this;
}

ArcGISRendererViewState::operator bool() const noexcept
{
	return m_handle != nullptr;
}

void* ArcGISRendererViewState::GetHandle() const
{
	return m_handle;
}
} // namespace State
} // namespace View
} // namespace GameEngine
} // namespace Esri
