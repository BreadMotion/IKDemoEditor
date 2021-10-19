#pragma once

#include <math.h>
#include "Types.h"


namespace Bread
{
	namespace Math
	{
		//----------------------------------------------------------------------------
		// s8最小値
		static constexpr s8		MIN_S8 = (-128);
		// s8最大値
		static constexpr s8		MAX_S8 = (127);
		// u8最小値
		static constexpr u8		MIN_U8 = (0);
		// u8最大値
		static constexpr u8		MAX_U8 = (255);

		// s16最小値
		static constexpr s16	MIN_S16 = (-32768);
		// s16最大値
		static constexpr s16	MAX_S16 = (32767);
		// u16最小値
		static constexpr u16	MIN_U16 = (0);
		// u16最大値
		static constexpr u16	MAX_U16 = (65535);

		// s32最小値
		static constexpr s32	MIN_S32 = (-2147483647 - 1);
		// s32最大値
		static constexpr s32	MAX_S32 = (2147483647);
		// u32最小値
		static constexpr u32	MIN_U32 = (0);
		// u32最大値
		static constexpr u32	MAX_U32 = (4294967295);

		// f32最小の正数
		static constexpr f32	MIN_F32 = (1.175494351e-38F);
		// f32最大値
		static constexpr f32	MAX_F32 = (3.402823466e+38F);
		// f32最大値平方根
		static constexpr f32	MAX_SQRTF = (1.84467435e+019F);

		// f64最小値
		static constexpr f64	MIN_F64 = (2.2250738585072014e-308);
		// f64最大値
		static constexpr f64	MAX_F64 = (1.7976931348623158e+308);


		//----------------------------------------------------------------------------
		// π
		static constexpr f32	PI = 3.14159265358979f;
		// π * 4
		static constexpr f32	PI_4 = PI * 4;
		// π * 2
		static constexpr f32	PI_2 = PI * 2;
		// π / 2
		static constexpr f32	I_H = PI / 2.0f;
		// π / 4
		static constexpr f32	PI_Q = PI / 4.0f;

		// π
		static constexpr f32	PI_F64 = 3.14159265358979f;
		// π * 4
		static constexpr f32	PI_4_F64 = PI * 4;
		// π * 2
		static constexpr f32	PI_2_F64 = PI * 2;
		// π / 2
		static constexpr f32	PI_H_F64 = PI / 2.0f;
		// π / 4
		static constexpr f32	PI_Q_F64 = PI / 4.0f;

		// 自然対数の底
		static constexpr f32	E = 2.71828182845904523536f;
		// イプシロン(零に近い任意の微少量)
		static constexpr f32	Epsilon = 1e-06f;


		// 32bit 正弦(sin) を計算します。
		static f32 __fastcall SinF32(const f32& x);
		inline f32 __fastcall SinF32(const f32& x)
		{
			return ::sinf(x);
		}

		//64bit 正弦(sin) を計算します。
		static f64 __fastcall SinF64(const f64& x);
		inline f64 __fastcall SinF64(const f64& x)
		{
			return ::sin(x);
		}

		// 32bit 余弦(cos) を計算します。
		static f32 __fastcall CosF32(const f32& x);
		inline f32 __fastcall CosF32(const f32& x)
		{
			return ::cosf(x);
		}

		// 64bit 余弦(cos) を計算します。
		static f64 __fastcall CosF64(const f64& x);
		inline f64 __fastcall CosF64(const f64& x)
		{
			return ::cos(x);
		}

		// 32bit 正接(tan) を計算します。
		static f32 __fastcall TanF32(const f32& x);
		inline f32 __fastcall TanF32(const f32& x)
		{
			return ::tanf(x);
		}

		// 64bit 正接(tan) を計算します。
		static f64 __fastcall TanF64(const f64& x);
		inline f64 __fastcall TanF64(const f64& x)
		{
			return ::tan(x);
		}

		// 32bit 逆正弦(asin) を計算します。
		static f32 __fastcall ASinF32(const f32& x);
		inline f32 __fastcall ASinF32(const f32& x)
		{
			return ::asinf(x);
		}

