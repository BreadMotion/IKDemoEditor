#pragma once

#include <math.h>
#include "Types.h"


namespace Bread
{
	namespace Math
	{
		//----------------------------------------------------------------------------
		// s8�ŏ��l
		static constexpr s8		MIN_S8 = (-128);
		// s8�ő�l
		static constexpr s8		MAX_S8 = (127);
		// u8�ŏ��l
		static constexpr u8		MIN_U8 = (0);
		// u8�ő�l
		static constexpr u8		MAX_U8 = (255);

		// s16�ŏ��l
		static constexpr s16	MIN_S16 = (-32768);
		// s16�ő�l
		static constexpr s16	MAX_S16 = (32767);
		// u16�ŏ��l
		static constexpr u16	MIN_U16 = (0);
		// u16�ő�l
		static constexpr u16	MAX_U16 = (65535);

		// s32�ŏ��l
		static constexpr s32	MIN_S32 = (-2147483647 - 1);
		// s32�ő�l
		static constexpr s32	MAX_S32 = (2147483647);
		// u32�ŏ��l
		static constexpr u32	MIN_U32 = (0);
		// u32�ő�l
		static constexpr u32	MAX_U32 = (4294967295);

		// f32�ŏ��̐���
		static constexpr f32	MIN_F32 = (1.175494351e-38F);
		// f32�ő�l
		static constexpr f32	MAX_F32 = (3.402823466e+38F);
		// f32�ő�l������
		static constexpr f32	MAX_SQRTF = (1.84467435e+019F);

		// f64�ŏ��l
		static constexpr f64	MIN_F64 = (2.2250738585072014e-308);
		// f64�ő�l
		static constexpr f64	MAX_F64 = (1.7976931348623158e+308);


		//----------------------------------------------------------------------------
		// ��
		static constexpr f32	PI = 3.14159265358979f;
		// �� * 4
		static constexpr f32	PI_4 = PI * 4;
		// �� * 2
		static constexpr f32	PI_2 = PI * 2;
		// �� / 2
		static constexpr f32	I_H = PI / 2.0f;
		// �� / 4
		static constexpr f32	PI_Q = PI / 4.0f;

		// ��
		static constexpr f32	PI_F64 = 3.14159265358979f;
		// �� * 4
		static constexpr f32	PI_4_F64 = PI * 4;
		// �� * 2
		static constexpr f32	PI_2_F64 = PI * 2;
		// �� / 2
		static constexpr f32	PI_H_F64 = PI / 2.0f;
		// �� / 4
		static constexpr f32	PI_Q_F64 = PI / 4.0f;

		// ���R�ΐ��̒�
		static constexpr f32	E = 2.71828182845904523536f;
		// �C�v�V����(��ɋ߂��C�ӂ̔�����)
		static constexpr f32	Epsilon = 1e-06f;


		// 32bit ����(sin) ���v�Z���܂��B
		static f32 __fastcall SinF32(const f32& x);
		inline f32 __fastcall SinF32(const f32& x)
		{
			return ::sinf(x);
		}

		//64bit ����(sin) ���v�Z���܂��B
		static f64 __fastcall SinF64(const f64& x);
		inline f64 __fastcall SinF64(const f64& x)
		{
			return ::sin(x);
		}

		// 32bit �]��(cos) ���v�Z���܂��B
		static f32 __fastcall CosF32(const f32& x);
		inline f32 __fastcall CosF32(const f32& x)
		{
			return ::cosf(x);
		}

		// 64bit �]��(cos) ���v�Z���܂��B
		static f64 __fastcall CosF64(const f64& x);
		inline f64 __fastcall CosF64(const f64& x)
		{
			return ::cos(x);
		}

		// 32bit ����(tan) ���v�Z���܂��B
		static f32 __fastcall TanF32(const f32& x);
		inline f32 __fastcall TanF32(const f32& x)
		{
			return ::tanf(x);
		}

		// 64bit ����(tan) ���v�Z���܂��B
		static f64 __fastcall TanF64(const f64& x);
		inline f64 __fastcall TanF64(const f64& x)
		{
			return ::tan(x);
		}

		// 32bit �t����(asin) ���v�Z���܂��B
		static f32 __fastcall ASinF32(const f32& x);
		inline f32 __fastcall ASinF32(const f32& x)
		{
			return ::asinf(x);
		}

		// 64bit �t����(asin) ���v�Z���܂��B
		static f64 __fastcall ASinF64(const f64& x);
		inline f64 __fastcall ASinF64(const f64& x)
		{
			return ::asin(x);
		}

