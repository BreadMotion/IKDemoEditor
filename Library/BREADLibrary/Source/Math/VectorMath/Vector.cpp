#include "pch.h"
#include "Math/Vector.h"
#include "Math/Arithmetic.h"


namespace Bread
{
	namespace Math
	{
		const Vector3		Vector3::Zero = { 0.0f, 0.0f, 0.0f };
		const Vector3		Vector3::OneX = { 1.0f, 0.0f, 0.0f };
		const Vector3		Vector3::OneY = { 0.0f, 1.0f, 0.0f };
		const Vector3		Vector3::OneZ = { 0.0f, 0.0f, 1.0f };
		const Vector3		Vector3::OneAll = { 1.0f, 1.0f, 1.0f };

		const Vector Vector::simInfinity = { 0x7F800000, 0x7F800000, 0x7F800000, 0x7F800000 };
		const Vector Vector::simQNaN     = { 0x7FC00000, 0x7FC00000, 0x7FC00000, 0x7FC00000 };

#pragma region Vector2S32
		Vector2S32 Vector2S32::operator- () const
		{
			return Vector2S32{ -x,-y };
		}

		Vector2S32& Vector2S32::operator += (const Vector2S32& __restrict v)
		{
			x = x + v.x;
			y = y + v.y;

			return *this;
		}

		Vector2S32& Vector2S32::operator -= (const Vector2S32& __restrict v)
		{
			x = x - v.x;
			y = y - v.y;

			return *this;
		}

		Vector2S32& Vector2S32::operator *= (const Vector2S32& __restrict v)
		{
			x = x * v.x;
			y = y * v.y;

			return *this;
		}

		Vector2S32& Vector2S32::operator /= (const Vector2S32& __restrict v)
		{
			x = x / v.x;
			y = y / v.y;

			return *this;
		}

		Vector2S32& Vector2S32::operator *= (const s32& __restrict f)
		{
			x = x * f;
			y = y * f;

			return *this;
		}

		Vector2S32& Vector2S32::operator /= (const s32& __restrict f)
		{
			x = x / f;
			y = y / f;

			return *this;
		}

		Vector2S32  Vector2S32::operator + (const Vector2S32& __restrict v) const
		{
			return Vector2S32{ x + v.x ,y + v.y };
		}

		Vector2S32  Vector2S32::operator - (const Vector2S32& __restrict v) const
		{
			return Vector2S32{ x - v.x , y - v.y };
		}

		Vector2S32  Vector2S32::operator * (const Vector2S32& __restrict v) const
		{
			return Vector2S32{ x * v.x,y * v.y };
		}

		Vector2S32  Vector2S32::operator / (const Vector2S32& __restrict v) const
		{
			return Vector2S32{ x / v.x ,y / v.y };
		}

		Vector2S32  Vector2S32::operator * (const s32& __restrict f) const
		{
			return Vector2S32{ x * f , y * f };
		}

		Vector2S32  Vector2S32::operator / (const s32& __restrict f) const
		{
			return Vector2S32{ x / f,y / f };
		}
#pragma endregion

#pragma region Vector3S32
		// 演算子のキャスティング
		Vector3S32::operator s32* ()
		{
			return (s32*)&v[0];
		}

		Vector3S32::operator const s32* () const
		{
			return (const s32*)&v[0];
		}

		// アクセス許可
		s32& Vector3S32::operator () (const u32& __restrict index)
		{
			return v[index];
		}

		s32  Vector3S32::operator () (const u32& __restrict index) const
		{
			return v[index];
		}

		Vector3S32  Vector3S32::operator- () const
		{
			return Vector3S32{ -x,-y,-z };
		}

		Vector3S32& Vector3S32::operator += (const Vector3S32& __restrict v)
		{
			x = x + v.x;
			y = y + v.y;
			z = z + v.z;

			return *this;
		}

		Vector3S32& Vector3S32::operator -= (const Vector3S32& __restrict v)
		{
			x = x - v.x;
			y = y - v.y;
			z = z - v.z;

			return *this;
		}