		// 64bit 逆正弦(asin) を計算します。
		static f64 __fastcall ASinF64(const f64& x);
		inline f64 __fastcall ASinF64(const f64& x)
		{
			return ::asin(x);
		}

		// 32bit 逆余弦(acos) を計算します。
		static f32 __fastcall ACosF32(const f32& x);
		inline f32 __fastcall ACosF32(const f32& x)
		{
			return ::acosf(x);
		}

		// 64bit 逆余弦(acos) を計算します。
		static f64 __fastcall ACosF64(const f64& x);
		inline f64 __fastcall ACosF64(const f64& x)
		{
			return ::acos(x);
		}

		// 32bit 逆正接(atan) を計算します。
		static f32 __fastcall ATanF32(const f32& x);
		inline f32 __fastcall ATanF32(const f32& x)
		{
			return ::atanf(x);
		}

		// 64bit 逆正接(atan) を計算します。
		static f64 __fastcall ATanF64(const f64& x);
		inline f64 __fastcall ATanF64(const f64& x)
		{
			return ::atan(x);
		}

		// 32bit y/xの逆正接(atan2) を計算します。
		static f32 __fastcall ATan2F32(const f32& y, const f32& x);
		inline f32 __fastcall ATan2F32(const f32& y, const f32& x)
		{
			return ::atan2f(y, x);
		}

		// 64bit y/xの逆正接(atan2) を計算します。
		static f64 __fastcall ATan2F64(const f64& y, const f64& x);
		inline f64 __fastcall ATan2F64(const f64& y, const f64& x)
		{
			return ::atan2(y, x);
		}

		// 32bit x の絶対値を計算し，結果を float 型で返します。
		static f32 __fastcall FabsF32(const f32& x);
		inline f32 __fastcall FabsF32(const f32& x)
		{
			return ::fabsf(x);
		}

		// 64bit x の絶対値を計算し，結果を double 型で返します。
		static f64 __fastcall FabsF64(const f64& x);
		inline f64 __fastcall FabsF64(const f64& x)
		{
			return ::fabs(x);
		}

		// 32bit 平方根を計算します。
		static f32 __fastcall SqrtF32(const f32& x);
		inline f32 __fastcall SqrtF32(const f32& x)
		{
			return ::sqrtf(x);
		}

		// 64bit 平方根を計算します。
		inline f64 __fastcall SqrtF64(const f64& x)
		{
			return ::sqrt(x);
		}

		// 32bit x 以上の最小の整数値を計算し，結果を float 型で返します。
		static f32 __fastcall CeilF32(const f32& x);
		inline f32 __fastcall CeilF32(const f32& x)
		{
			return ::ceilf(x);
		}

		// 64bit x 以上の最小の整数値を計算し，結果を double 型で返します。
		static f64 __fastcall CeilF64(const f64& x);
		inline f64 __fastcall CeilF64(const f64& x)
		{
			return ::ceil(x);
		}

		// 32bit x 以下の最大の整数値を計算し，結果を float 型で返します。
		static f32 __fastcall FloorF32(const f32& x);
		inline f32 __fastcall FloorF32(const f32& x)
		{
			return ::floorf(x);
		}

		// 64bit x 以下の最大の整数値を計算し，結果を double 型で返します。
		static f64 __fastcall FloorF64(const f64& x);
		inline f64 __fastcall FloorF64(const f64& x)
		{
			return ::floor(x);
		}

		// 32bit x の y 乗を計算し，結果を float 型で返します。
		static f32 __fastcall PowF32(const f32& x, const f32& y);
		inline f32 __fastcall PowF32(const f32& x, const f32& y)
		{
			return ::powf(x, y);
		}

		// 64bit x の y 乗を計算し，結果を double 型で返します。
		static f64 __fastcall PowF64(const f64& x, const f64& y);
		inline f64 __fastcall PowF64(const f64& x, const f64& y)
		{
			return ::pow(x, y);
		}

