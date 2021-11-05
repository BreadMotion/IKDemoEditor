#pragma once
#include <vector>
#include <string>
#include <memory>

#include "FND/DirtyFlag.h"
#include "Math/BreadMath.h"

namespace Bread
{
	namespace FrameWork
	{
		class Actor;
		class ModelObject;
		class Transform;

		struct ITransform
		{
		public:
			ITransform*              parent{nullptr};     //親ジョイントのポインター
			std::vector<ITransform*> child;               //子ジョイントのポインター配列

			Math::Vector3    scale;     //ジョイントのスケール値
			Math::Quaternion rotate;    //ジョイントの回転値
			Math::Vector3    translate; //ジョイントの平行移動値

			Math::Matrix     localTransform;
			Math::Matrix     worldTransform;
		};

		//全てのジョイントの基盤となる構造体
		struct IJoint : public ITransform
		{
			std::string        name;        //ジョイントの名前

			Math::Vector3      minRot;        //ジョイントの最小回転量(Euler)
			Math::Vector3      maxRot;        //ジョイントの最大回転量(Euler)
		};

		//自作でモデルのジョイント同士を組み合わせてオブジェクトを構築するクラス
		struct IJointAssembly
		{
			std::vector<ITransform*> joins;

			template <class Component>
			std::shared_ptr<Actor> AddJoint(std::shared_ptr<Actor> owner)
			{
				//子アクターの生成
				auto childActor{ owner->AddChildActor<Actor>() };
				{//子アクターの設定及びコンポーネントの追加
					childActor->AddComponent<Component>();

					//親アクターのモデルと子アクターのモデルのルートジョイントの親子関係を構築
					auto& childJoint{ childActor->GetComponent<ModelObject>()->GetNodes()->at(0) };

					//初めての追加じゃなければ親が存在するので親子関係を構築
					if (joins.size() > 0)
					{
						childJoint.parent = joins.back();
						joins.back()->child.emplace_back(childJoint);
					}
					//最後尾に追加
					joins.emplace_back(childJoint);
				}

				//一つのオブジェクトとして処理できるように登録する
				return childActor;
			}
		};

		//全てのポリゴンの基礎となる構造体
		struct IFace
		{
			struct VertexIndex
			{
				std::vector<Math::Vector3> vertex; //ポリゴンの頂点座標
			};
			std::vector<VertexIndex>       face;   //ポリゴン
		};

		struct IMeshNode
		{
			std::vector<Math::Matrix> boneTransform;
			u32 boneTransformCount = 0;
		};

		//全てのシェーダーの名前を格納する構造体
		struct IShaderName
		{
		protected:
			std::string shaderName;   //描画するシェーダの名前
		};

		//RenderManagerで使用するシェーダーを判別するため
		struct MyShaderName : public IShaderName
		{
		public:
			void SetShaderNema(const std::string& name)
			{
				shaderName = name;
			}
			const std::string& GetShaderName()
			{
				return shaderName;
			}
		};
	}
}