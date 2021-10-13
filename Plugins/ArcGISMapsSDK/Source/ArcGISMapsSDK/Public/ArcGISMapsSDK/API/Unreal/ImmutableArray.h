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
namespace Attributes
{
class Attribute;
class VisualizationAttribute;
class VisualizationAttributeDescription;
} // namespace Attributes
} // namespace GameEngine

namespace Unreal
{
template <typename T>
class ArrayBuilder;

template <typename T>
class ImmutableArray
{
public:
#pragma region Methods
	/// Get a value at a specific position.
	///
	/// - Remark: Retrieves the value at the specified position.
	/// - Parameter position: The position which you want to get the value.
	/// - Returns: The value, T, at the position requested.
	/// - Since: 100.0.0
	T At(size_t position) const;

	/// Creates a ArrayBuilder.
	///
	/// - Returns: A ArrayBuilder
	/// - SeeAlso: ArrayBuilder
	/// - Since: 100.9.0
	static ArrayBuilder<T> CreateBuilder();
#pragma endregion Methods

public:
#pragma region Special Members
	ImmutableArray() = default;

	explicit ImmutableArray(void* handle) : m_handle(handle)
	{
	}

	ImmutableArray(ImmutableArray<T>&& other) noexcept
	{
		using std::swap;
		swap(m_handle, other.m_handle);
	}

	ImmutableArray(const ImmutableArray<T>&) = delete;

	ImmutableArray<T>& operator=(const ImmutableArray<T>&) = delete;

	ImmutableArray<T>& operator=(ImmutableArray<T>&& other) noexcept
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
ARCGISMAPSSDK_API FString ImmutableArray<FString>::At(size_t position) const;

template <>
ARCGISMAPSSDK_API GameEngine::Attributes::Attribute ImmutableArray<GameEngine::Attributes::Attribute>::At(size_t position) const;

template <>
ARCGISMAPSSDK_API GameEngine::Attributes::VisualizationAttribute ImmutableArray<GameEngine::Attributes::VisualizationAttribute>::At(size_t position) const;

template <>
ARCGISMAPSSDK_API GameEngine::Attributes::VisualizationAttributeDescription ImmutableArray<GameEngine::Attributes::VisualizationAttributeDescription>::At(size_t position) const;

template <>
ARCGISMAPSSDK_API ArrayBuilder<FString> ImmutableArray<FString>::CreateBuilder();

template <>
ARCGISMAPSSDK_API ArrayBuilder<GameEngine::Attributes::VisualizationAttributeDescription>
ImmutableArray<GameEngine::Attributes::VisualizationAttributeDescription>::CreateBuilder();
} // namespace Unreal
} // namespace Esri
