#pragma once
#include <string>
#include "FrameWork/Component/Component.h"
#include "FrameWork/Object/Object.h"
#include "FrameWork/Component/Transform.h"

namespace Bread
{
	namespace FrameWork
	{
		class SphereModelComponent : public Component
		{
		public:
			SphereModelComponent()
			{
			}
			~SphereModelComponent()override {}

		public:
			//初期化
			void Initialize()            override;

			//事前更新
			void __fastcall PreUpdate()  override;

			//更新
			void __fastcall Update()     override;

			//事後更新
			void __fastcall NextUpdate() override;

			//描画
			void __fastcall Draw()override;

			//GUI
			void GUI()override;

		private:
			//生成したComponentの構築
			void ComponentConstruction();

			//各コンポーネントの構築関数
			void ModelObjectConstruction();
			void TransformConstruction();

		private:
			Math::Vector3                worldTargetPos{ Math::Vector3{0.0,300.0f,0.0f} };
			std::shared_ptr<ModelObject> model{ nullptr };
			std::shared_ptr<Transform>   transform{ nullptr };
		};
	};
}