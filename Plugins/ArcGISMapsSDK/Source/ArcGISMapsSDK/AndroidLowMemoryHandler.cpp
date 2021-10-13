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
#if PLATFORM_ANDROID
#include "jni.h"

#include "ArcGISMapsSDK/Components/ArcGISRendererComponent.h"

extern "C" {
// "level" holds the warning level value provided by the Android onTrimMemory(). Meaning of provided value is documented here:
// https://developer.android.com/reference/android/content/ComponentCallbacks2#onTrimMemory(int)
// Value 42 is custom and indicates onLowMemory() was called instead.
JNIEXPORT void Java_com_epicgames_ue4_GameActivity_forwardOnTrimEvent(JNIEnv* /*jni*/, jclass /*javaClass*/, jint /*level*/)
{
	UArcGISRendererComponent::HandleLowMemoryWarning();
}

JNIEXPORT void Java_com_epicgames_ue4_GameActivity_updateMemoryAvailabilityNative(JNIEnv* /*jni*/, jclass /*javaClass*/, jlong total, jlong inUse)
{
	UArcGISRendererComponent::UpdateMemoryAvailability(total, inUse);
}
} // extern "C"
#endif
