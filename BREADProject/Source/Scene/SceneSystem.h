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
	//����
	static std::unique_ptr<SceneSystem> Create();

	//������
	void Initialize(Bread::OS::IDisplay* display);

	//�X�V
	void Update(Bread::f32 elapsedTime);

	//�`��
	void Draw(Bread::f32 elapsedTime);

	//GUI
	void GUI();

	//�V�[���؂�ւ�
	void ChangeScene(SceneType sceneType, bool stack, bool fade);

	//�V�[���Z�b�g
	void SetScene(SceneType sceneType);

	//�ҋ@�V�[�����Z�b�g����
	void ReSetStackScene();

	Bread::OS::IDisplay*              GetDisplay()        { return display; }

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
	Scene* GetScene(SceneType sceneType)
	{
		Bread::u32 index = static_cast<Bread::u32>(sceneType);
		return  scenes[index].get();
	}
};