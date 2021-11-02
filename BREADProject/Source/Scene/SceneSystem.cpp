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
	//シーンの追加
	AddScene<SceneGame>();

	//追加したシーンの生成時初期化を行う
	for (Bread::u32 i = 0; i < scenes.size(); ++i)
		scenes[i]->Construct(this);

	//シーンを変更する
	ChangeScene(SceneType::Game, false, false);

	//MainThread以外のスレッドの開放するタイミングを知らせるフラグ
	MapInstance<bool>::instance["SceneSystemExist"] = true;

	//このマネージャーに登録されているActorが前フレームにDirtyフラグが立っていた場合、
	//Vertex情報の登録し直しを行う
	MapInstance<std::thread>::instance["TerrainManager_PolygonRegisterFunction1"]
		= std::thread(&TerrainManager::ReRegisterDirtyActorPolygon, &MapInstance<TerrainManager>::instance["TerrainModelManager"]);

	MapInstance<std::thread>::instance["TerrainManager_PolygonRegisterFunction2"]
		= std::thread(&TerrainManager::ReRegisterDirtyActorPolygon, &MapInstance<TerrainManager>::instance["CollisionModelManager"]);

	//情報を取得する空間の範囲を設定する
	//0 = 1   = 1 * 1 * 1
	//1 = 27  = 3 * 3 * 3
	//2 = 125 = 5 * 5 * 5
	//f(x)    = x^3
	MapInstance<TerrainManager>::instance["TerrainModelManager"]  .SetRenge(1);
	MapInstance<TerrainManager>::instance["CollisionModelManager"].SetRenge(1);
}

void SceneSystem::Update()
{
	//メインスレッドが稼働中
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

	//メインスレッドの処理終了
	MapInstance<bool>::instance["SyncMainThread"] = false;

	//TerrainManagerスレッドの待機終了
	while (MapInstance<TerrainManager>::instance["TerrainModelManager"].GetSync())
	{
	}
	while (MapInstance<TerrainManager>::instance["CollisionModelManager"].GetSync())
	{
	}
}
