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
#include "ArcGISMapsSDK/Renderer/RenderCommandThrottle.h"

#include "Misc/EngineVersionComparison.h"

#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/DataBufferView.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/SetMeshBufferCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/SetMeshCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/Parameters/SetTexturePixelDataCommandParameters.h"
#include "ArcGISMapsSDK/API/GameEngine/RenderCommandQueue/RenderCommandType.h"

namespace
{
template <typename TView>
void TSum(const TView& View, uint64& InOutSum)
{
	InOutSum += View.Size;
}

template <typename... TViews>
uint64 TResourceMemory(const TViews&... Views)
{
	uint64 Total{0};
	int dummy[] = {0, ((void)TSum(Views, Total), 0)...};
	return Total;
}

// Debugging: Stats for each "throttle"
//#define COLLECT_STATS
#ifdef COLLECT_STATS
struct FTimedStats
{
	uint32 CommandCount{0};
	uint32 ComposeCount{0};
	float MemoryMb{0};
	float ExecMs{0};
};
std::vector<FTimedStats> CollectedStats;
#endif

} // namespace

namespace Esri
{
namespace ArcGISMapsSDK
{
namespace Renderer
{
// Debugging: Total number of throttles happening in a session
uint32 FRenderCommandThrottle::ThrottleCount{0};

FRenderCommandThrottle::FRenderCommandThrottle() : StartTime(std::chrono::steady_clock::now())
{
#ifdef COLLECT_STATS
	CollectedStats.reserve(5000);
#endif
}

FRenderCommandThrottle::~FRenderCommandThrottle()
{
#ifdef COLLECT_STATS
	if (Stats.CommandCount > 0)
	{
		const auto ElapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - StartTime);
		FTimedStats TimedStats{Stats.CommandCount, Stats.ComposeCount, Stats.ResourceMemory / (1024.f * 1024.f), ElapsedTime.count() / 1000.f};
		CollectedStats.emplace_back(std::move(TimedStats));
	}
#endif
}

bool FRenderCommandThrottle::DoThrottle(const GameEngine::RenderCommandQueue::RenderCommand& RenderCommand)
{
	using namespace GameEngine::RenderCommandQueue;

	// Update stats from selected commands
	++Stats.CommandCount;
	switch (RenderCommand.GetType())
	{
		case RenderCommandType::MultipleCompose:
		case RenderCommandType::Compose:
		{
			++Stats.ComposeCount;
			break;
		}

		case RenderCommandType::SetTexturePixelData:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::SetTexturePixelDataCommandParameters>();
			Stats.ResourceMemory += TResourceMemory(Params.Pixels);
			break;
		}

		case RenderCommandType::SetMesh:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::SetMeshCommandParameters>();
			Stats.ResourceMemory += TResourceMemory(Params.Colors, Params.FeatureIndices, Params.Normals, Params.Positions, Params.Tangents,
													Params.Triangles, Params.UvRegionIds, Params.Uvs);
			Stats.Triangles = Params.Triangles.Size / 12;
			break;
		}

		case RenderCommandType::SetMeshBuffer:
		{
			const auto& Params = RenderCommand.GetParameters<Parameters::SetMeshBufferCommandParameters>();
			Stats.ResourceMemory += TResourceMemory(Params.Buffer);
			break;
		}

		case RenderCommandType::CommandGroupBegin:
		{
			Stats.bIsExecutingGroup = true;
			break;
		}

		case RenderCommandType::CommandGroupEnd:
		{
			Stats.bIsExecutingGroup = false;
			break;
		}

		default:
			break;
	}

	// Determine whether to throttle at this command
	if (!bEnableThrottle || Stats.bIsExecutingGroup)
	{
		return false;
	}
	if (Stats.ComposeCount >= MaxComposeCommands || Stats.ResourceMemory >= MaxResourceMemory || Stats.Triangles >= MaxTriangles)
	{
		++ThrottleCount;
		return true;
	}
	const auto ElapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - StartTime);
	if (ElapsedTime.count() >= MaxExecTimeMicroseconds)
	{
		++ThrottleCount;
		return true;
	}
	return false;
}
} // namespace Renderer
} // namespace ArcGISMapsSDK
} // namespace Esri
