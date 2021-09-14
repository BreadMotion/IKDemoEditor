#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"

#include "scene.h"
#include "SceneSystem.h"

#include "FrameWork/Input/InputDevice.h"
#include "Math/BreadMath.h"
#include "FrameWork/Shader/SkyMapShader.h"
#include "FND/Util.h"
#include "FND/Lambda.h"

#include "../Source/Graphics//Texture/Win/DirectX11/TextureDX11.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"
#include "../Source/Graphics/GraphicsDevice/Win/DirectX11/GraphicsDeviceDX11.h"
#include "OS/StartUp.h"
#include "../Source/OS/Display/Win/DisplayWin.h"

#include "FrameWork/Input/InputDevice.h"
#include "FrameWork/Object/Object.h"
#include "FrameWork/Component/Transform.h"
#include "FrameWork/Component/VelocityMap.h"
#include "FrameWork/Component/CCDIK.h"

using namespace Bread;
using namespace Bread::Math;

int FrameWork::Transform::thisEntityNum = 0;
const char* FrameWork::Transform::SequencerItemTypeNames[] = { "Camera","Music", "ScreenEffect", "FadeIn", "Animation" };

void Frustum(float left, float right, float bottom, float top, float znear, float zfar, float* m16);
void Perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, float* m16);
void OrthoGraphic(const float l, float r, float b, const float t, float zn, const float zf, float* m16);
template<class... T>
int count_arguments(T... args) {
	return sizeof...(args);
}

void SceneGame::Construct(SceneSystem* sceneSystem)
{
	using FND::SharedInstance;
	using namespace Bread;
	using namespace Bread::FrameWork;
	using namespace Bread::Math;

	this->sceneSystem = sceneSystem;
	display           = sceneSystem->GetDisplay();

	std::shared_ptr<Graphics::IGraphicsDevice> wpGraphicsDevice;
	graphicsDevice = wpGraphicsDevice = SharedInstance<Graphics::IGraphicsDevice>::instance;

	//デバッグ用
	{
		Graphics::DeviceDX11* device = dynamic_cast<Graphics::DeviceDX11*>(wpGraphicsDevice->GetDevice());
		primitive         = std::make_shared<GeometricPrimitive>(device->GetD3DDevice(), 1);
		cylinderPrimitive = std::make_shared<GeometricPrimitive>(device->GetD3DDevice(), 2);
	}

	//共通データの初期化
	{
		basicShader     = FrameWork::BasicShader::Create();
		basicSkinShader = FrameWork::BasicSkinShader::Create();
		standardShader  = FrameWork::StandardShader::Create();
		pbrShader       = FrameWork::PBRShader::Create();
		pbrSkinShader   = FrameWork::PBRSkinShader::Create();

		basicShader    ->Initialize(wpGraphicsDevice.get());
		basicSkinShader->Initialize(wpGraphicsDevice.get());
		standardShader->Initialize(wpGraphicsDevice.get());
		pbrShader      ->Initialize(wpGraphicsDevice.get());
		pbrSkinShader  ->Initialize(wpGraphicsDevice.get());
	}

	//フレームバッファー
	{
		frameBuffer[0] = FrameWork::FrameBuffer::Create();
		frameBuffer[1] = FrameWork::FrameBuffer::Create();
		frameBuffer[2] = FrameWork::FrameBuffer::Create();
		frameBuffer[3] = FrameWork::FrameBuffer::Create();
		frameBuffer[4] = FrameWork::FrameBuffer::Create();
		frameBuffer[5] = FrameWork::FrameBuffer::Create();

		frameBuffer[0]->Initialize(wpGraphicsDevice.get(), display->GetWidth(), display->GetHeight(), enableMSAA, 8, Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Graphics::TextureFormatDx::R24G8_TYPELESS);
		frameBuffer[1]->Initialize(wpGraphicsDevice.get(), display->GetWidth(), display->GetHeight(), false, 1, Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Graphics::TextureFormatDx::R24G8_TYPELESS);
		frameBuffer[2]->Initialize(wpGraphicsDevice.get(), display->GetWidth(), display->GetHeight(), false, 1, Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Graphics::TextureFormatDx::UNKNOWN);
		frameBuffer[3]->Initialize(wpGraphicsDevice.get(), display->GetWidth(), display->GetHeight(), false, 1, Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Graphics::TextureFormatDx::R24G8_TYPELESS);
		frameBuffer[4]->Initialize(wpGraphicsDevice.get(), display->GetWidth(), display->GetHeight(), false, 1, Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Graphics::TextureFormatDx::R24G8_TYPELESS);
		frameBuffer[5]->Initialize(wpGraphicsDevice.get(), display->GetWidth(), display->GetHeight(), false, 1, Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Graphics::TextureFormatDx::R24G8_TYPELESS);
	}

	//ポストプロセス
	{
		postProcessingEffects = FrameWork::PostProcessingEffects::Create();
		postProcessingEffects->Initialize(wpGraphicsDevice.get());
	}

	//シャドウマップ
	{
		shadowMap = FrameWork::FrameBuffer::Create();
		shadowMap->Initialize(wpGraphicsDevice.get(), 1024 * 5, 1024 * 5, false, 1, Graphics::TextureFormatDx::UNKNOWN, Graphics::TextureFormatDx::R32_TYPELESS);

		voidPS = Graphics::IShader::Create();
		//voidPS->LoadPS(graphicsDevice->GetDevice(), "ShadowMapPS.cso");
		lightSpaceCamera = std::make_unique<Graphics::Camera>();

		// 定数バッファ作成
		{
			Graphics::PhoenixBufferDesc bufferDesc = {};
			FND::MemSet(&bufferDesc, 0, sizeof(bufferDesc));
			bufferDesc.usage                = Graphics::PhoenixUsage::Dynamic;
			bufferDesc.bindFlags            = static_cast<s32>(Graphics::PhoenixBindFlag::ConstantBuffer);
			bufferDesc.cpuAccessFlags       = static_cast<s32>(Graphics::PhoenixCPUAccessFlag::CPUAccessWrite);
			bufferDesc.miscFlags            = 0;
			bufferDesc.byteWidth            = sizeof(ShaderConstants);
			bufferDesc.structureByteStride  = 0;

			shaderConstantsBuffer = Graphics::IBuffer::Create();
			shaderConstantsBuffer->Initialize(wpGraphicsDevice.get()->GetDevice(), bufferDesc);
		}

		// サンプラー作成
		{
			comparisonSampler = Graphics::ISampler::Create();
			comparisonSampler->Initialize(wpGraphicsDevice.get()->GetDevice(), Graphics::SamplerState::LinearBorder, false, true);
		}
	}

	//モーションブラー
	{
		motionBlur = FrameWork::MotionBlur::Create();
		motionBlur->Initialize(wpGraphicsDevice.get(), display->GetWidth(), display->GetHeight());
	}

	//ブルーム
	{
		quad = FrameWork::Quad::Create();
		quad->Initialize(wpGraphicsDevice.get(), Graphics::SamplerState::PointBorder);

		msaaResolve = FrameWork::MSAAResolve::Create();
		msaaResolve->Initialize(wpGraphicsDevice.get());

		bloom = FrameWork::Bloom::Create();
		bloom->Initialize(wpGraphicsDevice.get(), display->GetWidth(), display->GetHeight());
	}

	//トーンマップ
	{
		toneMap = FrameWork::ToneMap::Create();
		toneMap->Initialize(wpGraphicsDevice.get(), display->GetWidth(), display->GetHeight());
	}

	//IBL
	{
		/*for (int i = 0; i < 6; ++i)
		{
			skyFrameBuffer[i] = Phoenix::FrameWork::FrameBuffer::Create();
			skyFrameBuffer[i]->Initialize(graphicsDevice, 64, 64, false, 1, Phoenix::Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Phoenix::Graphics::TextureFormatDx::R24G8_TYPELESS);
		}

		ibl = Phoenix::FrameWork::IBL::Create();
		ibl->Initialize(graphicsDevice);*/
	}

	//GPUパーティクル
	{
		//testComputeShader = FrameWork::TestComputeShader::Create();
	}

	//フェード
	{
		//fadeTexture = Graphics::ITexture::Create();
		//fadeTexture->Initialize(graphicsDevice->GetDevice(),
		//	"..\\Data\\Assets\\Texture\\Fade\\Fade01.png", Graphics::MaterialType::Diffuse, Color::White);
		//roundFadeAlpha = 0.0f;
	}

	// ディゾルブ
	/*{
		dissolveCB = Phoenix::Graphics::IBuffer::Create();
		{
			Phoenix::Graphics::PhoenixBufferDesc desc = {};
			Phoenix::FND::MemSet(&desc, 0, sizeof(desc));
			desc.usage = Phoenix::Graphics::PhoenixUsage::Default;
			desc.bindFlags = static_cast<Phoenix::s32>(Phoenix::Graphics::PhoenixBindFlag::ConstantBuffer);
			desc.cpuAccessFlags = 0;
			desc.miscFlags = 0;
			desc.byteWidth = sizeof(DissolveCB);
			desc.structureByteStride = 0;
			if (!dissolveCB->Initialize(graphicsDevice->GetDevice(), desc))
			{
				return;
			}
		}

		embeddedDissolvePixelShader = Phoenix::Graphics::IShader::Create();
		embeddedDissolvePixelShader->LoadPS
		(
			graphicsDevice->GetDevice(),
			"BasicMaskPS.cso"
		);

		pbrDissolvePixelShader = Phoenix::Graphics::IShader::Create();
		pbrDissolvePixelShader->LoadPS
		(
			graphicsDevice->GetDevice(),
			"PhysicallyBasedRenderingDissolvePS.cso"
		);

		bossRedTexture = Phoenix::Graphics::ITexture::Create();
		bossRedTexture->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\Boss\\Mutant_diffuse_Red1.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color::White);

		dissolveTexture = Phoenix::Graphics::ITexture::Create();
		dissolveTexture->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\Mask\\Dissolve\\dissolve_animation1_2.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color::White);

		emissiveTexture = Phoenix::Graphics::ITexture::Create();
		emissiveTexture->Initialize(graphicsDevice->GetDevice(), "..\\Data\\Assets\\Texture\\Mask\\Dissolve\\dissolve_edgecolor_gray.png", Phoenix::Graphics::MaterialType::Diffuse, Phoenix::Math::Color::White);

		dissolveThreshold = 1.15f;
		dissolveEmissiveWidth = 0.01f;
		isTurn = false;
	}*/

	//リソースマネージャー
	{
#if 1
		using Bread::FND::SharedInstance;
		using Bread::OS::ResourceManager;
		using Bread::OS::ResourceType;

		SharedInstance<ResourceManager>::instance = std::dynamic_pointer_cast<ResourceManager>(OS::IResourceManager::Create());
		SharedInstance<ResourceManager>::instance->SetGraphicDevice(wpGraphicsDevice.get());
		SharedInstance<ResourceManager>::instance->Initialize(nullptr);
#endif
	}

	//スカイマップ
	{
		skyMap = FrameWork::SkyMap::Create();
		skyMap->Initialize(wpGraphicsDevice,
			"..\\Data\\Assets\\Texture\\SkyMap\\AllSkyFree\\Epic_BlueSunset\\Epic_BlueSunset03.dds");
	}
}

