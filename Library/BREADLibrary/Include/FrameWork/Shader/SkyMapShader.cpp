#include "pch.h"
#include "FrameWork/Shader/SkyMapShader.h"
#include "FND/Util.h"
#include "FND/STD.h"


namespace Bread
{
	namespace FrameWork
	{
		// 生成
		std::unique_ptr<IShader> SkyMapShader::Create()
		{
			return std::make_unique<SkyMapShader>();
		}

		// 初期化
		bool SkyMapShader::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Bread::Graphics::BreadInputElementDesc inputElementDesc[] =
			{
				// SemanticName	 SemanticIndex	Format														InputSlot	AlignedByteOffset	InputSlotClass										InstanceDataStepRate
				{"POSITION",	 0,				Bread::Graphics::BREAD_FORMAT_R32G32B32_FLOAT,			0,			0,					Bread::Graphics::BREAD_INPUT_PER_VERTEX_DATA,	0 },
				{"NORMAL",		 0,				Bread::Graphics::BREAD_FORMAT_R32G32B32_FLOAT,			1,			0,					Bread::Graphics::BREAD_INPUT_PER_VERTEX_DATA,	0 },
			};

			shader = Graphics::IShader::Create();
			shader->LoadVS
			(
				graphicsDevice->GetDevice(),
				"SkyMapVS.cso",
				inputElementDesc,
				Bread::FND::ArraySize(inputElementDesc)
			);
			shader->LoadPS
			(
				graphicsDevice->GetDevice(),
				"SkyMapPS.cso"
			);

			vbKinds.emplace_back(Graphics::VertexBufferKind::Position);
			vbKinds.emplace_back(Graphics::VertexBufferKind::Normal);

			return true;
		}

		// 終了化
		void SkyMapShader::Finalize()
		{
			vbKinds.clear();
			shader.reset();
		}

		// シェーダー開始
		void SkyMapShader::Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera)
		{
			Bread::Graphics::IContext* context = graphicsDevice->GetContext();

			Bread::Graphics::IBuffer* vsCBuffer[] =
			{
				context->GetConstantBufferScene(),
				context->GetConstantBufferMesh(),
				//context->GetConstantBufferBone()
			};
			context->SetConstantBuffers(Bread::Graphics::ShaderType::Vertex, 0, Bread::FND::ArraySize(vsCBuffer), vsCBuffer);
			//context->SetConstantBuffers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(psCBuffer), psCBuffer);

			Bread::Graphics::ISampler* sampler[] =
			{
				context->GetSamplerState(Bread::Graphics::SamplerState::LinearWrap)
			};
			context->SetSamplers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(sampler), sampler);

			// シーン定数バッファ更新
			context->UpdateConstantBufferScene(camera.GetView(), camera.GetProjection());

			shader->Activate(graphicsDevice->GetDevice());
		}

		// 描画
		void SkyMapShader::Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model)
		{
			Bread::Graphics::IContext* context = graphicsDevice->GetContext();

			Graphics::IModelResource* modelResource = model->GetModelResource();
			Graphics::ModelData       modelData     = modelResource->GetModelData();

			// メッシュ定数バッファ更新
			context->UpdateConstantBufferMesh(worldTransform);

			for (u32 i = 0; i < modelResource->GetMeshSize(); ++i)
			{
				if (0 < model->GetMeshNodes())
				{
					graphicsDevice->GetContext()->UpdateConstantBufferBone(model->GetBoneTransforms(i), model->GetBoneTransformCount(i));
				}

				Graphics::IMesh*          mesh     = modelResource->GetMesh(i);
				Graphics::ModelData::Mesh meshData = modelData.meshes[i];

				for (Graphics::ModelData::Subset& subset : meshData.subsets)
				{
					u32 size = static_cast<u32>(model->GetTextureSize(subset.materialIndex));
					for (u32 j = 0; j < size; ++j)
					{
						Graphics::ITexture* texture[] = { model->GetTexture(subset.materialIndex, j) };
						graphicsDevice->GetContext()->SetShaderResources(Graphics::ShaderType::Pixel, j, 1, texture);
					}
					mesh->Draw(graphicsDevice->GetDevice(), GetVectexBuferKinds(), static_cast<u32>(GetVectexBuferKindsSize()), subset.startIndex, subset.indexCount, Graphics::PrimitiveTopology::TriangleList);
				}
			}
		}

		// シェーダー終了
		void SkyMapShader::End(Graphics::IGraphicsDevice* graphicsDevice)
		{
			shader->Deactivate(graphicsDevice->GetDevice());
		}
	}
}