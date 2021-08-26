#include "SceneLabo.h"
#include "SceneSystem.h"
#include "FND/Operation.h"

std::unique_ptr<SceneLabo> SceneLabo::Create()
{
	return std::make_unique<SceneLabo>();
}

void SceneLabo::Construct(SceneSystem* sceneSystem) {
	this->sceneSystem = sceneSystem;
	display           = sceneSystem->GetDisplay();
	graphicsDevice    = sceneSystem->GetGraphicsDevice();

	//luaSystem = Bread::FrameWork::LuaSystem::Create();
	//luaSystem->Initialize(graphicsDevice);
}

void SceneLabo::Initialize()
{
	finished = false;

	judgeLoad = false;
	aliveCnt  = 0;
}

void SceneLabo::Update(Bread::f32 elapsedTime)
{

}

void SceneLabo::Draw(Bread::f32 elapsedTime)
{

}

void SceneLabo::GUI()
{

}