		Vector3S32& Vector3S32::operator *= (const Vector3S32& __restrict v)
		{
			x = x * v.x;
			y = y * v.y;
			z = z * v.z;

			return *this;
		}

		Vector3S32& Vector3S32::operator /= (const Vector3S32& __restrict v)
		{
			x = x / v.x;
			y = y / v.y;
			z = z / v.z;

			return *this;
		}

		Vector3S32& Vector3S32::operator *= (const s32& __restrict f)
		{
			x = x * f;
			y = y * f;
			z = z * f;

			return *this;
		}

		Vector3S32& Vector3S32::operator /= (const s32& __restrict f)
		{
			x = x / f;
			y = y / f;
			z = z / f;

			return *this;
		}

		Vector3S32  Vector3S32::operator + (const Vector3S32& __restrict v) const
		{
			return Vector3S32{ x + v.x,y + v.y,z + v.z };
		}

		Vector3S32  Vector3S32::operator - (const Vector3S32& __restrict v) const
		{
			return Vector3S32{ x - v.x, y - v.y,z - v.z };
		}

		Vector3S32  Vector3S32::operator * (const Vector3S32& __restrict v) const
		{
			return Vector3S32{ x * v.x,y * v.y, z * v.z };
		}

		Vector3S32  Vector3S32::operator / (const Vector3S32& __restrict v) const
		{
			return Vector3S32{ x / v.x ,y / v.y ,z / v.z };
		}

		Vector3S32  Vector3S32::operator * (const s32& __restrict f) const
		{
			return Vector3S32{ x * f,y * f,z * f };
		}

		Vector3S32  Vector3S32::operator / (const s32& __restrict f) const
		{
			return Vector3S32{ x / f,y / f,z / f };
		}

		// 比較演算子
		bool  Vector3S32::operator == (const Vector3S32& __restrict v) const
		{
			if ((::fabsf(x - v.x) <= Epsilon)
				&& (::fabsf(y - v.y) <= Epsilon)
				&& (::fabsf(z - v.z) <= Epsilon))
			{
				return true;
			}

			return false;
		}

		bool  Vector3S32::operator != (const Vector3S32& __restrict v) const
		{
			return !(*this == v);
		}

		Vector3S32 _fastcall operator * (const s32& __restrict f, const Vector3S32& __restrict v)
		{
			return Vector3S32{ f * v.x,f * v.y,f * v.z };
		}
#pragma endregion

#pragma region Vector4S32
		// 演算子のキャスティング
		Vector4S32::operator s32* ()
		{
			return (s32*)&v[0];
		}


		Vector4S32::operator const s32* () const
		{
			return (const s32*)&v[0];
		}

		//		// アクセス許可
		//		f32& Vector4::operator () (u32 index)
		//		{
		//			return v[index];
		//		}
		//
		//		f32 Vector4::operator () (u32 index) const
		//		{
		//			return v[index];
		//		}

		Vector4S32 Vector4S32::operator- () const
		{
			return Vector4S32{ -x,-y,-z,-w };
		}

		Vector4S32& Vector4S32::operator += (const Vector4S32& v)
		{
			x = x + v.x;
			y = y + v.y;
			z = z + v.z;
			w = w + v.w;

			return *this;
		}

		Vector4S32& Vector4S32::operator -= (const Vector4S32& v)
		{
			x = x - v.x;
			y = y - v.y;
			z = z - v.z;
			w = w - v.w;

			return *this;
		}

		Vector4S32& Vector4S32::operator *= (const Vector4S32& v)
		{
			x = x * v.x;
			y = y * v.y;
			z = z * v.z;
			w = w * v.w;

			return *this;
		}

		Vector4S32& Vector4S32::operator /= (const Vector4S32& v)
		{
			x = x / v.x;
			y = y / v.y;
			z = z / v.z;
			w = w / v.w;

			return *this;
		}

		Vector4S32& Vector4S32::operator *= (const s32& __restrict f)
		{
			x = x * f;
			y = y * f;
			z = z * f;
			w = w * f;

			return *this;
		}

