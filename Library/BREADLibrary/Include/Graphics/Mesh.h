#pragma once

#include <memory>
#include <string>
#include <vector>
#include "FND/Base.h"
#include "Math/AABB.h"
#include "Math/Color.h"
#include "Math/vector.h"
#include "Math/Matrix.h"
#include "Graphics/Device.h"
#include "Graphics/Buffer.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// 頂点データを画面にレンダリングする方法
		//****************************************************************************
		enum class PrimitiveTopology
		{
			PointList,			// ポイントリスト
			LineList,			// ラインリスト
			LineStrip,			// ラインストリップ
			TriangleList,		// トライアングルリスト
			TriangleStrip,		// トライアングルストリップ

			TypeNum
		};

		// 頂点バッファ種類
		enum class VertexBufferKind
		{
			Position,
			Normal,
			Tangent,
			Color,
			TexCoord0,
			TexCoord1,
			TexCoord2,
			TexCoord3,
			BlendWeight0,
			BlendWeight1,
			BlendIndex0,
			BlendIndex1,

			TypeNum
		};

		//****************************************************************************
		// メッシュの設定記述
		//****************************************************************************
		struct MeshDesc
		{
			PrimitiveTopology primitiveTopology = PrimitiveTopology::TriangleList;	// プリミティブトポロジー

			u32 vertexCount                       = 0;							// 頂点数
			u32 indexCount                        = 0;							// インデックス数
			const u16* indicesU16              = nullptr;				// 16bitインデックスデータ
			const u32* indicesU32              = nullptr;				// 32bitインデックスデータ
			const Math::Vector3* positions = nullptr;			    // 座標データ
			const Math::Vector3* normals  = nullptr;				// 法線データ
			const Math::Vector3* tangents = nullptr;			    // 接線データ
			const Math::Color* colors          = nullptr;				// 色データ
			const Math::Vector2* texcoords0 = nullptr;			// テクスチャ座標データ0
			const Math::Vector2* texcoords1 = nullptr;			// テクスチャ座標データ1
			const Math::Vector2* texcoords2 = nullptr;			// テクスチャ座標データ2
			const Math::Vector2* texcoords3 = nullptr;			// テクスチャ座標データ3
			const Math::Vector4* blendWeights0     = nullptr;	// ブレンドウェイトデータ0
			const Math::Vector4* blendWeights1     = nullptr;	// ブレンドウェイトデータ1
			const Math::Vector4_U8* blendIndices0 = nullptr;	// ブレンドインデックスデータ0
			const Math::Vector4_U8* blendIndices1 = nullptr;	// ブレンドインデックスデータ1

			// 頂点接線を計算
			void ComputeTangents();
		};

		//****************************************************************************
		// メッシュ操作インターフェース
		//****************************************************************************
		class IMesh : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IMesh> Create();

			// 初期化
			virtual bool Initialize(IDevice* device, const MeshDesc& desc) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// 描画
			virtual void Draw(IDevice* device, VertexBufferKind vbKind[], u32 kindNum, u32 start, u32 count, PrimitiveTopology primitiveTopology) = 0;

			// バウンディングボックス取得
			virtual const Math::AABB& GetBounds() = 0;

			// メッシュ設定記述取得
			virtual MeshDesc& GetDesc() = 0;

			// バウンディングボックス計算
			virtual void ComputeBounds() = 0;

			// バウンディングボックス計算
			virtual void ComputeBounds(Math::AABB& bounds, const Math::Matrix boneTransforms[]) = 0;

			// スキニング頂点計算
			virtual void ComputeSkinnedVertices(Math::Vector3 vertices[], const Math::Matrix boneTransforms[]) = 0;
		};
	} // namespace Graphics
} // namespace Bread