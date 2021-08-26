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
			//¶¬
			static std::shared_ptr<Actor> Create();

			CameraActor()
			{
			}
			~CameraActor()override {}

		public:
			//‰Šú‰»
			void Initialize() override;

			//XV
			void Update(const f32& dt) override;

		private:
			Graphics::Camera* camera = nullptr;
		};
	}
}