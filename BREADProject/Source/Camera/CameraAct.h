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
			void __fastcall PreUpdate() override;

			//�X�V
			void __fastcall Update() override;

			//����X�V
			void __fastcall NextUpdate() override;

		private:
			std::weak_ptr<Graphics::Camera> camera;
		};
	}
}