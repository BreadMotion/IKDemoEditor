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
			transform->SetTranslate(Math::Vector3::Zero);
		}

		std::shared_ptr<Actor> LinkingSphere::GetChildActor(const u32& index)
		{
			return jointActors[index];
		}

		std::shared_ptr<Actor> LinkingSphere::AddJoint(std::shared_ptr<Actor> owner)
		{
			//子アクターの生成
			auto childActor{ owner->AddChildActor<Actor>() };
			{//子アクターの設定及びコンポーネントの追加
				childActor->SetID(("SphereActor" + std::to_string(jointActors.size() + 1)));
				childActor->AddComponent<SphereModelComponent>();
			}

			//一つのオブジェクトとして処理できるように登録する
			return jointActors.emplace_back(childActor);
		}

		void LinkingSphere::GUI()
		{
			std::string guiName = "LinkingSphere : " + GetID();
			if (ImGui::CollapsingHeader(u8"球体ジョイント管理コンポーネント", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
			{
				ImGui::Text(u8"接続されたジョイント数 ; %d", jointActors.size());
			}
		}
	}
}