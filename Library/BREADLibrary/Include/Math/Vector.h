#pragma once

#include "Types.h"
#include <cereal/cereal.hpp>


namespace Bread
{
	namespace Math
	{
#pragma region Vector2
		class Vector2
		{
		public:
			f32 x, y;

		public:
			Vector2() = default;
			Vector2(Vector2&&) = default;
			Vector2(const Vector2&) = default;
			Vector2(const f32* pf) : x(pf[0]), y(pf[1]) {}
			Vector2(f32 fx, f32 fy) : x(fx), y(fy) {}

		public:
			Vector2& operator = (const Vector2&) = default;
			Vector2& operator = (Vector2&&) = default;

			Vector2 operator+ () const { return *this; }
			Vector2 operator- () const;

			Vector2& operator += (const Vector2 v);
			Vector2& operator -= (const Vector2 v);
			Vector2& operator *= (const Vector2 v);
			Vector2& operator /= (const Vector2 v);

			Vector2& operator *= (f32 f);
			Vector2& operator /= (f32 f);

			Vector2 operator + (const Vector2 v) const;
			Vector2 operator - (const Vector2 v) const;
			Vector2 operator * (const Vector2 v) const;
			Vector2 operator / (const Vector2 v) const;

			Vector2 operator * (f32 f) const;
			Vector2 operator / (f32 f) const;

		public:
			friend Vector2 operator * (f32 f, const Vector2 v);
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

			static const Vector3 Zero;		// ゼロベクトル
			static const Vector3 OneX;		// X軸単位ベクトル
			static const Vector3 OneY;		// Y軸単位ベクトル
			static const Vector3 OneZ;		// Z軸単位ベクトル
			static const Vector3 OneAll;	// オール単位ベクトル

		public:
			Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
			Vector3(f32 fx, f32 fy, f32 fz) : x(fx), y(fy), z(fz) {}
			Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}
			Vector3(const f32* pf) : x(pf[0]), y(pf[1]), z(pf[2]) {}

		public:
			// 演算子のキャスティング
			operator f32* ();
			operator const f32* () const;

			// アクセス許可
			f32& operator () (u32 index);
			f32 operator () (u32 index) const;

			Vector3& operator = (const Vector3&) = default;
			Vector3& operator = (Vector3&&) = default;

			Vector3 operator+ () const { return *this; }
			Vector3 operator- () const;

			Vector3& operator += (const Vector3 v);
			Vector3& operator -= (const Vector3 v);
			Vector3& operator *= (const Vector3 v);
			Vector3& operator /= (const Vector3 v);

			Vector3& operator *= (f32 f);
			Vector3& operator /= (f32 f);

			Vector3 operator + (const Vector3 v) const;
			Vector3 operator - (const Vector3 v) const;
			Vector3 operator * (const Vector3 v) const;
			Vector3 operator / (const Vector3 v) const;

			Vector3 operator * (f32 f) const;
			Vector3 operator / (f32 f) const;

			// 比較演算子
			bool operator == (const Vector3& v) const;
			bool operator != (const Vector3& v) const;

		public:
			friend Vector3 operator * (f32 f, const Vector3 v);
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
			Vector4() = default;
			Vector4(Vector4&&) = default;
			Vector4(const Vector4&) = default;
			Vector4(const f32* pf) : x(pf[0]), y(pf[1]), z(pf[2]), w(pf[3]) {}
			Vector4(f32 fx, f32 fy, f32 fz, f32 fw) : x(fx), y(fy), z(fz), w(fw) {}
			Vector4(const Vector3& v3, const f32 w) : x(v3.x), y(v3.y), z(v3.z), w(w) {}

		public:
			// 演算子のキャスティング
			operator f32* ();
			operator const f32* () const;

			//// アクセス許可
			//f32& operator () (u32 index);
			//f32	operator () (u32 index) const;

			Vector4& operator = (const Vector4&) = default;
			Vector4& operator = (Vector4&&) = default;

			Vector4 operator+ () const { return *this; }
			Vector4 operator- () const;

			Vector4& operator += (const Vector4 v);
			Vector4& operator -= (const Vector4 v);
			Vector4& operator *= (const Vector4 v);
			Vector4& operator /= (const Vector4 v);

			Vector4& operator *= (f32 f);
			Vector4& operator /= (f32 f);

			Vector4 operator + (const Vector4 v) const;
			Vector4 operator - (const Vector4 v) const;
			Vector4 operator * (const Vector4 v) const;
			Vector4 operator / (const Vector4 v) const;

			Vector4 operator * (f32 f) const;
			Vector4 operator / (f32 f) const;

		public:
			friend Vector4 operator * (f32 f, const Vector4 v);
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
		void serialize(Archive& archive, Vector2& v)
		{
			archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y));
		}
		template<class Archive>
		void serialize(Archive& archive, Vector3& v)
		{
			archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y), cereal::make_nvp("z", v.z));
		}
		template<class Archive>
		void serialize(Archive& archive, Vector4& v)
		{
			archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y), cereal::make_nvp("z", v.z), cereal::make_nvp("w", v.w));
		}

		template<class Archive>
		void serialize(Archive& archive, Vector4_U8& v)
		{
			archive(cereal::make_nvp("x", v.x), cereal::make_nvp("y", v.y), cereal::make_nvp("z", v.z), cereal::make_nvp("w", v.w));
		}
	} // namespace Math
} // namespace Bread