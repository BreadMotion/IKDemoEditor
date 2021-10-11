#pragma once
#include <memory>
#include <map>

#include "Types.h"
#include "OS/Display.h"
#include "OS/ResourceManager.h"

#include "Graphics/GraphicsDevice.h"
#include "Graphics/Camera.h"
#include "Graphics/Texture.h"

#include "FrameWork/Object/Object.h"
#include "FrameWork/Shader/Shader.h"
#include "FrameWork\Shader\BasicShader.h"
#include "FrameWork\Shader\BasicSkinShader.h"
#include "FrameWork\Shader\StandardShader.h"
#include "FrameWork\Shader\PBRShader.h"

#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "FrameWork\FrameBuffer/FrameBuffer.h"
#include "FrameWork/Actor/Actor.h"
#include "FrameWork\Quad/Quad.h"
#include "FrameWork\IBL/IBL.h"
#include "FrameWork\ComputeShader/TestComputeShader.h"
#include "FrameWork\COmputeShader/BitonicSort.h"
#include "FrameWork\ComputeShader/GPUParticle.h"
//#include "FrameWork\Script/Lua.h"

#include "../IKTarget/IKTargetActor.h"
#include "../Camera/CameraActor.h"
#include "../Primitive/GeometricPrimitive.h"

class SceneSystem;
class Scene
{
protected:
	SceneSystem* sceneSystem{ nullptr };

public:
	Scene() {}
	virtual ~Scene() {}

public:
	//コンストラクタ
	virtual void __fastcall Construct(SceneSystem* sceneSystem)  = 0;

	//初期化
	virtual void Initialize() = 0;

	//更新
	virtual void __fastcall Update() = 0;

	//描画
	virtual void _fastcall Draw() = 0;

	//GUI
	virtual void GUI() {}
};

class SceneGame : public Scene
{
private:
	struct ShaderConstants
	{
		Bread::Math::Matrix   lightViewProjection = {};
		Bread::Math::Vector3  color               = Bread::Math::Vector3();
		Bread::f32            bias                = 0.0008f;
	};
private:
	int selected = -1;
	std::shared_ptr<Bread::FrameWork::Actor> selectAct;

	bool mainWindow = false;

public:
	SceneGame() {}
	~SceneGame() {}

private:
	//コンストラクタ
	void __fastcall Construct(SceneSystem* sceneSystem) override;

	//初期化
	void Initialize()                        override;

	//更新
	void __fastcall Update()override;

	//描画
	void __fastcall Draw()  override;

	//GUI
	//void GUI() override;

	//setupGui
	void SetupGUI();
	void __fastcall ImGuizmoUpdate(float* ary);

public:
	//primitiveを描画する
	void __fastcall PrimitiveRender(Bread::Graphics::DeviceDX11* device,
		Bread::Math::Vector3 translate, Bread::Math::Vector3 rotate, Bread::Math::Vector3 scale, Bread::f32 alpha);
	void __fastcall CylinderPrimitiveRender(Bread::Graphics::DeviceDX11* device,
		Bread::Math::Vector3 cp1Translate, Bread::Math::Vector3 cp2Translate,
		Bread::Math::Vector3 cyilinderTranslate, Bread::Math::Vector3 rotate, Bread::Math::Vector3 scale, Bread::Math::Vector3 cyilinderScale);

public://light
	//平行光の更新
	void UpdateLightDirection();
};