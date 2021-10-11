#pragma once

#include "Types.h"
#include <cereal/cereal.hpp>
#include <immintrin.h>

namespace Bread
{
	namespace Math
	{
#pragma region Vector2S32
		class Vector2S32
		{
		public:
			union
			{
				struct
				{
					s32 x, y;
				};
				s32 v[2];
			};

		public:
			Vector2S32() = default;
			Vector2S32(Vector2S32&&) = default;
			Vector2S32(const Vector2S32&) = default;
			Vector2S32(const s32 * __restrict pf) : x(pf[0]), y(pf[1]) {}
			Vector2S32(const s32& __restrict fx, const s32& __restrict fy) : x(fx), y(fy) {}

		public:
			Vector2S32& _fastcall operator = (const Vector2S32&) = default;
			Vector2S32& _fastcall operator = (Vector2S32&&) = default;

			Vector2S32 _fastcall operator+ () const { return *this; }
			Vector2S32 _fastcall operator- () const;

			Vector2S32& _fastcall operator += (const Vector2S32& __restrict v);
			Vector2S32& _fastcall operator -= (const Vector2S32& __restrict v);
			Vector2S32& _fastcall operator *= (const Vector2S32& __restrict v);
			Vector2S32& _fastcall operator /= (const Vector2S32& __restrict v);

			Vector2S32& _fastcall operator *= (const s32 & __restrict f);
			Vector2S32& _fastcall operator /= (const s32 & __restrict f);

			Vector2S32 _fastcall operator + (const Vector2S32& __restrict v) const;
			Vector2S32 _fastcall operator - (const Vector2S32& __restrict v) const;
			Vector2S32 _fastcall operator * (const Vector2S32& __restrict v) const;
			Vector2S32 _fastcall operator / (const Vector2S32& __restrict v) const;

			Vector2S32 _fastcall operator * (const s32 & __restrict f) const;
			Vector2S32 _fastcall operator / (const s32 & __restrict f) const;
		};
#pragma endregion

#pragma region Vector3S32
		class Vector3S32
		{
		public:
			union
			{
				struct
				{
					s32 x, y, z;
				};
				s32 v[3];
			};

		public:
			Vector3S32() = default;
			Vector3S32(Vector3S32&&) = default;
			Vector3S32(const Vector3S32&) = default;
			Vector3S32(const s32* __restrict pf) : x(pf[0]), y(pf[1]), z(pf[2]) {}
			Vector3S32(const s32& __restrict fx, const s32& __restrict fy, const s32& __restrict fz) : x(fx), y(fy), z(fz) {}

		public:
			// 演算子のキャスティング
			operator s32* ();
			operator const s32* () const;

			// アクセス許可
			s32& _fastcall operator () (const u32& __restrict index);
			s32  _fastcall operator () (const u32& __restrict index) const;

			Vector3S32& _fastcall operator = (const Vector3S32&) = default;
			Vector3S32& _fastcall operator = (Vector3S32&&) = default;

			Vector3S32 _fastcall operator+ () const { return *this; }
			Vector3S32 _fastcall operator- () const;

			Vector3S32& _fastcall operator += (const Vector3S32& __restrict v);
			Vector3S32& _fastcall operator -= (const Vector3S32& __restrict v);
			Vector3S32& _fastcall operator *= (const Vector3S32& __restrict v);
			Vector3S32& _fastcall operator /= (const Vector3S32& __restrict v);

			Vector3S32& _fastcall operator *= (const s32 & __restrict f);
			Vector3S32& _fastcall operator /= (const s32 & __restrict f);

			Vector3S32 _fastcall operator + (const Vector3S32& __restrict v) const;
			Vector3S32 _fastcall operator - (const Vector3S32& __restrict v) const;
			Vector3S32 _fastcall operator * (const Vector3S32& __restrict v) const;
			Vector3S32 _fastcall operator / (const Vector3S32& __restrict v) const;

			Vector3S32 _fastcall operator * (const s32 & __restrict f) const;
			Vector3S32 _fastcall operator / (const s32 & __restrict f) const;

			// 比較演算子
			bool _fastcall operator == (const Vector3S32& __restrict v) const;
			bool _fastcall operator != (const Vector3S32& __restrict v) const;
		};
#pragma endregion

#pragma region Vector4S32
		class Vector4S32
		{
		public:
			union
			{
				struct
				{
					s32 x, y, z, w;
				};
				s32 v[4];
			};

