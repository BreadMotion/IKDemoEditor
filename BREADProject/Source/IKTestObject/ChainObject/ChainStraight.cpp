#include "ChainStraight.h"
#include "FND/Instance.h"
#include "FrameWork/Component/ChainFABRIKManager.h"

using Bread::FND::Instance; //ChainFABRIKManager

namespace Bread
{
	namespace FrameWork
	{
		//初期化
		void ChainStraight::Initialize()
		{
			ComponentConstruction();
			Instance<InverseKinematics::FARBIKManager>::instance.RegisterFABRIK(linkSphere->GetAllIJoint(), transform, &worldTargetPos);
		}

		//事前更新
		void ChainStraight::PreUpdate()
		{

		}

		//更新
		void ChainStraight::Update()
		{
		}

		//事後更新
		void ChainStraight::NextUpdate()
		{
			Instance<InverseKinematics::FARBIKManager>::instance.Update();
			Instance<InverseKinematics::FARBIKManager>::instance.GUI();
		}

		//生成したComponentの構築
		void ChainStraight::ComponentConstruction()
		{
			TransformConstruction();
			LinkingSphereConstruction();
		}

		//各コンポーネントの構築関数
		void ChainStraight::TransformConstruction()
		{
			transform = GetOwner()->GetComponent<Transform>();
			transform->SetID("chainStraightTransform");
			transform->SetScale(Math::Vector3{ 0.1f,0.1f ,0.1f });
		}

		void ChainStraight::LinkingSphereConstruction()
		{
			linkSphere = GetOwner()->AddComponent<LinkingSphere>();
			linkSphere->SetID("linkingSphere");

			{//チェーン状になるように子アクターを生成
			 //＆LinkingSphereに登録
				auto lastJoint = linkSphere->AddJoint(GetOwner());
				constexpr u32 chainMaxJoint{ 10 };
				for (u32 i = 0; i < chainMaxJoint; ++i)
				{
					lastJoint = linkSphere->AddJoint(lastJoint);
					lastJoint->GetComponent<Transform>()->SetTranslate(Math::Vector3{ 0.0f, 350.0f, 0.0f });
				}
			}
		}

		void ChainStraight::GUI()
		{
			std::string guiName = "ChainStraight : " + GetID();
			if (ImGui::CollapsingHeader(u8"直線チェーン", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
			{
				ImGui::DragFloat3("WorldTargetPos", worldTargetPos);
			}
		}
	}
}