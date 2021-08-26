#pragma once

#include <fbxsdk.h>
#include "Graphics/Model.h"
#include "Math/BreadMath.h"


namespace Bread
{
	namespace Loader
	{
		//****************************************************************************
		// モデルデータローダークラス
		//****************************************************************************
		class ModelDataLoader
		{
		private:
			std::vector<FbxSurfaceMaterial*>	fbxSurfaceMaterials;

		public:
			// モデルデータ読み込み
			bool Load(FbxScene* fbxScene, Graphics::ModelData& data, const char* fbxFilename, const char* dirname);

		private:
			// ノード処理
			void ProcessNodes(FbxNode* fbxNode, Graphics::ModelData& data, s32 parentIndex);

			// ノード処理
			void ProcessNode(FbxNode* fbxNode, Graphics::ModelData::Node& node, s32 parentIndex);

			// メッシュ処理
			void ProcessMeshes(FbxNode* fbxNode, Graphics::ModelData& data);

			// メッシュ処理
			void ProcessMesh(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh, Graphics::ModelData& data);

			// 頂点座標処理
			void ProcessVertexPositions(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh);

			// 頂点法線処理
			void ProcessVertexNormals(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh);

			// 頂点接線処理
			void ProcessVertexTangents(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh);

			// 頂点テクスチャ座標処理
			void ProcessVertexTexcoords(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh);

			// 頂点カラー処理
			void ProcessVertexColors(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh);

			// 頂点スキン処理
			void ProcessVertexSkins(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh, Graphics::ModelData& data);

			// サブセット処理
			void ProcessSubsets(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh);

			// マテリアル処理
			void ProcessMaterials(FbxScene* fbxScene, Graphics::ModelData& data, const char* dirname);

			// マテリアル処理
			void ProcessMaterial(FbxSurfaceMaterial* fbxSurfaceMaterial, Graphics::ModelData::Material& material, const char* dirname);

			// マテリアルプロパティ取得
			bool GetMaterialProperty(
				const FbxSurfaceMaterial* fbxSurfaceMaterial,
				const char* fbxColorPropertyName,
				const char* fbxFactorPropertyName,
				FbxDouble3& fbxColor,
				FbxFileTexture*& fbxTexture);

			bool GetMaterialProperty(
				const FbxSurfaceMaterial* fbxSurfaceMaterial,
				const char* fbxColorPropertyName,
				FbxDouble3& fbxColor,
				FbxFileTexture*& fbxTexture);

			bool GetMaterialProperty(
				FbxProperty fbxProperty,
				FbxDouble3& fbxColor,
				FbxFileTexture*& fbxTexture);

			// マテリアルファクタープロパティ取得
			bool GetMaterialFactorProperty(
				const FbxSurfaceMaterial* fbxSurfaceMaterial,
				const char* fbxColorPropertyName,
				FbxDouble& fbxFactor);

			bool GetMaterialFactorProperty(
				FbxProperty fbx_property,
				FbxDouble& fbxFactor);

			// 頂点接線を法線とUV座標から計算する
			void ComputeTangents(Graphics::ModelData::Mesh& mesh);

			// マテリアルインデックス検索
			s32 FindMaterialIndex(FbxScene* fbxScene, FbxSurfaceMaterial* fbxSurfaceMaterial);

			// ノードインデックスを検索
			static s32 FindNodeIndex(const Graphics::ModelData& data, const char* name);
		};

	} // namespace Loader
} // namespace Bread