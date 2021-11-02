#pragma once
#include <string>
#include "FrameWork/Actor/Actor.h"
#include "FrameWork/Component/Component.h"
#include "FrameWork/Object/Object.h"
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

		private:
			//��������Component�̍\�z
			void ComponentConstruction();

			//�e�R���|�[�l���g�̍\�z�֐�
			void TransformConstruction();

		private:
			std::shared_ptr<Transform>   transform{ nullptr };
			std::vector<IJoint*>         joints;
		};
	};
}