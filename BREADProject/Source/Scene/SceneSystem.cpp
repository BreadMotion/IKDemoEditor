#include "SceneSystem.h"
#include "Types.h"
#include "FND/Util.h"
#include "Graphics/GraphicsDevice.h"
#include "Graphics\Camera.h"
#include "FrameWork\Object\Object.h"

#include "FrameWork/Object/TerrainManager.h"

using Bread::FND::MapInstance;           // SyncMainThread , SyncTerrainManager , "SceneSystemExist", TerrainManager

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

	//MainThread�ȊO�̃X���b�h�̊J������^�C�~���O��m�点��t���O
	MapInstance<bool>::instance["SceneSystemExist"] = true;

	//���̃}�l�[�W���[�ɓo�^����Ă���Actor���O�t���[����Dirty�t���O�������Ă����ꍇ�A
	//Vertex���̓o�^���������s��
	MapInstance<std::thread>::instance["TerrainManager_PolygonRegisterFunction1"]
		= std::thread(&TerrainManager::ReRegisterDirtyActorPolygon, &MapInstance<TerrainManager>::instance["TerrainModelManager"]);

	MapInstance<std::thread>::instance["TerrainManager_PolygonRegisterFunction2"]
		= std::thread(&TerrainManager::ReRegisterDirtyActorPolygon, &MapInstance<TerrainManager>::instance["CollisionModelManager"]);

	//�����擾�����Ԃ͈̔͂�ݒ肷��
	//0 = 1   = 1 * 1 * 1
	//1 = 27  = 3 * 3 * 3
	//2 = 125 = 5 * 5 * 5
	//f(x)    = x^3
	MapInstance<TerrainManager>::instance["TerrainModelManager"]  .SetRenge(1);
	MapInstance<TerrainManager>::instance["CollisionModelManager"].SetRenge(1);
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
	while (MapInstance<TerrainManager>::instance["TerrainModelManager"].GetSync())
	{
	}
	while (MapInstance<TerrainManager>::instance["CollisionModelManager"].GetSync())
	{
	}
}
