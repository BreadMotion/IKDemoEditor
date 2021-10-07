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
using Bread::FrameWork::PBRShader;
using Bread::FrameWork::PBRSkinShader;

namespace Bread
{
	namespace Graphics
	{
		//������
		void RenderManager::Initialize()
		{
			shaders.clear();
		}

		//�`��
		void RenderManager::Render()
		{
			Graphics::IContext* context = SharedInstance<GraphicsDeviceDX11>::instance->GetContext();

			Graphics::Viewport* v = new Graphics::Viewport();
			context->GetViewports(1, &v);
			float aspectRatio = v->width / v->height;
			FND::SafeDelete(v);

			std::shared_ptr<Graphics::Camera> camera
			{ Instance<ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>() };

			static f32     radius    = 10.0f;
			static f32     alpha     = 1.0f;

			//	ibl->Clear(graphicsDevice);
			//	ibl->Activate(graphicsDevice);
			//	{
			//		// Draw skymap.
			//		{
			//			Bread::FrameWork::LightState* light = static_cast<Bread::FrameWork::PBRShader*>(pbrShader)->GetLight();
			//			Bread::Math::Color color = { 1.0f, 1.0f, 1.0f, 1.0f };
			//			float skyDimension = 50000;
			//			// ���[���h�s����쐬
			//			Bread::Math::Matrix skyWorldM;
			//			{
			//				Bread::Math::Vector3 scale = { skyDimension, skyDimension, skyDimension };
			//				Bread::Math::Vector3 rotate = { 0.0f, 90.0f * 0.01745f, 0.0f };
			//				Bread::Math::Vector3 translate = { 0.0f, 0.0f, 0.0f };
			//
			//				Bread::Math::Matrix S, R, T;
			//				S = Bread::Math::MatrixScaling(scale.x, scale.y, scale.z);
			//				R = Bread::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
			//				T = Bread::Math::MatrixTranslation(translate.x, translate.y, translate.z);
			//
			//				skyWorldM = S * R * T;
			//			}
			//			skyMap->Draw(graphicsDevice, skyWorldM, *camera, light->direction, color);
			//		}
			//
			//		// Draw stage.
			//		{
			//			// ���[���h�s����쐬
			//			Bread::Math::Matrix W;
			//			{
			//				Bread::Math::Vector3 scale = { 40.0f, 40.0f, 40.0f };
			//				Bread::Math::Vector3 rotate = { 0.0f, 0.0f, 0.0f };
			//				Bread::Math::Vector3 translate = { 0.0f, 0.0f, 0.0f };
			//
			//				Bread::Math::Matrix S, R, T;
			//				S = Bread::Math::MatrixScaling(scale.x, scale.y, scale.z);
			//				R = Bread::Math::MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
			//				T = Bread::Math::MatrixTranslation(translate.x, translate.y, translate.z);
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
			RenderShadows();

			// Generate VelocityMap
			RenderMotionBlur();

			// Work No_0 framebuffer.
			{
				frameBuffer[0]->Clear(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), 0, 0.5f, 0.5f, 0.5f, 1.0f);
				frameBuffer[0]->Activate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
				{
					// Set Shadow Data.
					{
						shaderContexts.lightViewProjection = MatrixTranspose(/*Bread::Math::MatrixInverse*/(lightSpaceCamera->GetView() * lightSpaceCamera->GetProjection()));
						Graphics::BreadMap map = Graphics::BreadMap::WriteDiscard;
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

						Graphics::ISampler* samplers[] =
						{
							comparisonSampler.get()
						};
						context->SetSamplers(Graphics::ShaderType::Pixel, 3, 1, samplers);

						Graphics::ITexture* texture[] = { shadowMap->GetDepthStencilSurface()->GetTexture() };
						SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetContext()->SetShaderResources(Graphics::ShaderType::Pixel, 8, 1, texture);
					}

					// Draw skymap.
					{
						FrameWork::LightState* light = static_cast<PBRShader*>(shaders["pbrShader"].get())->GetLight();
						Math::Color color = { 1.0f, 1.0f, 1.0f, 1.0f };
						float skyDimension = 20000;
						// ���[���h�s����쐬
						Matrix skyWorldM;
						{
							Vector3 scale = { skyDimension, skyDimension, skyDimension };
							Vector3 rotate = { 0.0f, 90.0f * 0.01745f, 0.0f };
							Vector3 translate = { 0.0f, 0.0f, 0.0f };

							Matrix S, R, T;
							S = MatrixScaling(scale.x, scale.y, scale.z);
							R = MatrixRotationRollPitchYaw(rotate.x, rotate.y, rotate.z);
							T = MatrixTranslation(translate.x, translate.y, translate.z);

							skyWorldM = S * R * T;
						}
						skyMap->Draw(skyWorldM,
							*camera,
							light->direction, color);
					}

					// Draw stage.
					{
#if 1
						Graphics::ContextDX11* contextDX11 = static_cast<Graphics::ContextDX11*>(context);
						f32 blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
						context->SetBlend(contextDX11->GetBlendState(Graphics::BlendState::AlphaToCoverageEnable), blendFactor, 0xFFFFFFFF);
						{
							RenderObjectNormal("basicShader");
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
						// ���b�V���`��
#if 1
						basicSkinShader->Begin(graphicsDevice, *camera);
						//basicSkinShader->Draw(graphicsDevice, boss->GetWorldMatrix(), boss->GetModel());
						// �G�t�F�N�g�`��
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

#if 0
							// Draw Effect.
							{
								Bread::Graphics::ContextDX11* contextDX11 = static_cast<Bread::Graphics::ContextDX11*>(context);
								Bread::f32 blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
								context->SetBlend(contextDX11->GetBlendState(Bread::Graphics::BlendState::AlphaToCoverageEnable), blendFactor, 0xFFFFFFFF);
								{
									gpuParticle->Draw(graphicsDevice, *camera);
									playerHitParticle->Draw(graphicsDevice, *camera);
									bossHitParticle->Draw(graphicsDevice, *camera);
									petalParticle->Draw(graphicsDevice, *camera);
									soilParticle->Draw(graphicsDevice, *camera);
									bossAuraParticle->Draw(graphicsDevice, *camera);
									playerStrongAttackParticle->Draw(graphicsDevice, *camera);
									//playerMeshParticle->Draw(graphicsDevice, *camera);
									/*for (Bread::s32 i = 0; i < 3; ++i)
									{
										dusterParticle[i]->Draw(graphicsDevice, *camera);
									}*/
								}
								context->SetBlend(contextDX11->GetBlendState(Bread::Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
							}
#endif
						RenderObjectNormal("basicSkinShader");
#endif
					}

					// Draw collision primitive.
					if (isHitCollision)
					{
						Graphics::DeviceDX11* device = static_cast<Graphics::DeviceDX11*>(SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice());

						Graphics::ContextDX11* contextDX11 = static_cast<Graphics::ContextDX11*>(context);
						context->SetBlend(contextDX11->GetBlendState(Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);

						//�A�N�^�[���ŕ`��
						for (auto& act : Instance<ActorManager>::instance.GetAllActor())
						{
							act->Draw();
						}
						context->SetBlend(contextDX11->GetBlendState(Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
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
					/*resolvedFramebuffer = 2;
					frameBuffer[resolvedFramebuffer]->Activate(graphicsDevice);
					{
						bloom->Blend(graphicsDevice, frameBuffer[0]->GetRenderTargetSurface()->GetTexture(), frameBuffer[1]->GetRenderTargetSurface()->GetTexture());
					}
					frameBuffer[resolvedFramebuffer]->Deactivate(graphicsDevice);*/

					quad->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(),
						frameBuffer[resolvedFramebuffer]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f,
						static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetWidth()),
						static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetHeight()));

					//toneMap->Draw(graphicsDevice, frameBuffer[resolvedFramebuffer]->renderTargerSurface[0]->GetTexture(), MapInstance<f32>::instance["elapsedTime"]);
				}
				else
				{
					quad->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(),
						frameBuffer[0]->renderTargerSurface[0]->GetTexture(), 0.0f, 0.0f,
						static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetWidth()),
						static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetHeight()));
				}
#endif
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

					motionBlur->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), frameBuffer[4]->renderTargerSurface[0]->GetTexture(), *camera, true);
				}
				frameBuffer[resolvedFramebuffer]->Deactivate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
			}

			// Final Draw
			{
				quad->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), frameBuffer[3]->renderTargerSurface[0]->GetTexture(), Vector2(0.0f, 0.0f), Vector2(1920.0f, 1080.0f), Vector2(0.0f, 0.0f), Vector2(1920.0f, 1080.0f));
			}

			// �O�t���[���Ƃ��ăJ��������ۑ�
			{
				context->UpdateConstantBufferPrevScene(camera->GetView(), camera->GetProjection());
			}
		}

		//���f���`��p��Shader(FlyWeight)
		RenderManager& RenderManager::RegisterModelRenderShader(const std::string& str, std::shared_ptr<FrameWork::IShader> shader)
		{
			shaders.try_emplace(str, shader);
			shaders[str]->Initialize(SharedInstance<GraphicsDeviceDX11>::instance.get());
			return *this;
		}

		void RenderManager::RenderNomal()
		{

		}

		void RenderManager::RenderObjectNormal(const std::string& shaderName)
		{
			shaders[shaderName]->Begin(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), *lightSpaceCamera);
			for (auto& actor : Instance<ActorManager>::instance.GetAllActor())
			{
				if (shaderName == actor->GetComponent<ModelObject>()->GetShaderMethod().GetShaderName())
				{
					shaders[shaderName]->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(),
						actor->GetComponent<Transform>()->GetWorldTransform(),
						actor->GetComponent<ModelObject>().get());

				}
			}
			shaders[shaderName]->End(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
		}

		void RenderManager::RenderShadows()
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
				LightState* light = static_cast<PBRShader*>(shaders["pbrShader"].get())->GetSunLight();
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

				RenderObjectShadow("basicSkinShader");
				RenderObjectShadow("basicShader");
			}
			shadowMap->Deactivate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
		}

		void RenderManager::RenderObjectShadow(const std::string& shaderName)
		{
			shaders[shaderName]->Begin(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), *lightSpaceCamera);
			voidPS->ActivatePS(SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice());
			for (auto& actor : Instance<ActorManager>::instance.GetAllActor())
			{
				if (shaderName == actor->GetComponent<ModelObject>()->GetShaderMethod().GetShaderName())
				{
					shaders[shaderName]->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(),
						actor->GetComponent<Transform>()->GetWorldTransform(),
						actor->GetComponent<ModelObject>().get());

				}
			}
			voidPS->DeactivatePS(SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice());
			shaders[shaderName]->End(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
		}

		void RenderManager::RenderMotionBlur()
		{
			motionBlur->ActivateVelocity(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
			{
				// �f�[�^�Z�b�g
				{
					motionBlur->velocityConstants.screenWidth = static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetWidth());
					motionBlur->velocityConstants.screenHeight = static_cast<f32>(UniqueInstance<OS::DisplayWin>::instance->GetHeight());
					motionBlur->velocityConstants.frameRate = MapInstance<f32>::instance["elapsedTime"] / 60.0f;
				}

				// Draw Actors
				RenderObjectMotionBlur("basicSkinShader");
				RenderObjectMotionBlur("basicShader");
			}
			motionBlur->DeactivateVelocity(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
		}

		void RenderManager::RenderObjectMotionBlur(const std::string& shaderName)
		{
			shaders[shaderName]->Begin(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(),
				*Instance<ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>());
			motionBlur->ActivateVelocityPS(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
			for (auto& actor : Instance<ActorManager>::instance.GetAllActor())
			{
				if (shaderName == actor->GetComponent<ModelObject>()->GetShaderMethod().GetShaderName())
				{
					shaders[shaderName]->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(),
						actor->GetComponent<Transform>()->GetWorldTransform(),
						actor->GetComponent<ModelObject>().get());

				}
			}
			motionBlur->DeactivateVelocityPS(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
			shaders[shaderName]->End(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
		}
	}//namespace Graphics
}//namespace Bread