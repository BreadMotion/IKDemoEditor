#pragma once
#include <stdio.h>
#include <memory>
#include "Types.h"
#include "FND/Util.h"
#include "FrameWork/Main.h"
#include "Math/BreadMath.h"
#include "Loader\Loader.h"
#include "Graphics\Shader.h"
#include "Graphics\Model.h"
#include "Graphics\Camera.h"
#include "FrameWork\Renderer\Renderer.h"
#include "FrameWork\Object\Object.h"
#include "FrameWork\Shader\Shader.h"
#include "imgui.h"
#include "Scene/SceneSystem.h"

class Main : public Bread::FrameWork::Main
{
private:
	using Super = Bread::FrameWork::Main;
	std::unique_ptr<SceneSystem> sceneSystem;

public:
	Main() {}
	~Main() {}

public:
	//‰Šú‰»
	bool Initialize(Bread::uintPtr instance)override;

	//I—¹ˆ—
	void Finalize()override;

	//XV
	void Update(Bread::f32 elapsedTime)override;

	//•`‰æ
	void Render(Bread::f32 elapsedTime)override;

	//Gui
	void GUI();
};