#pragma once
#include <string>
#include "FrameWork/Component/Component.h"
#include "Types.h"

#include "Graphics/Camera.h"

namespace Bread
{
	namespace FrameWork
	{
		class CameraComponent : public Component
		{
		public:
			CameraComponent()
			{
			}
			~CameraComponent()override {}

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
			std::shared_ptr<Graphics::Camera> camera;
		};
	}
}