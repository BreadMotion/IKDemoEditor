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
			std::weak_ptr<Graphics::IGraphicsDevice> graphicsDevice;
			std::vector<Bread::u32>* targetFaceIndex  = nullptr;

			Graphics::Camera* cameraAct = nullptr;
			float*            objMatrix  = nullptr;

			Math::Vector3 rayVec  = Math::Vector3::Zero;
			Math::Vector3 rayEnd  = Math::Vector3::Zero;
			Math::Vector3 rayStart = Math::Vector3::Zero;
			f32           length   = 0.0f;

		public:
			//生成
			static std::shared_ptr<Actor> __fastcall Create(std::shared_ptr<Graphics::IGraphicsDevice> graphicsDevice, Graphics::Camera* cam);

			IKTargetActor(std::shared_ptr<Graphics::IGraphicsDevice> graphicsDevice,Graphics::Camera* cam)
			{
				this->graphicsDevice = graphicsDevice;
				cameraAct                  = cam;
			}
			~IKTargetActor()override {}

		public:
			//初期化
			void Initialize() override;

			//事前更新
			void __fastcall PreUpdate(const f32& dt) override;

			//更新
			void __fastcall Update(const f32& dt) override;

			//事後更新
			void __fastcall NextUpdate(const f32& dt) override;

			//描画
			void __fastcall Draw(const f32& dt) override;

		public:
			void __fastcall SetObjMatrix(float* m)
			{
				objMatrix = m;
			}

			void __fastcall SetTargetFaceIndex(std::vector<Bread::u32>& targetFaceIndex)
			{
				this->targetFaceIndex = &targetFaceIndex;
			}

			void __fastcall SetTerrainModel(std::shared_ptr<ModelObject> target)
			{
				terrain = target;
			}

			void __fastcall SetTargetModel(std::shared_ptr<ModelObject> target)
			{
				IKModel = target;
			}

			void __fastcall SetRayVec(const Math::Vector3& v)
			{
				rayVec = v;
			}

			void __fastcall SetRayEnd(const Math::Vector3& v)
			{
				rayEnd = v;
			}
			void __fastcall SetRayStart(const Math::Vector3& v)
			{
				rayStart = v;
			}

			void __fastcall SetDistance(const f32& len)
			{
				length = len;
			}

		private:
			std::weak_ptr<Transform>          transform;
			std::weak_ptr<GeometricPrimitive> primitive;
			std::weak_ptr<ModelObject>        terrain;
			std::weak_ptr<ModelObject>        IKModel;
			std::weak_ptr<RayCastCom>         rayCast;
		};
	};
}