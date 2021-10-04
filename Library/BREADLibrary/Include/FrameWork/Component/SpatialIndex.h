#pragma once
#include "Math/BreadMath.h"
#include "FrameWork/Component/Component.h"

namespace Bread
{
	namespace FrameWork
	{
		class SpatialIndexComponent : public Component
		{
		private:
			Math::Vector3S32 SpatialIndex;

		public:
			// ������
			void Initialize()override;

			// �I����
			void Finalize()override;

			//���O�X�V
			void __fastcall PreUpdate(const f32&)override;

			// �X�V
			void __fastcall Update(const f32&)override;

			//����X�V
			void __fastcall NextUpdate(const f32&)override;

			// �`��
			void __fastcall Draw(const f32&)override;

			//imgui
			void GUI() {}
		};
	}//namespace Math
}//namespace Bread