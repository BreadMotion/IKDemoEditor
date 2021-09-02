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
			Graphics::IGraphicsDevice* graphicsDevice = nullptr;
			float* objMatrix = nullptr;

		public:
			//生成
			static std::shared_ptr<Actor> Create(Graphics::IGraphicsDevice* graphicsDevice);

			StageActor(Graphics::IGraphicsDevice* graphicDevice)
			{
				graphicsDevice = graphicDevice;
			}
			~StageActor()override {}

		public:
			//初期化
			void Initialize() override;

			//事前更新
			void PreUpdate(const f32& dt) override;

			//更新
			void Update(const f32& dt) override;

			//事後更新
			void NextUpdate(const f32& dt) override;

			//描画
			void Draw(const f32& dt)override;

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