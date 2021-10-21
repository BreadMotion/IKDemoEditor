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
			//空間の各軸の大きさを宣言する
			const f32 Height{ 300.0f };
			const f32 width { 300.0f };

		public:
			SpatialDivisionManager () = default;
			~SpatialDivisionManager() {}

		public:
			//渡された座標がどの空間座標にいるのかを返す
			Math::Vector3S32 SpatialCurrent(const Math::Vector3& position);
			Math::Vector3S32 SpatialCurrent(const f32 x, const f32 y, const f32 z);
		};
	}//namespace FrameWork
}//namespace Bread