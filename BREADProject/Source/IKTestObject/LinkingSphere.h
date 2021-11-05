#pragma once
#include <string>
#include <memory>
#include <vector>

#include "FrameWork/Component/Component.h"

#include "FrameWork/Object/BasicObjectElement/INode.h"
#include "FrameWork/Component/Transform.h"

namespace Bread
{
	namespace FrameWork
	{
		class LinkingSphere : public Component
		{
		public:
			LinkingSphere()
			{
			}
			~LinkingSphere()override {}

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
			void TransformConstruction();

		public://unique Function
			//このコンポーネントが追加したボーン関係を持ったアクターをインデックス番号から取得する
			std::shared_ptr<Actor> _fastcall GetChildActor(const u32& index);

			//このコンポーネントが追加したボーンのインデックス番号から取得する
			ITransform* _fastcall     GetIJoint(const u32& index);
			std::vector<ITransform*>* GetAllIJoint();
			IJointAssembly*       GetJointAssembly();

			//このコンポーネントが管理するボーン関係を持ったアクターを生成する
			std::shared_ptr<Actor> AddJoint(std::shared_ptr<Actor> owner);
			std::vector<std::shared_ptr<Actor>>* GetAllJointActors();

		private:
			std::vector<std::shared_ptr<Actor>> jointActors;
			IJointAssembly                      myJoint;
			std::shared_ptr<Transform>          transform{ nullptr };
		};
	};
}