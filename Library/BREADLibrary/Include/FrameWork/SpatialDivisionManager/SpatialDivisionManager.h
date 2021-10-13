#pragma once
#include "FND/Base.h"
#include "Math/BreadMath.h"

namespace Bread
{
	namespace FrameWork
	{
		//空間のブロックを管理するクラス
		//中身は渡された座標情報からブロックの幅、奥行、高さなどの情報から何ブロック目にいるのか計算するだけのクラス
		class SpatialDivisionManager
		{
		private:
			const f32 Height{ 200.0f };
			const f32 width { 200.0f };

		public:
			SpatialDivisionManager() = default;
			~SpatialDivisionManager() {}

		public:
			Math::Vector3S32 SpatialCurrent(const Math::Vector3& position);
			Math::Vector3S32 SpatialCurrent(const f32 x, const f32 y, const f32 z);
		};
	}//namespace FrameWork
}//namespace Bread