// COPYRIGHT 1995-2021 ESRI
// TRADE SECRETS: ESRI PROPRIETARY AND CONFIDENTIAL
// Unpublished material - all rights reserved under the
// Copyright Laws of the United States and applicable international
// laws, treaties, and conventions.
//
// For additional information, contact:
// Environmental Systems Research Institute, Inc.
// Attn: Contracts and Legal Services Department
// 380 New York Street
// Redlands, California, 92373
// USA
//
// email: contracts@esri.com

#include "ArcGISMapsSDK/API/Standard/Element.h"
#include "ArcGISMapsSDK/API/Standard/IntermediateFuture.h"
#include "ArcGISMapsSDK/API/Unreal/Future.h" // IWYU pragma: associated

namespace Esri
{
namespace Unreal
{
FutureBase::FutureBase(void* handle) : m_handle{new Standard::IntermediateFuture(handle)}
{
}

FutureBase::FutureBase(FutureBase&& other) noexcept : m_handle{other.m_handle}
{
	other.m_handle = nullptr;
}

FutureBase::~FutureBase()
{
	delete static_cast<Standard::IntermediateFuture*>(m_handle);
}

FutureBase& FutureBase::operator=(FutureBase&& other) noexcept
{
	if (this != &other)
	{
		using std::swap;

		swap(m_handle, other.m_handle);
	}

	return *this;
}

void* FutureBase::GetHandle() const
{
	return m_handle;
}

template <>
uint32_t FutureBase::Get() const
{
	auto intermediateFuture = (Standard::IntermediateFuture*)m_handle;

	auto element = intermediateFuture->Get();

	return element.GetValueAsUInt32();
}

void FutureBase::SetTaskCompleted(Unreal::FutureCompletedEvent taskCompleted)
{
	m_taskCompleted = std::move(taskCompleted);

	auto intermediateFuture = (Standard::IntermediateFuture*)m_handle;

	intermediateFuture->SetTaskCompleted([this]() {
		if (!m_taskCompleted)
		{
			return;
		}

		m_taskCompleted();
	});
}
} // namespace Unreal
} // namespace Esri
