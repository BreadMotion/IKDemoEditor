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
			//生成
			static std::shared_ptr<Actor> Create();

			CameraActor()
			{
			}
			~CameraActor()override {}

		public:
			//初期化
			void Initialize() override;

			//事前更新
			void PreUpdate(const f32& dt) override;

			//更新
			void Update(const f32& dt) override;

			//事後更新
			void NextUpdate(const f32& dt) override;

		private:
			Graphics::Camera* camera = nullptr;
		};
	}
}