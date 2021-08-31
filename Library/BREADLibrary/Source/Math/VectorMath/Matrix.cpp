#include <ivec.h>
#include <fvec.h>
#include <dvec.h>
#include <immintrin.h>

#include "pch.h"
#include "Math/BreadMath.h"

namespace Bread
{
	namespace Math
	{
		// スタティック
		const Matrix Matrix::Zero =
		Matrix
		(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);

		const Matrix Matrix::One =
		Matrix
		(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);

		f32& Matrix::operator () (u32 row, u32 col)
		{
			return m[row][col];
		}

		f32  Matrix::operator () (u32 row, u32 col) const
		{
			return m[row][col];
		}

		Matrix& Matrix::operator += (const Matrix& mat)
		{
			_11 += mat._11; _12 += mat._12; _13 += mat._13; _14 += mat._14;
			_21 += mat._21; _22 += mat._22; _23 += mat._23; _24 += mat._24;
			_31 += mat._31; _32 += mat._32; _33 += mat._33; _34 += mat._34;
			_41 += mat._41; _42 += mat._42; _43 += mat._43; _44 += mat._44;

			return *this;
		}

		Matrix& Matrix::operator -= (const Matrix& mat)
		{
			_11 -= mat._11; _12 -= mat._12; _13 -= mat._13; _14 -= mat._14;
			_21 -= mat._21; _22 -= mat._22; _23 -= mat._23; _24 -= mat._24;
			_31 -= mat._31; _32 -= mat._32; _33 -= mat._33; _34 -= mat._34;
			_41 -= mat._41; _42 -= mat._42; _43 -= mat._43; _44 -= mat._44;

			return *this;
		}

		Matrix& Matrix::operator *= (f32 f)
		{
			_11 *= f; _12 *= f; _13 *= f; _14 *= f;
			_21 *= f; _22 *= f; _23 *= f; _24 *= f;
			_31 *= f; _32 *= f; _33 *= f; _34 *= f;
			_41 *= f; _42 *= f; _43 *= f; _44 *= f;

			return *this;
		}

		Matrix& Matrix::operator *= (const Matrix& mat)
		{
			*this = MatrixMultiply(*this, mat);

			return *this;
		}

		Matrix& Matrix::operator /= (f32 f)
		{
			_11 /= f; _12 /= f; _13 /= f; _14 /= f;
			_21 /= f; _22 /= f; _23 /= f; _24 /= f;
			_31 /= f; _32 /= f; _33 /= f; _34 /= f;
			_41 /= f; _42 /= f; _43 /= f; _44 /= f;

			return *this;
		}

		Matrix& Matrix::operator = (const Matrix& mat)
		{
			_11 = mat._11; _12 = mat._12; _13 = mat._13; _14 = mat._14;
			_21 = mat._21; _22 = mat._22; _23 = mat._23; _24 = mat._24;
			_31 = mat._31; _32 = mat._32; _33 = mat._33; _34 = mat._34;
			_41 = mat._41; _42 = mat._42; _43 = mat._43; _44 = mat._44;

			return *this;
		}

		Matrix Matrix::operator + () const
		{
			return *this;
		}

		Matrix Matrix::operator - () const
		{
			return Matrix
			(
				-_11, -_12, -_13, -_14,
				-_21, -_22, -_23, -_24,
				-_31, -_32, -_33, -_34,
				-_41, -_42, -_43, -_44
			);
		}

		Matrix Matrix::operator + (const Matrix& mat) const
		{
			return Matrix
			(
				_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
				_21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
				_31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
				_41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44
			);
		}

		Matrix Matrix::operator - (const Matrix& mat) const
		{
			return Matrix
			(
				_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
				_21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
				_31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
				_41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44
			);
		}

		Matrix Matrix::operator * (f32 f) const
		{
			return Matrix
			(
				_11 * f, _12 * f, _13 * f, _14 * f,
				_21 * f, _22 * f, _23 * f, _24 * f,
				_31 * f, _32 * f, _33 * f, _34 * f,
				_41 * f, _42 * f, _43 * f, _44 * f
			);
		}

		Matrix Matrix::operator * (const Matrix& mat) const
		{
			Matrix matT;

			matT = MatrixMultiply(*this, mat);

			return matT;
		}

		Matrix Matrix::operator / (f32 f) const
		{
			return Matrix
			(
				_11 / f, _12 / f, _13 / f, _14 / f,
				_21 / f, _22 / f, _23 / f, _24 / f,
				_31 / f, _32 / f, _33 / f, _34 / f,
				_41 / f, _42 / f, _43 / f, _44 / f
			);
		}

		Matrix operator * (f32 f, const Matrix& mat)
		{
			return Matrix
			(
				f * mat._11, f * mat._12, f * mat._13, f * mat._14,
				f * mat._21, f * mat._22, f * mat._23, f * mat._24,
				f * mat._31, f * mat._32, f * mat._33, f * mat._34,
				f * mat._41, f * mat._42, f * mat._43, f * mat._44
			);
		}

		bool Matrix::operator == (const Matrix& mat) const
		{
			return 0 == memcmp(this, &mat, sizeof(Matrix));
		}

		bool Matrix::operator != (const Matrix& mat) const
		{
			return 0 != memcmp(this, &mat, sizeof(Matrix));
		}
	} // namespace Math
} // namespace Bread