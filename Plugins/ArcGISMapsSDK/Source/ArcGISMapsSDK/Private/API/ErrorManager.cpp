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

#include "ArcGISMapsSDK/API/ErrorManager.h" // IWYU pragma: associated
#include "ArcGISMapsSDK/API/Standard/ErrorHandler.h"
#include "ArcGISMapsSDK/API/Unreal/Exception.h"

namespace Esri
{
void ErrorManager::CheckError(const std::unique_ptr<Standard::ErrorHandler>& errorHandler)
{
	if (errorHandler->error)
	{
		throw std::move(errorHandler->error);
	}
}

std::unique_ptr<Standard::ErrorHandler> ErrorManager::CreateHandler()
{
	auto errorHandler = std::make_unique<Standard::ErrorHandler>();

	static auto callbackHandler = [](void* userData, RT_ErrorHandle error) {
		if (userData && error)
		{
			auto* errorHandler = reinterpret_cast<Standard::ErrorHandler*>(userData);

			errorHandler->error = Unreal::Exception{error};
		}
	};

	errorHandler->handler = callbackHandler;
	errorHandler->userData = errorHandler.get();

	return errorHandler;
}
} // namespace Esri
