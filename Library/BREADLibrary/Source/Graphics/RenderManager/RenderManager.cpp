#include "Graphics/RenderManager.h"
#include "Graphics\Camera.h"

#include "../Source/OS/Display/Win/DisplayWin.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../Source/Graphics//Texture/Win/DirectX11/TextureDX11.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"
#include "../Source/Graphics/GraphicsDevice/Win/DirectX11/GraphicsDeviceDX11.h"

#include "FND/Instance.h"
#include "FND/Util.h"

#include "FrameWork/Actor/ActorManager.h"
#include "FrameWork/Component/Transform.h"
#include "FrameWork/Object/Object.h"

#include "Math/BreadMath.h"

using namespace Bread::Math;
using Bread::FND::SharedInstance;
using Bread::FND::UniqueInstance;
using Bread::FND::Instance;

using Bread::FrameWork::ActorManager;
using Bread::FrameWork::ModelObject;
using Bread::FrameWork::Transform;
using Bread::FrameWork::LightState;

using Bread::FrameWork::BasicShader;
using Bread::FrameWork::BasicSkinShader;
using Bread::FrameWork::StandardShader;
using Bread::FrameWork::PBRShader;
using Bread::FrameWork::PBRSkinShader;

namespace Bread
{
	namespace Graphics
	{
		//初期化
		void RenderManager::Initialize()
		{
			shaders.clear();

			//デバッグ用
			Graphics::DeviceDX11* device{ dynamic_cast<Graphics::DeviceDX11*>(SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice()) };

			//共通データの初期化
			RegisterModelRenderShader(ShaderNameVal::basicShader,      BasicShader::Create());
			RegisterModelRenderShader(ShaderNameVal::basicSkinShader,  BasicSkinShader::Create());
			RegisterModelRenderShader(ShaderNameVal::standardShader,   StandardShader::Create());
			RegisterModelRenderShader(ShaderNameVal::pbrShader,        PBRShader::Create());
			RegisterModelRenderShader(ShaderNameVal::pbrSkinShader,    PBRSkinShader::Create());

			//フレームバッファー
			{
				frameBuffer[0] = FrameWork::FrameBuffer::Create();
				frameBuffer[1] = FrameWork::FrameBuffer::Create();
				frameBuffer[2] = FrameWork::FrameBuffer::Create();
				frameBuffer[3] = FrameWork::FrameBuffer::Create();
				frameBuffer[4] = FrameWork::FrameBuffer::Create();
				frameBuffer[5] = FrameWork::FrameBuffer::Create();

				frameBuffer[0]->Initialize(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), UniqueInstance<OS::DisplayWin>::instance->GetWidth(), UniqueInstance<OS::DisplayWin>::instance->GetHeight(), enableMSAA, 8, Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Graphics::TextureFormatDx::R24G8_TYPELESS);
				frameBuffer[1]->Initialize(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), UniqueInstance<OS::DisplayWin>::instance->GetWidth(), UniqueInstance<OS::DisplayWin>::instance->GetHeight(), false,      1, Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Graphics::TextureFormatDx::R24G8_TYPELESS);
				frameBuffer[2]->Initialize(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), UniqueInstance<OS::DisplayWin>::instance->GetWidth(), UniqueInstance<OS::DisplayWin>::instance->GetHeight(), false,      1, Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Graphics::TextureFormatDx::UNKNOWN);
				frameBuffer[3]->Initialize(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), UniqueInstance<OS::DisplayWin>::instance->GetWidth(), UniqueInstance<OS::DisplayWin>::instance->GetHeight(), false,      1, Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Graphics::TextureFormatDx::R24G8_TYPELESS);
				frameBuffer[4]->Initialize(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), UniqueInstance<OS::DisplayWin>::instance->GetWidth(), UniqueInstance<OS::DisplayWin>::instance->GetHeight(), false,      1, Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Graphics::TextureFormatDx::R24G8_TYPELESS);
				frameBuffer[5]->Initialize(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), UniqueInstance<OS::DisplayWin>::instance->GetWidth(), UniqueInstance<OS::DisplayWin>::instance->GetHeight(), false,      1, Graphics::TextureFormatDx::R16G16B16A16_FLOAT, Graphics::TextureFormatDx::R24G8_TYPELESS);
			}

			//ポストプロセス
			{
				postProcessingEffects = FrameWork::PostProcessingEffects::Create();
				postProcessingEffects->Initialize(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
			}

			//シャドウマップ
			{
				shadowMap = FrameWork::FrameBuffer::Create();
				shadowMap->Initialize(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), 1024 * 5, 1024 * 5, false, 1, Graphics::TextureFormatDx::UNKNOWN, Graphics::TextureFormatDx::R32_TYPELESS);

				voidPS = Graphics::IShader::Create();
				//voidPS->LoadPS(graphicsDevice->GetDevice(), "ShadowMapPS.cso");
				lightSpaceCamera = std::make_unique<Graphics::Camera>();

				// 定数バッファ作成
				{
					Graphics::BreadBufferDesc bufferDesc = {};
					FND::MemSet(&bufferDesc, 0, sizeof(bufferDesc));
					bufferDesc.usage               = Graphics::BreadUsage::Dynamic;
					bufferDesc.bindFlags           = static_cast<s32>(Graphics::BreadBindFlag::ConstantBuffer);
					bufferDesc.cpuAccessFlags      = static_cast<s32>(Graphics::BreadCPUAccessFlag::CPUAccessWrite);
					bufferDesc.miscFlags           = 0;
					bufferDesc.byteWidth           = sizeof(ShaderConstants);
					bufferDesc.structureByteStride = 0;

					shaderConstantsBuffer = Graphics::IBuffer::Create();
					shaderConstantsBuffer->Initialize(SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice(), bufferDesc);
				}

				// サンプラー作成
				{
					comparisonSampler = Graphics::ISampler::Create();
					comparisonSampler->Initialize(SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice(), Graphics::SamplerState::LinearBorder, false, true);
				}
			}

			//モーションブラー
			{
				motionBlur = FrameWork::MotionBlur::Create();
				motionBlur->Initialize(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(),
					UniqueInstance<OS::DisplayWin>::instance->GetWidth(), UniqueInstance<OS::DisplayWin>::instance->GetHeight());
			}

			//ブルーム
			{
				quad = FrameWork::Quad::Create();
				quad->Initialize(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), Graphics::SamplerState::PointBorder);

				msaaResolve = FrameWork::MSAAResolve::Create();
				msaaResolve->Initialize(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());

				bloom = FrameWork::Bloom::Create();
				bloom->Initialize(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(),
					UniqueInstance<OS::DisplayWin>::instance->GetWidth(), UniqueInstance<OS::DisplayWin>::instance->GetHeight());
			}

			//トーンマップ
			{
				toneMap = FrameWork::ToneMap::Create();
				toneMap->Initialize(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(),
					UniqueInstance<OS::DisplayWin>::instance->GetWidth(), UniqueInstance<OS::DisplayWin>::instance->GetHeight());
			}

			//リソースマネージャー
			{
				using Bread::OS::ResourceManager;
				using Bread::OS::ResourceType;

				SharedInstance<ResourceManager>::instance = std::dynamic_pointer_cast<ResourceManager>(OS::IResourceManager::Create());
				SharedInstance<ResourceManager>::instance->Initialize(nullptr);
			}

			//スカイマップ
			{
				skyMap = FrameWork::SkyMap::Create();
				skyMap->Initialize("..\\Data\\Assets\\Texture\\SkyMap\\AllSkyFree\\Epic_BlueSunset\\Epic_BlueSunset03.dds");
			}
		}

		//描画
		void RenderManager::Render()
		{
			Graphics::IContext* context{ SharedInstance<GraphicsDeviceDX11>::instance->GetContext() };
			Graphics::Viewport* v{ new Graphics::Viewport() };
			context->GetViewports(1, &v);
			float aspectRatio{ v->width / v->height };
			FND::SafeDelete(v);

			static f32 radius{ 10.0f };
			static f32 alpha{ 1.0f };

			screenSpaceCamera = Instance<ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>();

			// Generate ShadowMap
			GenerateShadows();

			// Generate VelocityMap
			GenerateMotionBlur();

			// Work No_0 framebuffer.
			{
				frameBuffer[0]->Clear(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), 0, 0.5f, 0.5f, 0.5f, 1.0f);
				frameBuffer[0]->Activate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
				{
					// Set Shadow Data.
					{
						lightSpaceCamera->SetEye(GetLocation(Instance<ActorManager>::instance.GetActorFromID("player")->GetComponent<Transform>()->GetWorldTransform()));					lightSpaceCamera->SetFocus((lightSpaceCamera->GetFront() * -1.0f) * 1000.0f);
						shaderContexts.lightViewProjection = MatrixTranspose(/*Bread::Math::MatrixInverse*/(lightSpaceCamera->GetView() * lightSpaceCamera->GetProjection()));

						Graphics::BreadMap map{ Graphics::BreadMap::WriteDiscard };
						Graphics::BreadMappedSubresource mapedBuffer;
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

						Graphics::ISampler* samplers[]
						{
							comparisonSampler.get()
						};
						context->SetSamplers(Graphics::ShaderType::Pixel, 3, 1, samplers);

						Graphics::ITexture* texture[] { shadowMap->GetDepthStencilSurface()->GetTexture() };
						SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetContext()->SetShaderResources(Graphics::ShaderType::Pixel, 8, 1, texture);
					}

					// Draw skymap.
					RenderSkyMap();

					// Draw mesh
					{
						Graphics::ContextDX11* contextDX11{ static_cast<Graphics::ContextDX11*>(context) };
						f32 blendFactor[4]{ 0.0f, 0.0f, 0.0f, 0.0f };
						context->SetBlend(contextDX11->GetBlendState(Graphics::BlendState::AlphaToCoverageEnable), blendFactor, 0xFFFFFFFF);
						{
							RenderObjectNormal(ShaderNameVal::basicShader);
						}
						context->SetBlend(contextDX11->GetBlendState(Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
					}

					// Draw skinMesh
					RenderObjectNormal(ShaderNameVal::basicSkinShader);

					// Draw collision primitive.
					if (isHitCollision)
					{
						RenderCollisionPrimitive(context);
					}
				}
				frameBuffer[0]->Deactivate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
			}

			// Generate Bloom Texture.
			u32 resolvedFramebuffer = 0;
			{
				if (enableMSAA)
				{
					resolvedFramebuffer = 1;

					msaaResolve->Resolve(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), frameBuffer[0].get(), frameBuffer[resolvedFramebuffer].get());

					bloom->Generate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), frameBuffer[resolvedFramebuffer]->GetRenderTargetSurface()->GetTexture(), false);

					frameBuffer[resolvedFramebuffer]->Clear(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), 0, 0.5f, 0.5f, 0.5f, 1.0f);
					frameBuffer[resolvedFramebuffer]->Activate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
					{
						quad->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(),
							frameBuffer[0]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f,
							static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetWidth()),
							static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetHeight()));

						bloom->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
					}
					frameBuffer[resolvedFramebuffer]->Deactivate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
				}
				else
				{
					bloom->Generate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), frameBuffer[resolvedFramebuffer]->GetRenderTargetSurface()->GetTexture(), false);

					resolvedFramebuffer = 1;

					frameBuffer[resolvedFramebuffer]->Clear(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), 0, 0.5f, 0.5f, 0.5f, 1.0f);
					frameBuffer[resolvedFramebuffer]->Activate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
					{
						quad->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(),
							frameBuffer[0]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f,
							static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetWidth()),
							static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetHeight()));
						bloom->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
					}
					frameBuffer[resolvedFramebuffer]->Deactivate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
				}
			}

			frameBuffer[3]->Clear(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), 0, 0.5f, 0.5f, 0.5f, 1.0f);
			frameBuffer[3]->Activate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
			{
				// Blend Bloom.
				if (bloomBlend)
				{

					quad->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(),
						frameBuffer[resolvedFramebuffer]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f,
						static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetWidth()),
						static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetHeight()));
				}
				else
				{
					quad->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(),
						frameBuffer[0]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f,
						static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetWidth()),
						static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetHeight()));
				}
			}
			frameBuffer[3]->Deactivate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());

			// Screen Filter
			frameBuffer[4]->Clear(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), 0, 0.5f, 0.5f, 0.5f, 1.0f);
			frameBuffer[4]->Activate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
			{
				quad->SetBright(bright);
				quad->SetContrast(contrast);
				quad->SetSaturate(saturate);
				quad->SetScreenColor(screenColor);

				quad->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), frameBuffer[3]->renderTargerSurface[0]->GetTexture(), Vector2(0.0f, 0.0f), Vector2(1920.0f, 1080.0f), Vector2(0.0f, 0.0f), Vector2(1920.0f, 1080.0f), 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, true, true, true, true, true, true);
			}
			frameBuffer[4]->Deactivate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());

			// Motion Blur
			resolvedFramebuffer = 4;
			if (isMotionBlur)
			{
				resolvedFramebuffer = 5;
				frameBuffer[resolvedFramebuffer]->Clear(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), 0, 0.5f, 0.5f, 0.5f, 1.0f);
				frameBuffer[resolvedFramebuffer]->Activate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
				{
					motionBlur->blurConstants.loop = 5;
					motionBlur->blurConstants.div = 1.0f / static_cast<f32>(motionBlur->blurConstants.loop + 1);

					motionBlur->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), frameBuffer[4]->renderTargerSurface[0]->GetTexture(), *screenSpaceCamera, true);
				}
				frameBuffer[resolvedFramebuffer]->Deactivate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
			}

			// Final Draw
			{
				quad->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), frameBuffer[3]->renderTargerSurface[0]->GetTexture(), Vector2(0.0f, 0.0f), Vector2(1920.0f, 1080.0f), Vector2(0.0f, 0.0f), Vector2(1920.0f, 1080.0f));
			}

			// 前フレームとしてカメラ情報を保存
			{
				context->UpdateConstantBufferPrevScene(screenSpaceCamera->GetView(), screenSpaceCamera->GetProjection());
			}
		}

		//モデル描画用のShader(FlyWeight)
		RenderManager& RenderManager::RegisterModelRenderShader(const std::string& str, std::shared_ptr<FrameWork::IShader> shader)
		{
			shaders.try_emplace(str, shader);
			shaders[str]->Initialize(SharedInstance<GraphicsDeviceDX11>::instance.get());
			return *this;
		}

		void RenderManager::RenderActor(const std::string& shaderName)
		{
			for (auto& actor : Instance<ActorManager>::instance.GetAllActor())
			{
				RenderChildActor(shaderName, actor);
			}
		}
		void RenderManager::RenderChildActor(const std::string& shaderName, std::shared_ptr<FrameWork::Actor> Act)
		{
			if (auto model = Act->GetComponent<ModelObject>())
			{
				if (shaderName == model->GetShaderMethod().GetShaderName())
				{
					shaders[shaderName]->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(),
						Act->GetComponent<Transform>()->GetWorldTransform(),
						model.get());
				}
			}

			for (auto&  childAct : Act->GetAllChildActor())
			{
				RenderChildActor(shaderName, childAct);
			}
		}

		void RenderManager::RenderSkyMap()
		{
			FrameWork::LightState* light{ static_cast<PBRShader*>(shaders[ShaderNameVal::pbrShader].get())->GetLight() };
			Math::Color color { 1.0f, 1.0f, 1.0f, 1.0f };
			constexpr float skyDimension{ 20000 };

			// ワールド行列を作成
			Matrix skyWorldM;
			{
				Vector3 scale    { skyDimension, skyDimension, skyDimension };
				Vector3 rotate   { 0.0f, 90.0f * 0.01745f, 0.0f };
				Vector3 translate{ 0.0f, 0.0f, 0.0f };

				Matrix S, R, T;
				S = MatrixScaling(scale.x, scale.y, scale.z);
				R = MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
				T = MatrixTranslation(translate.x, translate.y, translate.z);

				skyWorldM = S * R * T;
			}
			skyMap->Draw(skyWorldM, *screenSpaceCamera, light->direction, color);
		}

		void RenderManager::RenderCollisionPrimitive(Graphics::IContext* context)
		{
			Graphics::DeviceDX11* device = static_cast<Graphics::DeviceDX11*>(SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice());

			Graphics::ContextDX11* contextDX11 = static_cast<Graphics::ContextDX11*>(context);
			context->SetBlend(contextDX11->GetBlendState(Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);

			//TODO : 使ってないけどいずれ直せ
			//アクター側で描画
			for (auto& act : Instance<ActorManager>::instance.GetAllActor())
			{
				act->Draw();
				for (auto& childAct : act->GetAllChildActor())
				{
					childAct->Draw();
				}
			}
			context->SetBlend(contextDX11->GetBlendState(Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
		}

		void RenderManager::RenderNomal()
		{

		}

		void RenderManager::RenderObjectNormal(const std::string& shaderName)
		{
			shaders[shaderName]->Begin(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), *screenSpaceCamera);
			RenderActor(shaderName);
			shaders[shaderName]->End(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
		}

		void RenderManager::GenerateShadows()
		{
			Vector3 playerLocation
			{
				GetLocation(Instance<ActorManager>::instance
				.GetActorFromID("player")->GetComponent<Transform>()->GetWorldTransform())
			};

			shadowMap->Clear(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), 0, 1.0f, 1.0f, 1.0f, 1.0f);
			shadowMap->Activate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
			{
				float distance = dis;
				LightState* light{ static_cast<PBRShader*>(shaders[ShaderNameVal::pbrShader].get())->GetSunLight() };
				Vector4 L  { Vector4Normalize(-light->direction) };
				Vector4 P  { Vector4(playerLocation.x, playerLocation.y, playerLocation.z, 1.0f) };
				Vector4 Eye{ P - distance * L };

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

				RenderObjectShadow(ShaderNameVal::basicSkinShader);
				RenderObjectShadow(ShaderNameVal::basicShader);
			}
			shadowMap->Deactivate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
		}

		void RenderManager::RenderObjectShadow(const std::string& shaderName)
		{
			shaders[shaderName]->Begin(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), *lightSpaceCamera);
			voidPS->ActivatePS(SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice());
			RenderActor(shaderName);
			voidPS->DeactivatePS(SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice());
			shaders[shaderName]->End(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
		}

		void RenderManager::GenerateMotionBlur()
		{
			motionBlur->ActivateVelocity(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
			{
				// データセット
				{
					motionBlur->velocityConstants.screenWidth  = static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetWidth());
					motionBlur->velocityConstants.screenHeight = static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetHeight());
					motionBlur->velocityConstants.frameRate    = MapInstance<f32>::instance["elapsedTime"] / 60.0f;
				}

				// Draw Actors
				RenderObjectMotionBlur(ShaderNameVal::basicSkinShader);
				RenderObjectMotionBlur(ShaderNameVal::basicShader);
			}
			motionBlur->DeactivateVelocity(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
		}

		void RenderManager::RenderObjectMotionBlur(const std::string& shaderName)
		{
			shaders[shaderName]->Begin             (SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), *screenSpaceCamera);
			motionBlur         ->ActivateVelocityPS(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
			RenderActor(shaderName);
			motionBlur         ->DeactivateVelocityPS(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
			shaders[shaderName]->End                 (SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
		}

		void RenderManager::UpdateLightDirection()
		{
			dynamic_cast<FrameWork::PBRShader*>
				(shaders[ShaderNameVal::pbrShader].get())
				->GetLight()->direction = Vector4(-screenSpaceCamera->GetFront(), 1.0f);
		}

	}//namespace Graphics
}//namespace Bread