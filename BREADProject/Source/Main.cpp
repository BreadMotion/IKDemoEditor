#include "Main.h"
#include "FND/STD.h"
#include "OS/Path.h"
#include "OS/ResourceManager.h"
#include "FrameWork/Renderer/ModelRenderer.h"
#include "FrameWork/Shader/BasicShader.h"
#include "FrameWork/Shader/BasicSkinShader.h"
#include "FrameWork/Shader/StandardShader.h"

namespace Bread
{
	namespace OS
	{
		std::unique_ptr<IMain> IMain::Create()
		{
			return std::make_unique<Main>();
		}
	}
}

bool Main::Initialize(Bread::uintPtr instance)
{
	Super::Initialize(instance);

	//シーンの生成
	sceneSystem = SceneSystem::Create();
	sceneSystem->Initialize(display.get());

	return true;
}

void Main::Finalize()
{
	Super::Finalize();
}

void Main::Update()
{
    //シーンの更新
	{
		sceneSystem->Update();
	}

	//パラメーター調整GUI
	{
		GUI();
	}
}

void Main::GUI()
{
	sceneSystem->GUI();
}

void Main::Render()
{
	sceneSystem->Draw();
}