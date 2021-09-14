#pragma once
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

	Bread::OS::IDisplay* display = nullptr;

public:
	SceneSystem() {}
	~SceneSystem() {}

public:
	//生成
	static std::unique_ptr<SceneSystem> Create();

	//初期化
	void Initialize(Bread::OS::IDisplay* display);

	//更新
	void Update(Bread::f32 elapsedTime);

	//描画
	void Draw(Bread::f32 elapsedTime);

	//GUI
	void GUI();

	//シーン切り替え
	void ChangeScene(SceneType sceneType, bool stack, bool fade);

	//シーンセット
	void SetScene(SceneType sceneType);

	//待機シーンをセットする
	void ReSetStackScene();

	Bread::OS::IDisplay*              GetDisplay()        { return display; }

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
	Scene* GetScene(SceneType sceneType)
	{
		Bread::u32 index = static_cast<Bread::u32>(sceneType);
		return  scenes[index].get();
	}
};