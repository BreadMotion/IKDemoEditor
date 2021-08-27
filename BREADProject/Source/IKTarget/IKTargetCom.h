#pragma once
#include "Math/BreadMath.h"
#include "Graphics/GraphicsDevice.h"

#include "../Primitive/GeometricPrimitive.h"

#include "FrameWork/Component/Transform.h"
#include "../RayCast/RayCast.h"

namespace Bread
{
	namespace FrameWork
	{
		class IKTargetActor : public Actor
		{
		private:
			Graphics::IGraphicsDevice* graphicsDevice = nullptr;
			std::vector<Bread::u32>* targetFaceIndex  = nullptr;

			Graphics::Camera* cameraAct = nullptr;
			float*                      objMatrix  = nullptr;

			Math::Vector3 rayVec  = Math::Vector3::Zero;
			Math::Vector3 rayEnd  = Math::Vector3::Zero;
			Math::Vector3 rayStart = Math::Vector3::Zero;
			f32                  length   = 0.0f;

		public:
			//生成
			static std::shared_ptr<Actor> Create(Graphics::IGraphicsDevice* graphicsDevice, Graphics::Camera* cam);

			IKTargetActor(Graphics::IGraphicsDevice* graphicsDevice,Graphics::Camera* cam)
			{
				this->graphicsDevice = graphicsDevice;
				cameraAct                  = cam;
			}
			~IKTargetActor()override {}

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
			void Draw(const f32& dt) override;

		public:
			void SetObjMatrix(float* m)
			{
				objMatrix = m;
			}

			void SetTargetFaceIndex(std::vector<Bread::u32>& targetFaceIndex)
			{
				this->targetFaceIndex = &targetFaceIndex;
			}

			void SetTerrainModel(ModelObject* target)
			{
				terrain = target;
			}

			void SetTargetModel(ModelObject* target)
			{
				IKModel = target;
			}

			void SetRayVec(const Math::Vector3& v)
			{
				rayVec = v;
			}

			void SetRayEnd(const Math::Vector3& v)
			{
				rayEnd = v;
			}
			void SetRayStart(const Math::Vector3& v)
			{
				rayStart = v;
			}

			void SetDistance(const f32& len)
			{
				length = len;
			}

		private:
			Transform*               transform = nullptr;
			GeometricPrimitive* primitive  = nullptr;
			ModelObject*           terrain     = nullptr;
			ModelObject*           IKModel  = nullptr;
			RayCastCom*            rayCast    = nullptr;
		};
	};
}