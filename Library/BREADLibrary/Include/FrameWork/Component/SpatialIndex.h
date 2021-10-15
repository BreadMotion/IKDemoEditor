#pragma once
#include "Math/BreadMath.h"
#include "FrameWork/Component/Component.h"
#include "FrameWork/Component/Transform.h"

namespace Bread
{
	namespace FrameWork
	{
		//SpatialDivisionManagerとアクターの座標を使って算出された
		//空間のインデックス番号を保存するクラス
		class SpatialIndexComponent : public Component
		{
		private://SpatialIndexComponent's Data
			Math::Vector3S32 spatialIndex;

		public://override Function
			// 初期化
			void Initialize()override;

			// 終了化
			void Finalize()override;

			//事前更新
			void __fastcall PreUpdate()override;

			// 更新
			void __fastcall Update()override;

			//事後更新
			void __fastcall NextUpdate()override;

			// 描画
			void __fastcall Draw()override;

			//imgui
			void GUI();

		public:// public Get Set Function
			const Math::Vector3S32& GetSpatialIndex()
			{
				return spatialIndex;
			}

		private:// private other Componenet Pointer
			std::shared_ptr<Transform> transform;
		};
	}//namespace Math
}//namespace Bread