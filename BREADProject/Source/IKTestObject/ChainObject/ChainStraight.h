#pragma once
#include "Math/BreadMath.h"
#include "FrameWork/Component/Component.h"
#include "../LinkingSphere.h"
#include "FrameWork/Component/Transform.h""

namespace Bread
{
	namespace FrameWork
	{
		class ChainStraight : public Component
		{
		public://constructor
			ChainStraight() = default;
			~ChainStraight() override {}

		public://component's override function
			//������
			void Initialize()override;

			//���O�X�V
			void _fastcall PreUpdate()override;

			//�X�V
			void _fastcall Update()override;

			//����X�V
			void _fastcall NextUpdate()override;

			//GUI
			void GUI()override;

		private:
			//��������Component�̍\�z
			void ComponentConstruction();

			//�e�R���|�[�l���g�̍\�z�֐�
			void TransformConstruction();
			void LinkingSphereConstruction();

		private:
			Math::Vector3                  worldTargetPos{ Math::Vector3{0.0f,500.0,0.0f} };

			std::shared_ptr<Transform>     transform;
			std::shared_ptr<LinkingSphere> linkSphere;
		};
	}
}
