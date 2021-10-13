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
class Exception;
} // namespace Unreal

namespace GameEngine
{
namespace View
{
namespace State
{
enum class ArcGISRendererViewStatus;

class ARCGISMAPSSDK_API ArcGISRendererViewState
{
public:
	#pragma region Properties
	/// The ArcGISRenderer view error from the elevation source view state.
	///
	/// - SeeAlso: ArcGISRendererViewState
	/// - Since: 100.11.0
	Unreal::Exception GetError() const;

	/// The ArcGISRenderer view status from the elevation source view state.
	///
	/// - SeeAlso: ArcGISRendererViewStatus
	/// - Since: 100.11.0
	ArcGISRendererViewStatus GetStatus() const;
	#pragma endregion Properties

public:
    #pragma region Special Members
    explicit ArcGISRendererViewState(void* handle);
    ArcGISRendererViewState(const ArcGISRendererViewState&) = delete;
    ArcGISRendererViewState(ArcGISRendererViewState&& other) noexcept;
    ArcGISRendererViewState() = default;
    ~ArcGISRendererViewState();

    ArcGISRendererViewState& operator=(const ArcGISRendererViewState&) = delete;
    ArcGISRendererViewState& operator=(ArcGISRendererViewState&& other) noexcept;
    operator bool() const noexcept;

    void* GetHandle() const;
    #pragma endregion Special Members

protected:
    void* m_handle{ nullptr };
};
} // namespace State
} // namespace View
} // namespace GameEngine
} // namespace Esri
