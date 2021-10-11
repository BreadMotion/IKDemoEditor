#include "SceneLabo.h"
#include "SceneSystem.h"
#include "FND/Operation.h"

std::unique_ptr<SceneLabo> SceneLabo::Create()
{
	return std::make_unique<SceneLabo>();
}

void SceneLabo::Construct(SceneSystem* sceneSystem) {
	this->sceneSystem = sceneSystem;

	//luaSystem = Bread::FrameWork::LuaSystem::Create();
	//luaSystem->Initialize(graphicsDevice);
}

void SceneLabo::Initialize()
{
	finished = false;

	judgeLoad = false;
	aliveCnt  = 0;
}

void SceneLabo::Update()
{

}

void SceneLabo::Draw()
{

}

void SceneLabo::GUI()
{

}