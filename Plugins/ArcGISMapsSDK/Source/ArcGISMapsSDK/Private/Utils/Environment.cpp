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

#include "ArcGISMapsSDK/Utils/Environment.h"

#include "../buildnum.h"

#include "ArcGISMapsSDK/API/ArcGISRuntime/ArcGISRuntimeEnvironmentInternal.h"
#include "ArcGISMapsSDK/API/Standard/RuntimeClient.h"
#include "ArcGISMapsSDK/API/Unreal/Exception.h"
#include "ArcGISMapsSDK/Utils/Debug.h"

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Utils
{
void Environment::Initialize(const FString& ProductName, const FString& ProductVersion, const FString& TempDirectory, const FString& InstallDirectory)
{
	try
	{
		int32_t BuildNumParts[]{GE_BUILD_VER};
		char Version[255];
		snprintf(Version, 255, "%d.%d.%d", BuildNumParts[0], BuildNumParts[1], BuildNumParts[2]);

		ArcGISRuntime::ArcGISRuntimeEnvironmentInternal::SetRuntimeClient(Standard::RuntimeClient::Unreal, Version);
		ArcGISRuntime::ArcGISRuntimeEnvironmentInternal::SetProductInfo(ProductName, ProductVersion);
		ArcGISRuntime::ArcGISRuntimeEnvironmentInternal::EnableBreakOnException(false);
		ArcGISRuntime::ArcGISRuntimeEnvironmentInternal::EnableLeakDetection(false);
		ArcGISRuntime::ArcGISRuntimeEnvironmentInternal::SetTempDirectory(TempDirectory);
		ArcGISRuntime::ArcGISRuntimeEnvironmentInternal::SetInstallDirectory(InstallDirectory);
	}
	catch (const Unreal::Exception& Ex)
	{
		Esri::ArcGISMapsSDK::Utils::Debug::WriteLine("Environment::Initialize: %s", Ex.what());
	}
}
} // namespace Utils
} // namespace ArcGISMapsSDK
} // namespace Esri
