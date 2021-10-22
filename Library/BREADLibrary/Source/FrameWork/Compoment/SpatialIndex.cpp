#include "FND/Instance.h"
#include "FrameWork/Component/SpatialIndex.h"
#include "FrameWork/SpatialDivisionManager/SpatialDivisionManager.h"

using Bread::FND::Instance;
using Bread::Math::Vector3S32;
using Bread::Math::Vector3;

namespace Bread
{
	namespace FrameWork
	{
			// ������
			void SpatialIndexComponent::Initialize()
			{
				spatialIndex = { 0,0,0 };
				transform    = GetOwner()->GetComponent<Transform>();
			}

			// �I����
			void SpatialIndexComponent::Finalize() {}

			//���O�X�V
			void SpatialIndexComponent::PreUpdate() {}

			// �X�V
			void SpatialIndexComponent::Update()
			{
				//��ԕ����}�l�[�W���[�Ƃ��̃R���|�[�l���g�̏��L�҂ł���A�N�^�[��Transform�R���|�[�l���g���g����
				//�ǂ̋�ԂɃA�N�^�[�����݂���̂����X�V���ێ�����
				spatialIndex = Instance<SpatialDivisionManager>::instance
					.SpatialCurrent(Math::GetLocation(transform->GetWorldTransform()));
			}

			//����X�V
			void SpatialIndexComponent::NextUpdate() {}

			// �`��
			void SpatialIndexComponent::Draw(){}

			//imgui
			void SpatialIndexComponent::GUI()
			{
				if (ImGui::CollapsingHeader(u8"��ԃC���f�b�N�X", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
				{
					ImGui::DragInt3(u8"��Ԕԍ�", &spatialIndex.x);
				}
			}
	}//namespace Math
}//namespace Bread