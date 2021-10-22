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
	//����
	static std::unique_ptr<SceneSystem> Create();

	//������
	void __fastcall Initialize();

	//�X�V
	void __fastcall Update();

	//�`��
	void __fastcall Draw();

	//GUI
	void GUI();

	//�V�[���؂�ւ�
	void __fastcall ChangeScene(SceneType sceneType, bool stack, bool fade);

	//�V�[���Z�b�g
	void __fastcall SetScene(SceneType sceneType);

	//�ҋ@�V�[�����Z�b�g����
	void ReSetStackScene();

public:
	//�V�[����ǉ�
	template<class T>
	T* AddScene()
	{
		std::shared_ptr<T> scene = std::make_shared<T>();
		scenes.emplace_back(scene);
		return scene.get();
	}

	//�V�[���擾
	Scene* __fastcall GetScene(SceneType sceneType)
	{
		Bread::u32 index = static_cast<Bread::u32>(sceneType);
		return  scenes[index].get();
	}
};