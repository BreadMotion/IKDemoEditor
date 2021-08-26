#include "pch.h"
#include "Math/AABB.h"
#include "Math/Arithmetic.h"
#include "FND/Util.h"


namespace Bread
{
	namespace Math
	{
		const AABB AABB::Zero = { Math::Vector3(0.0f, 0.0f, 0.0f), Math::Vector3(0.0f, 0.0f, 0.0f) };

		//****************************************************************************
		// バウンディングボックス
		//****************************************************************************
		// リセット
		void AABB::Reset()
		{
			min.x = MAX_F32;
			min.y = MAX_F32;
			min.z = MAX_F32;
			max.x = -MAX_F32;
			max.y = -MAX_F32;
			max.z = -MAX_F32;
		}

		// 変換
		void AABB::Transform(const Matrix& transform)
		{
			Vector3 newMin = { transform.m[3][0], transform.m[3][1], transform.m[3][2] };
			Vector3 newMax = newMin;

			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					float e = min[j] * transform.m[j][i];
					float f = max[j] * transform.m[j][i];
					if (e < f)
					{
						newMin[i] += e;
						newMax[i] += f;

					}
					else
					{
						newMin[i] += f;
						newMax[i] += e;
					}

				}
			}

			min = newMin;
			max = newMax;
		}

		// 拡張
		void AABB::Extend(const AABB& aabb)
		{
			min.x = FND::Min(min.x, aabb.min.x);
			min.y = FND::Min(min.y, aabb.min.y);
			min.z = FND::Min(min.z, aabb.min.z);
			max.x = FND::Max(max.x, aabb.max.x);
			max.y = FND::Max(max.y, aabb.max.y);
			max.z = FND::Max(max.z, aabb.max.z);
		}

		// 拡張
		void AABB::Extend(const Vector3& point)
		{
			min.x = FND::Min(min.x, point.x);
			min.y = FND::Min(min.y, point.y);
			min.z = FND::Min(min.z, point.z);
			max.x = FND::Max(max.x, point.x);
			max.y = FND::Max(max.y, point.y);
			max.z = FND::Max(max.z, point.z);
		}

		// 含まれているか
		bool AABB::Contain(const Vector3& point)
		{
			if (point.x > max.x) return false;
			if (point.x < min.x) return false;

			if (point.y > max.y) return false;
			if (point.y < min.y) return false;

			if (point.z > max.z) return false;
			if (point.z < min.z) return false;

			return true;
		}

		// コーナー取得
		void AABB::GetCorners(Vector3 corners[8])
		{
			corners[0] = { min.x, min.y, min.z };
			corners[1] = { min.x, max.y, min.z };
			corners[2] = { max.x, min.y, min.z };
			corners[3] = { max.x, max.y, min.z };
			corners[4] = { min.x, min.y, max.z };
			corners[5] = { min.x, max.y, max.z };
			corners[6] = { max.x, min.y, max.z };
			corners[7] = { max.x, max.y, max.z };
		}

		// 球との交差判定
		bool AABB::Intersect(const Math::Vector3& center, f32 radius) const
		{
			// 各軸のはみ出した距離の総和が半径以上なら交差していない
			f32 totalDitance = 0.f;
			f32 distance;

			distance = min.x - center.x;
			if (distance > 0.f) { totalDitance += distance * distance; }
			distance = center.x - max.x;
			if (distance > 0.f) { totalDitance += distance * distance; }

			distance = min.y - center.y;
			if (distance > 0.f) { totalDitance += distance * distance; }
			distance = center.y - max.y;
			if (distance > 0.f) { totalDitance += distance * distance; }

			distance = min.z - center.z;
			if (distance > 0.f) { totalDitance += distance * distance; }
			distance = center.z - max.z;
			if (distance > 0.f) { totalDitance += distance * distance; }

			return (totalDitance <= (radius * radius));
		}

		// AABBとの交差判定
		bool AABB::Intersect(const Math::AABB& aabb) const
		{
			if (max.x < aabb.min.x || min.x > aabb.max.x) return false;
			if (max.z < aabb.min.z || min.z > aabb.max.z) return false;
			if (max.y < aabb.min.y || min.y > aabb.max.y) return false;
			return true;
		}

	} // namespace Math
} // namespace Bread