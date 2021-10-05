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
			void __fastcall PreUpdate() override;

			//更新
			void __fastcall Update() override;

			//事後更新
			void __fastcall NextUpdate() override;

		private:
			std::weak_ptr<Graphics::Camera> camera;
		};
	}
}