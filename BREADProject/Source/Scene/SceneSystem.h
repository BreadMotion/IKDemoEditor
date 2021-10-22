#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <memory>
#include "scene.h"
#include "OS/Display.h"
#include "Graphics/GraphicsDevice.h"
#include "SceneLabo.h"

enum class SceneType
{
	Game
};

class SceneSystem
{
private:
	std::vector<std::shared_ptr<Scene>> scenes;
	Scene* currentScene = nullptr;
	Scene* nextScene    = nullptr;
	Scene* stackScene   = nullptr;

public:
	SceneSystem() {}
	~SceneSystem() { MapInstance<std::thread>::instance["TerrainManager_PolygonRegisterFunction"].join(); }

public:
	//生成
	static std::unique_ptr<SceneSystem> Create();

	//初期化
	void __fastcall Initialize();

	//更新
	void __fastcall Update();

	//描画
	void __fastcall Draw();

	//GUI
	void GUI();

	//シーン切り替え
	void __fastcall ChangeScene(SceneType sceneType, bool stack, bool fade);

	//シーンセット
	void __fastcall SetScene(SceneType sceneType);

	//待機シーンをセットする
	void ReSetStackScene();

public:
	//シーンを追加
	template<class T>
	T* AddScene()
	{
		std::shared_ptr<T> scene = std::make_shared<T>();
		scenes.emplace_back(scene);
		return scene.get();
	}

	//シーン取得
	Scene* __fastcall GetScene(SceneType sceneType)
	{
		Bread::u32 index = static_cast<Bread::u32>(sceneType);
		return  scenes[index].get();
	}
};