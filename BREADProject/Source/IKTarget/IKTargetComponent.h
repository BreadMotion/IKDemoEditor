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
			// ������
			void Initialize();

			// �I����
			void Finalize();

			//���O�X�V
			void __fastcall PreUpdate();

			// �X�V
			void __fastcall Update();

			//����X�V
			void __fastcall NextUpdate();

			// �`��
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