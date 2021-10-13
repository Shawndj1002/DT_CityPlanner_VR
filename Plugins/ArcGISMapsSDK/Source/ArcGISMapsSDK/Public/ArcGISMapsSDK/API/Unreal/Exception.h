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

#pragma once

namespace Esri
{
namespace Unreal
{
class Exception : public std::exception
{
public:
	Exception() = default;
	explicit Exception(void* handle);
	Exception(const Exception&) = delete;
	Exception(Exception&& other) noexcept;
	ARCGISMAPSSDK_API ~Exception() override;

	Exception& operator=(const Exception&) = delete;
	Exception& operator=(Exception&& other) noexcept;

	explicit operator bool() const noexcept;

	ARCGISMAPSSDK_API FString GetAdditionalMessage() const;
	ARCGISMAPSSDK_API int32_t GetCode() const;
	ARCGISMAPSSDK_API FString GetMessage() const;

	ARCGISMAPSSDK_API const char* what() const noexcept override;

private:
	void* m_handle{nullptr};

	std::string AdditionalMessage;
	std::string Message;
};
} // namespace Unreal
} // namespace Esri