		// 32bit �t�]��(acos) ���v�Z���܂��B
		static f32 __fastcall ACosF32(const f32& x);
		inline f32 __fastcall ACosF32(const f32& x)
		{
			return ::acosf(x);
		}

		// 64bit �t�]��(acos) ���v�Z���܂��B
		static f64 __fastcall ACosF64(const f64& x);
		inline f64 __fastcall ACosF64(const f64& x)
		{
			return ::acos(x);
		}

		// 32bit �t����(atan) ���v�Z���܂��B
		static f32 __fastcall ATanF32(const f32& x);
		inline f32 __fastcall ATanF32(const f32& x)
		{
			return ::atanf(x);
		}

		// 64bit �t����(atan) ���v�Z���܂��B
		static f64 __fastcall ATanF64(const f64& x);
		inline f64 __fastcall ATanF64(const f64& x)
		{
			return ::atan(x);
		}

		// 32bit y/x�̋t����(atan2) ���v�Z���܂��B
		static f32 __fastcall ATan2F32(const f32& y, const f32& x);
		inline f32 __fastcall ATan2F32(const f32& y, const f32& x)
		{
			return ::atan2f(y, x);
		}

		// 64bit y/x�̋t����(atan2) ���v�Z���܂��B
		static f64 __fastcall ATan2F64(const f64& y, const f64& x);
		inline f64 __fastcall ATan2F64(const f64& y, const f64& x)
		{
			return ::atan2(y, x);
		}

		// 32bit x �̐�Βl���v�Z���C���ʂ� float �^�ŕԂ��܂��B
		static f32 __fastcall FabsF32(const f32& x);
		inline f32 __fastcall FabsF32(const f32& x)
		{
			return ::fabsf(x);
		}

		// 64bit x �̐�Βl���v�Z���C���ʂ� double �^�ŕԂ��܂��B
		static f64 __fastcall FabsF64(const f64& x);
		inline f64 __fastcall FabsF64(const f64& x)
		{
			return ::fabs(x);
		}

		// 32bit ���������v�Z���܂��B
		static f32 __fastcall SqrtF32(const f32& x);
		inline f32 __fastcall SqrtF32(const f32& x)
		{
			return ::sqrtf(x);
		}

		// 64bit ���������v�Z���܂��B
		inline f64 __fastcall SqrtF64(const f64& x)
		{
			return ::sqrt(x);
		}

		// 32bit x �ȏ�̍ŏ��̐����l���v�Z���C���ʂ� float �^�ŕԂ��܂��B
		static f32 __fastcall CeilF32(const f32& x);
		inline f32 __fastcall CeilF32(const f32& x)
		{
			return ::ceilf(x);
		}

		// 64bit x �ȏ�̍ŏ��̐����l���v�Z���C���ʂ� double �^�ŕԂ��܂��B
		static f64 __fastcall CeilF64(const f64& x);
		inline f64 __fastcall CeilF64(const f64& x)
		{
			return ::ceil(x);
		}

		// 32bit x �ȉ��̍ő�̐����l���v�Z���C���ʂ� float �^�ŕԂ��܂��B
		static f32 __fastcall FloorF32(const f32& x);
		inline f32 __fastcall FloorF32(const f32& x)
		{
			return ::floorf(x);
		}

		// 64bit x �ȉ��̍ő�̐����l���v�Z���C���ʂ� double �^�ŕԂ��܂��B
		static f64 __fastcall FloorF64(const f64& x);
		inline f64 __fastcall FloorF64(const f64& x)
		{
			return ::floor(x);
		}

		// 32bit x �� y ����v�Z���C���ʂ� float �^�ŕԂ��܂��B
		static f32 __fastcall PowF32(const f32& x, const f32& y);
		inline f32 __fastcall PowF32(const f32& x, const f32& y)
		{
			return ::powf(x, y);
		}

		// 64bit x �� y ����v�Z���C���ʂ� double �^�ŕԂ��܂��B
		static f64 __fastcall PowF64(const f64& x, const f64& y);
		inline f64 __fastcall PowF64(const f64& x, const f64& y)
		{
			return ::pow(x, y);
		}

		// 32bit x / y �̕��������_�]����v�Z���C���ʂ� float �^�ŕԂ��܂��B
		static f32 __fastcall FmodF32(const f32& x, const f32& y);
		inline f32 __fastcall FmodF32(const f32& x, const f32& y)
		{
			return ::fmodf(x, y);
		}

