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

#include "ArcGISMapsSDK/API/Standard/Error.h"
#include "ArcGISMapsSDK/API/Unreal/Exception.h" // IWYU pragma: associated

namespace Esri
{
namespace Unreal
{
Exception::Exception(void* handle) : m_handle{handle}
{
	if (m_handle)
	{
		auto error = reinterpret_cast<const Standard::Error*>(&m_handle);

		AdditionalMessage = TCHAR_TO_ANSI(*error->GetAdditionalMessage());
		Message = TCHAR_TO_ANSI(*error->GetMessage());
	}
}

Exception::Exception(Exception&& other) noexcept = default;

Exception::~Exception() = default;

Exception& Exception::operator=(Exception&& other) noexcept = default;

Exception::operator bool() const noexcept
{
	return m_handle != nullptr;
}

FString Exception::GetAdditionalMessage() const
{
	return AdditionalMessage.c_str();
}

int32_t Exception::GetCode() const
{
	auto error = reinterpret_cast<const Standard::Error*>(&m_handle);

	return error->GetCode();
}

FString Exception::GetMessage() const
{
	return Message.c_str();
}

const char* Exception::what() const noexcept
{
	return Message.c_str();
}
} // namespace Unreal
} // namespace Esri