		Vector4S32& Vector4S32::operator /= (const s32& __restrict f)
		{
			x = x / f;
			y = y / f;
			z = z / f;
			w = w / f;

			return *this;
		}

		Vector4S32  Vector4S32::operator + (const Vector4S32& __restrict v) const
		{
			Vector4S32 vT;

			vT.x = x + v.x;
			vT.y = y + v.y;
			vT.z = z + v.z;
			vT.w = w + v.w;

			return Vector4S32{ x + v.x ,y + v.y,z + v.z ,w + v.w };
		}

		Vector4S32  Vector4S32::operator - (const Vector4S32& __restrict v) const
		{
			return Vector4S32{ x - v.x ,y - v.y,z - v.z ,w - v.w };
		}

		Vector4S32  Vector4S32::operator * (const Vector4S32& __restrict v) const
		{
			return Vector4S32{ x * v.x ,y * v.y,z * v.z ,w * v.w };
		}

		Vector4S32  Vector4S32::operator / (const Vector4S32& __restrict v) const
		{
			return Vector4S32{ x / v.x ,y / v.y,z / v.z ,w / v.w };
		}

		Vector4S32  Vector4S32::operator * (const s32& __restrict f) const
		{
			return Vector4S32{ x * f ,y * f,z * f ,w * f };
		}

		Vector4S32  Vector4S32::operator / (const s32& __restrict f) const
		{
			Vector4S32 vT;

			vT.x = x / f;
			vT.y = y / f;
			vT.z = z / f;
			vT.w = w / f;

			return Vector4S32{ x / f ,y / f,z / f ,w / f };
		}

		Vector4S32 _fastcall operator * (const s32& __restrict f, const Vector4S32& __restrict v)
		{
			return Vector4S32{ f * v.x,f * v.y,f * v.z,f * v.w };
		}
#pragma endregion

#pragma region Vector2
		Vector2 Vector2::operator- () const
		{
			return Vector2{ -x,-y };
		}

		Vector2& Vector2::operator += (const Vector2& __restrict v)
		{
			x = x + v.x;
			y = y + v.y;

			return *this;
		}

		Vector2& Vector2::operator -= (const Vector2& __restrict v)
		{
			x = x - v.x;
			y = y - v.y;

			return *this;
		}

		Vector2& Vector2::operator *= (const Vector2& __restrict v)
		{
			x = x * v.x;
			y = y * v.y;

			return *this;
		}

		Vector2& Vector2::operator /= (const Vector2& __restrict v)
		{
			x = x / v.x;
			y = y / v.y;

			return *this;
		}

		Vector2& Vector2::operator *= (const f32& __restrict f)
		{
			x = x * f;
			y = y * f;

			return *this;
		}

		Vector2& Vector2::operator /= (const f32& __restrict f)
		{
			x = x / f;
			y = y / f;

			return *this;
		}

		Vector2  Vector2::operator + (const Vector2& __restrict v) const
		{
			return Vector2{ x + v.x ,y + v.y };
		}

		Vector2  Vector2::operator - (const Vector2& __restrict v) const
		{
			return Vector2{ x - v.x , y - v.y };
		}

		Vector2  Vector2::operator * (const Vector2& __restrict v) const
		{
			return Vector2{ x * v.x,y * v.y };
		}

		Vector2  Vector2::operator / (const Vector2& __restrict v) const
		{
			return Vector2{ x / v.x ,y / v.y };
		}

		Vector2  Vector2::operator * (const f32& __restrict f) const
		{
			return Vector2{ x * f , y * f };
		}

		Vector2  Vector2::operator / (const f32& __restrict f) const
		{
			return Vector2{ x / f,y / f };
		}

		Vector2 _fastcall operator * (const f32& __restrict f, const Vector2& __restrict v)
		{
			return Vector2{ f * v.x,f * v.y };
		}
#pragma endregion

#pragma region Vector3
		// 演算子のキャスティング
		Vector3::operator f32* ()
		{
			return (f32*)&v[0];
		}

		Vector3::operator const f32* () const
		{
			return (const f32*)&v[0];
		}

