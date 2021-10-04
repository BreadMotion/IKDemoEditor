#include <math.h>
#include "FrameWork/SpatialDivisionManager/SpatialDivisionManager.h"

namespace Bread
{
	namespace FrameWork
	{
		Math::Vector3S32 SpatialDivisionManager::SpatialCurrent(const Math::Vector3& position)
		{
			using Math::Vector3;
			using Math::Vector3S32;

			constexpr s32 ExclusionZero = 1;
			Vector3S32 sparialNumber
			{
			 static_cast<s32>(position.x / width),
			 static_cast<s32>(position.y / Height),
			 static_cast<s32>(position.z / width)
			};

			sparialNumber.x += sparialNumber.x < 0 ? -ExclusionZero : ExclusionZero;
			sparialNumber.y += sparialNumber.y < 0 ? -ExclusionZero : ExclusionZero;
			sparialNumber.z += sparialNumber.z < 0 ? -ExclusionZero : ExclusionZero;

			return sparialNumber;
		}
		Math::Vector3S32 SpatialDivisionManager::SpatialCurrent(const f32 x, const f32 y, const f32 z)
		{
			using Math::Vector3;
			using Math::Vector3S32;

			constexpr s32 ExclusionZero = 1;
			Vector3S32 sparialNumber
			{
			 static_cast<s32>(x / width),
			 static_cast<s32>(y / Height),
			 static_cast<s32>(z / width)
			};

			sparialNumber.x += sparialNumber.x < 0 ? -ExclusionZero : ExclusionZero;
			sparialNumber.y += sparialNumber.y < 0 ? -ExclusionZero : ExclusionZero;
			sparialNumber.z += sparialNumber.z < 0 ? -ExclusionZero : ExclusionZero;

			return sparialNumber;
		}
	}//namespace FrameWork
}//namespace Bread