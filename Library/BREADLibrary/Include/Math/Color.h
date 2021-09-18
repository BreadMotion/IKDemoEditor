#pragma once

#include "Types.h"
#include <cereal/cereal.hpp>


namespace Bread
{
	namespace Math
	{
		//============================================================================
		// �J���[
		//----------------------------------------------------------------------------
		class Color
		{
		public:
			union
			{
				struct
				{
					f32			r;					// ��
					f32			g;					// ��
					f32			b;					// ��
					f32			a;					// �A���t�@
				};
				f32				v[4];
			};

			static const Color	White;				// ��
			static const Color	Gray;				// �D
			static const Color	Black;				// ��
			static const Color	Red;				// ��
			static const Color	Green;				// ��
			static const Color	Blue;				// ��
			static const Color	Yellow;				// ��
			static const Color	Cyan;				// ��
			static const Color	Magenta;			// �Ԏ�
			static const Color	Pink;				// ��

		public:
			Color() : Color(White) {}
			Color(const f32& cr, const f32& cg, const f32& cb, const f32& ca) { r = cr; g = cg; b = cb; a = ca; }
			Color(const Color& c)                 { r = c.r; g = c.g; b = c.b; a = c.a; }
			Color(const f32* f)                   { r = f[0]; g = f[1]; b = f[2]; a = f[3]; }

		public:
			// �A�N�Z�X����
			f32& _fastcall operator () (const u32& index);
			f32 _fastcall  operator   () (const u32& index) const;

			// ���Z�q�̃L���X�e�B���O
			_fastcall operator f32* ();
			_fastcall operator const f32* () const;

			// �P�����Z�q
			Color _fastcall operator + () const;
			Color _fastcall operator - () const;

			// �񍀉��Z�q
			Color _fastcall operator + (const Color&) const;
			Color _fastcall operator - (const Color&) const;
			Color _fastcall operator * (const Color&) const;
			Color _fastcall operator * (const f32) const;
			Color _fastcall operator / (const f32) const;

			// ������Z�q
			Color _fastcall operator += (const Color&);
			Color _fastcall operator -= (const Color&);
			Color _fastcall operator *= (const Color&);
			Color _fastcall operator *= (const f32);

			// ��r���Z�q
			bool _fastcall operator == (const Color&) const;
			bool _fastcall operator != (const Color&) const;

		};


		//============================================================================
		// �J���[�Z�p�֐�
		//----------------------------------------------------------------------------
		// 2�̐F�l�����Z���āA�V�����F�l���쐬����B
		Color* _fastcall ColorAdd(Color* out, const Color* c1, const Color* c2);

		// 2�̐F�l�����Z���āA�V�����F�l���쐬����B
		Color* _fastcall ColorSubtract(Color* out, const Color* c1, const Color* c2);

		// �F�l���X�P�[�����O����B
		Color* _fastcall ColorScale(Color* out, const Color* c, f32 s);

		// 2�̐F���u�����h����B
		Color* _fastcall ColorModulate(Color* out, const Color* c1, const Color* c2);

		// �F�̃R���g���X�g�l�𒲐�����B
		Color* _fastcall ColorAdjustContrast(Color* out, const Color* c, f32 s);

		// �F�̍ʓx�l�𒲐�����B
		Color* _fastcall ColorAdjustSaturation(Color* out, const Color* c, f32 s);

		// ���`��Ԃ��g�p���ĐF�l���쐬����B
		Color* _fastcall ColorLerp(Color* out, const Color* c1, const Color* c2, f32 s);

		// ����F�l�ɑ΂��镉�̐F�l���쐬����B
		Color* _fastcall ColorNegative(Color* out, const Color* c);

		// �F��32bitARGB�l�Ƃ��Ď擾���܂��B
		u32 _fastcall ColorARGB32(const Color* c);

		// �F��32bitARGB�l����擾���܂��B
		Color* _fastcall ColorARGB32(Color* out, u32 argb32);

		// �F��32bitABGR�l�Ƃ��Ď擾���܂��B
		u32 _fastcall ColorABGR32(const Color* c);

		// �F��32bitABGR�l����擾���܂��B
		Color* _fastcall ColorABGR32(Color* out, u32 abgr32);


		template<class Archive>
		void _fastcall serialize(Archive& archive, Color& c)
		{
			archive(cereal::make_nvp("r", c.r), cereal::make_nvp("g", c.g), cereal::make_nvp("b", c.b), cereal::make_nvp("a", c.a));
		}

	}	// namespace math
}	// namespace ys