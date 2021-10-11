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
			void Initialize();

			// 終了化
			void Finalize();

			//事前更新
			void __fastcall PreUpdate();

			// 更新
			void __fastcall Update();

			//事後更新
			void __fastcall NextUpdate();

			// 描画
			void __fastcall Draw();

			//imgui
			void GUI();

			void Destroy()
			{
				delete this;
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