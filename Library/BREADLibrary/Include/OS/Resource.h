#pragma once

#include <memory>

#include "types.h"
#include "FND/Base.h"
#include "OS/Stream.h"


namespace Bread
{
	namespace OS
	{
		enum ResourceType
		{
			MODEL,
			ANIMATION,
			ALL,
		};

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
			virtual void __fastcall OnResourceLoaded(Resource* resource) = 0;

		};

		//****************************************************************************
		// リソース操作オブジェクト生成インターフェース
		//****************************************************************************
		class IResourceFactory : public FND::Base
		{
		public:
			// リソース操作オブジェクト生成
			virtual std::unique_ptr<Resource> __fastcall CreateResource(u32 type) = 0;

			// リソース読み込み
			virtual bool __fastcall LoadResource(Resource* resource, OS::IFileStream* stream, const char* filename) = 0;

		};

		//****************************************************************************
		// リソース操作オブジェクト管理インターフェース
		//****************************************************************************
		class IResourceManager : public FND::Base
		{
		public:
			// 生成
			static std::shared_ptr<IResourceManager> Create();

			// 初期化
			virtual bool __fastcall Initialize(const char* rootDirectory) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// 拡張子に対応するリソースファクトリ登録
			virtual void __fastcall RegisterFactory(const char* ext, std::shared_ptr<IResourceFactory> factory) = 0;

			// リソースイベントリスナー登録
			virtual void __fastcall RegisterListener(IResourceListener* listener) = 0;

			// リソース操作オブジェクト非同期読み込み
			virtual std::shared_ptr<Resource> __fastcall LoadAsync(const char* filename, u32 type = 0) = 0;

			// リソース操作オブジェクト同期読み込み
			virtual std::shared_ptr<Resource> __fastcall  LoadImmediate(const char* filename, u32 type) = 0;

			// 保留中のすべてのリソースが読み込まれるまで待つ
			virtual void WaitOnPending() = 0;

			// 削除するリソースがある場合、解放する
			virtual int GarbageCollect() = 0;

			// 非同期読み込み
			template<class T>
			std::shared_ptr<T> __fastcall LoadAsync(const char* filename, u32 type = 0)
			{
				return std::dynamic_pointer_cast<T>(LoadAsync(filename, type));
			}

			// 同期読み込み
			template<class T>
			std::shared_ptr<T> __fastcall LoadImmediate(const char* filename, u32 type = 0)
			{
				std::shared_ptr<Resource> resource = LoadImmediate(filename, type);
				if (!resource)
				{
					return nullptr;
				}

				return std::dynamic_pointer_cast<T>(resource);
			}
		};

	} // namespace OS
} // namespace Bread