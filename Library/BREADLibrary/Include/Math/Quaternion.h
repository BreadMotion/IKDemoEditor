#pragma once

#include "Types.h"
#include "Vector.h"
#include "Matrix.h"
#include <cereal/cereal.hpp>


namespace Bread
{
	namespace Math
	{
		class Quaternion
		{
		public:
			f32 x, y, z, w;
			static const Quaternion	Zero;		// ゼロクォータニオン

		public:
			Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
			Quaternion(const f32* pf);
			Quaternion(f32 fx, f32 fy, f32 fz, f32 fw);
			~Quaternion() {}

		public:
			Quaternion Mul(const Quaternion& q) const;

			Vector3 RotateVector(const Vector3& target);
			Vector3 LocalFront();
			Vector3 LocalUp();
			Vector3 LocalRight();

			Matrix RequireRotationMatrix();

		public:
			operator f32* ();
			operator const f32* () const;

		public:
			Quaternion& operator += (const Quaternion& q);
			Quaternion& operator -= (const Quaternion& q);
			Quaternion& operator *= (const Quaternion& q);
			Quaternion& operator *= (f32 f);
			Quaternion& operator /= (f32 f);

			Quaternion  operator + () const;
			Quaternion  operator - () const;

			Quaternion operator + (const Quaternion& q) const;
			Quaternion operator - (const Quaternion& q) const;
			Quaternion operator * (const Quaternion& q) const;
			Quaternion operator * (f32 f) const;
			Quaternion operator / (f32 f) const;

			friend Quaternion operator * (f32 f, const Quaternion& q);

			bool operator == (const Quaternion& q) const;
			bool operator != (const Quaternion& q) const;
		};

		template<class Archive>
		void serialize(Archive& archive, Quaternion& q)
		{
			archive(cereal::make_nvp("x", q.x), cereal::make_nvp("y", q.y), cereal::make_nvp("z", q.z), cereal::make_nvp("w", q.w));
		}
	} // namespace Math
} // namespace Bread