void SceneGame::Initialize()
{
	std::shared_ptr<Graphics::IGraphicsDevice> wpGraphicsDevice = graphicsDevice.lock();
	if (!wpGraphicsDevice)
	{
		return;
	}
	//デバッグ用
	{
		texSize        = Vector2(256.0f, 256.0f);
		isHitCollision = true;
		enableMSAA = false;
		bloomBlend = true;


		for (int i = 0; i < 10; ++i)
		{
			active[i] = false;
		}
	}

	//アクターの生成＆初期化
	{
		using namespace Bread::FrameWork;
		actors.insert(std::make_pair(stageS,  StageActor::Create(wpGraphicsDevice)));
		actors.insert(std::make_pair(cameraS, CameraActor::Create()));

		actors[stageS]->SetID(stageS);
		actors[cameraS]->SetID(cameraS);
		actors[stageS]->Initialize();
		actors[cameraS]->Initialize();

		actors.insert(std::make_pair(playerS,
			PlayerActor::Create(wpGraphicsDevice,
				actors[cameraS]->GetComponent<Graphics::Camera>(),
				actors[stageS]->GetComponent<ModelObject>()
			)));

		actors[playerS]->SetID(playerS);
		actors[playerS]->Initialize();
	}

	//カメラの初期化
	{
		std::shared_ptr<Graphics::Camera> camera = actors[cameraS]->GetComponent<Graphics::Camera>();
		camera->SetEye({ 0.0f ,3000.0f ,1000.0f });
		camera->SetRotateX(0.5f);
		camera->SetRotateY(0.0f);
		camera->SetTargetPos({ 0.0f ,2600.0f ,0.0f }, Vector3::Zero);
		camera->SetTarget({ 0.0f ,2600.0f ,0.0f },    Vector3::Zero);
		camera->SetLookAt({ 0.0f ,3000.0f ,1000.0f }, { 10.0f ,2600.0f ,0.0f }, Vector3{ 0.0f,1.0f,0.0f });

		tempCameraFouce   = Vector3(0.0f, 0.0f, 0.0f);
		sphereLinearSpeed = 0.0f;
		distanceToFouceFromCamera = 0.0f;
	}

	currentShader = basicSkinShader;

	screenColor = Color(1.0f, 1.0f, 1.0f, 1.0f);

	// GPUパーティクル
	{
		//	testComputeShader->Initialize(graphicsDevice);
		//	bitonicSort->Initialize(graphicsDevice);
		//	gpuParticle->Initialize(graphicsDevice, "PlayerHitEffectCS.cso", "..\\Data\\Assets\\Texture\\Effect\\Fire\\FireOrigin.png", true); // particle
	}
}

void SceneGame::Update(const Bread::f32& elapsedTime)
{
	using namespace Bread::FrameWork;

	PlayerActor* actor  = dynamic_cast<PlayerActor*>(actors[playerS].get());
	Matrix       matrix = actor->GetComponent<Transform>()->GetWorldTransform();

	StageActor* stageActor  = dynamic_cast<StageActor*>(actors[stageS].get());
	Matrix      stageMatrix = stageActor->GetComponent<Transform>()->GetWorldTransform();

	std::weak_ptr<IKTargetActor> targetFootIK_L = actor->GetChildActorFromID<IKTargetActor>("leftFootTarget");
	Matrix         targetM_L   = targetFootIK_L.lock()->GetComponent<Transform>()->GetWorldTransform();

	std::shared_ptr<IKTargetActor> targetFootIK_R = actor->GetChildActorFromID<IKTargetActor>("rightFootTarget");
	Matrix         targetM_R      = targetFootIK_R->GetComponent<Transform>()->GetWorldTransform();

	//事前更新
	for (auto& act : actors)
	{
		act.second->PreUpdate(elapsedTime);
	}

	float objMatrixAry1[16] =
	{ matrix._11, matrix._12, matrix._13, matrix._14,
	  matrix._21, matrix._22, matrix._23, matrix._24,
	  matrix._31, matrix._32, matrix._33, matrix._34,
	  matrix._41, matrix._42, matrix._43, matrix._44
	};
	float objMatrixAry2[16] =
	{   stageMatrix._11, stageMatrix._12, stageMatrix._13, stageMatrix._14,
		 stageMatrix._21, stageMatrix._22, stageMatrix._23, stageMatrix._24,
		 stageMatrix._31, stageMatrix._32, stageMatrix._33, stageMatrix._34,
		 stageMatrix._41, stageMatrix._42, stageMatrix._43, stageMatrix._44
	};
	float targetMatrixAry3[16] =
	{
		 targetM_L._11, targetM_L._12, targetM_L._13, targetM_L._14,
		 targetM_L._21, targetM_L._22, targetM_L._23, targetM_L._24,
		 targetM_L._31, targetM_L._32, targetM_L._33, targetM_L._34,
		 targetM_L._41, targetM_L._42, targetM_L._43, targetM_L._44
	};
	float targetMatrixAry4[16] =
	{
		 targetM_R._11, targetM_R._12, targetM_R._13, targetM_R._14,
		 targetM_R._21, targetM_R._22, targetM_R._23, targetM_R._24,
		 targetM_R._31, targetM_R._32, targetM_R._33, targetM_R._34,
		 targetM_R._41, targetM_R._42, targetM_R._43, targetM_R._44
	};

	actor->SetObjMatrix(objMatrixAry1);
	stageActor->SetObjMatrix(objMatrixAry2);
	targetFootIK_L.lock()->SetObjMatrix(targetMatrixAry3);
	targetFootIK_R->SetObjMatrix(targetMatrixAry4);

	SetupGUI();
	GUI();

	if (selectAct)
	{
		if (selectAct.get() == actor)
		{
			ImGuizmoUpdate(objMatrixAry1);
		}
		else if (selectAct.get() == stageActor)
		{
			ImGuizmoUpdate(objMatrixAry2);
		}
		else if (selectAct == targetFootIK_L.lock())
		{
			ImGuizmoUpdate(targetMatrixAry3);
		}
		else if (selectAct == targetFootIK_R)
		{
			ImGuizmoUpdate(targetMatrixAry4);
		}
	}

	//更新
	for (auto& act : actors)
	{
		act.second->Update(elapsedTime);
	}

	//事後更新
	for (auto& act : actors)
	{
		act.second->NextUpdate(elapsedTime);
	}

	UpdateLightDirection();
}

