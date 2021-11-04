#include "ChainStraight.h"

namespace Bread
{
	namespace FrameWork
	{
		//������
		void ChainStraight::Initialize()
		{
			ComponentConstruction();
		}

		//���O�X�V
		void ChainStraight::PreUpdate()
		{

		}

		//�X�V
		void ChainStraight::Update()
		{

		}

		//����X�V
		void ChainStraight::NextUpdate()
		{

		}

		//��������Component�̍\�z
		void ChainStraight::ComponentConstruction()
		{
			TransformConstruction();
			LinkingSphereConstruction();
		}

		//�e�R���|�[�l���g�̍\�z�֐�
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

			{//�`�F�[����ɂȂ�悤�Ɏq�A�N�^�[�𐶐�
			 //��LinkingSphere�ɓo�^
				auto lastJoint = linkSphere->AddJoint(GetOwner());
				constexpr u32 chainMaxJoint{ 10 };
				for (u32 i = 0; i < chainMaxJoint; ++i)
				{
					lastJoint = linkSphere->AddJoint(lastJoint);
					lastJoint->GetComponent<Transform>()->SetTranslate(Math::Vector3{ 0.0f, 300.0f, 0.0f });
				}
			}
		}

		void ChainStraight::GUI()
		{
			std::string guiName = "ChainStraight : " + GetID();
			if (ImGui::CollapsingHeader(u8"�����`�F�[��", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
			{
			}
		}
	}
}