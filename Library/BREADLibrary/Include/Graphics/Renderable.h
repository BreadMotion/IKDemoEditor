#pragma once

#include "FND/Flag.h"
#include "Math/AABB.h"
#include "Math/Color.h"
#include "Math/Matrix.h"
#include "Graphics/Mesh.h"
#include "Graphics/Material.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// 描画ノード制御フラグ
		//****************************************************************************
		struct RenderableNodeFlag : public FND::Flag32
		{
			static const u32 None = 0;
			static const u32 ShadowCaster = (1 << 0);		// 影を落とす。
			static const u32 TestAgainstBBoxOnly = (1 << 1);	// ヒットテストをバウンディングボックスのみにする。
			static const u32 NotPickable = (1 << 2);			// このノードはピックの対象にしない。
			static const u32 Foreground = (1 << 3);			// 最前面に表示する。
			static const u32 Skinning = (1 << 4);			// スキニングする。

			FLAG32_CONSTRUCT(RenderableNodeFlag)
		};

		//****************************************************************************
		// 描画ノード
		//****************************************************************************
		struct RenderableNode
		{
			IMesh* mesh = nullptr;								// メッシュ
			const Material* material = nullptr;					// マテリアル
			u32 startVertex = 0;								// 開始頂点
			u32 vertexCount = 0;								// 頂点数
			Math::Matrix worldTransform = Math::Matrix::One;	// ワールド変換行列
			const Math::Matrix* boneTransforms = nullptr;		// ボーン変換行列
			u32 boneTransformCount = 0;							// ボーン変換行列数
			Math::AABB bounds = Math::AABB::Zero;				// バウンディングボックス
			RenderableNodeFlag flags = 0;						// 描画ノード制御フラグ
		};

		//****************************************************************************
		// 描画切り替え判定フラグ
		//****************************************************************************
		struct RenderFlag : public FND::Flag32
		{
		public:
			static const u32 None = 0;
			static const u32 Textured = (1 << 0);			// テクスチャ表示します。
			static const u32 Lit = (1 << 1);				// ライティング処理のON/OFF
			static const u32 RenderBackFace = (1 << 2);		// 背面表示します。
			static const u32 DisableDepthTest = (1 << 3);	// 深度テストをしません。
			static const u32 DisableDepthWrite = (1 << 4);	// 深度書き込みをしません。
			static const u32 AlphaBlend = (1 << 5);			// アルファブレンドします。
			static const u32 Skinning = (1 << 6);			// スキニングします。
			static const u32 Foreground = (1 << 7);			// 最前面に描画します。
			static const u32 All = 0xFFFFFFFF;				// リソースをマップ可能にし、CPU がそのリソースの内容を読み取れるようにします。
							 
			FLAG32_CONSTRUCT(RenderFlag)
		};
	} // namespace Graphics
} // namespace Bread