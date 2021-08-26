#include "pch.h"
#include "FrameWork/ComputeShader/TestComputeShader.h"
#include "FND/Util.h"
#include "FND/STD.h"


namespace Bread
{
	namespace FrameWork
	{
		// ê∂ê¨
		std::unique_ptr<IComputeShader> TestComputeShader::Create()
		{
			return std::make_unique<TestComputeShader>();
		}

		// èâä˙âª
		bool TestComputeShader::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();

			shader = Graphics::IComputeShader::Create();
			shader->Load(device, "TestComputeShader.cso");

			for (int i = 0; i < particleMaxSize; i++)
			{
				particle[i].pos = Math::Vector3(0.0f, 0.0f, 0.0f);
				resultParticle[i].pos = Math::Vector3(0.0f, 0.0f, 0.0f);
			}

			particleBuffer01 = Bread::Graphics::IBuffer::Create();
			if (!ComputeShaderBufferFactor::CreateStructuredBuffer(device, Bread::Graphics::PhoenixUsage::Default, static_cast<Bread::u32>(Bread::Graphics::PhoenixBindFlag::UnorderedAccess) | static_cast<Bread::u32>(Bread::Graphics::PhoenixBindFlag::ShaderResource), sizeof(ParticleParameter) * particleMaxSize, particleMaxSize, static_cast<Bread::s32>(Bread::Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferStructured), &particle[0], particleBuffer01.get()))
			{
				return false;
			}

			particleBufferResult = Bread::Graphics::IBuffer::Create();
			if (!ComputeShaderBufferFactor::CreateStructuredBuffer(device, Bread::Graphics::PhoenixUsage::Default, static_cast<Bread::u32>(Bread::Graphics::PhoenixBindFlag::UnorderedAccess) | static_cast<Bread::u32>(Bread::Graphics::PhoenixBindFlag::ShaderResource), sizeof(ParticleParameter) * particleMaxSize, particleMaxSize, static_cast<Bread::s32>(Bread::Graphics::PhoenixResouceMiscFlag::ResouceMiscBufferStructured), nullptr, particleBufferResult.get()))
			{
				return false;
			}

			particleBufferSRV01 = Bread::Graphics::ITexture::Create();
			if (!ComputeShaderBufferFactor::CreateBufferSRV(device, particleBuffer01.get(), particleBufferSRV01.get()))
			{
				return false;
			}

			particleBufferUAV = Bread::Graphics::ITexture::Create();
			if (!ComputeShaderBufferFactor::CreateBufferUAV(device, particleBufferResult.get(), particleBufferUAV.get()))
			{
				return false;
			}

			return true;
		}

		// èIóπâª
		void TestComputeShader::Finalize()
		{
			shader.reset();
		}

		// é¿çs
		void TestComputeShader::Run(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();
			Graphics::IContext* context = graphicsDevice->GetContext();

			shader->Activate(device);
			{
				Graphics::ITexture* texture[] = { particleBufferSRV01.get() };
				context->SetShaderResources(Graphics::ShaderType::Compute, 0, 1, texture);

				Graphics::ITexture* uavTexture[] = { particleBufferUAV.get() };
				context->SetUnorderedAccess(0, 1, uavTexture, nullptr);

				shader->Dispatch(device, particleMaxSize, 1, 1);

				Graphics::ITexture* nullUAVTexture[] = { nullptr };
				context->SetUnorderedAccess(0, 1, nullUAVTexture, nullptr);

				Graphics::ITexture* nullTexture[] = { nullptr };
				context->SetShaderResources(Graphics::ShaderType::Compute, 0, 1, nullTexture);
			}
			shader->Deactivate(device);

			std::unique_ptr<Graphics::IBuffer> copyBuffer = Graphics::IBuffer::Create();
			ComputeShaderBufferFactor::CreateAndCopyToBuffer(device, context, particleBufferResult.get(), copyBuffer.get());

			Graphics::PhoenixMap map = Graphics::PhoenixMap::Read;
			Graphics::PhoenixMappedSubresource mapedBuffer;
			ParticleParameter* result = nullptr;
			{
				context->Map(copyBuffer.get(), 0, map, 0, &mapedBuffer);
				result = static_cast<ParticleParameter*>(mapedBuffer.data);
				context->Unmap(copyBuffer.get(), 0);
			}

			for (int i = 0; i < particleMaxSize; i++)
			{
				resultParticle[i].pos = result[i].pos;
			}
		}
	}
}