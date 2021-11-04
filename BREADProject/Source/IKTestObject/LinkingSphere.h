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
			//������
			void Initialize()            override;

			//���O�X�V
			void __fastcall PreUpdate()  override;

			//�X�V
			void __fastcall Update()     override;

			//����X�V
			void __fastcall NextUpdate() override;

			//�`��
			void __fastcall Draw()override;

			//GUI
			void GUI()override;

		private:
			//��������Component�̍\�z
			void ComponentConstruction();

			//�e�R���|�[�l���g�̍\�z�֐�
			void TransformConstruction();

		public://unique Function

			//���̃R���|�[�l���g���ǉ������{�[���֌W���������A�N�^�[���C���f�b�N�X�ԍ�����擾����
			std::shared_ptr<Actor> GetChildActor(const u32& index);

			//���̃R���|�[�l���g���Ǘ�����{�[���֌W���������A�N�^�[�𐶐�����
			std::shared_ptr<Actor> AddJoint(std::shared_ptr<Actor> owner);

		private:
			std::vector<std::shared_ptr<Actor>> jointActors;
			std::shared_ptr<Transform>   transform{ nullptr };
		};
	};
}