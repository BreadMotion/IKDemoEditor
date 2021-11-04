#pragma once
#include "FrameWork/Component/Component.h"
#include "../LinkingSphere.h"
#include "FrameWork/Component/Transform.h""

namespace Bread
{
	namespace FrameWork
	{
		class ChainStraight : public Component
		{
		public://constructor
			ChainStraight() = default;
			~ChainStraight() override {}

		public://component's override function
			//初期化
			void Initialize()override;

			//事前更新
			void _fastcall PreUpdate()override;

			//更新
			void _fastcall Update()override;

			//事後更新
			void _fastcall NextUpdate()override;

			//GUI
			void GUI()override;

		private:
			//生成したComponentの構築
			void ComponentConstruction();

			//各コンポーネントの構築関数
			void TransformConstruction();
			void LinkingSphereConstruction();

		private:
			std::shared_ptr<Transform>     transform;
			std::shared_ptr<LinkingSphere> linkSphere;
		};
	}
}

