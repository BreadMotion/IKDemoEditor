#include "pch.h"
#include "FrameWork/Shader/BasicShader.h"
#include "FND/Util.h"
#include "FND/STD.h"


namespace Bread
{
	namespace FrameWork
	{
		// 生成
		std::unique_ptr<IShader> BasicShader::Create()
		{
			return std::make_unique<BasicShader>();
		}

		// 初期化
		bool BasicShader::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Bread::Graphics::BreadInputElementDesc inputElementDesc[] =
			{
				// SemanticName	 SemanticIndex	Format														InputSlot	AlignedByteOffset	InputSlotClass										InstanceDataStepRate
				{"POSITION",	 0,				Bread::Graphics::BREAD_FORMAT_R32G32B32_FLOAT,			    0,			0,					Bread::Graphics::BREAD_INPUT_PER_VERTEX_DATA,	    0 },
				{"TEXCOORD",	 0,				Bread::Graphics::BREAD_FORMAT_R32G32_FLOAT,		            1,			0,					Bread::Graphics::BREAD_INPUT_PER_VERTEX_DATA,	    0 },
			};

			shader = Graphics::IShader::Create();
			shader->LoadVS
			(
				graphicsDevice->GetDevice(),
				"BasicVS.cso",
				inputElementDesc,
				Bread::FND::ArraySize(inputElementDesc)
			);
			shader->LoadPS
			(
				graphicsDevice->GetDevice(),
				"BasicPS.cso"
			);

			cbMatrial = Bread::Graphics::IBuffer::Create();
			{
				Bread::Graphics::BreadBufferDesc desc = {};
				Bread::FND::MemSet(&desc, 0, sizeof(desc));
				desc.usage                     = Bread::Graphics::BreadUsage::Default;
				desc.bindFlags               = static_cast<Bread::s32>(Bread::Graphics::BreadBindFlag::ConstantBuffer);
				desc.cpuAccessFlags      = 0;
				desc.miscFlags                = 0;
				desc.byteWidth               = sizeof(CbMaterial);
				desc.structureByteStride = 0;
				if (!cbMatrial->Initialize(graphicsDevice->GetDevice(), desc))
				{
					return false;
				}
			}

			vbKinds.emplace_back(Graphics::VertexBufferKind::Position);
			vbKinds.emplace_back(Graphics::VertexBufferKind::TexCoord0);

			return true;
		}

		// 終了化
		void BasicShader::Finalize()
		{
			vbKinds.clear();
			cbMatrial.reset();
			shader.reset();
		}

		// シェーダー開始
		void BasicShader::Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera)
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
				cbMatrial.get()
			};
			context->SetConstantBuffers(Bread::Graphics::ShaderType::Vertex, 0, Bread::FND::ArraySize(vsCBuffer), vsCBuffer);
			context->SetConstantBuffers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(psCBuffer), psCBuffer);

			Bread::Graphics::ISampler* sampler[] =
			{
				context->GetSamplerState(Bread::Graphics::SamplerState::LinearWrap)
			};
			context->SetSamplers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(sampler), sampler);

			// シーン定数バッファ更新
			context->UpdateConstantBufferScene(camera.GetView(), camera.GetProjection());

			// メッシュ定数バッファ更新
			CbMaterial cb = {};
			//cb.color = { 5.0f, 5.0f, 5.0f, 1.0f };
			cb.color = { 1.0f, 1.0f, 1.0f, 1.0f };
			context->UpdateSubresource(cbMatrial.get(), 0, 0, &cb, 0, 0);

			shader->Activate(graphicsDevice->GetDevice());
		}

		// 描画
		void BasicShader::Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model)
		{
			Bread::Graphics::IContext*    context              = graphicsDevice->GetContext();

			Graphics::IModelResource*  modelResource = model->GetModelResource();

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
		void BasicShader::End(Graphics::IGraphicsDevice* graphicsDevice)
		{
			shader->Deactivate(graphicsDevice->GetDevice());
		}
	}
}