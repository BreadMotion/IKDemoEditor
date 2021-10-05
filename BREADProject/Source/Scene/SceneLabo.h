#pragma once
#include "scene.h"
#include "SceneSystem.h"
//#include "FrameWork\Script\Lua.h"

class SceneLabo :public Scene
{
private:
	bool finished = false;

	//Lua
	//std::unique_ptr<Bread::FrameWork::LuaSystem> luaSystem;
	Bread::s8  filePass[1028] = "";
	bool       judgeLoad      = false;
	Bread::s32 aliveCnt       = 0;

public:
	SceneLabo() {}
	~SceneLabo() {}

public:
	//生成
	static std::unique_ptr<SceneLabo> Create();

	//コンストラクタ
	void __fastcall Construct(SceneSystem* sceneSystem) override;

	//初期化
	void Initialize()override;

	//更新
	void __fastcall Update()      override;

	//描画
	void __fastcall Draw()        override;

	//GUI
	void GUI()                               override;

	//終了処理
	bool Finish() { return finished; }
};