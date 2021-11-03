#include "LinkingSphere.h"
#include "Types.h"
#include "FND/Instance.h"

#include "Graphics/RenderManager.h"

#include "FrameWork/Actor/Actor.h"
#include "FrameWork/Object/Object.h"

#include "Sphere/SphereModelComponent.h"

#include "FrameWork/Actor/ActorManager.h"
#include "Math/BreadMath.h"

using Bread::FND::MapInstance;

using namespace Bread::Math;

namespace Bread
{
	namespace FrameWork
	{
		void LinkingSphere::Initialize()
		{
			//コンポーネントの構築
			ComponentConstruction();

#if 1
			struct LocalFunction
			{
				static void GenerateBones(std::shared_ptr<Actor> owner,std::vector<IJoint*>& localJoint)
				{
					static s32 iterate{ 0 };
					//１０回回っていなかったら
					if (++iterate < 10)
					{
						//子アクターの生成
						auto childActor{ owner->AddChildActor<Actor>() };
						{//子アクターの設定及びコンポーネントの追加
							childActor->SetID(("SphereActor" + std::to_string(iterate)));
							childActor->AddComponent<SphereModelComponent>();
						}

						//生成したアクターの親に前回のジョイントを設定
						//＆生成したアクターを前回のジョイントの子供に設定
						auto& childJoint{ childActor->GetComponent<ModelObject>()->GetNodes()->at(0) };
						if (localJoint.size() > 0)
						{
							childJoint.parent = localJoint.back();
							localJoint.back()->child.emplace_back(&childJoint);
						}

						//一つのオブジェクトとして処理できるように登録する
						localJoint.emplace_back(&childJoint);

						//再帰処理
						GenerateBones(childActor, localJoint);
					}
				}
			};
			LocalFunction::GenerateBones(GetOwner(), joints);
#endif
		}

		void LinkingSphere::PreUpdate()
		{
		}

		void LinkingSphere::Update()
		{
		}

		void LinkingSphere::NextUpdate()
		{
		}

		void LinkingSphere::Draw()
		{

		}

		void LinkingSphere::ComponentConstruction()
		{
			TransformConstruction();
		}

		void LinkingSphere::TransformConstruction()
		{
		/*	transform = GetOwner()->GetComponent<Transform>();
			transform->SetID("linkingSphereTransform");
			transform->SetTranslate(Math::Vector3{ 1000.0f,0.0f,0.0f });*/
		}
	}
}