#pragma once
#include <string>
#include "FrameWork/Component/Component.h"
#include "FrameWork/Object/Object.h"
#include "FrameWork/Component/Transform.h"

namespace Bread
{
	namespace FrameWork
	{
		class StageComponent : public Component
		{
		public:
			StageComponent()
			{
			}
			~StageComponent()override {}

		public:
			//初期化
			void Initialize() override;

			//事前更新
			void __fastcall PreUpdate() override;

			//更新
			void __fastcall Update() override;

			//事後更新
			void __fastcall NextUpdate() override;

			//描画
			void __fastcall Draw()override;

		private:
			//生成したComponentの構築
			void ComponentConstruction();

			//各コンポーネントの構築関数
			void ModelObjectConstruction();
			void TransformConstruction();

		public:
			// 当たり判定データ取得
			const std::vector<CollisionData> GetCollisionDatas()
			{
				return collisions;
			}

		private:
			std::vector<CollisionData>   collisions;
			std::shared_ptr<ModelObject> model         {nullptr  };
			std::shared_ptr<ModelObject> collisionModel{ nullptr };
			std::shared_ptr<Transform>   transform     {nullptr  };
		};
	};
}