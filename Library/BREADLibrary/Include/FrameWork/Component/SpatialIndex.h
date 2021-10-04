#pragma once
#include "Math/BreadMath.h"
#include "FrameWork/Component/Component.h"

namespace Bread
{
	namespace FrameWork
	{
		class SpatialIndexComponent : public Component
		{
		private:
			Math::Vector3S32 SpatialIndex;

		public:
			// 初期化
			void Initialize()override;

			// 終了化
			void Finalize()override;

			//事前更新
			void __fastcall PreUpdate(const f32&)override;

			// 更新
			void __fastcall Update(const f32&)override;

			//事後更新
			void __fastcall NextUpdate(const f32&)override;

			// 描画
			void __fastcall Draw(const f32&)override;

			//imgui
			void GUI() {}
		};
	}//namespace Math
}//namespace Bread