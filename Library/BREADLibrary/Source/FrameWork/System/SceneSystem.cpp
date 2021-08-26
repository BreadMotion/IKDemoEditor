#include "pch.h"
#include "FrameWork/System/SceneSystem.h"


namespace Bread
{
	namespace FrameWork
	{
		// 初期化
		void IScene::Initialize()
		{
			Super::Initialize();


		}

		// 終了化
		void IScene::Finalize()
		{

		}

		// コンストラクタ
		void SceneSystem::Construct()
		{

		}

		// デストラクタ
		void SceneSystem::Destruct()
		{
			nextScene.reset();
			currentScene.reset();
			scenes.clear();
		}

		// 初期化
		void SceneSystem::Initialize()
		{

		}

		// 終了化
		void SceneSystem::Finalize()
		{
			nextScene.reset();
			currentScene.reset();
			scenes.clear();
		}

		// 更新
		void SceneSystem::Update()
		{
			if (nextScene != nullptr)
			{
				currentScene = nextScene;
				currentScene->Initialize();
				nextScene.reset();
			}

			currentScene->Update();
		}

		// 描画
		void SceneSystem::Draw()
		{
			currentScene->Draw();
		}

		// シーンを削除
		void SceneSystem::RemoveScene(std::shared_ptr<IScene> scene)
		{

		}
	}
}