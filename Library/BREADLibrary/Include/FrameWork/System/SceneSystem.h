#pragma once

#include <map>
#include "FrameWork/System/System.h"
#include "FrameWork/Actor/Actor.h"


namespace Bread
{
	namespace FrameWork
	{
		class IScene : public Actor
		{
		protected:
			using Super = Actor;

		public:
			IScene() {}
			virtual ~IScene() {}

		public:
			// 初期化
			void Initialize() override;

			// 終了化
			void Finalize() override;
		};

		class SceneSystem : public ISystem
		{
		private:
			std::vector<std::shared_ptr<IScene>> scenes;
			std::shared_ptr<IScene>              currentScene;
			std::shared_ptr<IScene>              nextScene;

		public:
			SceneSystem() {}
			~SceneSystem() override {}

		public:
			// 生成
			static std::shared_ptr<SceneSystem> Create()
			{
				return std::make_shared<SceneSystem>();
			}

			// コンストラクタ
			void Construct()  override;

			// デストラクタ
			void Destruct()   override;

			// 初期化
			void Initialize() override;

			// 終了化
			void Finalize()   override;

			// 更新
			void Update()     override;

			// 描画
			void Draw()       override;

		public:
			// シーンを削除
			void RemoveScene(std::shared_ptr<IScene> scene);

		public:
			// シーンを所得
			template<class T>
			std::shared_ptr<T> GetScene()
			{
				for (auto scene : scenes)
				{
					std::shared_ptr<T> scene = std::dynamic_pointer_cast<std::shared_ptr<T>>(scene);
					if (scene != nullptr)
					{
						return scene;
					}
				}
				return nullptr;
			}

			// シーンを追加
			template<class T>
			std::shared_ptr<T> AddScene()
			{
				std::shared_ptr<T> scene = std::make_shared<T>();
				
				scene->Construct();
				scenes.emplace_back(scene);

				return scene;
			}

			//	シーンの切り替え
			template<class T>
			void ChangeScene()
			{
				std::shared_ptr<T> scene = GetScene<T>();
				if (scene != nullptr)
				{
					nextScene = scene;
				}
			}
		};
	}
}