		// 32bit x / y の浮動小数点余剰を計算し，結果を float 型で返します。
		static f32 __fastcall FmodF32(const f32& x, const f32& y);
		inline f32 __fastcall FmodF32(const f32& x, const f32& y)
		{
			return ::fmodf(x, y);
		}

		// 64bit x / y の浮動小数点余剰を計算し，結果を double 型で返します。
		inline f64 __fastcall FmodF64(const f64& x, const f64& y);
		inline f64 __fastcall FmodF64(const f64& x, const f64& y)
		{
			return ::fmod(x, y);
		}

		// value を整数部と小数部に分解し，整数部を iptr が指すオブジェクトに格納し、
		// 小数部は戻り値として float 型で返します。
		static f32 __fastcall ModfF32(const f32& value, f32* iptr);
		inline f32 __fastcall ModfF32(const f32& value, f32* iptr)
		{
			return ::modff(value, iptr);
		}

		// value を整数部と小数部に分解し，整数部を iptr が指すオブジェクトに格納し、
		// 小数部は戻り値として double 型で返します。
		static f64 __fastcall ModfF64(const f64& value, f64* iptr);
		inline f64 __fastcall ModfF64(const f64& value, f64* iptr)
		{
			return ::modf(value, iptr);
		}

		// 32bit 引数 x が NaN (非数) かどうかを判定します。
		static bool __fastcall IsNanF32(const f32& x);
		inline bool __fastcall IsNanF32(const f32& x)
		{
			return isnan(x);
		}

		// 64bit 引数の値が NaN (非数) かどうかを判定します。(isnan)
		static bool __fastcall IsNanF64(const f64& x);
		inline bool __fastcall IsNanF64(const f64& x)
		{
			return isnan(x);
		}

		// 32bit 引数の値が 0.0 かどうかを判定します。
		static bool __fastcall IsZero(const f32& x);
		inline bool __fastcall IsZero(const f32& x)
		{
			return FabsF32(x) <= Epsilon ? true : false;
		}


		// 32bit 引数の値が 0.0 ではないかどうかを判定します。
		static bool __fastcall NotZero(const f32& x);
		inline bool __fastcall NotZero(const f32& x)
		{
			return FabsF32(x) > Epsilon ? true : false;
		}

		// 32bit 引数の値が同一かどうかを判定します。
		static bool __fastcall Equal(const f32& a, const f32& b);
		inline bool __fastcall Equal(const f32& a, const f32& b)
		{
			return FabsF32(a - b) <= Epsilon ? true : false;
		}

		// 32bit 角度ラップ処理します。
		static f32 __fastcall DirLimit(const f32& x);
		inline f32 __fastcall DirLimit(const f32& x)
		{
			return x >= 0.0f ? FmodF32(x + PI, PI_2) - PI : FmodF32(x - PI, PI_2) + PI;
		}

		// 32bit 角度からラジアンへ変換します。
		static f32 __fastcall ToRadian(const f32& x);
		inline f32 __fastcall ToRadian(const f32& x)
		{
			return x * (PI / 180.0f);
		}

		// 32bit ラジアンから角度へ変換します。
		static f32 __fastcall ToDegree(const f32& x);
		inline f32 __fastcall ToDegree(const f32& x)
		{
			return 180.0f * (x / PI);
		}

		/**一般的な方法で絶対値を計算します */
		template< class T >
		static constexpr T __fastcall Abs(const T A)
		{
			return (A >= (T)0) ? A : -A;
		}

		/** tから0の関係に応じて1,0、または-1を返します*/
		template< class T >
		static constexpr T __fastcall Sign(const T A)
		{
			return (A > (T)0) ? (T)1 : ((A < (T)0) ? (T)-1 : (T)0);
		}

		/** Returns higher value in a generic way */
		/**一般的な方法で高い値を返します*/
		template< class T >
		static constexpr T __fastcall  Max(const T A, const T B)
		{
			return (A >= B) ? A : B;
		}

		/** Returns lower value in a generic way */
		/** 一般的な方法で低い値を返します*/
		template< class T >
		static constexpr T __fastcall Min(const T A, const T B)
		{
			return (A <= B) ? A : B;
		}

	} // namespace Math
} // namespace Bread