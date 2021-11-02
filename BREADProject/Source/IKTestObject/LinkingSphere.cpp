#include "Types.h"
#include "FND/Instance.h"

#include "Graphics/RenderManager.h"

#include "LinkingSphere.h"
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
						childActor->SetID("SphereActor" + std::to_string(iterate));
						childActor->AddComponent<SphereModelComponent>();

						//生成したアクターのモデルコンポーネントが持つRootJointを前回追加したアクターのRootJointの親に設定する
						IJoint& childJoint{ childActor->GetComponent<ModelObject>()->GetNodes()->at(0) };
						if (localJoint.size() != 0)
						{
							localJoint.back()->parent = &childJoint;
						}

						//一つのオブジェクトとして処理できるように登録する
						localJoint.emplace_back(&childJoint);

						//再帰処理
						GenerateBones(childActor, localJoint);
					}
				}
			};
			LocalFunction::GenerateBones(GetOwner(), joints);
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
			transform = GetOwner()->GetComponent<Transform>();
			transform->SetID("linkingSphereTransform");
			transform->SetTranslate(Math::Vector3{ 1000.0f,0.0f,0.0f });
		}
	}
}