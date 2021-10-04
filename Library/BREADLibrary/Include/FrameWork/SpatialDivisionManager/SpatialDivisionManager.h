#pragma once
#include "FND/Base.h"
#include "Math/BreadMath.h"

namespace Bread
{
	namespace FrameWork
	{
		class SpatialDivisionManager
		{
		private:
			const f32 Height = 100.0f;
			const f32 width  = 100.0f;

		public:
			SpatialDivisionManager() = default;
			~SpatialDivisionManager() {}

		public:
			Math::Vector3S32 SpatialCurrent(const Math::Vector3& position);
			Math::Vector3S32 SpatialCurrent(const f32 x, const f32 y, const f32 z);
		};
	}//namespace FrameWork
}//namespace Bread