		// アクセス許可
		f32&  Vector3::operator () (const u32& __restrict index)
		{
			return v[index];
		}

		f32  Vector3::operator () (const u32& __restrict index) const
		{
			return v[index];
		}

		Vector3  Vector3::operator- () const
		{
			return Vector3{ -x,-y,-z };
		}

		Vector3&  Vector3::operator += (const Vector3& __restrict v)
		{
			x = x + v.x;
			y = y + v.y;
			z = z + v.z;

			return *this;
		}

		Vector3&  Vector3::operator -= (const Vector3& __restrict v)
		{
			x = x - v.x;
			y = y - v.y;
			z = z - v.z;

			return *this;
		}

		Vector3&  Vector3::operator *= (const Vector3& __restrict v)
		{
			x = x * v.x;
			y = y * v.y;
			z = z * v.z;

			return *this;
		}

		Vector3&  Vector3::operator /= (const Vector3& __restrict v)
		{
			x = x / v.x;
			y = y / v.y;
			z = z / v.z;

			return *this;
		}

		Vector3&  Vector3::operator *= (const f32& __restrict f)
		{
			x = x * f;
			y = y * f;
			z = z * f;

			return *this;
		}

		Vector3&  Vector3::operator /= (const f32& __restrict f)
		{
			x = x / f;
			y = y / f;
			z = z / f;

			return *this;
		}

		Vector3  Vector3::operator + (const Vector3& __restrict v) const
		{
			return Vector3{ x + v.x,y + v.y,z + v.z };
		}

		Vector3  Vector3::operator - (const Vector3& __restrict v) const
		{
			return Vector3{ x - v.x, y - v.y,z - v.z };
		}

		Vector3  Vector3::operator * (const Vector3& __restrict v) const
		{
			return Vector3{ x * v.x,y * v.y, z * v.z };
		}

		Vector3  Vector3::operator / (const Vector3& __restrict v) const
		{
			return Vector3{ x / v.x ,y / v.y ,z / v.z };
		}

		Vector3  Vector3::operator * (const f32& __restrict f) const
		{
			return Vector3{ x * f,y * f,z * f };
		}

		Vector3  Vector3::operator / (const f32& __restrict f) const
		{
			return Vector3{ x / f,y / f,z / f };
		}

		// 比較演算子
		bool  Vector3::operator == (const Vector3& __restrict v) const
		{
			if ((::fabsf(x - v.x) <= Epsilon)
			 && (::fabsf(y - v.y) <= Epsilon)
			 && (::fabsf(z - v.z) <= Epsilon))
			{
				return true;
			}

			return false;
		}

		bool  Vector3::operator != (const Vector3& __restrict v) const
		{
			return !(*this == v);
		}

		Vector3 _fastcall operator * (const f32& __restrict f, const Vector3& __restrict v)
		{
			return Vector3{ f * v.x,f * v.y,f * v.z };
		}
#pragma endregion

#pragma region Vector4
		// 演算子のキャスティング
		Vector4::operator f32* ()
		{
			return (f32*)&v[0];
		}


		Vector4::operator const f32* () const
		{
			return (const f32*)&v[0];
		}

//		// アクセス許可
//		f32& Vector4::operator () (u32 index)
//		{
//			return v[index];
//		}
//
//		f32 Vector4::operator () (u32 index) const
//		{
//			return v[index];
//		}

		Vector4 Vector4::operator- () const
		{
			return Vector4{ -x,-y,-z,-w };
		}

		Vector4&  Vector4::operator += (const Vector4& __restrict v)
		{
			x = x + v.x;
			y = y + v.y;
			z = z + v.z;
			w = w + v.w;

			return *this;
		}

		Vector4&  Vector4::operator -= (const Vector4& __restrict v)
		{
			x = x - v.x;
			y = y - v.y;
			z = z - v.z;
			w = w - v.w;

			return *this;
		}

		Vector4&  Vector4::operator *= (const Vector4& __restrict v)
		{
			x = x * v.x;
			y = y * v.y;
			z = z * v.z;
			w = w * v.w;

			return *this;
		}

