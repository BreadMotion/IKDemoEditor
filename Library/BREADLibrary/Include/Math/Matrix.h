#pragma once

#include "Types.h"
#include "Math/Vector.h"
#include <cereal/cereal.hpp>


namespace Bread
{
	namespace Math
	{
		class Matrix
		{
		public:
			union
			{
				struct
				{
					f32 _11, _12, _13, _14; // right
					f32 _21, _22, _23, _24; // up
					f32 _31, _32, _33, _34; // forward
					f32 _41, _42, _43, _44;
				};
				f32 m[4][4];
				f32 f[16];
			};
			static const Matrix	Zero;	// ゼロマトリクス
			static const Matrix	One;	// 単位マトリクス

		public:
			Matrix() = default;
			Matrix(const f32* pf) :
				_11(pf[0]),  _12(pf[1]),  _13(pf[2]),  _14(pf[3]),
				_21(pf[4]),  _22(pf[5]),  _23(pf[6]),  _24(pf[7]),
				_31(pf[8]),  _32(pf[9]),  _33(pf[10]), _34(pf[11]),
				_41(pf[12]), _42(pf[13]), _43(pf[14]), _44(pf[15]) {}
			Matrix
			(
				f32 _f11, f32 _f12, f32 _f13, f32 _f14,
				f32 _f21, f32 _f22, f32 _f23, f32 _f24,
				f32 _f31, f32 _f32, f32 _f33, f32 _f34,
				f32 _f41, f32 _f42, f32 _f43, f32 _f44
			) :
				_11(_f11), _12(_f12), _13(_f13), _14(_f14),
				_21(_f21), _22(_f22), _23(_f23), _24(_f24),
				_31(_f31), _32(_f32), _33(_f33), _34(_f34),
				_41(_f41), _42(_f42), _43(_f43), _44(_f44) {}
			~Matrix() {}

		public:
			f32& operator () (u32 row, u32 col);
			f32  operator () (u32 row, u32 col) const;

		public:
			Matrix& operator += (const Matrix& mat);
			Matrix& operator -= (const Matrix& mat);
			Matrix& operator *= (f32 f);
			Matrix& operator *= (const Matrix& mat);
			Matrix& operator /= (f32 f);

			Matrix& operator = (const Matrix& mat);

			Matrix operator + () const;
			Matrix operator - () const;

			Matrix operator + (const Matrix& mat) const;
			Matrix operator - (const Matrix& mat) const;
			Matrix operator * (f32 f) const;
			Matrix operator * (const Matrix& mat) const;
			Matrix operator / (f32 f) const;

			friend Matrix operator * (f32 f, const Matrix& mat);

			bool operator == (const Matrix& mat) const;
			bool operator != (const Matrix& mat) const;
		};

		template<class Archive>
		void serialize(Archive& archive, Matrix& m)
		{
			archive(
				cereal::make_nvp("_11", m._11), cereal::make_nvp("_12", m._12), cereal::make_nvp("_13", m._13), cereal::make_nvp("_14", m._14),
				cereal::make_nvp("_21", m._21), cereal::make_nvp("_22", m._22), cereal::make_nvp("_23", m._23), cereal::make_nvp("_24", m._24),
				cereal::make_nvp("_31", m._31), cereal::make_nvp("_32", m._32), cereal::make_nvp("_33", m._33), cereal::make_nvp("_34", m._34),
				cereal::make_nvp("_41", m._41), cereal::make_nvp("_42", m._42), cereal::make_nvp("_43", m._43), cereal::make_nvp("_44", m._44)
			);
		}
	} // namespace Math
} // namespace Bread