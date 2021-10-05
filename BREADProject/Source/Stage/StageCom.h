#pragma once
#include <string>
#include "FrameWork/Component/Component.h"
#include "Graphics/GraphicsDevice.h"
#include "Types.h"

#include "Graphics/Camera.h"
#include "FrameWork/Input/InputDevice.h"
#include "FrameWork/Object/Object.h"
#include "FrameWork/Component/Transform.h"
#include "FrameWork/Component/VelocityMap.h"
#include "FrameWork/Component/CCDIK.h"

namespace Bread
{
	namespace FrameWork
	{
		class StageActor : public Actor
		{
		private:
			std::weak_ptr<Graphics::IGraphicsDevice> graphicsDevice;
			float* objMatrix = nullptr;

		public:
			//����
			static std::shared_ptr<Actor> Create(std::shared_ptr<Graphics::IGraphicsDevice> graphicsDevice);

			StageActor(std::shared_ptr<Graphics::IGraphicsDevice> graphicDevice)
			{
				graphicsDevice = graphicDevice;
			}
			~StageActor()override {}

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

		public:
			void SetObjMatrix(float* m)
			{
				objMatrix = m;
			}

			// �����蔻��f�[�^�擾
			const std::vector<CollisionData> GetCollisionDatas()
			{
				return collisions;
			}
		private:
			std::vector<CollisionData> collisions;

			std::weak_ptr<ModelObject> stageModel;
			std::weak_ptr<Transform>   transform;
		};
	};
}