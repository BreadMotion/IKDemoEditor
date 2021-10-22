#include "SceneSystem.h"
#include "Types.h"
#include "FND/Util.h"
#include "Graphics/GraphicsDevice.h"
#include "Graphics\Camera.h"
#include "FrameWork\Object\Object.h"

#include "FrameWork/Object/TerrainManager.h"

using Bread::FND::Instance;              //TerrainManager
using Bread::FND::MapInstance;           // SyncMainThread , SyncTerrainManager

using Bread::FrameWork::TerrainManager;

std::unique_ptr<SceneSystem> SceneSystem::Create()
{
	return std::make_unique<SceneSystem>();
}

void SceneSystem::Initialize()
{
	//�V�[���̒ǉ�
	AddScene<SceneGame>();

	//�ǉ������V�[���̐��������������s��
	for (Bread::u32 i = 0; i < scenes.size(); ++i)
		scenes[i]->Construct(this);

	//�V�[����ύX����
	ChangeScene(SceneType::Game, false, false);

	//TODO : ������
	//���̃}�l�[�W���[�ɓo�^����Ă���Actor���O�t���[����Dirty�t���O�������Ă����ꍇ�A
	//Vertex���̓o�^���������s��
	MapInstance<std::thread>::instance["TerrainManager_PolygonRegisterFunction"]
		= std::thread(&TerrainManager::ReRegisterDirtyActorPolygon, &Instance<TerrainManager>::instance);
}

void SceneSystem::Update()
{
	//���C���X���b�h���ғ���
	MapInstance<bool>::instance["SyncMainThread"] = true;

	if (stackScene)
	{
		stackScene->Update();
	}
	currentScene->Update();
}

void SceneSystem::GUI()
{

}

void SceneSystem::ChangeScene(SceneType sceneType, bool stack, bool fade)
{
	if (stack)
	{
		stackScene = scenes[static_cast<int>(sceneType)].get();
	}
	currentScene = scenes[static_cast<int>(sceneType)].get();

	currentScene->Initialize();
}

void SceneSystem::SetScene(SceneType sceneType)
{
	nextScene = GetScene(sceneType);
	nextScene->Initialize();
}

void SceneSystem::ReSetStackScene()
{
	currentScene = stackScene;
	stackScene   = nullptr;
}

void SceneSystem::Draw()
{
	if (stackScene)
	{
		stackScene->Draw();
	}
	currentScene->Draw();

	//���C���X���b�h�̏����I��
	MapInstance<bool>::instance["SyncMainThread"] = false;

	//TerrainManager�X���b�h�̑ҋ@�I��
	while (MapInstance<bool>::instance["SyncTerrainManager"])
	{
	}
}
