#include "pch.h"
#include "FrameWork/IBL/IBL.h"
#include "FND/Util.h"
#include "FND/Instance.h"

#include "../Source/Graphics/GraphicsDevice/Win/DirectX11/GraphicsDeviceDX11.h"

using Bread::FND::Instance;
using Bread::FND::SharedInstance;

namespace Bread
{
	namespace FrameWork
	{
		std::unique_ptr<IBL> IBL::Create()
		{
			return std::make_unique<IBL>();
		}

		void IBL::Initialize()
		{
			skyIBL = FrameBuffer::Create();
			skyIBL->Initialize
			(
				SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(),
				64, 64, false, 1,
				Graphics::TextureFormatDx::R16G16B16A16_FLOAT,
				Graphics::TextureFormatDx::UNKNOWN,
				true,
				false,
				true,
				true,
				6
			);

			// Camera
			{
				Math::Vector3 pos   = Math::Vector3(0.0f, 0.0f, 0.0f);
				Math::Vector4 tar[] = { Math::Vector4(1, 0, 0, 0), Math::Vector4(-1, 0, 0, 0), Math::Vector4(0, 1, 0, 0),  Math::Vector4(0, -1, 0, 0), Math::Vector4(0, 0, 1, 0), Math::Vector4(0, 0, -1, 0) };
				Math::Vector4 up[]  = { Math::Vector4(0, 1, 0, 0), Math::Vector4(0, 1, 0, 0),  Math::Vector4(0, 0, -1, 0), Math::Vector4(0, 0, 1, 0),  Math::Vector4(0, 1, 0, 0), Math::Vector4(0, 1, 0, 0) };

				for (int i = 0; i < cameraSize; ++i)
				{
					Math::Vector4 dir = Math::Vector4Rotate(tar[i], Math::QuaternionIdentity());

					captureCamera[i] = std::make_unique<Bread::Graphics::Camera>();
					captureCamera[i]->SetLookAt(pos, Math::Vector3(dir), Math::Vector3(up[i]));
					captureCamera[i]->SetPerspective(0.5f * 3.141592f, 1.0f, 0.1f, 100.0f);
				}
			}
		}

		void IBL::Finalize()
		{
			skyIBL.reset();
		}

		void IBL::Clear(float r, float g, float b, float a)
		{
			for (int i = 0; i < 6; ++i)
			{
				skyIBL->ClearRenderTargetView(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get(), i, r, g, b, a);
			}
		}

		void IBL::Activate()
		{
			skyIBL->ActivateAllRenderTargetView(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
		}

		void IBL::Deactivate()
		{
			skyIBL->Deactivate(SharedInstance<Graphics::GraphicsDeviceDX11>::instance.get());
		}


		std::unique_ptr<SkyMap> SkyMap::Create()
		{
			return std::make_unique<SkyMap>();
		}

		void SkyMap::Initialize(const s8* cubemapFilename)
		{
			Graphics::IDevice* device = SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice();
			model = FrameWork::ModelObject::Create();
			model->Initialize();
			model->Load("..\\Data\\Assets\\Model\\SkyMap\\sphere.fbx");

			texture = Graphics::ITexture::Create();
			texture->Initialize(device, cubemapFilename, Graphics::MaterialType::Diffuse, Math::Color(1.0f, 1.0f, 1.0f, 1.0f));

			Bread::Graphics::BreadInputElementDesc inputElementDesc[] =
			{
				// SemanticName	 SemanticIndex	Format											InputSlot	AlignedByteOffset	InputSlotClass									InstanceDataStepRate
				{"POSITION",	 0,				Bread::Graphics::BREAD_FORMAT_R32G32B32_FLOAT,	0,			0,					Bread::Graphics::BREAD_INPUT_PER_VERTEX_DATA,	0 }
			};
			shader = Graphics::IShader::Create();
			shader->LoadVS
			(
				device,
				"skyMapVS.cso",
				inputElementDesc,
				Bread::FND::ArraySize(inputElementDesc)
			);
			shader->LoadPS
			(
				device,
				"skyMapPS.cso"
			);

			Graphics::BreadBufferDesc desc = {};
			desc.byteWidth           = sizeof(ShaderConstants);
			desc.bindFlags           = static_cast<u32>(Graphics::BreadBindFlag::ConstantBuffer);
			desc.cpuAccessFlags      = static_cast<u32>(Graphics::BreadCPUAccessFlag::CPUAccessWrite);
			desc.usage               = Graphics::BreadUsage::Dynamic;
			desc.miscFlags           = 0;
			desc.structureByteStride = 0;

			constantBuffer = Graphics::IBuffer::Create();
			constantBuffer->Initialize(device, desc);

			vbKinds.emplace_back(Graphics::VertexBufferKind::Position);
		}

		void SkyMap::Finalize()
		{

		}

		void SkyMap::Draw(const Math::Matrix& world, const Graphics::Camera& camera, const Math::Vector4& lightDirection, const Math::Color& color)
		{
			Graphics::IDevice*         device  = SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice();
			Bread::Graphics::IContext* context = SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetContext();

			shader->Activate(device);
			{
				Bread::Graphics::IBuffer* vsCBuffer[] =
				{
					context->GetConstantBufferScene(),
					context->GetConstantBufferMesh()
				};
				Bread::Graphics::IBuffer* psCBuffer[] =
				{
					constantBuffer.get()
				};
				context->SetConstantBuffers(Bread::Graphics::ShaderType::Vertex, 0, Bread::FND::ArraySize(vsCBuffer), vsCBuffer);
				context->SetConstantBuffers(Bread::Graphics::ShaderType::Pixel,  0, Bread::FND::ArraySize(psCBuffer), psCBuffer);

				Bread::Graphics::ISampler* sampler[] =
				{
					context->GetSamplerState(Bread::Graphics::SamplerState::PointClamp)
				};
				context->SetSamplers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(sampler), sampler);

				// シーン定数バッファ更新
				context->UpdateConstantBufferScene(camera.GetView(), camera.GetProjection());

				// メッシュ定数バッファ更新
				context->UpdateConstantBufferMesh(world);

				ShaderConstants cb = {};
				cb.lightDirection = lightDirection;
				cb.color              = color;
				context->UpdateSubresource(constantBuffer.get(), 0, 0, &cb, 0, 0);

				Graphics::ITexture* textures[] = { texture.get() };
				SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetContext()->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, textures);

				Graphics::IModelResource* modelResource = model->GetModelResource();
				for (u32 i = 0; i < modelResource->GetMeshSize(); ++i)
				{
					if (0 < model->GetMeshNodes())
					{
						SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetContext()->UpdateConstantBufferBone(model->GetBoneTransforms(i), model->GetBoneTransformCount(i));
					}

					Graphics::IMesh* mesh = modelResource->GetMesh(i);

					for (const Graphics::ModelData::Subset& subset : modelResource->GetModelData().meshes[i].subsets)
					{
						mesh->Draw(SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice(), vbKinds.data(), static_cast<u32>(vbKinds.size()), subset.startIndex, subset.indexCount, Graphics::PrimitiveTopology::TriangleList);
					}
				}
			}
			shader->Deactivate(device);
		}
	}
}