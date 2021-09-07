#pragma once

#include <map>
#include <string>
#include <memory>
#include <vector>

#include "Graphics/GraphicsDevice.h"
#include "Graphics/Model.h"
#include "Graphics/Animation.h"
#include "OS/CriticalSection.h"
#include "OS/Resource.h"
#include "OS/Stream.h"
#include "OS/Thread.h"
#include "OS/Event.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// リソースマネージャー
		//****************************************************************************
		class ResourceManager final : public IResourceManager, public IThreadWorker
		{
		private:
			using ResourceInfoMap = std::map<std::string, std::shared_ptr<Resource>>;

			std::map<std::string, std::shared_ptr<IResourceFactory>> factories;
			std::vector<IResourceListener*>                          listeners;

			std::unique_ptr<ICriticalSection> criticalSection;
			std::unique_ptr<IFileStream>      file;
			std::unique_ptr<IThread>          thread;
			std::unique_ptr<IEvent>           event;

			Graphics::IGraphicsDevice* graphicDevice = nullptr;

			ResourceInfoMap loaded;
			ResourceInfoMap pending;

			bool exitRequested = false;

		public:
			ResourceManager() {}
			~ResourceManager() override {}

		public:
			// 初期化
			bool Initialize(const char* rootDirectory) override;

			// 終了化
			void Finalize() override;

			// 拡張子に対応するリソースファクトリ登録
			void RegisterFactory(const char* ext, std::shared_ptr<IResourceFactory> factory) override;

			// リソースイベントリスナー登録
			void RegisterListener(IResourceListener* listener) override;

			// リソース操作オブジェクト非同期読み込み
			std::shared_ptr<Resource> LoadAsync(const char* filename, u32 type = 0) override;

			// リソース操作オブジェクト同期読み込み
			std::shared_ptr<Resource> LoadImmediate(const char* filename, u32 type = 0) override;

			// 保留中のすべてのリソースが読み込まれるまで待つ
			void WaitOnPending() override;

			// 削除するリソースがある場合、解放する
			int GarbageCollect() override;

			// スレッド実行処理
			void Execute() override;

			// スレッド強制終了
			void Exit() override;

		public:
			//グラフィックデバイスの設定
			void SetGraphicDevice(Graphics::IGraphicsDevice* graphicDevice)
			{
				this->graphicDevice = graphicDevice;
			}

			//リソースの取得
			std::shared_ptr<Resource> GetResource(const char* filename);

		private:
			// ファクトリ取得
			IResourceFactory* GetFactory(const char* filename);

			// リソース読み込み
			bool LoadResource(Resource* resource, const char* filename);
		};
	} // namespace OS
} // namespace Bread