#pragma once
#include <string>
#include "FrameWork/Component/Component.h"
#include "FrameWork/Object/Object.h"
#include "FrameWork/Component/Transform.h"

namespace Bread
{
	namespace FrameWork
	{
		class StageComponent : public Component
		{
		public:
			StageComponent()
			{
			}
			~StageComponent()override {}

		public:
			//������
			void Initialize() override;

			//���O�X�V
			void __fastcall PreUpdate() override;

			//�X�V
			void __fastcall Update() override;

			//����X�V
			void __fastcall NextUpdate() override;

			//�`��
			void __fastcall Draw()override;

		private:
			//��������Component�̍\�z
			void ComponentConstruction();

			//�e�R���|�[�l���g�̍\�z�֐�
			void ModelObjectConstruction();
			void TransformConstruction();

		public:
			// �����蔻��f�[�^�擾
			const std::vector<CollisionData> GetCollisionDatas()
			{
				return collisions;
			}

		private:
			std::vector<CollisionData>   collisions;
			std::shared_ptr<ModelObject> model         {nullptr  };
			std::shared_ptr<ModelObject> collisionModel{ nullptr };
			std::shared_ptr<Transform>   transform     {nullptr  };
		};
	};
}