void SceneGame::Draw(const Bread::f32& elapsedTime)
{
	using namespace Bread::FrameWork;
	std::shared_ptr<Graphics::IGraphicsDevice> wpGraphicsDevice = graphicsDevice.lock();
	if (!wpGraphicsDevice)
	{
		return;
	}

	Graphics::IContext* context = wpGraphicsDevice.get()->GetContext();

	Graphics::Viewport* v = new Graphics::Viewport();
	context->GetViewports(1, &v);
	float aspectRatio = v->width / v->height;
	FND::SafeDelete(v);

	std::shared_ptr<ModelObject> playerObject = actors[playerS]->GetComponent<ModelObject>();
	const Matrix    playerMatrix   = actors[playerS]->GetComponent<Transform>()->GetWorldTransform();
	const Vector3   playerLocation = GetLocation(playerMatrix);

	std::shared_ptr<ModelObject> stageObject = actors[stageS]->GetComponent<ModelObject>();
	const Matrix    stageMatrix  = actors[stageS]->GetComponent<Transform>()->GetWorldTransform();
	const Vector3  stageLocation = GetLocation(stageMatrix);


	std::shared_ptr<Actor> targetIK = actors[playerS]->GetChildActor<IKTargetActor>();
	std::shared_ptr<Transform> tIKTransform  = targetIK->GetComponent<Transform>();
	const Vector3  tLocation     = GetLocation(tIKTransform->GetWorldTransform());
	static f32     radius        = 10.0f;
	static f32     alpha         = 1.0f;

	std::shared_ptr<Graphics::Camera> camera = actors[cameraS]->GetComponent<Graphics::Camera>();

	//	ibl->Clear(graphicsDevice);
	//	ibl->Activate(graphicsDevice);
	//	{
	//		// Draw skymap.
	//		{
	//			Phoenix::FrameWork::LightState* light = static_cast<Phoenix::FrameWork::PBRShader*>(pbrShader)->GetLight();
	//			Phoenix::Math::Color color = { 1.0f, 1.0f, 1.0f, 1.0f };
	//			float skyDimension = 50000;
	//			// ワールド行列を作成
	//			Phoenix::Math::Matrix skyWorldM;
	//			{
	//				Phoenix::Math::Vector3 scale = { skyDimension, skyDimension, skyDimension };
	//				Phoenix::Math::Vector3 rotate = { 0.0f, 90.0f * 0.01745f, 0.0f };
	//				Phoenix::Math::Vector3 translate = { 0.0f, 0.0f, 0.0f };
	//
	//				Phoenix::Math::Matrix S, R, T;
	//				S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
	//				R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
	//				T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);
	//
	//				skyWorldM = S * R * T;
	//			}
	//			skyMap->Draw(graphicsDevice, skyWorldM, *camera, light->direction, color);
	//		}
	//
	//		// Draw stage.
	//		{
	//			// ワールド行列を作成
	//			Phoenix::Math::Matrix W;
	//			{
	//				Phoenix::Math::Vector3 scale = { 40.0f, 40.0f, 40.0f };
	//				Phoenix::Math::Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	//				Phoenix::Math::Vector3 translate = { 0.0f, 0.0f, 0.0f };
	//
	//				Phoenix::Math::Matrix S, R, T;
	//				S = Phoenix::Math::MatrixScaling(scale.x, scale.y, scale.z);
	//				R = Phoenix::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
	//				T = Phoenix::Math::MatrixTranslation(translate.x, translate.y, translate.z);
	//
	//				W = S * R * T;
	//			}
	//#if 1
	//			basicShader->Begin(graphicsDevice, *camera);
	//			basicShader->Draw(graphicsDevice, W, stageModel);
	//			basicShader->End(graphicsDevice);
	//#else
	//			standardShader->Begin(graphicsDevice, camera);
	//			standardShader->Draw(graphicsDevice, W, stageModel);
	//			standardShader->End(graphicsDevice);
	//#endif
	//		}
	//	}
	//	ibl->Deactivate(graphicsDevice);

#if 1
	// Generate ShadowMap
	{
		shadowMap->Clear(wpGraphicsDevice.get(), 0, 1.0f, 1.0f, 1.0f, 1.0f);
		shadowMap->Activate(wpGraphicsDevice.get());
		{
			float distance = dis;
			LightState* light = static_cast<PBRShader*>(pbrShader.get())->GetSunLight();
			Vector4 L   = Vector4Normalize(-light->direction);
			Vector4 P   = Vector4(playerLocation.x, playerLocation.y, playerLocation.z, 1.0f);
			Vector4 Eye = P - distance * L;

			lightSpaceCamera->SetEye(Vector3(Eye.x, Eye.y, Eye.z));
			lightSpaceCamera->SetFocus(Vector3(P.x, P.y, P.z));

			Vector3 up, right;
			{
				right = Vector3Cross(Vector3(L.x, L.y, L.z), Vector3::OneY);
				right = Vector3Normalize(right);

				up = Vector3Cross(right, Vector3(L.x, L.y, L.z));
				up = Vector3Normalize(up);

				right = Vector3Cross(Vector3(L.x, L.y, L.z), Vector3::OneY);
				up    = Vector3Cross(right, Vector3(L.x, L.y, L.z));
			}

			Matrix projection = MatrixOrtho(width, height /*/ aspectRatio*/, nearZ, farZ);
			lightSpaceCamera->SetProjection(projection);
			lightSpaceCamera->SetLookAt(lightSpaceCamera->GetEye(), lightSpaceCamera->GetFocus(), up);

			// Draw player
			{
				if (currentShader)
				{
					currentShader->Begin(wpGraphicsDevice.get(), *lightSpaceCamera);
					voidPS->ActivatePS(wpGraphicsDevice.get()->GetDevice());
					{
						currentShader->Draw(wpGraphicsDevice.get(), playerMatrix, playerObject.get());
					}
					voidPS->DeactivatePS(wpGraphicsDevice.get()->GetDevice());
					currentShader->End(wpGraphicsDevice.get());
				}
			}

			// Draw stage.
			{
				basicShader->Begin(wpGraphicsDevice.get(), *lightSpaceCamera);
				voidPS->ActivatePS(wpGraphicsDevice.get()->GetDevice());
				{
					basicShader->Draw(wpGraphicsDevice.get(), stageMatrix, stageObject.get());
				}
				voidPS->DeactivatePS(wpGraphicsDevice.get()->GetDevice());
				basicShader->End(wpGraphicsDevice.get());
			}
		}
		shadowMap->Deactivate(wpGraphicsDevice.get());
	}

	// Generate VelocityMap
	{
		motionBlur->ActivateVelocity(wpGraphicsDevice.get());
		{
			// データセット
			{
				motionBlur->velocityConstants.screenWidth  = static_cast<f32>(display->GetWidth());
				motionBlur->velocityConstants.screenHeight = static_cast<f32>(display->GetHeight());
				motionBlur->velocityConstants.frameRate = elapsedTime / 60.0f;
			}

			// Draw player and enemies.
			{
				if (currentShader)
				{
					currentShader->Begin(wpGraphicsDevice.get(), *camera);
					motionBlur->ActivateVelocityPS(wpGraphicsDevice.get());
					{
						currentShader->Draw(wpGraphicsDevice.get(), playerMatrix, playerObject.get());
					}
					motionBlur->DeactivateVelocityPS(wpGraphicsDevice.get());
					currentShader->End(wpGraphicsDevice.get());
				}
			}

			// Draw stage.
			{
				basicShader->Begin(wpGraphicsDevice.get(), *camera);
				motionBlur->ActivateVelocityPS(wpGraphicsDevice.get());
				{
					basicShader->Draw(wpGraphicsDevice.get(), stageMatrix, stageObject.get());
				}
				motionBlur->DeactivateVelocityPS(wpGraphicsDevice.get());
				basicShader->End(wpGraphicsDevice.get());
			}
		}
		motionBlur->DeactivateVelocity(wpGraphicsDevice.get());
	}

	// Work No_0 framebuffer.
	{
		frameBuffer[0]->Clear(wpGraphicsDevice.get(), 0, 0.5f, 0.5f, 0.5f, 1.0f);
		frameBuffer[0]->Activate(wpGraphicsDevice.get());
		{
			// Set Shadow Data.
			{
				shaderContexts.lightViewProjection = MatrixTranspose(/*Phoenix::Math::MatrixInverse*/(lightSpaceCamera->GetView() * lightSpaceCamera->GetProjection()));
				Graphics::PhoenixMap map = Graphics::PhoenixMap::WriteDiscard;
				Graphics::PhoenixMappedSubresource mapedBuffer;
				{
					context->Map(shaderConstantsBuffer.get(), 0, map, 0, &mapedBuffer);
					FND::MemCpy(mapedBuffer.data, &shaderContexts, sizeof(ShaderConstants));
					context->Unmap(shaderConstantsBuffer.get(), 0);
				}
				Graphics::IBuffer* psCBuffer[] =
				{
					shaderConstantsBuffer.get()
				};
				context->SetConstantBuffers(Graphics::ShaderType::Pixel, 3, FND::ArraySize(psCBuffer), psCBuffer);

				Graphics::ISampler* samplers[] =
				{
					comparisonSampler.get()
				};
				context->SetSamplers(Graphics::ShaderType::Pixel, 3, 1, samplers);

				Graphics::ITexture* texture[] = { shadowMap->GetDepthStencilSurface()->GetTexture() };
				wpGraphicsDevice.get()->GetContext()->SetShaderResources(Graphics::ShaderType::Pixel, 8, 1, texture);
			}

			// Draw skymap.
			{
				FrameWork::LightState* light = static_cast<PBRShader*>(pbrShader.get())->GetLight();
				Math::Color color = { 1.0f, 1.0f, 1.0f, 1.0f };
				float skyDimension = 20000;
				// ワールド行列を作成
				Matrix skyWorldM;
				{
					Vector3 scale       = { skyDimension, skyDimension, skyDimension };
					Vector3 rotate     = { 0.0f, 90.0f * 0.01745f, 0.0f };
					Vector3 translate = { 0.0f, 0.0f, 0.0f };

					Matrix S, R, T;
					S = MatrixScaling(scale.x, scale.y, scale.z);
					R = MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
					T = MatrixTranslation(translate.x, translate.y, translate.z);

					skyWorldM = S * R * T;
				}
				skyMap->Draw(wpGraphicsDevice.get(), skyWorldM, *camera, light->direction, color);
			}

			// Draw stage.
			{
#if 1
				Graphics::ContextDX11* contextDX11 = static_cast<Graphics::ContextDX11*>(context);
				f32 blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
				context->SetBlend(contextDX11->GetBlendState(Graphics::BlendState::AlphaToCoverageEnable), blendFactor, 0xFFFFFFFF);
				{
					basicShader->Begin(wpGraphicsDevice.get(), *camera);
					basicShader->Draw(wpGraphicsDevice.get(), stageMatrix, stageObject.get());
					basicShader->End(wpGraphicsDevice.get());
				}
				context->SetBlend(contextDX11->GetBlendState(Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
#elif 0
				standardShader->Begin(graphicsDevice, camera);
				standardShader->Draw(graphicsDevice, W, bossStageModel);
				standardShader->End(graphicsDevice);
#else
				pbrSkinShader->Begin(graphicsDevice, *camera);
				pbrSkinShader->Draw(graphicsDevice, W, bossStageModel);
				pbrSkinShader->End(graphicsDevice);
#endif
			}

			// Draw player and boss.
			{
#if 0
				// メッシュ描画
#if 1
				basicSkinShader->Begin(graphicsDevice, *camera);
				//basicSkinShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
				// エフェクト描画
				{
					//commonData->renderer->BeginRendering();
					//commonData->manager->Draw();
					//commonData->renderer->EndRendering();
				}
				//basicSkinShader->Draw(graphicsDevice, player->GetWorldMatrix(), player->GetModel());
				basicSkinShader->End(graphicsDevice);
#else
				standardShader->Begin(graphicsDevice, camera);
				standardShader->Draw(graphicsDevice, player->GetWorldMatrix(), player->GetModel());
				standardShader->End(graphicsDevice);
#endif

#if 1
				//basicSkinShader->Begin(graphicsDevice, camera);
				//basicSkinShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
				//basicSkinShader->End(graphicsDevice);
#else
				standardShader->Begin(graphicsDevice, camera);
				standardShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
				standardShader->End(graphicsDevice);
#endif

				pbrShader->Begin(graphicsDevice, *camera);
				pbrShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
				pbrShader->Draw(graphicsDevice, player->GetWorldMatrix(), player->GetModel());
				pbrShader->End(graphicsDevice);
#else
				if (currentShader)
				{
					/*
					currentShader->Begin(graphicsDevice, *camera);
					{
						pbrDissolvePixelShader->Activate(graphicsDevice->GetDevice());
						{
							// ピクセルシェーダー用バッファ更新
							{
								DissolveCB cb = {};
								{
									cb.dissolveThreshold = dissolveThreshold;
									cb.dissolveEmissiveWidth = dissolveEmissiveWidth;
									cb.dummy[0] = 0.0f;
									cb.dummy[1] = 0.0f;
								}
								Phoenix::Graphics::IBuffer* psCBuffer[] =
								{
									dissolveCB.get()
								};
								context->UpdateSubresource(dissolveCB.get(), 0, 0, &cb, 0, 0);
								context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Pixel, 2, Phoenix::FND::ArraySize(psCBuffer), psCBuffer);
							}

							// ピクセルシェーダー用テクスチャ更新
							{
								Phoenix::Graphics::ITexture* texture[] =
								{
									bossRedTexture.get(),
									dissolveTexture.get(),
									emissiveTexture.get()
								};
								context->SetShaderResources(Phoenix::Graphics::ShaderType::Pixel, 6, 3, texture);
							}

							// Draw.
							{
								currentShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
							}

							// ピクセルシェーダー用テクスチャ更新
							{
								Phoenix::Graphics::ITexture* texture[] =
								{
									nullptr,
									nullptr,
									nullptr
								};
								context->SetShaderResources(Phoenix::Graphics::ShaderType::Pixel, 6, 3, texture);
							}

							// ピクセルシェーダー用バッファ更新
							{
								Phoenix::Graphics::IBuffer* psCBuffer[] =
								{
									nullptr
								};
								context->SetConstantBuffers(Phoenix::Graphics::ShaderType::Pixel, 2, Phoenix::FND::ArraySize(psCBuffer), psCBuffer);
							}
						}
						pbrDissolvePixelShader->Deactivate(graphicsDevice->GetDevice());
					}
					currentShader->End(graphicsDevice);
					*/

#if 0
					// Draw Effect.
					{
						Phoenix::Graphics::ContextDX11* contextDX11 = static_cast<Phoenix::Graphics::ContextDX11*>(context);
						Phoenix::f32 blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
						context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::AlphaToCoverageEnable), blendFactor, 0xFFFFFFFF);
						{
							gpuParticle->Draw(graphicsDevice, *camera);
							playerHitParticle->Draw(graphicsDevice, *camera);
							bossHitParticle->Draw(graphicsDevice, *camera);
							petalParticle->Draw(graphicsDevice, *camera);
							soilParticle->Draw(graphicsDevice, *camera);
							bossAuraParticle->Draw(graphicsDevice, *camera);
							playerStrongAttackParticle->Draw(graphicsDevice, *camera);
							//playerMeshParticle->Draw(graphicsDevice, *camera);
							/*for (Phoenix::s32 i = 0; i < 3; ++i)
							{
								dusterParticle[i]->Draw(graphicsDevice, *camera);
							}*/
						}
						context->SetBlend(contextDX11->GetBlendState(Phoenix::Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
					}
#endif

					currentShader->Begin(wpGraphicsDevice.get(), *camera);
					currentShader->Draw(wpGraphicsDevice.get(), playerMatrix, playerObject.get());
					currentShader->End(wpGraphicsDevice.get());
				}
#endif
			}

			// Draw collision primitive.
			if (isHitCollision)
			{
				Graphics::DeviceDX11* device = static_cast<Graphics::DeviceDX11*>(wpGraphicsDevice.get()->GetDevice());

				Graphics::ContextDX11* contextDX11 = static_cast<Graphics::ContextDX11*>(context);
				context->SetBlend(contextDX11->GetBlendState(Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);

				//アクター側で描画
				for (auto& act : actors)
				{
					act.second->Draw(elapsedTime);
				}

				context->SetBlend(contextDX11->GetBlendState(Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
			}
		}
		frameBuffer[0]->Deactivate(wpGraphicsDevice.get());
	}

	// Generate Bloom Texture.
	u32 resolvedFramebuffer = 0;
	{
		if (enableMSAA)
		{
			resolvedFramebuffer = 1;

			msaaResolve->Resolve(wpGraphicsDevice.get(), frameBuffer[0].get(), frameBuffer[resolvedFramebuffer].get());

			bloom->Generate(wpGraphicsDevice.get(), frameBuffer[resolvedFramebuffer]->GetRenderTargetSurface()->GetTexture(), false);

			frameBuffer[resolvedFramebuffer]->Clear(wpGraphicsDevice.get(), 0, 0.5f, 0.5f, 0.5f, 1.0f);
			frameBuffer[resolvedFramebuffer]->Activate(wpGraphicsDevice.get());
			{
				quad->Draw(wpGraphicsDevice.get(), frameBuffer[0]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f, static_cast<f32>(display->GetWidth()), static_cast<f32>(display->GetHeight()));
				bloom->Draw(wpGraphicsDevice.get());
			}
			frameBuffer[resolvedFramebuffer]->Deactivate(wpGraphicsDevice.get());
		}
		else
		{
			bloom->Generate(wpGraphicsDevice.get(), frameBuffer[resolvedFramebuffer]->GetRenderTargetSurface()->GetTexture(), false);

			resolvedFramebuffer = 1;

			frameBuffer[resolvedFramebuffer]->Clear(wpGraphicsDevice.get(), 0, 0.5f, 0.5f, 0.5f, 1.0f);
			frameBuffer[resolvedFramebuffer]->Activate(wpGraphicsDevice.get());
			{
				quad->Draw(wpGraphicsDevice.get(), frameBuffer[0]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f, static_cast<f32>(display->GetWidth()), static_cast<f32>(display->GetHeight()));
				bloom->Draw(wpGraphicsDevice.get());
			}
			frameBuffer[resolvedFramebuffer]->Deactivate(wpGraphicsDevice.get());
		}
	}

	frameBuffer[3]->Clear(wpGraphicsDevice.get(), 0, 0.5f, 0.5f, 0.5f, 1.0f);
	frameBuffer[3]->Activate(wpGraphicsDevice.get());
	{
		// Blend Bloom.
		if (bloomBlend)
		{
			/*resolvedFramebuffer = 2;
			frameBuffer[resolvedFramebuffer]->Activate(graphicsDevice);
			{
				bloom->Blend(graphicsDevice, frameBuffer[0]->GetRenderTargetSurface()->GetTexture(), frameBuffer[1]->GetRenderTargetSurface()->GetTexture());
			}
			frameBuffer[resolvedFramebuffer]->Deactivate(graphicsDevice);*/

			quad->Draw(wpGraphicsDevice.get(), frameBuffer[resolvedFramebuffer]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f, static_cast<f32>(display->GetWidth()), static_cast<f32>(display->GetHeight()));

			//toneMap->Draw(graphicsDevice, frameBuffer[resolvedFramebuffer]->renderTargerSurface[0]->GetTexture(), elapsedTime);
		}
		else
		{
			quad->Draw(wpGraphicsDevice.get(), frameBuffer[0]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f, static_cast<f32>(display->GetWidth()), static_cast<f32>(display->GetHeight()));
		}
#endif
	}
	frameBuffer[3]->Deactivate(wpGraphicsDevice.get());

	// Screen Filter
	frameBuffer[4]->Clear(wpGraphicsDevice.get(), 0, 0.5f, 0.5f, 0.5f, 1.0f);
	frameBuffer[4]->Activate(wpGraphicsDevice.get());
	{
		quad->SetBright(bright);
		quad->SetContrast(contrast);
		quad->SetSaturate(saturate);
		quad->SetScreenColor(screenColor);

		quad->Draw(wpGraphicsDevice.get(), frameBuffer[3]->renderTargerSurface[0]->GetTexture(), Vector2(0.0f, 0.0f), Vector2(1920.0f, 1080.0f), Vector2(0.0f, 0.0f), Vector2(1920.0f, 1080.0f), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, true, true, true, true, true, true);
	}
	frameBuffer[4]->Deactivate(wpGraphicsDevice.get());

	// Motion Blur
	resolvedFramebuffer = 4;
	if (isMotionBlur)
	{
		resolvedFramebuffer = 5;
		frameBuffer[resolvedFramebuffer]->Clear(wpGraphicsDevice.get(), 0, 0.5f, 0.5f, 0.5f, 1.0f);
		frameBuffer[resolvedFramebuffer]->Activate(wpGraphicsDevice.get());
		{
			motionBlur->blurConstants.loop = 5;
			motionBlur->blurConstants.div = 1.0f / static_cast<f32>(motionBlur->blurConstants.loop + 1);

			motionBlur->Draw(wpGraphicsDevice.get(), frameBuffer[4]->renderTargerSurface[0]->GetTexture(), *camera, true);
		}
		frameBuffer[resolvedFramebuffer]->Deactivate(wpGraphicsDevice.get());
	}

	// Final Draw
	{
		quad->Draw(wpGraphicsDevice.get(), frameBuffer[3]->renderTargerSurface[0]->GetTexture(), Vector2(0.0f, 0.0f), Vector2(1920.0f, 1080.0f), Vector2(0.0f, 0.0f), Vector2(1920.0f, 1080.0f));
	}

	f32 width  = static_cast<f32>(display->GetWidth());
	f32 height = static_cast<f32>(display->GetHeight());

	// Draw frameBuffer Texture.
	{
		if (active[0])
		{
			if (frameBuffer[0])
				quad->Draw(wpGraphicsDevice.get(), frameBuffer[0]->renderTargerSurface[0]->GetTexture(), texSize.x * 0, texSize.y * 0, texSize.x, texSize.y);
		}
		if (active[1])
		{
			if (frameBuffer[0])
				quad->Draw(wpGraphicsDevice.get(), frameBuffer[0]->depthStencilSurface->GetTexture(), texSize.x * 1, texSize.y * 0, texSize.x, texSize.y);
		}
		if (active[2])
		{
			if (frameBuffer[1])
				quad->Draw(wpGraphicsDevice.get(), frameBuffer[1]->renderTargerSurface[0]->GetTexture(), texSize.x * 2, texSize.y * 0, texSize.x, texSize.y);
		}
		if (active[3])
		{
			if (frameBuffer[1])
				quad->Draw(wpGraphicsDevice.get(), frameBuffer[1]->depthStencilSurface->GetTexture(), texSize.x * 3, texSize.y * 0, texSize.x, texSize.y);
		}
		if (active[4])
		{
			if (frameBuffer[2])
				quad->Draw(wpGraphicsDevice.get(), frameBuffer[2]->renderTargerSurface[0]->GetTexture(), texSize.x * 4, texSize.y * 0, texSize.x, texSize.y);
		}
		if (active[5])
		{
		    //	if (frameBuffer[2])
			//quad->Draw(graphicsDevice, frameBuffer[2]->depthStencilSurface->GetTexture(), texSize.x * 5, texSize.y * 0, texSize.x, texSize.y);
		}
		if (active[6])
		{
			if (frameBuffer[3])
				quad->Draw(wpGraphicsDevice.get(), frameBuffer[3]->renderTargerSurface[0]->GetTexture(), texSize.x * 0, texSize.y * 1, texSize.x, texSize.y);
		}
		if (active[7])
		{
			if (frameBuffer[3])
				quad->Draw(wpGraphicsDevice.get(), frameBuffer[3]->depthStencilSurface->GetTexture(), texSize.x * 1, texSize.y * 1, texSize.x, texSize.y);
		}
		if (active[8])
		{
			if (frameBuffer[4])
				quad->Draw(wpGraphicsDevice.get(), frameBuffer[4]->renderTargerSurface[0]->GetTexture(), texSize.x * 2, texSize.y * 1, texSize.x, texSize.y);
		}
		if (active[9])
		{
			if (frameBuffer[4])
				quad->Draw(wpGraphicsDevice.get(), frameBuffer[4]->depthStencilSurface->GetTexture(), texSize.x * 3, texSize.y * 1, texSize.x, texSize.y);
		}
		if (active[10])
		{
			if (frameBuffer[5])
				quad->Draw(wpGraphicsDevice.get(), frameBuffer[5]->renderTargerSurface[0]->GetTexture(), texSize.x * 4, texSize.y * 1, texSize.x, texSize.y);
		}
		if (active[11])
		{
			if (frameBuffer[5])
				quad->Draw(wpGraphicsDevice.get(), frameBuffer[5]->depthStencilSurface->GetTexture(), texSize.x * 5, texSize.y * 2, texSize.x, texSize.y);
		}
		if (active[12])
		{
			if (motionBlur)
				quad->Draw(wpGraphicsDevice.get(), motionBlur->GetVelocityFrameBuffer()->renderTargerSurface[0]->GetTexture(), texSize.x * 0, texSize.y * 2, texSize.x, texSize.y);
		}
		if (active[13])
		{
			//if(motionBlur)
			//quad->Draw(graphicsDevice, motionBlur->GetVelocityFrameBuffer()->depthStencilSurface->GetTexture(), texSize.x * 1, texSize.y * 2, texSize.x, texSize.y);
		}
		if (active[14])
		{
			if (shadowMap)
				quad->Draw(wpGraphicsDevice.get(), shadowMap->GetDepthStencilSurface()->GetTexture(), texSize.x * 2, texSize.y * 2, texSize.x, texSize.y);
		}
	}

#pragma region MyRegion

	using namespace ImGui;
	ImGui::SetNextWindowPos(ImVec2(350, 25));
	ImGui::SetNextWindowSize(ImVec2(150, 500));

	ImGui::Begin(u8"タイトル");
	if (Button("frameBuffer : 0 R"))active[0] = !active[0];
	if (Button("frameBuffer : 0 D"))active[1] = !active[1];
	if (Button("frameBuffer : 1 R"))active[2] = !active[2];
	if (Button("frameBuffer : 1 D"))active[3] = !active[3];
	if (Button("frameBuffer : 2 R"))active[4] = !active[4];
	if (Button("frameBuffer : 2 D"))active[5] = !active[5];
	if (Button("frameBuffer : 3 R"))active[6] = !active[4];
	if (Button("frameBuffer : 3 D"))active[7] = !active[5];
	if (Button("frameBuffer : 4 R"))active[8] = !active[4];
	if (Button("frameBuffer : 4 R"))active[9] = !active[4];
	if (Button("frameBuffer : 5 D"))active[10] = !active[10];
	if (Button("frameBuffer : 5 D"))active[11] = !active[11];
	if(Button("velocityMap      R"))active[12] = !active[12];
	if (Button("velocityMap     D"))active[13] = !active[13];
	if (Button("Depthstencil    D"))active[14] = !active[14];
	//if (TreeNode("shadowMap"))
	//{
	//	shadowMap->GUI();
	//	TreePop();
	//}
	//if (TreeNode("frameBuffer : 0 "))
	//{
	//	if (frameBuffer[0])
	//	{
	//		frameBuffer[0]->GUI();
	//	}
	//	TreePop();
	//}
	//if (TreeNode("frameBuffer : 1 "))
	//{
	//	if (frameBuffer[1])
	//	{
	//		frameBuffer[1]->GUI();
	//	}
	//	TreePop();
	//}
	//if (TreeNode("frameBuffer : 2 "))
	//{
	//	if (frameBuffer[2])
	//	{
	//		frameBuffer[2]->GUI();
	//	}
	//	TreePop();
	//}
	//if (TreeNode("frameBuffer : 3 "))
	//{
	//	if (frameBuffer[3])
	//	{
	//		frameBuffer[3]->GUI();
	//	}
	//	TreePop();
	//}
	//if (TreeNode("frameBuffer : 4 "))
	//{
	//	if (frameBuffer[4])
	//	{
	//		frameBuffer[4]->GUI();
	//	}
	//	TreePop();
	//}
	//if (TreeNode("frameBuffer : 5 "))
	//{
	//	if (frameBuffer[5])
	//	{
	//		frameBuffer[5]->GUI();
	//	}
	//	TreePop();
	//}
	ImGui::End();

#pragma endregion

	// 前フレームとしてカメラ情報を保存
	{
		context->UpdateConstantBufferPrevScene(camera->GetView(), camera->GetProjection());
	}
}

void SceneGame::SetupGUI()
{
	using namespace ImGui;

	ImGuiIO& io = ImGui::GetIO();

	Bread::FrameWork::Actor* cameraActor = actors[cameraS].get();
	std::shared_ptr<Graphics::Camera> camera = cameraActor->GetComponent<Graphics::Camera>();
	f32    fov = camera->GetFovY();
	Matrix pro = camera->GetProjection();
	float  viewWidth = 10.f; // for orthographic

	float cameraProjection[16] =
	{ pro._11, pro._12, pro._13, pro._14,
	  pro._21, pro._22, pro._23, pro._24,
	  pro._31, pro._32, pro._33, pro._34,
	  pro._41, pro._42, pro._43, pro._44
	};

	{//setUP viewtype
		Graphics::ViewType viewType = camera->GetViewType();
		if (viewType == Graphics::ViewType::Perspective)
		{
			Perspective(fov, io.DisplaySize.x / io.DisplaySize.y, 0.1f, 100.f, cameraProjection);
			ImGuizmo::SetOrthographic(false);
		}
		else if ((viewType == Graphics::ViewType::Orthographic))
		{
			float viewHeight = viewWidth * io.DisplaySize.y / io.DisplaySize.x;
			OrthoGraphic(-viewWidth, viewWidth, -viewHeight, viewHeight, 1000.f, -1000.f, cameraProjection);
			ImGuizmo::SetOrthographic(true);
		}
	}
}

void SceneGame::ImGuizmoUpdate(float* ary)
{
	using namespace Bread::FrameWork;
	ImGuiIO& io = ImGui::GetIO();

	Bread::FrameWork::Actor* cameraActor = actors[cameraS].get();
	std::shared_ptr<Graphics::Camera> camera = cameraActor->GetComponent<Graphics::Camera>();

	Bread::Math::Matrix m = camera->GetView();
	float cameraView[16] =
	{ m._11, m._12, m._13, m._14,
	  m._21, m._22, m._23, m._24,
	  m._31, m._32, m._33, m._34,
	  m._41, m._42, m._43, m._44 };

	Actor* actor = actors[playerS].get();
	std::shared_ptr<Transform> transform = actor->GetComponent<Transform>();

	static bool editTransform = true;

	const Matrix camMat = camera->GetView();
	const Matrix camPro = camera->GetProjection();

	float Projection[16] =
	{ camPro._11, camPro._12,camPro._13, camPro._14,
	  camPro._21, camPro._22,camPro._23, camPro._24,
	  camPro._31, camPro._32,camPro._33, camPro._34,
	  camPro._41, camPro._42,camPro._43, camPro._44 };

	float view[16] =
	{ camMat._11, camMat._12, camMat._13, camMat._14,
	  camMat._21, camMat._22, camMat._23, camMat._24,
	  camMat._31, camMat._32, camMat._33, camMat._34,
	  camMat._41, camMat._42, camMat._43, camMat._44 };

	ImGuizmo::BeginFrame();
	ImGui::SetNextWindowPos(ImVec2(350, std::fabsf(256 - display->GetHeight())));
	ImGui::SetNextWindowSize(ImVec2(display->GetWidth() - 350, 256));

	ImGui::Begin(u8"シーケンサー");
	static const float identityMatrix[16] =
	{ 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f };

	ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
	//ImGuizmo::DrawGrid(cameraView, cameraProjection, identityMatrix, 100.f);
	ImGui::Separator();

	ImGuizmo::SetID(0);
	transform->EditTransform(camera.get(), view, Projection, ary, true);

	transform->SequenceEditorGUI();
	ImGui::End();

	const float disatnce = camera->GetDistanceFromLookAt();
	ImGuizmo::ViewManipulate(cameraView, disatnce, ImVec2(io.DisplaySize.x - 128, 0), ImVec2(128, 128), 0x10101010);
}

void SceneGame::GUI()
{
	using namespace ImGui;
	using namespace Bread;
	using namespace Bread::FrameWork;
	ImGuiIO& io = ImGui::GetIO();

	Bread::FrameWork::Actor* cameraActor = actors[cameraS].get();
	std::shared_ptr<Graphics::Camera> camera = cameraActor->GetComponent<Graphics::Camera>();

	static bool watchWindow             = false;
	static bool outlineWindow           = true;
	static bool componentWindow         = true;
	static bool controllRasterizeWindow = true;

	std::shared_ptr<Graphics::IGraphicsDevice> wpGraphicsDevice = graphicsDevice.lock();
	if (!wpGraphicsDevice)
	{
		return;
	}

	if (BeginMainMenuBar())
	{
		if (BeginMenu("File"))
		{
			if (MenuItem("dump"))
			{
			}
			ImGui::EndMenu();
		}
		if (BeginMenu("Window"))
		{
			if (MenuItem("open SceneGame window"))
			{
				mainWindow = !mainWindow;
			}
			if (MenuItem("open Watch Val window"))
			{
				watchWindow = !watchWindow;
			}
			if (MenuItem("open Outline Window"))
			{
				outlineWindow = !outlineWindow;
			}
			if (MenuItem("open component Window"))
			{
				componentWindow = !componentWindow;
			}
			if (MenuItem("open rasterize Window"))
			{
				controllRasterizeWindow = !controllRasterizeWindow;
			}
			ImGui::EndMenu();
		}
		OS::DisplayWin* dis = dynamic_cast<OS::DisplayWin*>(display);
		ImGui::Text("fps : %f", dis->fpsVal);
		ImGui::Text("frameRate : %f", dis->rate);
		ImGui::EndMainMenuBar();
	}


	int rastIndex = 0;
	if (controllRasterizeWindow)
	{
		ImGui::SetNextWindowPos(ImVec2(500 , 25));
		ImGui::SetNextWindowSize(ImVec2(100, display->GetHeight() - 800));

		Begin(u8"ラスタライザー");
		Graphics::GraphicsDeviceDX11* gd11 = dynamic_cast<Graphics::GraphicsDeviceDX11*>(wpGraphicsDevice.get());
		for (; rastIndex <= (int)Graphics::RasterizerState::TypeNum;)
		{
			if (ImGui::Selectable(std::to_string(rastIndex).c_str(), selected == rastIndex))//TODO : 列挙型を文字列に変換できるようにする
			{
				selected = rastIndex;
				gd11->rasterizerState = (Graphics::RasterizerState)rastIndex;
			}
			rastIndex++;
		}
		ImGui::End();
	}

	if (watchWindow)
	{
		s32 actIndex = 0;
		for (auto& act : actors)
		{
			std::string winName = (act.first + u8"ウォッチ");

			ImGui::SetNextWindowPos(ImVec2(550.0f + (actIndex * 300.0f), 25.0f));
			ImGui::SetNextWindowSize(ImVec2(300.0f, display->GetHeight() - 500.0f));

			Begin(winName.c_str());
			for (auto& val : act.second->mAry)
			{
				Vector3 pos   = GetLocation(*val.second),
					         scale = GetScale(*val.second);
				Quaternion rotate = GetRotation(*val.second);

				ImGui::Text((act.first + " : " + val.first).c_str());

				DragFloat3("pos",     &pos.x);
				DragFloat4("rotate", &rotate.x);
				DragFloat3("scale",   &scale.x);

				ImGui::Separator();
			}
			for (auto& val : act.second->qAry) {
				Quaternion rotate = *val.second;
				DragFloat4("rotate", &rotate.x);

				ImGui::Text((act.first + " : " + val.first).c_str());

				ImGui::Separator();
			}
			for (auto& val : act.second->cAry) {
				Color color = *val.second;
				ImGui::DragFloat4("color", &color.r);

				ImGui::Text((act.first + " : " + val.first).c_str());

				ImGui::Separator();
			}
			for (auto& val : act.second->v2Ary)
			{
				Vector2 v = *val.second;
				DragFloat2("vector2", &v.x);

				ImGui::Text((act.first + " : " + val.first).c_str());

				ImGui::Separator();
			}
			for (auto& val : act.second->v3Ary)
			{
				Vector3 v = *val.second;
				DragFloat3("vector3", &v.x);

				ImGui::Text((act.first + " : " + val.first).c_str());

				ImGui::Separator();
			}
			for (auto& val : act.second->v4Ary)
			{
				Vector4 v = *val.second;
				ImGui::DragFloat4("vector4", &v.x);

				ImGui::Text((act.first + " : " + val.first).c_str());

				ImGui::Separator();
			}
			ImGui::End();
			actIndex++;
		}
	}

	auto setCameraView([](Transform* m, Graphics::Camera* camera) {
		Vector3 target = GetLocation(m->GetWorldTransform());
		f32 xSin = sinf(camera->GetRotateX());
		f32 xCos = cosf(camera->GetRotateX());
		f32 ySin = sinf(camera->GetRotateY());
		f32 yCos = cosf(camera->GetRotateY());

		Vector3 front   = { -xCos * ySin, -xSin, -xCos * yCos };
		Vector3 _right  = { yCos, 0.0f, -ySin };
		Vector3 _up     = Math::Vector3Cross(_right, front);

		f32     distance   = camera->GetDistanceFromLookAt();
		Vector3 _target    = target;
		Vector3 _distance  = { distance, distance, distance };
		Vector3 _pos       = _target - (front * _distance);

		camera->SetEye(_pos);
		camera->SetTarget(_target, Vector3::Zero);
		camera->SetFocus(_target);
		camera->SetLookAt(_pos, _target, _up);
		camera->DataUpdate();
		});

	if (outlineWindow)
	{
		SetNextWindowPos(ImVec2(0, ((display->GetHeight() - 255))));
		SetNextWindowSize(ImVec2(350, 255));
		Begin(u8"アウトライン");

		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_SpanFullWidth  | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiSelectableFlags_AllowItemOverlap | ImGuiTreeNodeFlags_DefaultOpen;
		ImGuiSelectableFlags selectFlags      = ImGuiSelectableFlags_DontClosePopups;

		auto createActorFunction{ [&](Actor* owner) {
			ImGui::SetNextWindowSize(ImVec2(300.0f,200.0f));
			if (ImGui::BeginPopupModal("Create Actor Setting"))
			{
				static char actName[128]     = {};
				std::string createActor_name = {};
				ImGui::Text("ActorName"); ImGui::SameLine();
				ImGui::InputText("##edit", actName, IM_ARRAYSIZE(actName));
				createActor_name = actName;

				static int item = 1;
				std::vector<std::string> actorsCombo =
				{ "actor\0" , "player\0" , "stage\0" , "camera\0" , "IKTarget\0" };
				std::string  allActorsCombo = {};
				for (auto& combo : actorsCombo)
				{
					allActorsCombo += combo + '\0';
				}

				ImGui::Combo("Actor", &item, allActorsCombo.c_str() + '\0');

				if (createActor_name.size() <= 0)
				{
					ImGui::Text(u8"×無名のアクターは存在できません");
				}

				if (ImGui::Button("Create"))
				{
					std::shared_ptr<Actor> chashActor = nullptr;
					if (createActor_name.size())
					{
						switch (item)
						{
						case 1://normal Actor
							if (owner)
							{
								chashActor = owner->AddChildActor<Actor>();
							}
							else
							{
								actors.insert(std::pair(actorsCombo[item + 1], Actor::Create()));
								chashActor = actors[actorsCombo[item + 1]];
							}
							break;

#pragma region MyRegion

							//case 2://player
							//	if (owner)
							//	{
							//		chashActor = owner->AddChildActor<PlayerCom>();
							//	}
							//	else
							//	{
							//		actors.insert(std::pair(actorsCombo[item + 1], PlayerCom::Create()));
							//		chashActor = actors[actorsCombo[item + 1]].get();
							//	}
							//	break;

							//case 3://stage
							//	if (owner)
							//	{
							//		chashActor = owner->AddChildActor<StageCom>();
							//	}
							//	else
							//	{
							//		actors.insert(std::pair(actorsCombo[item + 1], StageCom::Create()));
							//		chashActor = actors[actorsCombo[item + 1]].get();
							//	}
							//	break;

							//case 4://camera
							//	if (owner)
							//	{
							//		chashActor = owner->AddChildActor<CameraAct>();
							//	}
							//	else
							//	{
							//		actors.insert(std::pair(actorsCombo[item + 1], CameraAct::Create()));
							//		chashActor = actors[actorsCombo[item + 1]].get();
							//	}
							//	break;

							//case 5://IKTarget
							//	if (owner)
							//	{
							//		chashActor = owner->AddChildActor<IKTargetCom>();
							//	}
							//	else
							//	{
							//		actors.insert(std::pair(actorsCombo[item + 1], IKTargetCom::Create()));
							//		chashActor = actors[actorsCombo[item + 1]].get();
							//	}
							//	break;

							//default:
							//	if (owner)
							//	{
							//		chashActor = owner->AddChildActor<Actor>();
							//	}
							//	else
							//	{
							//		actors.insert(std::pair(actorsCombo[item + 1], Actor::Create()));
							//		chashActor = actors[actorsCombo[item + 1]].get();
							//	}
							//	break;
#pragma endregion
						}
						chashActor->SetID(actorsCombo[item + 1]);
						ImGui::CloseCurrentPopup();
					}

				}ImGui::SameLine();

				if (ImGui::Button("Close"))
				{
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		} };

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::Button("Create Actor"))
			{
				ImGui::OpenPopup("Create Actor Setting");
			}
			ImGui::EndPopup();
		}

		for (auto& actor : actors)
		{
			//アクターの格納階層
			if (ImGui::TreeNodeEx((actor.second->GetID() + ("- FIle")).c_str(), treeNodeFlags))
			{
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::Button("Create Actor"))
					{
						ImGui::OpenPopup("Create Actor Setting");
					}
					createActorFunction(nullptr);
					ImGui::EndPopup();
				}

				//アクターの選択
				if (ImGui::Selectable(actor.second->GetID().c_str(), selectAct == actor.second ,selectFlags | ImGuiTreeNodeFlags_FramePadding))
				{
					selectAct = actor.second;

					//注視点を選択アクターに移す
					if (std::shared_ptr<Transform> transform = actor.second->GetComponent<Transform>(); transform != nullptr)
					{
						setCameraView(transform.get(), camera.get());
					}
				}

				auto actorTree = FND::Lambda{ [&](auto f, std::vector<std::shared_ptr<Actor>>& act) -> std::vector<std::shared_ptr<Actor>>*{
				for (auto& chilAct : act)
				{
					//アクターの格納階層
					if (ImGui::TreeNodeEx((chilAct->GetID() + (" - File")).c_str(), treeNodeFlags))
					{
						if (ImGui::BeginPopupContextItem())
						{
							if (ImGui::Button("Create Actor"))
							{
								ImGui::OpenPopup("Create Actor Setting");
							}
							createActorFunction(chilAct.get());
							ImGui::EndPopup();
						}

						//アクターの選択
						if (ImGui::Selectable(chilAct->GetID().c_str(), selectAct == chilAct, selectFlags))
						{
							selectAct = chilAct;

							//注視点を選択アクターに移す
							if (std::shared_ptr<Transform> transform = chilAct->GetComponent<Transform>(); transform != nullptr)
							{
								setCameraView(transform.get(), camera.get());
							}
						}

						//再帰
						f(chilAct->GetAllChildActor());

						ImGui::TreePop();
					}
				}
				return &act;
				} }(actor.second->GetAllChildActor());
				ImGui::TreePop();
			}
		}



		ImGui::End();
	}

	if (componentWindow)
	{
		SetNextWindowSize(ImVec2(350, display->GetHeight() - 280.0f));
		SetNextWindowPos(ImVec2(0.0f, 25.0f));
		Begin(u8"コンポーネント");
		if (selectAct)
		{
			char  name[128] = {};
			FND::StrCpy(name, sizeof(name), selectAct->GetID().c_str());
			ImGui::Text("ActorName"); ImGui::SameLine();
			ImGui::InputText(("##" + selectAct->GetID()).c_str(), name, IM_ARRAYSIZE(name));
			selectAct->SetID(name);

			for (auto& component : selectAct->GetAllComponent())
			{
				component->GUI();
				Separator();
			}
		}
		ImGui::End();
	}
}

void SceneGame::PrimitiveRender(
	Graphics::DeviceDX11* device,
	Vector3 translate, Vector3 rotate, Vector3 scale, f32 alpha)
{
	std::shared_ptr<Graphics::Camera> camera = actors[cameraS]->GetComponent<Graphics::Camera>();
	// ワールド行列を作成
	Matrix W;
	{
		Matrix S, R, T;
		S = MatrixScaling(scale.x, scale.y, scale.z);
		R = MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
		T = MatrixTranslation(translate.x, translate.y, translate.z);

		W = S * R * T;
	}

	primitive->Render
	(
		device->GetD3DContext(),
		ConvertToFloat4x4FromVector4x4(W * camera->GetView() * camera->GetProjection()),
		ConvertToFloat4x4FromVector4x4(W),
		DirectX::XMFLOAT4(1, 1, 1, 1),
		DirectX::XMFLOAT4(0.0f, 0.6f, 0.0f, alpha),
		false
	);
}

void SceneGame::CylinderPrimitiveRender(
	Graphics::DeviceDX11* device,
	Vector3 cp1Translate, Vector3 cp2Translate, Vector3 cyilinderTranslate,
	Vector3 rotate, Vector3 scale, Vector3 cyilinderScale)
{
	std::shared_ptr<Graphics::Camera> camera = actors[cameraS]->GetComponent<Graphics::Camera>();
	// Cylinder
	{
		// ワールド行列を作成
		Matrix W;
		{
			Matrix S, R, T;
			S = MatrixScaling(cyilinderScale.x, cyilinderScale.y, cyilinderScale.z);
			R = MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
			T = MatrixTranslation(cyilinderTranslate.x, cyilinderTranslate.y, cyilinderTranslate.z);

			W = S * R * T;
		}

		cylinderPrimitive->Render
		(
			device->GetD3DContext(),
			ConvertToFloat4x4FromVector4x4(W * camera->GetView() * camera->GetProjection()),
			ConvertToFloat4x4FromVector4x4(W),
			DirectX::XMFLOAT4(1, 1, 1, 1),
			DirectX::XMFLOAT4(0.0f, 0.6f, 0.0f, 0.5f),
			false
		);
	}
}

void SceneGame::UpdateLightDirection()
{
	std::shared_ptr<Graphics::Camera> camera = actors[cameraS]->GetComponent<Graphics::Camera>();

	FrameWork::LightState* light = static_cast<FrameWork::PBRShader*>(pbrShader.get())->GetLight();
	light->direction = Vector4(-camera->GetFront(), 1.0f);
}


void Frustum(float left, float right, float bottom, float top, float znear, float zfar, float* m16)
{
	float temp, temp2, temp3, temp4;
	temp = 2.0f * znear;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;
	m16[0] = temp / temp2;
	m16[1] = 0.0;
	m16[2] = 0.0;
	m16[3] = 0.0;
	m16[4] = 0.0;
	m16[5] = temp / temp3;
	m16[6] = 0.0;
	m16[7] = 0.0;
	m16[8] = (right + left) / temp2;
	m16[9] = (top + bottom) / temp3;
	m16[10] = (-zfar - znear) / temp4;
	m16[11] = -1.0f;
	m16[12] = 0.0;
	m16[13] = 0.0;
	m16[14] = (-temp * zfar) / temp4;
	m16[15] = 0.0;
}

void Perspective(float fovyInDegrees, float aspectRatio, float znear, float zfar, float* m16)
{
	float ymax, xmax;
	ymax = znear * tanf(fovyInDegrees * 3.141592f / 180.0f);
	xmax = ymax * aspectRatio;
	Frustum(-xmax, xmax, -ymax, ymax, znear, zfar, m16);
}

void OrthoGraphic(const float l, float r, float b, const float t, float zn, const float zf, float* m16)
{
	m16[0] = 2 / (r - l);
	m16[1] = 0.0f;
	m16[2] = 0.0f;
	m16[3] = 0.0f;
	m16[4] = 0.0f;
	m16[5] = 2 / (t - b);
	m16[6] = 0.0f;
	m16[7] = 0.0f;
	m16[8] = 0.0f;
	m16[9] = 0.0f;
	m16[10] = 1.0f / (zf - zn);
	m16[11] = 0.0f;
	m16[12] = (l + r) / (l - r);
	m16[13] = (t + b) / (b - t);
	m16[14] = zn / (zn - zf);
	m16[15] = 1.0f;
}