		public:
			Vector4S32() = default;
			Vector4S32(Vector4S32&&) = default;
			Vector4S32(const Vector4S32&) = default;
			Vector4S32(const s32 * __restrict pf) : x(pf[0]), y(pf[1]), z(pf[2]), w(pf[3]) {}
			Vector4S32(const s32& __restrict fx, const s32& __restrict fy, const s32& __restrict fz, const s32& __restrict fw) : x(fx), y(fy), z(fz), w(fw) {}

		public:
			// 演算子のキャスティング
			operator s32* ();
			operator const s32* () const;

			Vector4S32& _fastcall operator = (const Vector4S32&) = default;
			Vector4S32& _fastcall operator = (Vector4S32&&) = default;

			Vector4S32 _fastcall operator+ () const { return *this; }
			Vector4S32 _fastcall operator- () const;

			Vector4S32& _fastcall operator += (const Vector4S32& v);
			Vector4S32& _fastcall operator -= (const Vector4S32& v);
			Vector4S32& _fastcall operator *= (const Vector4S32& v);
			Vector4S32& _fastcall operator /= (const Vector4S32& v);

			Vector4S32& _fastcall operator *= (const s32 & __restrict f);
			Vector4S32& _fastcall operator /= (const s32 & __restrict f);

			Vector4S32 _fastcall operator + (const Vector4S32& __restrict v) const;
			Vector4S32 _fastcall operator - (const Vector4S32& __restrict v) const;
			Vector4S32 _fastcall operator * (const Vector4S32& __restrict v) const;
			Vector4S32 _fastcall operator / (const Vector4S32& __restrict v) const;

			Vector4S32 _fastcall operator * (const s32 & __restrict f) const;
			Vector4S32 _fastcall operator / (const s32 & __restrict f) const;
		};
#pragma endregion

#pragma region Vector2
		class Vector2
		{
		public:
			union
			{
				struct
				{
					f32 x, y;
				};
				f32 v[2];
			};

		public:
			Vector2()               = default;
			Vector2(Vector2&&)      = default;
			Vector2(const Vector2&) = default;
			Vector2(const f32* __restrict pf) : x(pf[0]), y(pf[1]) {}
			Vector2(const f32& __restrict fx, const f32& __restrict fy) : x(fx), y(fy) {}

		public:
			Vector2& _fastcall operator = (const Vector2&) = default;
			Vector2& _fastcall operator = (Vector2&&)      = default;

			Vector2 _fastcall operator+ () const { return *this; }
			Vector2 _fastcall operator- () const;

			Vector2& _fastcall operator += (const Vector2& __restrict v);
			Vector2& _fastcall operator -= (const Vector2& __restrict v);
			Vector2& _fastcall operator *= (const Vector2& __restrict v);
			Vector2& _fastcall operator /= (const Vector2& __restrict v);

			Vector2& _fastcall operator *= (const f32& __restrict f);
			Vector2& _fastcall operator /= (const f32& __restrict f);

			Vector2 _fastcall operator + (const Vector2& __restrict v) const;
			Vector2 _fastcall operator - (const Vector2& __restrict v) const;
			Vector2 _fastcall operator * (const Vector2& __restrict v) const;
			Vector2 _fastcall operator / (const Vector2& __restrict v) const;

			Vector2 _fastcall operator * (const f32& __restrict f) const;
			Vector2 _fastcall operator / (const f32& __restrict f) const;

		public:
			friend Vector2 _fastcall  operator * (const f32& __restrict f, const Vector2& __restrict v);
		};
#pragma endregion

#pragma region Vector3
		class Vector3
		{
		public:
			union
			{
				struct
				{
					f32 x, y, z;
				};
				f32 v[3];
			};

			static const Vector3 Zero;      // ゼロベクトル
			static const Vector3 OneX;		// X軸単位ベクトル
			static const Vector3 OneY;		// Y軸単位ベクトル
			static const Vector3 OneZ;		// Z軸単位ベクトル
			static const Vector3 OneAll;	// オール単位ベクトル

		public:
			Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
			Vector3(const f32& __restrict fx, const f32& __restrict fy, const f32& __restrict fz) : x(fx), y(fy), z(fz) {}
			Vector3(const Vector3& __restrict v) : x(v.x), y(v.y), z(v.z) {}
			Vector3(const f32* __restrict pf) : x(pf[0]), y(pf[1]), z(pf[2]) {}

		public:
			// 演算子のキャスティング
			operator f32* ();
			operator const f32* () const;

			// アクセス許可
			f32& _fastcall operator () (const u32& __restrict index);
			f32  _fastcall operator () (const u32& __restrict index) const;

			Vector3& _fastcall operator = (const Vector3&) = default;
			Vector3& _fastcall operator = (Vector3&&)      = default;

