#include "pch.h"
#include "Math/Color.h"
#include "Math/Arithmetic.h"


namespace Bread
{
	namespace Math
	{
		// スタティック
		const Color Color::White   = { 1.0f, 1.0f, 1.0f, 1.0f };
		const Color Color::Gray    = { 0.5f, 0.5f, 0.5f, 1.0f };
		const Color Color::Black   = { 0.0f, 0.0f, 0.0f, 1.0f };
		const Color Color::Red     = { 1.0f, 0.0f, 0.0f, 1.0f };
		const Color Color::Green   = { 0.0f, 1.0f, 0.0f, 1.0f };
		const Color Color::Blue    = { 0.0f, 0.0f, 1.0f, 1.0f };
		const Color Color::Yellow  = { 1.0f, 1.0f, 0.0f, 1.0f };
		const Color Color::Cyan    = { 0.0f, 1.0f, 1.0f, 1.0f };
		const Color Color::Magenta = { 1.0f, 1.0f, 0.0f, 1.0f };
		const Color Color::Pink    = { 1.0f, 0.5f, 0.5f, 1.0f };

		//============================================================================
		// Color
		//----------------------------------------------------------------------------
		// アクセス許可
		f32&
			_fastcall Color::operator () (const u32& index)
		{
			return v[index];
		}

		f32
			_fastcall Color::operator () (const u32& index) const
		{
			return v[index];
		}

		// 演算子のキャスティング
		_fastcall Color::operator f32* ()
		{
			return (f32*)&v[0];
		}

		_fastcall Color::operator const f32* () const
		{
			return (const f32*)&v[0];
		}

		// 単項演算子
		Color
			_fastcall Color::operator + () const
		{
			return (*this);
		}

		Color
			_fastcall Color::operator - () const
		{
			Color ret;
			ret.r = -r;
			ret.g = -g;
			ret.b = -b;
			ret.a = -a;
			return ret;
		}

		// 二項演算子
		Color
			_fastcall Color::operator + (const Color& color) const
		{
			Color ret;
			return *ColorAdd(&ret, this, &color);
		}

		Color
			_fastcall Color::operator - (const Color& color) const
		{
			Color ret;
			return *ColorSubtract(&ret, this, &color);
		}

		Color
			_fastcall Color::operator * (const Color& color) const
		{
			Color ret;
			return *ColorModulate(&ret, this, &color);
		}

		Color
			_fastcall Color::operator * (const f32 f) const
		{
			Color ret;
			return *ColorScale(&ret, this, f);
		}

		Color
			_fastcall Color::operator / (const f32 f) const
		{
			Color ret;
			return *ColorScale(&ret, this, 1.0f / f);
		}

		// 代入演算子
		Color
			_fastcall Color::operator += (const Color& color)
		{
			return *ColorAdd(this, this, &color);
		}

		Color
			_fastcall Color::operator -= (const Color& color)
		{
			return *ColorSubtract(this, this, &color);
		}

		Color
			_fastcall Color::operator *= (const Color& color)
		{
			return *ColorModulate(this, this, &color);
		}

		Color
			Color::operator *= (const f32 f)
		{
			return *ColorScale(this, this, f);
		}

		// 比較演算子
		bool
			_fastcall Color::operator == (const Color& color) const
		{
			if ((FabsF32(r - color.r) <= Epsilon)
				&& (FabsF32(g - color.g) <= Epsilon)
				&& (FabsF32(b - color.b) <= Epsilon)
				&& (FabsF32(a - color.a) <= Epsilon)
				) {
				return true;
			}

			return false;
		}

		bool
			_fastcall Color::operator != (const Color& color) const
		{
			return !(*this == color);
		}

		//============================================================================
		//	カラー算術関数
		//----------------------------------------------------------------------------
		// 2 つの色値を加算して、新しい色値を作成する。
		Color*								// [R] 2 つの色値の合計である Color 構造体へのポインタを返す。
			_fastcall ColorAdd(
				Color* out,			        // [O] 演算結果である Color 構造体へのポインタ。
				const Color* c1,			// [I] 処理の基になる Color 構造体へのポインタ。
				const Color* c2				// [I] 処理の基になる Color 構造体へのポインタ。
			)
		{
			out->r = (c1->r) + (c2->r);
			out->g = (c1->g) + (c2->g);
			out->b = (c1->b) + (c2->b);
			out->a = (c1->a) + (c2->a);

			return out;
		}

