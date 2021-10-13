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
using System;
using System.IO;
using UnrealBuildTool;

public class ArcGISMapsSDK : ModuleRules
{
    public ArcGISMapsSDK(ReadOnlyTargetRules Target) : base(Target)
    {
        bEnableExceptions = true;
        bEnforceIWYU = false;

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = "Private/ArcGISMapsSDKPrivatePCH.h";

        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory, "Internal"));

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "Engine",
            "GeoReferencing",
            "GeometricObjects"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "CoreUObject",
            "EngineSettings",
            "Projects",
            "RenderCore",
            "RHI"
        });

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PublicDefinitions.AddRange(new string[] {
                "GE_ANDROID"
            });

            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "AndroidLowMemoryHandlerUPL.xml"));

            // Copy library files to the building folder
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "ArcGISMapsSDK_APL.xml"));

            // Add the library
            PublicAdditionalLibraries.Add("$(PluginDir)/Binaries/ThirdParty/ArcGISMapsSDK/Android/armeabi-v7a/libruntimecore_armeabi-v7a.so");
            PublicAdditionalLibraries.Add("$(PluginDir)/Binaries/ThirdParty/ArcGISMapsSDK/Android/arm64-v8a/libruntimecore_arm64-v8a.so");
        }

        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PublicDefinitions.AddRange(new string[] {
                "GE_IOS"
            });

            // Add the import library
            PublicAdditionalLibraries.Add("$(PluginDir)/Binaries/ThirdParty/ArcGISMapsSDK/iOS/libprelink.a");

            PublicFrameworks.AddRange(new string[] {
                "CoreText"
            });
        }

        if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicDefinitions.AddRange(new string[] {
                "GE_MACOS"
            });

            // Add the import library
            PublicAdditionalLibraries.Add("$(PluginDir)/Binaries/ThirdParty/ArcGISMapsSDK/Mac/libruntimecore.dylib");

            RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/ArcGISMapsSDK/Mac/libruntimecore.dylib");
        }

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicDefinitions.AddRange(new string[] {
                "_CRT_SECURE_NO_WARNINGS",
                "GE_WINDOWS"
            });

            // Add the import library
            PublicAdditionalLibraries.Add("$(PluginDir)/Binaries/ThirdParty/ArcGISMapsSDK/Win64/runtimecore.lib");

            RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/ArcGISMapsSDK/Win64/runtimecore.dll");

            PublicDelayLoadDLLs.Add("runtimecore.dll");
        }
    }
}
