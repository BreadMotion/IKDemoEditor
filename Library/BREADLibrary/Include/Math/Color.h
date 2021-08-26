#pragma once

#include "Types.h"
#include <cereal/cereal.hpp>


namespace Bread
{
	namespace Math
	{
		//============================================================================
		// カラー
		//----------------------------------------------------------------------------
		class Color
		{
		public:
			union {
				struct {
					f32			r;					// 赤
					f32			g;					// 緑
					f32			b;					// 青
					f32			a;					// アルファ
				};
				f32				v[4];
			};

			static const Color	White;				// 白
			static const Color	Gray;				// 灰
			static const Color	Black;				// 黒
			static const Color	Red;				    // 赤
			static const Color	Green;				// 緑
			static const Color	Blue;				    // 青
			static const Color	Yellow;				// 黄
			static const Color	Cyan;				// 青緑
			static const Color	Magenta;			// 赤紫
			static const Color	Pink;				    // 桃

		public:
			Color() : Color(White) {}
			Color(f32 cr, f32 cg, f32 cb, f32 ca) { r = cr; g = cg; b = cb; a = ca; }
			Color(const Color& c)                     { r = c.r; g = c.g; b = c.b; a = c.a; }
			Color(const f32* f)                          { r = f[0]; g = f[1]; b = f[2]; a = f[3]; }

		public:
			// アクセス許可
			f32& operator () (u32 index);
			f32  operator   () (u32 index) const;

			// 演算子のキャスティング
			operator f32* ();
			operator const f32* () const;

			// 単項演算子
			Color operator + () const;
			Color operator - () const;

			// 二項演算子
			Color operator + (const Color&) const;
			Color operator - (const Color&) const;
			Color operator * (const Color&) const;
			Color operator * (const f32) const;
			Color operator / (const f32) const;

			// 代入演算子
			Color operator += (const Color&);
			Color operator -= (const Color&);
			Color operator *= (const Color&);
			Color operator *= (const f32);

			// 比較演算子
			bool operator == (const Color&) const;
			bool operator != (const Color&) const;

		};


		//============================================================================
		// カラー算術関数
		//----------------------------------------------------------------------------
		// 2つの色値を加算して、新しい色値を作成する。
		Color* ColorAdd(Color* out, const Color* c1, const Color* c2);

		// 2つの色値を減算して、新しい色値を作成する。
		Color* ColorSubtract(Color* out, const Color* c1, const Color* c2);

		// 色値をスケーリングする。
		Color* ColorScale(Color* out, const Color* c, f32 s);

		// 2つの色をブレンドする。
		Color* ColorModulate(Color* out, const Color* c1, const Color* c2);

		// 色のコントラスト値を調整する。
		Color* ColorAdjustContrast(Color* out, const Color* c, f32 s);

		// 色の彩度値を調整する。
		Color* ColorAdjustSaturation(Color* out, const Color* c, f32 s);

		// 線形補間を使用して色値を作成する。
		Color* ColorLerp(Color* out, const Color* c1, const Color* c2, f32 s);

		// ある色値に対する負の色値を作成する。
		Color* ColorNegative(Color* out, const Color* c);

		// 色を32bitARGB値として取得します。
		u32 ColorARGB32(const Color* c);

		// 色を32bitARGB値から取得します。
		Color* ColorARGB32(Color* out, u32 argb32);

		// 色を32bitABGR値として取得します。
		u32 ColorABGR32(const Color* c);

		// 色を32bitABGR値から取得します。
		Color* ColorABGR32(Color* out, u32 abgr32);


		template<class Archive>
		void serialize(Archive& archive, Color& c)
		{
			archive(cereal::make_nvp("r", c.r), cereal::make_nvp("g", c.g), cereal::make_nvp("b", c.b), cereal::make_nvp("a", c.a));
		}

	}	// namespace math
}	// namespace ys