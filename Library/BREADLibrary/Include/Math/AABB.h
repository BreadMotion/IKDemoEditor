#pragma once

#include "Math/Matrix.h"
#include "Math/Vector.h"


namespace Bread
{
	namespace Math
	{
		//****************************************************************************
		// バウンディングボックス
		//****************************************************************************
		class AABB
		{
		public:
			Vector3 min; // 最小値
			Vector3 max; // 最大値

			static const AABB Zero; // ゼロ

		public:
			AABB() {}
			AABB(const Vector3& min, const Vector3& max) { this->min = min; this->max = max; }
			~AABB() {}

		public:
			// AABBの範囲をリセットします
			void Reset();

			// 与えられた行列を元にAABBの範囲を設定します
			void __fastcall Transform(const Matrix& transform);

			// 与えられたAABBを含むように範囲を拡張します
			void __fastcall Extend(const AABB& aabb);

			// 与えられた点を含むように範囲を拡張します
			void __fastcall Extend(const Vector3& point);

			// 与えられた点が範囲内に含まれているか判定します
			bool __fastcall Contain(const Vector3& point);

			// AABBのコーナー位置を取得します
			void __fastcall GetCorners(Vector3 corners[8]);

			// AABBの中心位置を取得します
			Vector3 __fastcall GetCenter() const { return (min + max) / 2.0f; }

			// 空か
			bool __fastcall IsEmpty() const { return min == max; }

			// 球との交差判定
			bool __fastcall Intersect(const Math::Vector3& center, f32 radius) const;

			// AABBとの交差判定
			bool __fastcall Intersect(const Math::AABB& aabb) const;
		};

	} // namespace Math
} // namespace Bread