#include "pch.h"
#include "FrameWork/Shader/PBRShader.h"
#include "FND/Util.h"
#include "FND/STD.h"
#include "../Source/Graphics/Light/LightingState.h"


namespace Bread
{
	namespace FrameWork
	{
		// 生成
		std::unique_ptr<IShader> PBRShader::Create()
		{
			return std::make_unique<PBRShader>();
		}

		// 初期化
		bool PBRShader::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Bread::Graphics::PhoenixInputElementDesc inputElementDesc[] =
			{
				// SemanticName	 SemanticIndex	Format														InputSlot	AlignedByteOffset	InputSlotClass										InstanceDataStepRate
				{"POSITION",	 0,				Bread::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT,			0,			0,					Bread::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"NORMAL",		 0,				Bread::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT,			1,			0,					Bread::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"TANGENT",		 0,				Bread::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT,			2,			0,					Bread::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"TEXCOORD",	 0,				Bread::Graphics::PHOENIX_FORMAT_R32G32_FLOAT,				3,			0,					Bread::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"BLENDWEIGHT",	 0,				Bread::Graphics::PHOENIX_FORMAT_R32G32B32A32_FLOAT,		    4,			0,					Bread::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"BLENDWEIGHT",	 1,				Bread::Graphics::PHOENIX_FORMAT_R32G32B32A32_FLOAT,		    5,			0,					Bread::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"BLENDINDICES", 0,				Bread::Graphics::PHOENIX_FORMAT_R8G8B8A8_UINT,			    6,			0,					Bread::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"BLENDINDICES", 1,				Bread::Graphics::PHOENIX_FORMAT_R8G8B8A8_UINT,			    7,			0,					Bread::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
			};

			shader = Graphics::IShader::Create();
			shader->LoadVS
			(
				graphicsDevice->GetDevice(),
				"PhysicallyBasedRenderingVS.cso",
				inputElementDesc,
				Bread::FND::ArraySize(inputElementDesc)
			);
			shader->LoadPS
			(
				graphicsDevice->GetDevice(),
				"PhysicallyBasedRenderingPS.cso"
			);

			cbMaterial = Bread::Graphics::IBuffer::Create();
			cbScene    = Bread::Graphics::IBuffer::Create();
			{
				Bread::Graphics::PhoenixBufferDesc desc = {};
				Bread::FND::MemSet(&desc, 0, sizeof(desc));
				desc.usage               = Bread::Graphics::PhoenixUsage::Default;
				desc.bindFlags           = static_cast<Bread::s32>(Bread::Graphics::PhoenixBindFlag::ConstantBuffer);
				desc.cpuAccessFlags      = 0;
				desc.miscFlags           = 0;
				desc.structureByteStride = 0;

				desc.byteWidth = sizeof(CbMaterial);
				if (!cbMaterial->Initialize(graphicsDevice->GetDevice(), desc))
				{
					return false;
				}

				desc.byteWidth = sizeof(CbScene);
				if (!cbScene->Initialize(graphicsDevice->GetDevice(), desc))
				{
					return false;
				}
			}

			vbKinds.emplace_back(Graphics::VertexBufferKind::Position);
			vbKinds.emplace_back(Graphics::VertexBufferKind::Normal);
			vbKinds.emplace_back(Graphics::VertexBufferKind::Tangent);
			vbKinds.emplace_back(Graphics::VertexBufferKind::TexCoord0);
			vbKinds.emplace_back(Graphics::VertexBufferKind::BlendWeight0);
			vbKinds.emplace_back(Graphics::VertexBufferKind::BlendWeight1);
			vbKinds.emplace_back(Graphics::VertexBufferKind::BlendIndex0);
			vbKinds.emplace_back(Graphics::VertexBufferKind::BlendIndex1);

			light      = std::make_unique<LightState>();
			sunLight   = std::make_unique<LightState>();
			pointLight = std::make_unique<PointLightState>();
			material   = std::make_unique<MaterialState>();

			light->direction = Math::Vector4(0.0f, 1.0f, 0.0f, 1.0f);
			light->color     = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);

