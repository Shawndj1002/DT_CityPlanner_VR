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
namespace GameEngine
{
namespace Security
{
class ArcGISAuthenticationConfiguration;
} // namespace Security
} // namespace GameEngine

namespace Unreal
{
ARCGISMAPSSDK_API void DestroyDictionary(void* object);

template<typename Key, typename Value>
class Dictionary final
{
public:
	Dictionary();

	~Dictionary()
	{
		DestroyDictionary(this);
	}

	explicit Dictionary(void* handle) : m_handle(handle)
	{
	}

	Dictionary(Dictionary&& other)
	{
		using std::swap;
		swap(m_handle, other.m_handle);
	}

	Dictionary(const Dictionary&) = delete;

	Dictionary& operator=(const Dictionary&) = delete;

	Dictionary& operator=(Dictionary&& other) noexcept
	{
		if (this != &other)
		{
			using std::swap;
			swap(m_handle, other.m_handle);
		}

		return *this;
	}

	void Insert(const Key& key, const Value& value);

	void RemoveAll();

	void* GetHandle() const
	{
		return m_handle;
	}

private:
	void* m_handle{nullptr};
};

template <>
ARCGISMAPSSDK_API Dictionary<FString, GameEngine::Security::ArcGISAuthenticationConfiguration>::Dictionary();

template <>
ARCGISMAPSSDK_API void Dictionary<FString, GameEngine::Security::ArcGISAuthenticationConfiguration>::Insert(const FString& key, const GameEngine::Security::ArcGISAuthenticationConfiguration& value);

template <>
ARCGISMAPSSDK_API void Dictionary<FString, GameEngine::Security::ArcGISAuthenticationConfiguration>::RemoveAll();
} // namespace Unreal
} // namespace Esri
