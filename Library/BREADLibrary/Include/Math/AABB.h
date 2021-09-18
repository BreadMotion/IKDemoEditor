#pragma once

#include "Math/Matrix.h"
#include "Math/Vector.h"


namespace Bread
{
	namespace Math
	{
		//****************************************************************************
		// �o�E���f�B���O�{�b�N�X
		//****************************************************************************
		class AABB
		{
		public:
			Vector3 min; // �ŏ��l
			Vector3 max; // �ő�l

			static const AABB Zero; // �[��

		public:
			AABB() {}
			AABB(const Vector3& min, const Vector3& max) { this->min = min; this->max = max; }
			~AABB() {}

		public:
			// AABB�͈̔͂����Z�b�g���܂�
			void Reset();

			// �^����ꂽ�s�������AABB�͈̔͂�ݒ肵�܂�
			void __fastcall Transform(const Matrix& transform);

			// �^����ꂽAABB���܂ނ悤�ɔ͈͂��g�����܂�
			void __fastcall Extend(const AABB& aabb);

			// �^����ꂽ�_���܂ނ悤�ɔ͈͂��g�����܂�
			void __fastcall Extend(const Vector3& point);

			// �^����ꂽ�_���͈͓��Ɋ܂܂�Ă��邩���肵�܂�
			bool __fastcall Contain(const Vector3& point);

			// AABB�̃R�[�i�[�ʒu���擾���܂�
			void __fastcall GetCorners(Vector3 corners[8]);

			// AABB�̒��S�ʒu���擾���܂�
			Vector3 __fastcall GetCenter() const { return (min + max) / 2.0f; }

			// ��
			bool __fastcall IsEmpty() const { return min == max; }

			// ���Ƃ̌�������
			bool __fastcall Intersect(const Math::Vector3& center, f32 radius) const;

			// AABB�Ƃ̌�������
			bool __fastcall Intersect(const Math::AABB& aabb) const;
		};

	} // namespace Math
} // namespace Bread