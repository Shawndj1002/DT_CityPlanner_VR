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
class DataBufferBase
{
public:
	void* GetData() const;

	size_t GetItemCount() const;

	size_t GetItemSize() const;

	size_t GetSizeBytes() const;

public:
	DataBufferBase() = default;
	explicit DataBufferBase(void* handle);
	DataBufferBase(const DataBufferBase&) = delete;
	DataBufferBase(DataBufferBase&& other) noexcept;
	~DataBufferBase();

	DataBufferBase& operator=(const DataBufferBase&) = delete;
	DataBufferBase& operator=(DataBufferBase&& other) noexcept;

	explicit operator bool() const noexcept;

private:
	void* m_handle{nullptr};
};

template <typename T>
class DataBuffer
{
public:
	class ConstIterator
	{
	public:
		ConstIterator(T* pointer) : pointer(pointer)
		{
		}

		ConstIterator& operator++()
		{
			++pointer;

			return *this;
		}

		bool operator!=(const ConstIterator& rhs) const
		{
			return pointer != rhs.pointer;
		}

		const T& operator*() const
		{
			return *pointer;
		}

	private:
		T* pointer;
	};

	class Iterator
	{
	public:
		Iterator(T* pointer) : pointer(pointer)
		{
		}

		Iterator& operator++()
		{
			++pointer;

			return *this;
		}

		bool operator!=(const Iterator& rhs) const
		{
			return pointer != rhs.pointer;
		}

		T& operator*()
		{
			return *pointer;
		}

	private:
		T* pointer;
	};

public:
	void* GetData() const
	{
		return dataBufferBase.GetData();
	}

	template <typename U>
	U* GetData() const
	{
		return (U*)dataBufferBase.GetData();
	}

	size_t GetItemCount() const
	{
		return dataBufferBase.GetItemCount();
	}

	size_t GetItemSize() const
	{
		return dataBufferBase.GetItemSize();
	}

	size_t GetSizeBytes() const
	{
		return dataBufferBase.GetSizeBytes();
	}

	std::vector<T> ToVector() const
	{
		return std::vector<T>((T*)GetData(), (T*)GetData() + GetItemCount());
	}

	template <typename U>
	std::vector<U> ToVector() const
	{
		return std::vector<U>((U*)GetData(), (U*)GetData() + (GetSizeBytes() / sizeof(U)));
	}

	Iterator begin()
	{
		return Iterator((T*)GetData());
	}

	ConstIterator begin() const
	{
		return ConstIterator((T*)GetData());
	}

	Iterator end()
	{
		return Iterator((T*)GetData() + GetItemCount());
	}

	ConstIterator end() const
	{
		return ConstIterator((T*)GetData() + GetItemCount());
	}

public:
	DataBuffer() = default;

	explicit DataBuffer(void* handle) : dataBufferBase(handle)
	{
	}

	DataBuffer(const DataBuffer<T>&) = delete;

	DataBuffer(DataBuffer<T>&& other) noexcept
	{
		using std::swap;

		swap(dataBufferBase, other.dataBufferBase);
	}

	~DataBuffer()
	{
#if PLATFORM_COMPILER_HAS_IF_CONSTEXPR
		if constexpr (!std::is_trivially_destructible<T>::value)
#else
		if (!std::is_trivially_destructible<T>::value)
#endif
		{
			if (dataBufferBase)
			{
				auto count = GetItemCount();
				auto element = (T*)GetData();

				while (count)
				{
					element->~T();

					++element;
					--count;
				}
			}
		}
	}

	DataBuffer<T>& operator=(const DataBuffer<T>&) = delete;

	DataBuffer<T>& operator=(DataBuffer<T>&& other) noexcept
	{
		if (this != &other)
		{
			using std::swap;

			swap(dataBufferBase, other.dataBufferBase);
		}

		return *this;
	}

	explicit operator bool() const noexcept
	{
		return (bool)dataBufferBase;
	}

private:
	DataBufferBase dataBufferBase;
};
} // namespace Unreal
} // namespace Esri
