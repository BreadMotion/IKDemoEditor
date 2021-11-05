#pragma once
#include <string>
#include "FrameWork/Component/Component.h"
#include "FrameWork/Object/Object.h"
#include "FrameWork/Component/Transform.h"

namespace Bread
{
	namespace FrameWork
	{
		class SphereModelComponent : public Component
		{
		public:
			SphereModelComponent()
			{
			}
			~SphereModelComponent()override {}

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
			void ModelObjectConstruction();
			void TransformConstruction();

		private:
			Math::Vector3                worldTargetPos{ Math::Vector3{0.0,300.0f,0.0f} };
			std::shared_ptr<ModelObject> model{ nullptr };
			std::shared_ptr<Transform>   transform{ nullptr };
		};
	};
}