			Vector3 _fastcall operator+ () const { return *this; }
			Vector3 _fastcall operator- () const;

			Vector3& _fastcall operator += (const Vector3& __restrict v);
			Vector3& _fastcall operator -= (const Vector3& __restrict v);
			Vector3& _fastcall operator *= (const Vector3& __restrict v);
			Vector3& _fastcall operator /= (const Vector3& __restrict v);

			Vector3& _fastcall operator *= (const f32& __restrict f);
			Vector3& _fastcall operator /= (const f32& __restrict f);

			Vector3 _fastcall operator + (const Vector3& __restrict v) const;
			Vector3 _fastcall operator - (const Vector3& __restrict v) const;
			Vector3 _fastcall operator * (const Vector3& __restrict v) const;
			Vector3 _fastcall operator / (const Vector3& __restrict v) const;

			Vector3 _fastcall operator * (const f32& __restrict f) const;
			Vector3 _fastcall operator / (const f32& __restrict f) const;

			// 比較演算子
			bool _fastcall operator == (const Vector3& __restrict v) const;
			bool _fastcall operator != (const Vector3& __restrict v) const;

		public:
			friend Vector3 _fastcall operator * (const f32& __restrict f, const Vector3& __restrict v);
		};
#pragma endregion

#pragma region Vector4
		class Vector4
		{
		public:
			union
			{
				struct
				{
					f32 x, y, z, w;
				};
				f32 v[4];
			};

		public:
			Vector4()               = default;
			Vector4(Vector4&&)      = default;
			Vector4(const Vector4&) = default;
			Vector4(const f32* __restrict pf) : x(pf[0]), y(pf[1]), z(pf[2]), w(pf[3]) {}
			Vector4(const f32& __restrict fx, const f32& __restrict fy, const f32& __restrict fz, const f32& __restrict fw) : x(fx), y(fy), z(fz), w(fw) {}
			Vector4(const Vector3& __restrict v3, const f32& __restrict w) : x(v3.x), y(v3.y), z(v3.z), w(w) {}

		public:
			// 演算子のキャスティング
			operator f32* ();
			operator const f32* () const;

			//// アクセス許可
			//f32& operator () (u32 index);
			//f32	operator () (u32 index) const;

			Vector4& _fastcall operator = (const Vector4&) = default;
			Vector4& _fastcall operator = (Vector4&&)      = default;

			Vector4 _fastcall operator+ () const { return *this; }
			Vector4 _fastcall operator- () const;

			Vector4& _fastcall operator += (const Vector4& vv);
			Vector4& _fastcall operator -= (const Vector4& vv);
			Vector4& _fastcall operator *= (const Vector4& vv);
			Vector4& _fastcall operator /= (const Vector4& vv);

			Vector4& _fastcall operator *= (const f32& __restrict f);
			Vector4& _fastcall operator /= (const f32& __restrict f);

			Vector4 _fastcall operator + (const Vector4& __restrict v) const;
			Vector4 _fastcall operator - (const Vector4& __restrict v) const;
			Vector4 _fastcall operator * (const Vector4& __restrict v) const;
			Vector4 _fastcall operator / (const Vector4& __restrict v) const;

			Vector4 _fastcall operator * (const f32& __restrict f) const;
			Vector4 _fastcall operator / (const f32& __restrict f) const;

