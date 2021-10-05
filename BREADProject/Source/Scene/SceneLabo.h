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
	//����
	static std::unique_ptr<SceneLabo> Create();

	//�R���X�g���N�^
	void __fastcall Construct(SceneSystem* sceneSystem) override;

	//������
	void Initialize()override;

	//�X�V
	void __fastcall Update()      override;

	//�`��
	void __fastcall Draw()        override;

	//GUI
	void GUI()                               override;

	//�I������
	bool Finish() { return finished; }
};