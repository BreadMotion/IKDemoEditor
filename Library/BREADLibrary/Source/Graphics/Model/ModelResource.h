#pragma once

#include "Graphics/Model.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// モデルリソースクラス
		//****************************************************************************
		class ModelResource : public IModelResource
		{
		private:
			std::vector<std::unique_ptr<Graphics::IMesh>> meshes;
			ModelData data;

		public:
			// 初期化
			bool Initialize(Graphics::IDevice* device, const char* filename);

			// モデルデータ取得
			const ModelData& GetModelData() override { return data; }

			// メッシュ取得
			IMesh* GetMesh(sizeT index) override { return meshes.at(index).get(); }

			// メッシュサイズ取得
			sizeT GetMeshSize() override { return meshes.size(); }
		};

		//****************************************************************************
		// モデルリソースファクトリクラス
		//****************************************************************************
		class ModelResourceFactory : public IModelResourceFactory
		{
		private:
			Graphics::IDevice* device = nullptr;

		public:
			ModelResourceFactory(Graphics::IDevice* device) : device(device) {}

			// リソース作成
			std::unique_ptr<OS::Resource> CreateResource(u32 type) override;

			// リソース読み込み
			bool LoadResource(OS::Resource* resource, OS::IFileStream* stream, const char* filename)  override;

		};
	} // namespace Graphics
} // namespace Bread