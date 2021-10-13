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

#include "ArcGISMapsSDK/API/GameEngine/IntermediateDataBuffer.h"
#include "ArcGISMapsSDK/API/Unreal/DataBuffer.h" // IWYU pragma: associated

namespace Esri
{
namespace Unreal
{
void* DataBufferBase::GetData() const
{
	auto intermediateDataBuffer = reinterpret_cast<const GameEngine::IntermediateDataBuffer*>(this);

	return intermediateDataBuffer->GetData();
}

size_t DataBufferBase::GetItemCount() const
{
	auto intermediateDataBuffer = reinterpret_cast<const GameEngine::IntermediateDataBuffer*>(this);

	return intermediateDataBuffer->GetSizeBytes() / intermediateDataBuffer->GetItemSize();
}

size_t DataBufferBase::GetItemSize() const
{
	auto intermediateDataBuffer = reinterpret_cast<const GameEngine::IntermediateDataBuffer*>(this);

	return intermediateDataBuffer->GetItemSize();
}

size_t DataBufferBase::GetSizeBytes() const
{
	auto intermediateDataBuffer = reinterpret_cast<const GameEngine::IntermediateDataBuffer*>(this);

	return intermediateDataBuffer->GetSizeBytes();
}

DataBufferBase::DataBufferBase(void* handle) : m_handle{handle}
{
}

DataBufferBase::DataBufferBase(DataBufferBase&& other) noexcept : m_handle{other.m_handle}
{
	other.m_handle = nullptr;
}

DataBufferBase::~DataBufferBase()
{
	auto intermediateDataBuffer = reinterpret_cast<GameEngine::IntermediateDataBuffer*>(this);

	intermediateDataBuffer->~IntermediateDataBuffer();
}

DataBufferBase& DataBufferBase::operator=(DataBufferBase&& other) noexcept
{
	if (this != &other)
	{
		using std::swap;

		swap(m_handle, other.m_handle);
	}

	return *this;
}

DataBufferBase::operator bool() const noexcept
{
	return m_handle != nullptr;
}
} // namespace Unreal
} // namespace Esri
