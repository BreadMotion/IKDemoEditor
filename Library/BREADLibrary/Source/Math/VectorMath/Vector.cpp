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

#pragma region Vector2
		Vector2 Vector2::operator- () const
		{
			Vector2 v;

			v.x = -x;
			v.y = -y;

			return v;
		}

		Vector2& Vector2::operator += (const Vector2 v)
		{
			x = x + v.x;
			y = y + v.y;

			return *this;
		}

		Vector2& Vector2::operator -= (const Vector2 v)
		{
			x = x - v.x;
			y = y - v.y;

			return *this;
		}

		Vector2& Vector2::operator *= (const Vector2 v)
		{
			x = x * v.x;
			y = y * v.y;

			return *this;
		}

		Vector2& Vector2::operator /= (const Vector2 v)
		{
			x = x / v.x;
			y = y / v.y;

			return *this;
		}

		Vector2& Vector2::operator *= (f32 f)
		{
			x = x * f;
			y = y * f;

			return *this;
		}

		Vector2& Vector2::operator /= (f32 f)
		{
			x = x / f;
			y = y / f;

			return *this;
		}

		Vector2 Vector2::operator + (const Vector2 v) const
		{
			Vector2 vT;

			vT.x = x + v.x;
			vT.y = y + v.y;

			return vT;
		}

		Vector2 Vector2::operator - (const Vector2 v) const
		{
			Vector2 vT;

			vT.x = x - v.x;
			vT.y = y - v.y;

			return vT;
		}

		Vector2 Vector2::operator * (const Vector2 v) const
		{
			Vector2 vT;

			vT.x = x * v.x;
			vT.y = y * v.y;

			return vT;
		}

		Vector2 Vector2::operator / (const Vector2 v) const
		{
			Vector2 vT;

			vT.x = x / v.x;
			vT.y = y / v.y;

			return vT;
		}

		Vector2 Vector2::operator * (f32 f) const
		{
			Vector2 vT;

			vT.x = x * f;
			vT.y = y * f;

			return vT;
		}

		Vector2 Vector2::operator / (f32 f) const
		{
			Vector2 vT;

			vT.x = x / f;
			vT.y = y / f;

			return vT;
		}

		Vector2 operator * (f32 f, const Vector2 v)
		{
			Vector2 vT;

			vT.x = f * v.x;
			vT.y = f * v.y;

			return vT;
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
		f32& Vector3::operator () (u32 index)
		{
			return v[index];
		}

		f32 Vector3::operator () (u32 index) const
		{
			return v[index];
		}

		Vector3 Vector3::operator- () const
		{
			Vector3 v;

			v.x = -x;
			v.y = -y;
			v.z = -z;

			return v;
		}

		Vector3& Vector3::operator += (const Vector3 v)
		{
			x = x + v.x;
			y = y + v.y;
			z = z + v.z;

			return *this;
		}

		Vector3& Vector3::operator -= (const Vector3 v)
		{
			x = x - v.x;
			y = y - v.y;
			z = z - v.z;

			return *this;
		}

		Vector3& Vector3::operator *= (const Vector3 v)
		{
			x = x * v.x;
			y = y * v.y;
			z = z * v.z;

			return *this;
		}

		Vector3& Vector3::operator /= (const Vector3 v)
		{
			x = x / v.x;
			y = y / v.y;
			z = z / v.z;

			return *this;
		}

		Vector3& Vector3::operator *= (f32 f)
		{
			x = x * f;
			y = y * f;
			z = z * f;

			return *this;
		}

		Vector3& Vector3::operator /= (f32 f)
		{
			x = x / f;
			y = y / f;
			z = z / f;

			return *this;
		}

		Vector3 Vector3::operator + (const Vector3 v) const
		{
			Vector3 vT;

			vT.x = x + v.x;
			vT.y = y + v.y;
			vT.z = z + v.z;

			return vT;
		}

		Vector3 Vector3::operator - (const Vector3 v) const
		{
			Vector3 vT;

			vT.x = x - v.x;
			vT.y = y - v.y;
			vT.z = z - v.z;

			return vT;
		}

		Vector3 Vector3::operator * (const Vector3 v) const
		{
			Vector3 vT;

			vT.x = x * v.x;
			vT.y = y * v.y;
			vT.z = z * v.z;

			return vT;
		}

		Vector3 Vector3::operator / (const Vector3 v) const
		{
			Vector3 vT;

			vT.x = x / v.x;
			vT.y = y / v.y;
			vT.z = z / v.z;

			return vT;
		}

		Vector3 Vector3::operator * (f32 f) const
		{
			Vector3 vT;

			vT.x = x * f;
			vT.y = y * f;
			vT.z = z * f;

			return vT;
		}

		Vector3 Vector3::operator / (f32 f) const
		{
			Vector3 vT;

			vT.x = x / f;
			vT.y = y / f;
			vT.z = z / f;

			return vT;
		}

		// 比較演算子
		bool Vector3::operator == (const Vector3& v) const
		{
			if ((::fabsf(x - v.x) <= Epsilon)
			 && (::fabsf(y - v.y) <= Epsilon)
			 && (::fabsf(z - v.z) <= Epsilon))
			{
				return true;
			}

			return false;
		}

		bool Vector3::operator != (const Vector3& v) const
		{
			return !(*this == v);
		}

		Vector3 operator * (f32 f, const Vector3 v)
		{
			Vector3 vT;

			vT.x = f * v.x;
			vT.y = f * v.y;
			vT.z = f * v.z;

			return vT;
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
			Vector4 v;

			v.x = -x;
			v.y = -y;
			v.z = -z;
			v.w = -w;

			return v;
		}

		Vector4& Vector4::operator += (const Vector4 v)
		{
			x = x + v.x;
			y = y + v.y;
			z = z + v.z;
			w = w + v.w;

			return *this;
		}

		Vector4& Vector4::operator -= (const Vector4 v)
		{
			x = x - v.x;
			y = y - v.y;
			z = z - v.z;
			w = w - v.w;

			return *this;
		}

		Vector4& Vector4::operator *= (const Vector4 v)
		{
			x = x * v.x;
			y = y * v.y;
			z = z * v.z;
			w = w * v.w;

			return *this;
		}

		Vector4& Vector4::operator /= (const Vector4 v)
		{
			x = x / v.x;
			y = y / v.y;
			z = z / v.z;
			w = w / v.w;

			return *this;
		}

		Vector4& Vector4::operator *= (f32 f)
		{
			x = x * f;
			y = y * f;
			z = z * f;
			w = w * f;

			return *this;
		}

		Vector4& Vector4::operator /= (f32 f)
		{
			x = x / f;
			y = y / f;
			z = z / f;
			w = w / f;

			return *this;
		}

		Vector4 Vector4::operator + (const Vector4 v) const
		{
			Vector4 vT;

			vT.x = x + v.x;
			vT.y = y + v.y;
			vT.z = z + v.z;
			vT.w = w + v.w;

			return vT;
		}

		Vector4 Vector4::operator - (const Vector4 v) const
		{
			Vector4 vT;

			vT.x = x - v.x;
			vT.y = y - v.y;
			vT.z = z - v.z;
			vT.w = w - v.w;

			return vT;
		}

		Vector4 Vector4::operator * (const Vector4 v) const
		{
			Vector4 vT;

			vT.x = x * v.x;
			vT.y = y * v.y;
			vT.z = z * v.z;
			vT.w = w * v.w;

			return vT;
		}

		Vector4 Vector4::operator / (const Vector4 v) const
		{
			Vector4 vT;

			vT.x = x / v.x;
			vT.y = y / v.y;
			vT.z = z / v.z;
			vT.w = w / v.w;

			return vT;
		}

		Vector4 Vector4::operator * (f32 f) const
		{
			Vector4 vT;

			vT.x = x * f;
			vT.y = y * f;
			vT.z = z * f;
			vT.w = w * f;

			return vT;
		}

		Vector4 Vector4::operator / (f32 f) const
		{
			Vector4 vT;

			vT.x = x / f;
			vT.y = y / f;
			vT.z = z / f;
			vT.w = w / f;

			return vT;
		}

		Vector4 operator * (f32 f, const Vector4 v)
		{
			Vector4 vT;

			vT.x = f * v.x;
			vT.y = f * v.y;
			vT.z = f * v.z;
			vT.w = f * v.w;

			return vT;
		}
#pragma endregion
	} // namespace Math
} // namespace Bread