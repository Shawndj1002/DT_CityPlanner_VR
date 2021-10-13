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
#if defined(GE_ANDROID)
#include "android/log.h"
#elif defined(GE_WINDOWS)
#include <iostream>
#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#else
#include <iostream>
#endif

#include "ArcGISMapsSDK/Utils/Debug.h"

#if defined(GE_ANDROID)
#define ANDROID_LOG_TAG "ArcGISMapsSDKLib"
#endif

namespace
{
constexpr std::size_t c_assert_message_buffer_size_bytes = 2048;
} // anonymous namespace

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Utils
{
void Debug::Write(const char* fmt, ...)
{
	va_list ptr;
	char buf[c_assert_message_buffer_size_bytes];

	va_start(ptr, fmt);
	vsprintf(buf, fmt, ptr);

#if defined(GE_WINDOWS)
	if (IsDebuggerPresent())
	{
		OutputDebugStringA(buf);
	}
	else
	{
		std::cout << buf;
	}
#elif defined(GE_ANDROID)
	__android_log_print(ANDROID_LOG_DEBUG, ANDROID_LOG_TAG, "%s", buf);
#else
	std::cout << buf;
#endif
	va_end(ptr);
}

void Debug::WriteLine(const char* fmt, ...)
{
	va_list ptr;
	char buf[c_assert_message_buffer_size_bytes];

	va_start(ptr, fmt);
	vsprintf(buf, fmt, ptr);

#if defined(GE_WINDOWS)
	if (IsDebuggerPresent())
	{
		OutputDebugStringA(buf);
		OutputDebugStringA("\n");
	}
	else
	{
		std::cout << buf << std::endl;
	}
#elif defined(GE_ANDROID)
	__android_log_print(ANDROID_LOG_DEBUG, ANDROID_LOG_TAG, "%s", buf);
#else
	std::cout << buf << std::endl;
#endif
	va_end(ptr);
}
} // namespace Utils
} // namespace ArcGISMapsSDK
} // namespace Esri
