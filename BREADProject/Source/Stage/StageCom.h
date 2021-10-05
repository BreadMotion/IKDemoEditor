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
			//生成
			static std::shared_ptr<Actor> Create(std::shared_ptr<Graphics::IGraphicsDevice> graphicsDevice);

			StageActor(std::shared_ptr<Graphics::IGraphicsDevice> graphicDevice)
			{
				graphicsDevice = graphicDevice;
			}
			~StageActor()override {}

		public:
			//初期化
			void Initialize() override;

			//事前更新
			void __fastcall PreUpdate() override;

			//更新
			void __fastcall Update() override;

			//事後更新
			void __fastcall NextUpdate() override;

			//描画
			void __fastcall Draw()override;

		public:
			void SetObjMatrix(float* m)
			{
				objMatrix = m;
			}

			// 当たり判定データ取得
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