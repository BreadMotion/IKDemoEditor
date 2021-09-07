#pragma once

#include <fbxsdk.h>
//#include "fbxsdk.h"
#include "Loader/Loader.h"


namespace Bread
{
	namespace Loader
	{
		//****************************************************************************
		// FBX読み込み操作オブジェクトクラス
		//****************************************************************************
		class Loader final : public ILoader
		{
		private:
			FbxManager* fbxManager = nullptr;
			FbxScene*   fbxScene   = nullptr;
			std::string filename;

		public:
			Loader() {}
			~Loader() override { Finalize(); }

		public:
			// 初期化
			bool Initialize(const char* fileName) override;

			// 終了化
			void Finalize() override;

			// モデルデータ読み込み
			bool Load(Graphics::ModelData& data, const char* dirname) override;

			// アニメーションデータ読み込み
			bool Load(Graphics::AnimationData& data) override;

		private:
			// 三角形化
			void Triangulate();
		};

	} // namespace Loader
} // namespace Bread