		// 2 つの色値を減算して、新しい色値を作成する。
		Color*								// [R] 2 つの色値の差分である Color 構造体へのポインタを返す。
			_fastcall ColorSubtract(
				Color* out,			        // [O] 演算結果である Color 構造体へのポインタ。
				const Color* c1,			// [I] 処理の基になる Color 構造体へのポインタ。
				const Color* c2				// [I] 処理の基になる Color 構造体へのポインタ。
			)
		{
			out->r = (c1->r) - (c2->r);
			out->g = (c1->g) - (c2->g);
			out->b = (c1->b) - (c2->b);
			out->a = (c1->a) - (c2->a);

			return out;
		}

		// 色値をスケーリングする。
		Color*								// [R] スケーリングされた色値である Color 構造体へのポインタを返す。
			_fastcall ColorScale(
				Color* out,			        // [O] 演算結果である Color 構造体へのポインタ。
				const Color* c,				// [I] 処理の基になる Color 構造体へのポインタ。
				f32			 s				// [I] スケーリング係数。色を 4D ベクトルのように処理して、それをスケーリングする。s の値に制限はない。s が 1 の場合、結果の色は元の色である。
			)
		{
			out->r = s * (c->r);
			out->g = s * (c->g);
			out->b = s * (c->b);
			out->a = s * (c->a);

			return out;
		}

		// 2 つの色をブレンドする。
		Color*								// [R] ブレンド結果である Color 構造体へのポインタを返す。
			_fastcall ColorModulate(
				Color* out,			        // [O] 演算結果である Color 構造体へのポインタ。
				const Color* c1,			// [I] 処理の基になる Color 構造体へのポインタ。
				const Color* c2				// [I] 処理の基になる Color 構造体へのポインタ。
			)
		{
			out->r = (c1->r) * (c2->r);
			out->g = (c1->g) * (c2->g);
			out->b = (c1->b) * (c2->b);
			out->a = (c1->a) * (c2->a);

			return out;
		}

		// 色のコントラスト値を調整する。
		Color*								// [R] コントラスト調整結果の Color 構造体へのポインタを返す。
			_fastcall ColorAdjustContrast(
				Color* out,			        // [O] 演算結果である Color 構造体へのポインタ。
				const Color* c,				// [I] 処理の基になる Color 構造体へのポインタ。
				f32			 s				// [I] コントラスト値。このパラメータは、50% グレーと指定色 c の間で線形補間する。c の値に制限はない。このパラメータが 0 の場合、返される色は 50% グレーである。このパラメータが 1 の場合、返される色は元の色である。
			)
		{
			out->r = 0.5f + s * (c->r - 0.5f);
			out->g = 0.5f + s * (c->g - 0.5f);
			out->b = 0.5f + s * (c->b - 0.5f);
			out->a = c->a;

			return out;
		}

		// 色の彩度値を調整する。
		Color*								// [R] 飽和調整結果の Color 構造体へのポインタを返す。
			_fastcall ColorAdjustSaturation(
				Color*       out,			// [O] 演算結果である Color 構造体へのポインタ。
				const Color* c,				// [I] 処理の基になる Color 構造体へのポインタ。
				f32			 s				// [I] 飽和値。このパラメータは、グレー スケールに変換された色と元の色 c の間で線形補間する。s の値に制限はない。s が 0 の場合、返される色はグレースケール カラーである。s が 1 の場合、返される色は元の色である。
			)
		{
			f32 grey = c->r * 0.2125f + c->g * 0.7154f + c->b * 0.0721f;

			out->r = grey + s * (c->r - grey);
			out->g = grey + s * (c->g - grey);
			out->b = grey + s * (c->b - grey);
			out->a = c->a;

			return out;
		}

