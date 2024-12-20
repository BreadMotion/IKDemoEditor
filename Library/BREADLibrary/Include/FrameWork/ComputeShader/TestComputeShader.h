#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Buffer.h"
#include "FrameWork/ComputeShader/ComputeShader.h"


namespace Bread
{
	namespace FrameWork
	{
		struct ParticleParameter
		{
			Math::Vector3 pos;
		};

		class TestComputeShader : public IComputeShader
		{
		public:
			static const u32 particleMaxSize = 32;

		private:
			std::unique_ptr<Graphics::IComputeShader> shader;

			std::unique_ptr<Graphics::IBuffer> particleBuffer01;
			std::unique_ptr<Graphics::IBuffer> particleBuffer02;
			std::unique_ptr<Graphics::IBuffer> particleBufferResult;

			std::unique_ptr<Graphics::ITexture> particleBufferSRV01;
			std::unique_ptr<Graphics::ITexture> particleBufferSRV02;
			std::unique_ptr<Graphics::ITexture> particleBufferUAV;

			ParticleParameter particle[particleMaxSize];

		public:
			ParticleParameter resultParticle[particleMaxSize];

		public:
			TestComputeShader() {}
			~TestComputeShader() {}

		public:
			// 生成
			static std::unique_ptr<IComputeShader> Create();

			// 初期化
			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice) override;

			// 終了化
			void Finalize() override;

			// 実行
			void Run(Graphics::IGraphicsDevice* graphicsDevice) override;
		};
	}
}