		// 64bit x / y �̕��������_�]����v�Z���C���ʂ� double �^�ŕԂ��܂��B
		inline f64 __fastcall FmodF64(const f64& x, const f64& y);
		inline f64 __fastcall FmodF64(const f64& x, const f64& y)
		{
			return ::fmod(x, y);
		}

		// value �𐮐����Ə������ɕ������C�������� iptr ���w���I�u�W�F�N�g�Ɋi�[���A
		// �������͖߂�l�Ƃ��� float �^�ŕԂ��܂��B
		static f32 __fastcall ModfF32(const f32& value, f32* iptr);
		inline f32 __fastcall ModfF32(const f32& value, f32* iptr)
		{
			return ::modff(value, iptr);
		}

		// value �𐮐����Ə������ɕ������C�������� iptr ���w���I�u�W�F�N�g�Ɋi�[���A
		// �������͖߂�l�Ƃ��� double �^�ŕԂ��܂��B
		static f64 __fastcall ModfF64(const f64& value, f64* iptr);
		inline f64 __fastcall ModfF64(const f64& value, f64* iptr)
		{
			return ::modf(value, iptr);
		}

		// 32bit ���� x �� NaN (��) ���ǂ����𔻒肵�܂��B
		static bool __fastcall IsNanF32(const f32& x);
		inline bool __fastcall IsNanF32(const f32& x)
		{
			return isnan(x);
		}

		// 64bit �����̒l�� NaN (��) ���ǂ����𔻒肵�܂��B(isnan)
		static bool __fastcall IsNanF64(const f64& x);
		inline bool __fastcall IsNanF64(const f64& x)
		{
			return isnan(x);
		}

		// 32bit �����̒l�� 0.0 ���ǂ����𔻒肵�܂��B
		static bool __fastcall IsZero(const f32& x);
		inline bool __fastcall IsZero(const f32& x)
		{
			return FabsF32(x) <= Epsilon ? true : false;
		}


		// 32bit �����̒l�� 0.0 �ł͂Ȃ����ǂ����𔻒肵�܂��B
		static bool __fastcall NotZero(const f32& x);
		inline bool __fastcall NotZero(const f32& x)
		{
			return FabsF32(x) > Epsilon ? true : false;
		}

		// 32bit �����̒l�����ꂩ�ǂ����𔻒肵�܂��B
		static bool __fastcall Equal(const f32& a, const f32& b);
		inline bool __fastcall Equal(const f32& a, const f32& b)
		{
			return FabsF32(a - b) <= Epsilon ? true : false;
		}

		// 32bit �p�x���b�v�������܂��B
		static f32 __fastcall DirLimit(const f32& x);
		inline f32 __fastcall DirLimit(const f32& x)
		{
			return x >= 0.0f ? FmodF32(x + PI, PI_2) - PI : FmodF32(x - PI, PI_2) + PI;
		}

		// 32bit �p�x���烉�W�A���֕ϊ����܂��B
		static f32 __fastcall ToRadian(const f32& x);
		inline f32 __fastcall ToRadian(const f32& x)
		{
			return x * (PI / 180.0f);
		}

		// 32bit ���W�A������p�x�֕ϊ����܂��B
		static f32 __fastcall ToDegree(const f32& x);
		inline f32 __fastcall ToDegree(const f32& x)
		{
			return 180.0f * (x / PI);
		}

		/**��ʓI�ȕ��@�Ő�Βl���v�Z���܂� */
		template< class T >
		static constexpr T __fastcall Abs(const T A)
		{
			return (A >= (T)0) ? A : -A;
		}

		/** t����0�̊֌W�ɉ�����1,0�A�܂���-1��Ԃ��܂�*/
		template< class T >
		static constexpr T __fastcall Sign(const T A)
		{
			return (A > (T)0) ? (T)1 : ((A < (T)0) ? (T)-1 : (T)0);
		}

		/** Returns higher value in a generic way */
		/**��ʓI�ȕ��@�ō����l��Ԃ��܂�*/
		template< class T >
		static constexpr T __fastcall  Max(const T A, const T B)
		{
			return (A >= B) ? A : B;
		}

		/** Returns lower value in a generic way */
		/** ��ʓI�ȕ��@�ŒႢ�l��Ԃ��܂�*/
		template< class T >
		static constexpr T __fastcall Min(const T A, const T B)
		{
			return (A <= B) ? A : B;
		}

	} // namespace Math
} // namespace Bread