		// 線形補間を使用して色値を作成する。
		Color*							    // [R] 線形補間結果の Color 構造体へのポインタを返す。
			_fastcall ColorLerp(
				Color*       out,			// [O] 演算結果である Color 構造体へのポインタ。
				const Color* c1,			// [I] 処理の基になる Color 構造体へのポインタ。
				const Color* c2,			// [I] 処理の基になる Color 構造体へのポインタ。
				f32			 s				// [I] 色 c1 と c2 を 4D ベクトルとして処理し、これらの間を線形補間するパラメータ。s の値に制限はない。
			)
		{
			out->r = (1.0f - s) * (c1->r) + s * (c2->r);
			out->g = (1.0f - s) * (c1->g) + s * (c2->g);
			out->b = (1.0f - s) * (c1->b) + s * (c2->b);
			out->a = (1.0f - s) * (c1->a) + s * (c2->a);

			return out;
		}

		// ある色値に対する負の色値を作成する。
		Color*								// [R] 色値の負の色である Color 構造体へのポインタを返す。
			_fastcall ColorNegative(
				Color* out,			        // [O] 演算結果である Color 構造体へのポインタ。
				const Color* c				// [I] 処理の基になる Color 構造体へのポインタ。
			)
		{
			out->r = 1.0f - c->r;
			out->g = 1.0f - c->g;
			out->b = 1.0f - c->b;
			out->a = c->a;

			return out;
		}

		// 色を32bitARGB値として取得
		u32									// [R] 32bitARGB値
			_fastcall ColorARGB32(
				const Color* c				// [I] 処理の基になる Color 構造体へのポインタ。
			)
		{
			// 32bitカラーに変換
			u8 cr = c->r >= 1.0f ? 0xff : (u8)(c->r * 255.0f + 0.5f);
			u8 cg = c->g >= 1.0f ? 0xff : (u8)(c->g * 255.0f + 0.5f);
			u8 cb = c->b >= 1.0f ? 0xff : (u8)(c->b * 255.0f + 0.5f);
			u8 ca = c->a >= 1.0f ? 0xff : (u8)(c->a * 255.0f + 0.5f);
			return ((ca << 24) | (cr << 16) | (cg << 8) | cb);
		}

		// 色を32bitARGB値として取得
		Color*							// [R] 変換後の色である Color 構造体へのポインタを返す。
			_fastcall ColorARGB32(
				Color* out,			    // [O] 演算結果である Color 構造体へのポインタ。
				u32	   col32			// [I] 処理の基になる 32bitARGB値
			)
		{
			const f32 f = 1.0f / 255.0f;
			out->r = f * (f32)(u8)(col32 >> 16);
			out->g = f * (f32)(u8)(col32 >> 8);
			out->b = f * (f32)(u8)(col32 >> 0);
			out->a = f * (f32)(u8)(col32 >> 24);

			return out;
		}

		// 色を32bitABGR値として取得
		u32										// [R] 32bitABGR値
			_fastcall ColorABGR32(
				const Color* c				    // [I] 処理の基になる Color 構造体へのポインタ。
			)
		{
			// 32bitカラーに変換
			u8 cr = c->r >= 1.0f ? 0xff : (u8)(c->r * 255.0f + 0.5f);
			u8 cg = c->g >= 1.0f ? 0xff : (u8)(c->g * 255.0f + 0.5f);
			u8 cb = c->b >= 1.0f ? 0xff : (u8)(c->b * 255.0f + 0.5f);
			u8 ca = c->a >= 1.0f ? 0xff : (u8)(c->a * 255.0f + 0.5f);
			return ((ca << 24) | (cb << 16) | (cg << 8) | cr);
		}

		// 色を32bitABGR値として取得
		Color*							// [R] 変換後の色である Color 構造体へのポインタを返す。
			_fastcall ColorABGR32(
				Color* out,			    // [O] 演算結果である Color 構造体へのポインタ。
				u32		col32			// [I] 処理の基になる 32bitABGR値
			)
		{
			const f32 f = 1.0f / 255.0f;
			out->r = f * (f32)(u8)(col32 >> 0);
			out->g = f * (f32)(u8)(col32 >> 8);
			out->b = f * (f32)(u8)(col32 >> 16);
			out->a = f * (f32)(u8)(col32 >> 24);

			return out;
		}


	}	// namespace math
}	// namespace ys