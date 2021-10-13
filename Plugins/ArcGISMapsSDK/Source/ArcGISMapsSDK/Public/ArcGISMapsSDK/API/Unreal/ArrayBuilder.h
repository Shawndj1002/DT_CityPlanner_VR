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
template<typename T> class ImmutableArray;

template <typename T>
class ArrayBuilder
{
public:
#pragma region Methods
	/// Add a value to the ArrayBuilder.
	///
	/// - Parameter value: The value that is to be added.
	/// - Since: 100.9.0
	void Add(const T& value);

	/// Creates a Array containing the values added to this ArrayBuilder.
	///
	/// - Remark: The order of the values in the returned Array matches the order in which the
	/// values were added to this ArrayBuilder.
	///
	/// This call empties this ArrayBuilder of values, but leaves it in a valid
	/// (re-usable) state.
	/// - Returns: A Array containing the values added to this ArrayBuilder.
	/// - Since: 100.9.0
	Unreal::ImmutableArray<T> MoveToArray();
#pragma endregion Methods

public:
#pragma region Special Members
	explicit ArrayBuilder(void* handle) : m_handle(handle)
	{
	}

	ArrayBuilder(ArrayBuilder<T>&& other) noexcept
	{
		using std::swap;
		swap(m_handle, other.m_handle);
	}

	ArrayBuilder(const ArrayBuilder<T>&) = delete;

	ArrayBuilder<T>& operator=(const ArrayBuilder<T>&) = delete;

	ArrayBuilder<T>& operator=(ArrayBuilder<T>&& other) noexcept
	{
		if (this != &other)
		{
			using std::swap;
			swap(m_handle, other.m_handle);
		}

		return *this;
	}

	void* GetHandle() const
	{
		return m_handle;
	}

	void SetHandle(void* handle)
	{
		m_handle = handle;
	}
#pragma endregion Special Members

protected:
	void* m_handle{nullptr};
};

template <>
ARCGISMAPSSDK_API void ArrayBuilder<FString>::Add(const FString& value);

template <>
ARCGISMAPSSDK_API void ArrayBuilder<GameEngine::Attributes::VisualizationAttributeDescription>::Add(const GameEngine::Attributes::VisualizationAttributeDescription& value);

template <>
ARCGISMAPSSDK_API Unreal::ImmutableArray<FString> ArrayBuilder<FString>::MoveToArray();

template <>
ARCGISMAPSSDK_API Unreal::ImmutableArray<GameEngine::Attributes::VisualizationAttributeDescription> ArrayBuilder<GameEngine::Attributes::VisualizationAttributeDescription>::MoveToArray();
} // namespace Unreal
} // namespace Esri
