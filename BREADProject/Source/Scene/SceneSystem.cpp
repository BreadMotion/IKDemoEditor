#include "SceneSystem.h"
//#include "../Player/player.h"
#include "Types.h"
#include "FND/Util.h"
#include "Graphics/GraphicsDevice.h"
#include "Graphics\Camera.h"
#include "FrameWork\Object\Object.h"
#include "FrameWork\Shader\Shader.h"
#include "FrameWork\Shader\BasicShader.h"
#include "FrameWork\Shader\BasicSkinShader.h"
#include "FrameWork\Shader\StandardShader.h"

std::unique_ptr<SceneSystem> SceneSystem::Create()
{
	return std::make_unique<SceneSystem>();
}

void SceneSystem::Initialize(Bread::OS::IDisplay* display)
{
	this->display        = display;

	AddScene<SceneGame>();

	for (Bread::u32 i = 0; i < scenes.size(); ++i)
		scenes[i]->Construct(this);

	ChangeScene(SceneType::Game, false, false);
}

void SceneSystem::Update()
{
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

}
