#pragma once

#include <memory>
#include "types.h"
#include "FND/Base.h"
#include "OS/Stream.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// リソース操作基底クラスです。
		//****************************************************************************
		class Resource : public FND::Base
		{
		private:
			bool ready = false;

		public:
			// 準備完了状態に設定
			void SetReady() { ready = true; }

			// 準備完了状態か判定
			bool IsReady() const { return ready; }
		};

		//****************************************************************************
		// リソース操作イベントをキャッチするインターフェース
		//****************************************************************************
		class IResourceListener : public FND::Base
		{
		public:
			// リソースが読み込まれたときに呼ばれる
			virtual void OnResourceLoaded(Resource* resource) = 0;

		};

		//****************************************************************************
		// リソース操作オブジェクト生成インターフェース
		//****************************************************************************
		class IResourceFactory : public FND::Base
		{
		public:
			// リソース操作オブジェクト生成
			virtual std::unique_ptr<Resource> CreateResource(u32 type) = 0;

			// リソース読み込み
			virtual bool LoadResource(Resource* resource, OS::IFileStream* stream, const char* filename) = 0;

		};

		//****************************************************************************
		// リソース操作オブジェクト管理インターフェース
		//****************************************************************************
		class IResourceManager : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IResourceManager> Create();

			// 初期化
			virtual bool Initialize(const char* rootDirectory) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// 拡張子に対応するリソースファクトリ登録
			virtual void RegisterFactory(const char* ext, std::shared_ptr<IResourceFactory> factory) = 0;

			// リソースイベントリスナー登録
			virtual void RegisterListener(IResourceListener* listener) = 0;

			// リソース操作オブジェクト非同期読み込み
			virtual std::shared_ptr<Resource> LoadAsync(const char* filename, u32 type = 0) = 0;

			// リソース操作オブジェクト同期読み込み
			virtual std::shared_ptr<Resource> LoadImmediate(const char* filename, u32 type = 0) = 0;

			// 保留中のすべてのリソースが読み込まれるまで待つ
			virtual void WaitOnPending() = 0;

			// 削除するリソースがある場合、解放する
			virtual int GarbageCollect() = 0;

			// 非同期読み込み
			template<class T>
			std::shared_ptr<T> LoadAsync(const char* filename, u32 type = 0)
			{
				return std::dynamic_pointer_cast<T>(LoadAsync(filename, type));
			}

			// 同期読み込み
			template<class T>
			std::shared_ptr<T> LoadImmediate(const char* filename, u32 type = 0)
			{
				return std::dynamic_pointer_cast<T>(LoadImmediate(filename, type));
			}
		};

	} // namespace OS
} // namespace Bread