#pragma once
#include "FrameWork/Component/Component.h"
#include "Math/BreadMath.h"
#include "Graphics/GraphicsDevice.h"

#include "../Primitive/GeometricPrimitive.h"

#include "FrameWork/Component/Transform.h"
#include "../RayCast/RayCast.h"

namespace Bread
{
	namespace FrameWork
	{
		class [[]] IKTargetComponent : public Component
		{
		private:
			Math::Vector3 rayVec   {Math::Vector3::Zero};
			Math::Vector3 rayEnd   {Math::Vector3::Zero};
			Math::Vector3 rayStart {Math::Vector3::Zero};
			f32           length{ 0.0f };

		public:
			IKTargetComponent() = default;
			~IKTargetComponent() {}

		public:
			// 初期化
			void Initialize()override;

			// 終了化
			void Finalize()override {}

			//事前更新
			void __fastcall PreUpdate()override;

			// 更新
			void __fastcall Update()override;

			//事後更新
			void __fastcall NextUpdate()override;

			// 描画
			void __fastcall Draw()override;

			//imgui
			void GUI() override {}

			void Destroy()
			{
				delete this;
			}

		public:
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
			void ComponentConstruction();

			void TransformConstruction();
			void RayCastConstruction();
			void PrimitiveConstruction();

		private:
			std::shared_ptr<Transform>           transform{ nullptr };
			std::shared_ptr<RayCastCom>          rayCast{ nullptr };
			std::shared_ptr<GeometricPrimitive>  primitive{ nullptr };
		};
	}
}//namespace Bread