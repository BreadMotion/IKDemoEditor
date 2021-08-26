#pragma once

#include "Graphics/Mesh.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// メッシュ操作オブジェクト基底クラス
		//****************************************************************************
		class Mesh : public IMesh
		{
		private:
			MeshDesc desc;
			Math::AABB bounds;

		protected:
			Mesh() {}
			~Mesh() override {}

		public:
			// バウンディングボックス取得
			const Math::AABB& GetBounds() override { return bounds; }

			// メッシュ設定記述取得
			MeshDesc& GetDesc() override { return desc; }

			// バウンディングボックスを計算
			void ComputeBounds() override;

			// バウンディングボックス計算
			void ComputeBounds(Math::AABB& bounds, const Math::Matrix boneTransforms[]) override;

			// スキニング頂点計算
			void ComputeSkinnedVertices(Math::Vector3 vertices[], const Math::Matrix boneTransforms[]) override;

		protected:
			// 基底初期化
			void BaseInitialize(const MeshDesc& desc)
			{
				this->desc = desc;
				ComputeBounds();
			}
		};

	}	// namespace grp
}	// namespace ys