		Vector4&  Vector4::operator /= (const Vector4& __restrict v)
		{
			x = x / v.x;
			y = y / v.y;
			z = z / v.z;
			w = w / v.w;

			return *this;
		}

		Vector4&  Vector4::operator *= (const f32& __restrict f)
		{
			x = x * f;
			y = y * f;
			z = z * f;
			w = w * f;

			return *this;
		}

		Vector4&  Vector4::operator /= (const f32& __restrict f)
		{
			x = x / f;
			y = y / f;
			z = z / f;
			w = w / f;

			return *this;
		}

		Vector4  Vector4::operator + (const Vector4& __restrict v) const
		{
			Vector4 vT;

			vT.x = x + v.x;
			vT.y = y + v.y;
			vT.z = z + v.z;
			vT.w = w + v.w;

			return Vector4{ x + v.x ,y + v.y,z + v.z ,w + v.w };
		}

		Vector4  Vector4::operator - (const Vector4& __restrict v) const
		{
			return Vector4{ x - v.x ,y - v.y,z - v.z ,w - v.w };
		}

		Vector4  Vector4::operator * (const Vector4& __restrict v) const
		{
			return Vector4{ x * v.x ,y * v.y,z * v.z ,w * v.w };
		}

		Vector4  Vector4::operator / (const Vector4& __restrict v) const
		{
			return Vector4{ x / v.x ,y / v.y,z / v.z ,w / v.w };
		}

		Vector4  Vector4::operator * (const f32& __restrict f) const
		{
			return Vector4{ x * f ,y * f,z * f ,w * f };
		}

		Vector4  Vector4::operator / (const f32& __restrict f) const
		{
			Vector4 vT;

			vT.x = x / f;
			vT.y = y / f;
			vT.z = z / f;
			vT.w = w / f;

			return Vector4{ x / f ,y / f,z / f ,w / f };
		}

		Vector4 _fastcall operator * (const f32& __restrict f, const Vector4& __restrict v)
		{
			return Vector4{ f * v.x,f * v.y,f * v.z,f * v.w };
		}
#pragma endregion

#pragma region Vector
		// 演算子のキャスティング
		Vector::operator f32* ()
		{
			return f;
		}


		Vector::operator const f32* () const
		{
			return f;
		}

		Vector& __vectorcall Vector::operator += (const Vector v)
		{
			*this = _mm_add_ps(*this, v);

			return *this;
		}

		Vector& __vectorcall Vector::operator -= (const Vector v)
		{
			*this = _mm_sub_ps(*this, v);

			return *this;
		}

		Vector& __vectorcall Vector::operator *= (const Vector v)
		{
			*this = _mm_mul_ps(*this, v);

			return *this;
		}

		Vector& __vectorcall Vector::operator /= (const Vector v)
		{
			*this = _mm_div_ps(*this, v);

			return *this;
		}

		Vector&  Vector::operator *= (const f32& __restrict f)
		{
			*this = _mm_mul_ps(*this, Vector{ f });

			return *this;
		}

		Vector&  Vector::operator /= (const f32& __restrict f)
		{
			*this = _mm_div_ps(*this, Vector{ f });

			return *this;
		}

		Vector __vectorcall Vector::operator + (const Vector v) const
		{
			return _mm_add_ps(*this, v);
		}

		Vector __vectorcall Vector::operator - (const Vector v) const
		{
			return _mm_sub_ps(*this, v);
		}

		Vector __vectorcall Vector::operator * (const Vector v) const
		{
			return _mm_mul_ps(*this, v);
		}

		Vector __vectorcall Vector::operator / (const Vector v) const
		{
			return _mm_div_ps(*this, v);
		}

		Vector  Vector::operator * (const f32& __restrict f) const
		{
			return _mm_mul_ps(*this, Vector{ f });
		}

		Vector  Vector::operator / (const f32& __restrict f) const
		{
			return _mm_div_ps(*this, Vector{ f });
		}
#pragma endregion
	} // namespace Math
} // namespace Bread