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

		//指定したインデックスのジョイントのアクターを取得する
		std::shared_ptr<Actor> LinkingSphere::GetChildActor(const u32& index)
		{
			return jointActors[index];
		}
		std::vector<ITransform*>* LinkingSphere::GetAllIJoint()
		{
			return &myJoint.joins;
		}
		IJointAssembly* LinkingSphere::GetJointAssembly()
		{
			return &myJoint;
		}

		//指定したインデックスのジョイントを取得する
		ITransform* LinkingSphere::GetIJoint(const u32& index)
		{
			return myJoint.joins.at(index);
		}
		std::vector<std::shared_ptr<Actor>>* LinkingSphere::GetAllJointActors()
		{
			return &jointActors;
		}

		std::shared_ptr<Actor> LinkingSphere::AddJoint(std::shared_ptr<Actor> owner)
		{
			//一つのオブジェクトとして処理できるように登録する
			jointActors.emplace_back(myJoint.AddJoint<SphereModelComponent>(owner));
			jointActors.back()->SetID("linkSphere:" + std::to_string(jointActors.size()));
			return jointActors.back();
		}

		void LinkingSphere::GUI()
		{
#if 0
			std::string guiName = "LinkingSphere : " + GetID();
			if (ImGui::CollapsingHeader(u8"球体ジョイント管理コンポーネント", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
			{
				ImGui::Text(u8"接続されたジョイント数 ; %d", jointActors.size());

				ITransform* test{ myJoint.joins.at(0) };
				static IJoint* selectNode{ dynamic_cast<IJoint*>(test) };
				{
					static char buf1[128] = "";
					ImGui::InputText("FIlter", buf1, 128, ImGuiInputTextFlags_EnterReturnsTrue);

					ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
					ImGui::BeginChild(u8"ジョイントリスト", ImVec2(ImGui::GetWindowContentRegionWidth() * 1.0f, 260), true);

					u32 iterate{ 0 };
					for (auto* node : myJoint.joins)
					{
						IJoint* jointnode{ dynamic_cast<IJoint*>(node) };
						iterate++;
						std::string t1{ buf1 + std::to_string(iterate) };

						if (!strstr((jointnode->name + std::to_string(iterate)).c_str(), t1.c_str()))continue;

						if (ImGui::Selectable((jointnode->name + std::to_string(iterate)).c_str(), (selectNode->name + std::to_string(iterate)) == (jointnode->name + std::to_string(iterate))))
						{
							selectNode = jointnode;
						}
					}
					ImGui::EndChild();
					ImGui::PopStyleVar();
				}

				std::string jName{ selectNode->name };

				ImGui::DragFloat3("translate", &selectNode->translate.x);
				ImGui::DragFloat4("rotate"   , &selectNode->rotate.x);

				Math::Vector3 euler{ Math::ConvertToRollPitchYawFromQuaternion(selectNode->rotate) };
				euler.x = Math::ToDegree(euler.x); euler.y = Math::ToDegree(euler.y); euler.z = Math::ToDegree(euler.z);
				ImGui::DragFloat3("euler", euler);

				ImGui::DragFloat3("scale", &selectNode->scale.x);
				ImGui::Separator();

				ImGui::DragFloat3("worldT", Math::GetLocation(selectNode->worldTransform));
				ImGui::DragFloat4("worldR", Math::GetRotation(selectNode->worldTransform));
				ImGui::DragFloat3("worldS", Math::GetScale(selectNode->worldTransform));
			}
#endif
		}
	}
}