			sunLight->direction = Math::Vector4(0.5f, 0.5f, 0.0f, 1.0f);
			sunLight->color     = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);

			pointLight->position = Math::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
			pointLight->color    = Math::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
			pointLight->distance = 0.0f;
			pointLight->decay    = 0.0f;
			pointLight->dummy    = Math::Vector2(0.0f, 0.0f);

			material->albedo    = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			material->metallic  = 0.35f; //0.0f
			material->roughness = 0.73f; //0.47f

			return true;
		}

		// 終了化
		void PBRShader::Finalize()
		{
			vbKinds.clear();
			cbMaterial.reset();
			cbScene.reset();
			light.reset();
			shader.reset();
		}

		// シェーダー開始
		void PBRShader::Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera)
		{
			Bread::Graphics::IContext* context = graphicsDevice->GetContext();

			Bread::Graphics::IBuffer* vsCBuffer[] =
			{
				context->GetConstantBufferScene(),
				context->GetConstantBufferMesh(),
				context->GetConstantBufferBone()
			};
			Bread::Graphics::IBuffer* psCBuffer[] =
			{
				cbMaterial.get(),
				cbScene.get()
			};
			context->SetConstantBuffers(Bread::Graphics::ShaderType::Vertex, 0, Bread::FND::ArraySize(vsCBuffer), vsCBuffer);
			context->SetConstantBuffers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(psCBuffer), psCBuffer);

			Bread::Graphics::ISampler* sampler[] =
			{
				context->GetSamplerState(Bread::Graphics::SamplerState::LinearWrap)
			};
			context->SetSamplers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(sampler), sampler);

			// 頂点シェーダー用シーン定数バッファ更新
			{
				context->UpdateConstantBufferScene(camera.GetView(), camera.GetProjection());
			}

			// ピクセルシェーダー用シーンバッファ更新
			{
				CbScene cb = {};
				cb.dirLight[0].direction = light->direction;
				cb.dirLight[1].direction = Math::Vector4(1.0f, 0.0f, 0.0f, 1.0f); // sunLight->direction Math::Vector4(1.0f, 0.0f, 0.0f, 1.0f) // 太陽からの方向
				cb.dirLight[0].color     = light->color;
				cb.dirLight[1].color     = sunLight->color;
				cb.pointLight.position   = pointLight->position;
				cb.pointLight.color      = pointLight->color;
				cb.pointLight.distance   = pointLight->distance;
				cb.pointLight.decay      = pointLight->decay;
				cb.pointLight.dummy      = pointLight->dummy;
				cb.viewPos = Math::Vector4(camera.GetEye(), 0.0f);
				context->UpdateSubresource(cbScene.get(), 0, 0, &cb, 0, 0);
			}

			shader->Activate(graphicsDevice->GetDevice());
		}

		// 描画
		void PBRShader::Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model)
		{
			Bread::Graphics::IContext* context       = graphicsDevice->GetContext();

			Graphics::IModelResource*  modelResource = model->GetModelResource();

			CbMaterial cb = {};
			cb.albedo        = material->albedo;
			cb.metallic      = material->metallic;
			cb.roughness  = material->roughness;
			cb.padding01  = 0.0f;
			cb.padding02  = 0.0f;
			context->UpdateSubresource(cbMaterial.get(), 0, 0, &cb, 0, 0);

			// メッシュ定数バッファ更新
			context->UpdateConstantBufferMesh(worldTransform);

			for (u32 i = 0; i < modelResource->GetMeshSize(); ++i)
			{
				if (0 < model->GetMeshNodes())
				{
					context->UpdateConstantBufferBone(model->GetBoneTransforms(i), model->GetBoneTransformCount(i));
				}

				Graphics::IMesh* mesh = modelResource->GetMesh(i);

				for (const Graphics::ModelData::Subset& subset : modelResource->GetModelData().meshes[i].subsets)
				{
					u32 size = static_cast<u32>(model->GetTextureSize(subset.materialIndex));
					for (u32 j = 0; j < size; ++j)
					{
						Graphics::ITexture* texture[] = { model->GetTexture(subset.materialIndex, j) };
						context->SetShaderResources(Graphics::ShaderType::Pixel, j, 1, texture);
					}
					mesh->Draw(graphicsDevice->GetDevice(), GetVectexBuferKinds(), static_cast<u32>(GetVectexBuferKindsSize()), subset.startIndex, subset.indexCount, Graphics::PrimitiveTopology::TriangleList);
				}
			}
		}

		// シェーダー終了
		void PBRShader::End(Graphics::IGraphicsDevice* graphicsDevice)
		{
			shader->Deactivate(graphicsDevice->GetDevice());
		}


		// 生成
		std::unique_ptr<IShader> PBRSkinShader::Create()
		{
			return std::make_unique<PBRSkinShader>();
		}

		// 初期化
		bool PBRSkinShader::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Bread::Graphics::PhoenixInputElementDesc inputElementDesc[] =
			{
				// SemanticName	 SemanticIndex	Format														InputSlot	AlignedByteOffset	InputSlotClass										InstanceDataStepRate
				{"POSITION",	 0,				Bread::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT,			0,			0,					Bread::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"NORMAL",		 0,				Bread::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT,			1,			0,					Bread::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"TANGENT",		 0,				Bread::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT,			2,			0,					Bread::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"TEXCOORD",	 0,				Bread::Graphics::PHOENIX_FORMAT_R32G32_FLOAT,				3,			0,					Bread::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
			};

			shader = Graphics::IShader::Create();
			shader->LoadVS
			(
				graphicsDevice->GetDevice(),
				"PhysicallyBasedRenderingSkinVS.cso",
				inputElementDesc,
				Bread::FND::ArraySize(inputElementDesc)
			);
			shader->LoadPS
			(
				graphicsDevice->GetDevice(),
				"PhysicallyBasedRenderingSkinPS.cso"
			);

			cbMaterial = Bread::Graphics::IBuffer::Create();
			cbScene    = Bread::Graphics::IBuffer::Create();
			{
				Bread::Graphics::PhoenixBufferDesc desc = {};
				Bread::FND::MemSet(&desc, 0, sizeof(desc));
				desc.usage               = Bread::Graphics::PhoenixUsage::Default;
				desc.bindFlags           = static_cast<Bread::s32>(Bread::Graphics::PhoenixBindFlag::ConstantBuffer);
				desc.cpuAccessFlags      = 0;
				desc.miscFlags           = 0;
				desc.structureByteStride = 0;

				desc.byteWidth = sizeof(CbMaterial);
				if (!cbMaterial->Initialize(graphicsDevice->GetDevice(), desc))
				{
					return false;
				}

				desc.byteWidth = sizeof(CbScene);
				if (!cbScene->Initialize(graphicsDevice->GetDevice(), desc))
				{
					return false;
				}
			}

			vbKinds.emplace_back(Graphics::VertexBufferKind::Position);
			vbKinds.emplace_back(Graphics::VertexBufferKind::Normal);
			vbKinds.emplace_back(Graphics::VertexBufferKind::Tangent);
			vbKinds.emplace_back(Graphics::VertexBufferKind::TexCoord0);

			light    = std::make_unique<LightState>();
			material = std::make_unique<MaterialState>();

			light->direction = Math::Vector4(0.0f, 1.0f, 0.0f, 1.0f);
			light->color     = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);

			material->albedo    = Math::Vector4(0.875f, 0.875f, 0.875f, 1.0f);
			material->metallic  = 0.6f;
			material->roughness = 1.0f;

			return true;
		}

		// 終了化
		void PBRSkinShader::Finalize()
		{
			vbKinds.clear();
			cbMaterial.reset();
			cbScene.reset();
			light.reset();
			shader.reset();
		}

		// シェーダー開始
		void PBRSkinShader::Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera)
		{
			Bread::Graphics::IContext* context = graphicsDevice->GetContext();

			Bread::Graphics::IBuffer* vsCBuffer[] =
			{
				context->GetConstantBufferScene(),
				context->GetConstantBufferMesh(),
				context->GetConstantBufferBone()
			};
			Bread::Graphics::IBuffer* psCBuffer[] =
			{
				cbMaterial.get(),
				cbScene.get()
			};
			context->SetConstantBuffers(Bread::Graphics::ShaderType::Vertex, 0, Bread::FND::ArraySize(vsCBuffer), vsCBuffer);
			context->SetConstantBuffers(Bread::Graphics::ShaderType::Pixel,  0, Bread::FND::ArraySize(psCBuffer), psCBuffer);

			Bread::Graphics::ISampler* sampler[] =
			{
				context->GetSamplerState(Bread::Graphics::SamplerState::LinearWrap)
			};
			context->SetSamplers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(sampler), sampler);

			// 頂点シェーダー用シーン定数バッファ更新
			{
				context->UpdateConstantBufferScene(camera.GetView(), camera.GetProjection());
			}

			// ピクセルシェーダー用シーンバッファ更新
			{
				CbScene cb = {};
				cb.dirLight[0].direction = light->direction;
				cb.dirLight[1].direction = Math::Vector4(1.0f, 0.0f, 0.0f, 1.0f); // 太陽からの方向
				cb.dirLight[0].color     = light->color;
				cb.dirLight[1].color     = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
				cb.viewPos               = Math::Vector4(camera.GetEye(), 0.0f);
				context->UpdateSubresource(cbScene.get(), 0, 0, &cb, 0, 0);
			}

			shader->Activate(graphicsDevice->GetDevice());
		}

		// 描画
		void PBRSkinShader::Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model)
		{
			Bread::Graphics::IContext* context = graphicsDevice->GetContext();

			Graphics::IModelResource* modelResource = model->GetModelResource();

			CbMaterial cb = {};
			cb.albedo    = material->albedo;
			cb.metallic  = material->metallic;
			cb.roughness = material->roughness;
			cb.padding01 = 0.0f;
			cb.padding02 = 0.0f;
			context->UpdateSubresource(cbMaterial.get(), 0, 0, &cb, 0, 0);

			// メッシュ定数バッファ更新
			context->UpdateConstantBufferMesh(worldTransform);

			for (u32 i = 0; i < modelResource->GetMeshSize(); ++i)
			{
				/*if (0 < model->GetMeshNodes())
				{
					graphicsDevice->GetContext()->UpdateConstantBufferBone(model->GetBoneTransforms(i), model->GetBoneTransformCount(i));
				}*/

				Graphics::IMesh* mesh = modelResource->GetMesh(i);

				for (const Graphics::ModelData::Subset& subset : modelResource->GetModelData().meshes[i].subsets)
				{
					u32 size = static_cast<u32>(model->GetTextureSize(subset.materialIndex));
					for (u32 j = 0; j < size; ++j)
					{
						Graphics::ITexture* texture[] = { model->GetTexture(subset.materialIndex, j) };
						context->SetShaderResources(Graphics::ShaderType::Pixel, j, 1, texture);
					}
					mesh->Draw(graphicsDevice->GetDevice(), GetVectexBuferKinds(), static_cast<u32>(GetVectexBuferKindsSize()), subset.startIndex, subset.indexCount, Graphics::PrimitiveTopology::TriangleList);
				}
			}
		}

		// シェーダー終了
		void PBRSkinShader::End(Graphics::IGraphicsDevice* graphicsDevice)
		{
			shader->Deactivate(graphicsDevice->GetDevice());
		}
	}
}