		public:
			friend Vector4 _fastcall operator * (const f32& __restrict f, const Vector4& __restrict v);
		};
#pragma endregion

#pragma region Vector
		__declspec(align(16)) class Vector
		{
		public:
			union
			{
				float f[4];
				vector v;
			};

			static const Vector simInfinity;
			static const Vector simQNaN;

		public:
			Vector() = default;
			Vector(const vector* __restrict v) : v(*v) {}
			Vector(const vector& __restrict v) : v(v)  {}
			Vector(const Vector* __restrict v) : v(*v) {}
			Vector(const Vector& __restrict v) : v(v)  {}
			//Vector(const Vector2* __restrict v) { *this = LoadVector<Vector2>(*v); }
			//Vector(const Vector2& __restrict v) { *this = LoadVector<Vector2>(v); }
			//Vector(const Vector3* __restrict v) { *this = LoadVector<Vector3>(*v); }
			//Vector(const Vector3& __restrict v) { *this = LoadVector<Vector3>(v); }
			//Vector(const Vector4* __restrict v) { *this = LoadVector<Vector4>(*v); }
			//Vector(const Vector4& __restrict v) { *this = LoadVector<Vector4>(v); }

			template <class T>
			Vector(const T* __restrict v) { *this = LoadVector<T>(*v); }
			template <class T>
			Vector(const T& __restrict v) { *this = LoadVector<T>(v); }

			Vector(const float& f1, const float& f2, const float& f3, const float& f4)
			{
				*this = LoadVector<Vector4>(Vector4{ f1, f2, f3, f4 });
			}
			Vector(const float& f)
			{
				*this = LoadVector<Vector4>(Vector4{ f, f, f, f });
			}
			Vector(const float* f)
			{
				*this = LoadVector<Vector4>(Vector4{ f[0], f[1], f[2], f[3] });
			}

			inline operator vector() const { return v; }

		public:
			template <class T>
			vector __vectorcall LoadVector(T v)
			{
				return _mm_load_ps(v.v);
			}

			//template <class T>
			Vector4 StoreVector()
			{
				Vector4 val;
				_mm_store_ps(&val.x, v);

				return val;
			}

		public:
			// 演算子のキャスティング
			operator f32* ();
			operator const f32* () const;

			Vector& __vectorcall operator = (const Vector&) = default;
			Vector& __vectorcall operator = (Vector&&)      = default;

			Vector __vectorcall operator+ () const { return *this; }
			Vector __vectorcall operator- () const { return *this; }

			Vector& __vectorcall operator += (const Vector v);
			Vector& __vectorcall operator -= (const Vector v);
			Vector& __vectorcall operator *= (const Vector v);
			Vector& __vectorcall operator /= (const Vector v);

			Vector& _fastcall operator *= (const f32& __restrict f);
			Vector& _fastcall operator /= (const f32& __restrict f);

			Vector __vectorcall operator + (const Vector v) const;
			Vector __vectorcall operator - (const Vector v) const;
			Vector __vectorcall operator * (const Vector v) const;
			Vector __vectorcall operator / (const Vector v) const;

			Vector _fastcall operator * (const f32& __restrict f) const;
			Vector _fastcall operator / (const f32& __restrict f) const;
		};
#pragma endregion

		struct Vector2_S8 { union { struct { s8 x, y; }; s8 v[2]; }; };
		struct Vector2_U8 { union { struct { u8 x, y; }; u8 v[2]; }; };
		struct Vector2_S16 { union { struct { s16 x, y; }; s16 v[2]; }; };
		struct Vector2_U16 { union { struct { u16 x, y; }; u16 v[2]; }; };
		struct Vector2_S32 { union { struct { s32 x, y; }; s32 v[2]; }; };
		struct Vector2_U32 { union { struct { u32 x, y; }; u32 v[2]; }; };

		struct Vector3_S8 { union { struct { s8 x, y, z; }; s8 v[3]; }; };
		struct Vector3_U8 { union { struct { u8 x, y, z; }; u8 v[3]; }; };
		struct Vector3_S16 { union { struct { s16 x, y, z; }; s16 v[3]; }; };
		struct Vector3_U16 { union { struct { u16 x, y, z; }; u16 v[3]; }; };
		struct Vector3_S32 { union { struct { s32 x, y, z; }; s32 v[3]; }; };
		struct Vector3_U32 { union { struct { u32 x, y, z; }; u32 v[3]; }; };

		struct Vector4_S8 { union { struct { s8 x, y, z, w; }; s8 v[4]; }; };
		struct Vector4_U8 { union { struct { u8 x, y, z, w; }; u8 v[4]; }; };
		struct Vector4_S16 { union { struct { s16 x, y, z, w; }; s16 v[4]; }; };
		struct Vector4_U16 { union { struct { u16 x, y, z, w; }; u16 v[4]; }; };
		struct Vector4_S32 { union { struct { s32 x, y, z, w; }; s32 v[4]; }; };
		struct Vector4_U32 { union { struct { u32 x, y, z, w; }; u32 v[4]; }; };

		template<class Archive>
		void _fastcall serialize(Archive& archive, Vector2& v)
		{
			archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y));
		}
		template<class Archive>
		void _fastcall serialize(Archive& archive, Vector3& v)
		{
			archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y), cereal::make_nvp("z", v.z));
		}
		template<class Archive>
		void _fastcall serialize(Archive& archive, Vector4& v)
		{
			archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y), cereal::make_nvp("z", v.z), cereal::make_nvp("w", v.w));
		}

		template<class Archive>
		void _fastcall serialize(Archive& archive, Vector4_U8& v)
		{
			archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y), cereal::make_nvp("z", v.z), cereal::make_nvp("w", v.w));
		}
	} // namespace Math
} // namespace Bread