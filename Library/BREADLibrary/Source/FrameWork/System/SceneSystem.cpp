#include "pch.h"
#include "FrameWork/System/SceneSystem.h"


namespace Bread
{
	namespace FrameWork
	{
		// ������
		void IScene::Initialize()
		{
			Super::Initialize();


		}

		// �I����
		void IScene::Finalize()
		{

		}

		// �R���X�g���N�^
		void SceneSystem::Construct()
		{

		}

		// �f�X�g���N�^
		void SceneSystem::Destruct()
		{
			nextScene.reset();
			currentScene.reset();
			scenes.clear();
		}

		// ������
		void SceneSystem::Initialize()
		{

		}

		// �I����
		void SceneSystem::Finalize()
		{
			nextScene.reset();
			currentScene.reset();
			scenes.clear();
		}

		// �X�V
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

		// �`��
		void SceneSystem::Draw()
		{
			currentScene->Draw();
		}

		// �V�[�����폜
		void SceneSystem::RemoveScene(std::shared_ptr<IScene> scene)
		{

		}
	}
}