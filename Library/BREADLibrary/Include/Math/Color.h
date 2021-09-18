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
			union
			{
				struct
				{
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
			static const Color	Red;				// 赤
			static const Color	Green;				// 緑
			static const Color	Blue;				// 青
			static const Color	Yellow;				// 黄
			static const Color	Cyan;				// 青緑
			static const Color	Magenta;			// 赤紫
			static const Color	Pink;				// 桃

		public:
			Color() : Color(White) {}
			Color(const f32& cr, const f32& cg, const f32& cb, const f32& ca) { r = cr; g = cg; b = cb; a = ca; }
			Color(const Color& c)                 { r = c.r; g = c.g; b = c.b; a = c.a; }
			Color(const f32* f)                   { r = f[0]; g = f[1]; b = f[2]; a = f[3]; }

		public:
			// アクセス許可
			f32& _fastcall operator () (const u32& index);
			f32 _fastcall  operator   () (const u32& index) const;

			// 演算子のキャスティング
			_fastcall operator f32* ();
			_fastcall operator const f32* () const;

			// 単項演算子
			Color _fastcall operator + () const;
			Color _fastcall operator - () const;

			// 二項演算子
			Color _fastcall operator + (const Color&) const;
			Color _fastcall operator - (const Color&) const;
			Color _fastcall operator * (const Color&) const;
			Color _fastcall operator * (const f32) const;
			Color _fastcall operator / (const f32) const;

			// 代入演算子
			Color _fastcall operator += (const Color&);
			Color _fastcall operator -= (const Color&);
			Color _fastcall operator *= (const Color&);
			Color _fastcall operator *= (const f32);

			// 比較演算子
			bool _fastcall operator == (const Color&) const;
			bool _fastcall operator != (const Color&) const;

		};


		//============================================================================
		// カラー算術関数
		//----------------------------------------------------------------------------
		// 2つの色値を加算して、新しい色値を作成する。
		Color* _fastcall ColorAdd(Color* out, const Color* c1, const Color* c2);

		// 2つの色値を減算して、新しい色値を作成する。
		Color* _fastcall ColorSubtract(Color* out, const Color* c1, const Color* c2);

		// 色値をスケーリングする。
		Color* _fastcall ColorScale(Color* out, const Color* c, f32 s);

		// 2つの色をブレンドする。
		Color* _fastcall ColorModulate(Color* out, const Color* c1, const Color* c2);

		// 色のコントラスト値を調整する。
		Color* _fastcall ColorAdjustContrast(Color* out, const Color* c, f32 s);

		// 色の彩度値を調整する。
		Color* _fastcall ColorAdjustSaturation(Color* out, const Color* c, f32 s);

		// 線形補間を使用して色値を作成する。
		Color* _fastcall ColorLerp(Color* out, const Color* c1, const Color* c2, f32 s);

		// ある色値に対する負の色値を作成する。
		Color* _fastcall ColorNegative(Color* out, const Color* c);

		// 色を32bitARGB値として取得します。
		u32 _fastcall ColorARGB32(const Color* c);

		// 色を32bitARGB値から取得します。
		Color* _fastcall ColorARGB32(Color* out, u32 argb32);

		// 色を32bitABGR値として取得します。
		u32 _fastcall ColorABGR32(const Color* c);

		// 色を32bitABGR値から取得します。
		Color* _fastcall ColorABGR32(Color* out, u32 abgr32);


		template<class Archive>
		void _fastcall serialize(Archive& archive, Color& c)
		{
			archive(cereal::make_nvp("r", c.r), cereal::make_nvp("g", c.g), cereal::make_nvp("b", c.b), cereal::make_nvp("a", c.a));
		}

	}	// namespace math
}	// namespace ys