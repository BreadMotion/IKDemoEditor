#include "pch.h"
#include <string>

#include "OS/ResourceManager.h"
#include "OS/Path.h"
#include "../Source/Loader/Loader.h"
#include "FND/Assert.h"
#include "FND/Logger.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// リソースマネージャー
		//****************************************************************************
		// 生成
		std::unique_ptr<IResourceManager> IResourceManager::Create()
		{
			return std::make_unique<ResourceManager>();
		}

		// 初期化
		bool ResourceManager::Initialize(const char* rootDirectory)
		{
			file = IFileStream::Create();
			if (!file->Initialize(rootDirectory))
			{
				return false;
			}

			criticalSection = ICriticalSection::Create();
			if (!criticalSection->Initialize())
			{
				return false;
			}

			event = IEvent::Create();
			if (!event->Initialize())
			{
				return false;
			}

			thread = IThread::Create();
			if (!thread->Initialize())
			{
				return false;
			}

			this->RegisterFactory("ani", Bread::Graphics::IAnimationResourceFactory::Create());
			this->RegisterFactory("mdl", Bread::Graphics::IModelResourceFactory::Create(graphicDevice->GetDevice()));

			file = OS::IFileStream::Create();
			file->Initialize(nullptr);

			return true;
		}

		// 終了化
		void ResourceManager::Finalize()
		{
			GarbageCollect();

			if (criticalSection.get() != nullptr)
			{
				criticalSection->Enter();
			}

			if (thread.get() != nullptr)
			{
				thread->Join();
			}

			if (event.get() != nullptr)
			{
				event->Set();
			}

			thread.reset();
			event.reset();
			file.reset();

			for (auto&& load : loaded)
			{
				load.second.reset();
			}
			loaded.clear();
			for (auto&& pend : pending)
			{
				pend.second.reset();
			}
			pending.clear();

			listeners.clear();
			factories.clear();
		}

		// 拡張子に対応するリソースファクトリ登録
		void ResourceManager::RegisterFactory(const char* ext, std::shared_ptr<IResourceFactory> factory)
		{
			factories[ext] = factory;
		}

		// リソースイベントリスナー登録
		void ResourceManager::RegisterListener(IResourceListener* listener)
		{
			listeners.emplace_back(listener);
		}

		// リソース操作オブジェクト非同期読み込み
		std::shared_ptr<Resource> ResourceManager::LoadAsync(const char* filename, u32 type)
		{
			CriticalSectionLock lock(criticalSection.get());

			std::shared_ptr<Resource> resource;

			// キャッシュに存在するかチェック
			{
				auto it = loaded.find(filename);
				if (it != loaded.end())
				{
					resource = it->second;
				}
			}
			// ペンディングに存在するかチェック
			if (resource == nullptr)
			{
				auto it = pending.find(filename);
				if (it != pending.end())
				{
					resource = it->second;
				}
			}

			// 新しいリソース情報を作成しペンディングに追加する
			if (resource == nullptr)
			{
				IResourceFactory* factory = GetFactory(filename);
				if (factory == nullptr)
				{
					return nullptr;
				}

				resource = factory->CreateResource(type);
				pending[filename] = resource;

				event->Set();	// スレッドを起こす
			}

			return resource;
		}

		// リソース操作オブジェクト同期読み込み
		std::shared_ptr<Resource> ResourceManager::LoadImmediate(const char* filename, u32 type)
		{
			CriticalSectionLock lock(criticalSection.get());

			std::shared_ptr<Resource> resource = nullptr;
			std::string fileName;
			{
				const char* fullPass      = OS::Path::GetFullPath(filename);
				std::string animFullPass  = std::string(fullPass);
				std::string Filename;

				//モデルのロード
				auto ModelDataLoad = ([](Loader::ILoader* iloader,const char* filename, const std::string& Filename) {
					Graphics::ModelData data;
					if (!iloader->Load(data, OS::Path::GetDirectoryName(filename)))
					{
						return false;
					}
					Graphics::ModelData::Serialize(data, Filename.c_str());
					});
				//アニメーションのロード
				auto AnimDataLoad = ([](Loader::ILoader* iloader, const std::string& Filename) {
					Graphics::AnimationData data;
					if (!iloader->Load(data))
					{
						return false;
					}
					Graphics::AnimationData::Serialize(data, Filename.c_str());
					});
				//ファイルパスの調整
				if (!OS::Path::CheckFileExtension(fullPass, "fbx") && file->Exists(Filename.c_str()))
				{
					return false;
				}
				std::unique_ptr<Loader::ILoader> loader = Loader::ILoader::Create();
				if (!loader->Initialize(filename))
				{
					return false;
				}

				switch (type)
				{
				case ResourceType::MODEL:
				{
					Filename = OS::Path::ChangeFileExtension(Filename.c_str(), "mdl");
					if (!ModelDataLoad(loader.get(), filename, Filename))
					{
						return false;
					}
					break;
				}
				case ResourceType::ANIMATION:
				{
					Filename = OS::Path::ChangeFileExtension(Filename.c_str(), "ani");
					if (!AnimDataLoad(loader.get(), Filename))
					{
						return false;
					}
					break;
				}
				}

				fileName = OS::Path::GetFileNameWithoutExtension(Filename.c_str());
			}

			// キャッシュに存在するかチェック
			{
				auto it = loaded.find(filename);
				if (it != loaded.end())
				{
					resource = it->second;
				}
			}
			// ペンディングに存在するかチェック
			if (resource == nullptr)
			{
				auto it = pending.find(filename);
				if (it != pending.end())
				{
					resource = it->second;
				}
			}
			// 新しいリソース情報を作成し、即時ロードする
			if (resource == nullptr)
			{
				IResourceFactory* factory = GetFactory(filename);
				if (factory == nullptr)
				{
					return nullptr;
				}

				resource = factory->CreateResource(type);
				bool load = LoadResource(resource.get(), filename);
				if (!load)
				{
					BREAD_LOG_OS_ERROR("リソースファイルの読み込みに失敗しました。: %s\n", filename);
					return nullptr;
				}
				else
				{
					loaded[filename] = resource;
				}
			}

			return resource;
		}

		// 保留中のすべてのリソースが読み込まれるまで待つ
		void ResourceManager::WaitOnPending()
		{
			size_t pendingCount = 0;

			do
			{
				if (criticalSection)
				{
					CriticalSectionLock lock(criticalSection.get());
					pendingCount = pending.size();
				}

				if (pendingCount > 0)
				{
					IThread::Sleep(16);
				}
			} while (pendingCount);
		}

		// 削除するリソースがある場合、解放する
		int ResourceManager::GarbageCollect()
		{
			s32 numCollected = 0;

			WaitOnPending();
			PHOENIX_ASSERT(pending.size() == 0);

			// WaitOnPending()の前でロックするとデッドロックになる可能性がある。
			if (criticalSection)
			{
				CriticalSectionLock lock(criticalSection.get());

				bool foundOne = true;
				while (foundOne)
				{
					foundOne = false;
					auto it = loaded.begin();
					while (it != loaded.end())
					{
						std::shared_ptr<Resource> resource = it->second;
						PHOENIX_ASSERT(resource != nullptr);

						if (resource.use_count() <= 1)
						{
							foundOne = true;
							BREAD_LOG_OS_VERVOSE("Unloading %s\n", it->first.c_str());
							resource.reset();
							++numCollected;

							auto temp = it;
							++it;
							loaded.erase(temp);
						}
						else
						{
							++it;
						}
					}
				}
				BREAD_LOG_OS_VERVOSE("GarbageCollect completed\n");
				BREAD_LOG_OS_VERVOSE("Active Resources# %u\n", loaded.size());
			}

			return numCollected;
		}

		// スレッド実行処理
		void ResourceManager::Execute()
		{
			std::string filename;
			while (!exitRequested)
			{
				event->Wait();
				if (exitRequested)
				{
					break;
				}

				std::shared_ptr<Resource> resource;
				bool pend = true;
				while (pend)
				{
					// ペンディング中の最初のファイル名を取得
					{
						CriticalSectionLock lock(criticalSection.get());

						auto it = pending.begin();
						if (it == pending.end())
						{
							pend = false;
							continue;
						}

						PHOENIX_ASSERT(it != pending.end());

						// クリティカルセクション外で使えるようにコピー
						filename = it->first.c_str();
						resource = it->second;
					}

					// リソース読み込み
					LoadResource(resource.get(), filename.c_str());

					// キャッシュに追加し、ペンディングを削除する
					{
						CriticalSectionLock lock(criticalSection.get());
						loaded[filename.c_str()] = resource;
						pending.erase(filename.c_str());
					}

					// リスナーにロードしたことを伝える
					for (auto&& listener : listeners)
					{
						listener->OnResourceLoaded(resource.get());
					}
				}
			}
		}

		// スレッド強制終了
		void ResourceManager::Exit()
		{
			exitRequested = true;
		}

		// ファクトリ取得
		IResourceFactory* ResourceManager::GetFactory(const char* filename)
		{
			const char* ext = Path::GetFileExtension(filename);

			IResourceFactory* factory = nullptr;
			auto it = factories.find(ext);
			if (it != factories.end())
			{
				factory = it->second.get();
			}
			if (factory == nullptr)
			{
				BREAD_LOG_OS_ERROR("指定のファイルに対応したリソースファクトリが見つかりませんでした。'%s'\n", filename);
			}

			return factory;
		}

		//リソースの取得
		std::shared_ptr<Resource> ResourceManager::GetResource(const char* filename)
		{
			return loaded[filename];
		}

		//グラフィックデバイスの設定
		void ResourceManager::SetGraphicsDevice(Graphics::IGraphicsDevice* graphicDevice)
		{
			this->graphicDevice = graphicDevice;
		}

		// リソース読み込み
		bool ResourceManager::LoadResource(Resource* resource, const char* filename)
		{
			if (!file->Exists(filename))
			{
				BREAD_LOG_OS_ERROR("指定のリソースファイルが存在しません。'%s'\n", filename);
				return false;
			}

			IResourceFactory* factory = GetFactory(filename);
			if (factory == nullptr)
			{
				return false;
			}

			bool loaded = factory->LoadResource(resource, file.get(), filename);
			if (loaded)
			{
				resource->SetReady();
				BREAD_LOG_OS_VERVOSE("Loaded '%s'\n", filename);
			}
			else
			{
				BREAD_LOG_OS_ERROR("指定のリソースファイルの読み込みに失敗しました。'%s'\n");
			}

			return loaded;
		}
	} // namespace OS
} // namespace Bread