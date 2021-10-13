#pragma once
#include "Math/BreadMath.h"
#include "FrameWork/Component/Component.h"
#include "FrameWork/Component/Transform.h"

namespace Bread
{
	namespace FrameWork
	{
		//SpatialDivisionManager�ƃA�N�^�[�̍��W���g���ĎZ�o���ꂽ
		//��Ԃ̃C���f�b�N�X�ԍ���ۑ�����N���X
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
			void __fastcall PreUpdate()override;

			// �X�V
			void __fastcall Update()override;

			//����X�V
			void __fastcall NextUpdate()override;

			// �`��
			void __fastcall Draw()override;

			//imgui
			void GUI();

		public:
			const Math::Vector3S32& GetSpatialIndex()
			{
				return SpatialIndex;
			}

		private:
			std::shared_ptr<Transform> transform;
		};
	}//namespace Math
}//namespace Bread