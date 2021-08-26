#include "pch.h"
#include "FrameWork/Renderer/ModelRenderer.h"
#include "Graphics/Model.h"
#include "FND/Util.h"


namespace Bread
{
	namespace FrameWork
	{
		void ModelRenderer::Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera)
		{
			Bread::Graphics::IContext* context = graphicsDevice->GetContext();

			// シーン定数バッファ更新
			context->UpdateConstantBufferScene(camera.GetView(), camera.GetProjection());
		}

		void ModelRenderer::Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model, IShader* shader)
		{
			Bread::Graphics::IContext* context = graphicsDevice->GetContext();

			Graphics::IModelResource* modelResource = model->GetModelResource();
			Graphics::ModelData modelData = modelResource->GetModelData();

			// メッシュ定数バッファ更新
			context->UpdateConstantBufferMesh(worldTransform);

			for (s32 i = 0; i < modelResource->GetMeshSize(); ++i)
			{
				//for (u32 i = 0; i < model->GetMaterialSize(); ++i)
				{
					/*u32 size = model->GetTextureSize(i);
					for (u32 j = 0; j < size; ++j)
					{
						Graphics::ITexture* texture[] = { model->GetTexture(i, j) };
						graphicsDevice->GetContext()->SetShaderResources(Graphics::ShaderType::Pixel, size * i + j, 1, texture);
					}*/

					u32 size = model->GetTextureSize(i);
					for (u32 j = 0; j < size; ++j)
					{
						Graphics::ITexture* texture[] = { model->GetTexture(i, j) };
						graphicsDevice->GetContext()->SetShaderResources(Graphics::ShaderType::Pixel, j, 1, texture);
					}
				}

				if (0 < model->GetMeshNodes())
				{
					graphicsDevice->GetContext()->UpdateConstantBufferBone(model->GetBoneTransforms(i), model->GetBoneTransformCount(i));
				}

				Graphics::IMesh* mesh = modelResource->GetMesh(i);
				Graphics::ModelData::Mesh meshData = modelData.meshes[i];

				for (Graphics::ModelData::Subset& subset : meshData.subsets)
				{
					mesh->Draw(graphicsDevice->GetDevice(), shader->GetVectexBuferKinds(), shader->GetVectexBuferKindsSize(), subset.startIndex, subset.indexCount, Graphics::PrimitiveTopology::TriangleList);
				}
			}
		}

		void ModelRenderer::End(Graphics::IGraphicsDevice* graphicsDevice)
		{
			// No data.
		}
	}
}