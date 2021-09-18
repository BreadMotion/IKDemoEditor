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
			Quaternion(const f32& fx, const f32& fy, const f32& fz,const f32& fw);
			~Quaternion() {}

		public:
			Quaternion _fastcall Mul(const Quaternion& q) const;

			Vector3 _fastcall RotateVector(const Vector3& target);
			Vector3 LocalFront();
			Vector3 LocalUp();
			Vector3 LocalRight();

			Matrix RequireRotationMatrix();

		public:
			_fastcall operator f32* ();
			_fastcall operator const f32* () const;

		public:
			Quaternion& _fastcall operator += (const Quaternion& q);
			Quaternion& _fastcall operator -= (const Quaternion& q);
			Quaternion& _fastcall operator *= (const Quaternion& q);
			Quaternion& _fastcall operator *= (const f32& f);
			Quaternion& _fastcall operator /= (const f32& f);

			Quaternion  _fastcall operator + () const;
			Quaternion  _fastcall operator - () const;

			Quaternion _fastcall operator + (const Quaternion& q) const;
			Quaternion _fastcall operator - (const Quaternion& q) const;
			Quaternion _fastcall operator * (const Quaternion& q) const;
			Quaternion _fastcall operator * (const f32& f) const;
			Quaternion _fastcall operator / (const f32& f) const;

			friend Quaternion operator * (f32 f, const Quaternion& q);

			bool _fastcall operator == (const Quaternion& q) const;
			bool _fastcall operator != (const Quaternion& q) const;
		};

		template<class Archive>
		void _fastcall serialize(Archive& archive, Quaternion& q)
		{
			archive(cereal::make_nvp("x", q.x), cereal::make_nvp("y", q.y), cereal::make_nvp("z", q.z), cereal::make_nvp("w", q.w));
		}
	} // namespace Math
} // namespace Bread