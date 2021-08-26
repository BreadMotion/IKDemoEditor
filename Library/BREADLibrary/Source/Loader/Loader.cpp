#include "pch.h"
#include "FND/Logger.h"
#include "Loader.h"
#include "ModelDataLoader.h"
#include "AnimationDataLoader.h"


namespace Bread
{
	namespace Loader
	{
		//****************************************************************************
		// FBX読み込み操作オブジェクト
		//****************************************************************************
		// 生成
		std::unique_ptr<ILoader> ILoader::Create()
		{
			return std::make_unique<Loader>();
		}

		// 初期化
		bool Loader::Initialize(const char* fileName)
		{
			fbxManager = FbxManager::Create();

			// FBXに対する入出力を定義する
			FbxIOSettings* fbxIos = FbxIOSettings::Create(fbxManager, IOSROOT);	// 特別な理由がない限りIOSROOTを指定
			fbxManager->SetIOSettings(fbxIos);

			// インポータを生成
			FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "");
			if (!fbxImporter->Initialize(fileName, -1, fbxManager->GetIOSettings()))	// -1でファイルフォーマット自動判定
			{
				BREAD_LOG_FBX_ERROR("FbxImporter::Initialize() : Failed!!\n");
				return false;
			}

			// SceneオブジェクトにFBXファイル内の情報を流し込む
			fbxScene = FbxScene::Create(fbxManager, "scene");
			fbxImporter->Import(fbxScene);
			fbxImporter->Destroy();	// シーンを流し込んだらImporterは解放してOK

			filename = fileName;

			return true;
		}

		// 終了化
		void Loader::Finalize()
		{
			// マネージャ解放
			if (fbxManager != nullptr)
			{
				fbxManager->Destroy();		// 関連するすべてのオブジェクトが解放される
				fbxManager = nullptr;
			}
		}

		// モデルデータ読み込み
		bool Loader::Load(Graphics::ModelData& data, const char* dirname)
		{
			Triangulate();

			ModelDataLoader modelDataLoader;
			return modelDataLoader.Load(fbxScene, data, filename.c_str(), dirname);
		}

		// アニメーションデータ読み込み
		bool Loader::Load(Graphics::AnimationData& data)
		{
			AnimationDataLoader animationDataLoader;
			return animationDataLoader.Load(fbxScene, data);
		}

		// 三角形化
		void Loader::Triangulate()
		{
			// ジオメトリを三角形化する。
			FbxGeometryConverter fbx_geometry_converter(fbxManager);
			fbx_geometry_converter.Triangulate(fbxScene, true);
			fbx_geometry_converter.RemoveBadPolygonsFromMeshes(fbxScene);
		}

	}	// namespace fbx
}	// namespace ys