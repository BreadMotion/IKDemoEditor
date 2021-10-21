#pragma once
#include <string>
#include "FrameWork/Component/Component.h"
#include "Types.h"

#include "Graphics/GraphicsDevice.h"

#include "FrameWork/Object/Object.h"
#include "FrameWork/Component/Transform.h"

namespace Bread
{
	namespace FrameWork
	{
		//���C�L���X�g���s���A���ʂ����N���X
		class RayCastCom : public Component
		{
		private://���C�L���X�g�̓����f�[�^

			Math::Vector3  start{ Math::Vector3::Zero };           //���C�L���X�g�̎n�_
			Math::Vector3  end  { Math::Vector3::Zero };           //���C�L���X�g�̏I�_

			bool hitFlag{ false };                                 //���C�L���X�g�̔��茋��
			bool useFlag{ false };                                 //���C�L���X�g�̎��s

			f32 minDot    { 0.0f   };                              //���C�L���X�g����������Ίp�̍ŏ��l

		public:
			// �q�b�g���ʁi���C�L���X�g�Ŏ��o�����j
			struct HitResult
			{
				Math::Vector3   position = { 0,0,0 }; // ���C�ƃ|���S���̌�_
				Math::Vector3   normal   = { 0,0,0 }; // �Փ˂����|���S���̖@���x�N�g��
				f32             distance = 0.0f;      // ���C�̎n�_�����_�܂ł̋���
				Math::Vector3   start    = { 0,0,0 }; // �Փ˂������C�̎n�_�ʒu ��MultiRayCast�̂�
				Math::Vector3   end      = { 0,0,0 }; // �Փ˂������C�̏I�_�ʒu ��MultiRayCast�̂�
			};
			HitResult hitResult;

		public://constractor
			explicit RayCastCom()
			{
			}
			~RayCastCom()override {}

		public://Component Override

			//������
			void Initialize() override;

			//���O�X�V
			void __fastcall PreUpdate()override {}

			//�X�V
			void __fastcall Update() override;

			//����X�V
			void __fastcall NextUpdate()override {}

			//�`��
			void __fastcall Draw()override;

			//GUI
			void GUI() override
			{
				using namespace ImGui;
				std::string            guiName{ "RayCast : " + GetID() };

				if (ImGui::CollapsingHeader(u8"���C�L���X�g", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
				{
					char  name[128] = {};
					FND::StrCpy(name, sizeof(name), GetID().c_str());

					ImGui::Text(u8"���O"); ImGui::SameLine();
					ImGui::InputText(("##" + GetID()).c_str(), name, IM_ARRAYSIZE(name));
					SetID(name);

					ImGui::Separator();

					Text(u8"�ŏ��p�x  : %f", minDot);

					Checkbox("hitFlag", &hitFlag); ImGui::SameLine();
					Checkbox("useFlag", &useFlag);

					Separator();

					DragFloat3("start"       , start              );
					DragFloat3("end"         , end                );
					DragFloat3("hitPos"      , hitResult.position );
					DragFloat3("hitNormal"   , hitResult.normal   );
					DragFloat("toHitDistance", &hitResult.distance);
					DragFloat3("hitStart"    , hitResult.start    );
					DragFloat3("hitEnd"      , hitResult.end      );
				}
			}

		public://RayCast Interface

			//���C�L���X�g�̎n�_��ݒ�
			void __fastcall SetStartPosition(const Math::Vector3& startPos)
			{
				start = startPos;
			}

			//���C�L���X�g�̏I�_��ݒ�
			void __fastcall SetEndPosition(const Math::Vector3& endPos)
			{
				end   = endPos;
			}

			//���C�L���X�g�̋�����ݒ�
			void __fastcall SetDistance(const f32 distance)
			{
				hitResult.distance = distance;
			}

			//���C�L���X�g���������Ă��邩�m�点��
			const bool& GetHItFlag()
			{
				return hitFlag;
			}

			//���C�L���X�g���s���Ă��邩��m�点��
			const bool& GetUseFlag()
			{
				return useFlag;
			}

		public:
			//Target�̃��f���Ɛ��i���C�j�Ƃ̓����蔻����s��
			bool IntersectRayVsModel();

		};
	};
}