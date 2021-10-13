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
using FutureCompletedEvent = std::function<void()>;

class FutureBase
{
public:
	FutureBase() = default;
	explicit FutureBase(void* handle);
	FutureBase(const FutureBase&) = delete;
	FutureBase(FutureBase&& other) noexcept;
	~FutureBase();

	FutureBase& operator=(const FutureBase&) = delete;
	FutureBase& operator=(FutureBase&& other) noexcept;

	void* GetHandle() const;

	template <typename T>
	T Get() const;

	void SetTaskCompleted(Unreal::FutureCompletedEvent taskCompleted);

private:
	void* m_handle{nullptr};
	Unreal::FutureCompletedEvent m_taskCompleted;
};

template <typename T>
class Future
{
public:
	Future() = default;

	explicit Future(void* handle) : futureBase(FutureBase(handle))
	{
	}

	Future(const Future<T>&) = delete;

	Future(Future<T>&& other) noexcept
	{
		using std::swap;

		swap(futureBase, other.futureBase);
	}

	Future<T>& operator=(const Future<T>&) = delete;

	Future<T>& operator=(Future<T>&& other) noexcept
	{
		if (this != &other)
		{
			using std::swap;

			swap(futureBase, other.futureBase);
		}

		return *this;
	}

	void* GetHandle() const
	{
		return futureBase.GetHandle();
	}

	T Get() const
	{
		return futureBase.Get<T>();
	}

	void SetTaskCompleted(Unreal::FutureCompletedEvent taskCompleted)
	{
		futureBase.SetTaskCompleted(taskCompleted);
	}

private:
	FutureBase futureBase;
};

template <>
ARCGISMAPSSDK_API uint32_t FutureBase::Get() const;
} // namespace Unreal
} // namespace Esri
