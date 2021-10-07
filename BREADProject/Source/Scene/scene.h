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

#include "../Player/Player.h"
#include "../IKTarget/IKTargetCom.h"
#include "../Stage/StageCom.h"
#include "../Camera/CameraAct.h"
#include "../Primitive/GeometricPrimitive.h"

class SceneSystem;
class Scene
{
protected:
	SceneSystem*                                                    sceneSystem    = nullptr;
	Bread::OS::IDisplay*                                            display        = nullptr;
	std::map<std::string ,std::shared_ptr<Bread::FrameWork::Actor>> actors         = {};

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
	virtual void GUI() = 0;
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
	const std::string playerS   = "player";
	const std::string ikTS      = "footikTarget";
	const std::string stageS    = "stage";
	const std::string cameraS   = "camera";

	int selected = -1;
	std::shared_ptr<Bread::FrameWork::Actor> selectAct;

	bool mainWindow = false;

private:
	//シーン内オブジェクト
	std::shared_ptr < Bread::FrameWork::IShader> basicShader     = nullptr;
	std::shared_ptr < Bread::FrameWork::IShader> basicSkinShader = nullptr;
	std::shared_ptr < Bread::FrameWork::IShader> standardShader  = nullptr;
	std::shared_ptr < Bread::FrameWork::IShader> pbrShader       = nullptr;
	std::shared_ptr < Bread::FrameWork::IShader> pbrSkinShader   = nullptr;
	std::shared_ptr < Bread::FrameWork::IShader> currentShader   = nullptr;

	//フレームバッファ
	std::unique_ptr<Bread::FrameWork::FrameBuffer> frameBuffer[6];

	//ポストプロセス
	std::unique_ptr<Bread::FrameWork::PostProcessingEffects> postProcessingEffects;

	//シャドウマップ
	std::unique_ptr < Bread::FrameWork::FrameBuffer> shadowMap;
	std::unique_ptr < Bread::Graphics::IShader>      voidPS;
	std::unique_ptr < Bread::Graphics::Camera>       lightSpaceCamera;
	std::unique_ptr < Bread::Graphics::IBuffer>      shaderConstantsBuffer;
	std::unique_ptr < Bread::Graphics::ISampler>     comparisonSampler;
	ShaderConstants shaderContexts;

	//モーションブラー
	bool isMotionBlur = false;
	std::unique_ptr<Bread::FrameWork::MotionBlur>  motionBlur;

	//ブルーム
	std::unique_ptr<Bread::FrameWork::Quad>        quad;
	std::unique_ptr<Bread::FrameWork::MSAAResolve> msaaResolve;
	std::unique_ptr<Bread::FrameWork::Bloom>       bloom;

	//スカイマップ
	std::unique_ptr<Bread::FrameWork::SkyMap>      skyMap;

	//トーンマップ
	std::unique_ptr<Bread::FrameWork::ToneMap>     toneMap;

	//スクリーンフィルター
	Bread::f32 bright   = 0.1f; //明度
	Bread::f32 contrast = 0.9f; //濃淡
	Bread::f32 saturate = 1.1f; //彩度
	Bread::Math::Color screenColor = Bread::Math::Color::White;

	//IBL
	std::unique_ptr<Bread::FrameWork::FrameBuffer> skyFrameBuffer;
	std::unique_ptr<Bread::FrameWork::IBL>         ibl;

	// フェード
	bool onFade = false;
	std::unique_ptr<Bread::Graphics::ITexture> fadeTexture;
	Bread::f32 roundFadeAlpha = 0.0f;

	//操作可
	bool onControl = false;

private://debug
	std::shared_ptr<Bread::FrameWork::GeometricPrimitive> primitive;
	std::shared_ptr<Bread::FrameWork::GeometricPrimitive> cylinderPrimitive;
	Bread::Math::Vector2 texSize;

	Bread::Math::Vector3 tempCameraFouce = Bread::Math::Vector3::Zero;
	Bread::f32 sphereLinearSpeed         = 0.0f;
	Bread::f32 distanceToFouceFromCamera = 0.0f;

	std::unique_ptr<Bread::FrameWork::IComputeShader> testComputeShader;
	std::unique_ptr<Bread::FrameWork::BitonicSort>    bitonicSort;
	std::unique_ptr<Bread::FrameWork::GPUParticle>    gpuParticle;

	Bread::f32 dis    = 10000.0f;
	Bread::f32 width  = 10000.0f;
	Bread::f32 height = 10000.0f;
	Bread::f32 nearZ  = 1.0f;
	Bread::f32 farZ   = 10000.0f;

	bool isHitCollision = true;
	bool enableMSAA     = false;
	bool bloomBlend     = false;
	bool active[20]     = { false };

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
	void GUI() override;

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