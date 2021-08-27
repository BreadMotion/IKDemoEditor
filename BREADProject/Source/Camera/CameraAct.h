#pragma once
#include <string>
#include "FrameWork/Component/Component.h"
#include "Types.h"

#include "Graphics/Camera.h"

namespace Bread
{
	namespace FrameWork
	{
		class CameraActor : public Actor
		{
		public:
			//����
			static std::shared_ptr<Actor> Create();

			CameraActor()
			{
			}
			~CameraActor()override {}

		public:
			//������
			void Initialize() override;

			//���O�X�V
			void PreUpdate(const f32& dt) override;

			//�X�V
			void Update(const f32& dt) override;

			//����X�V
			void NextUpdate(const f32& dt) override;

		private:
			Graphics::Camera* camera = nullptr;
		};
	}
}