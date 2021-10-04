#pragma once

#include "Graphics/Shader.h"
#include "Graphics/GraphicsDevice.h"


namespace Bread
{
	namespace FrameWork
	{
		class ComputeShaderBufferFactor
		{
		public:
			// コンピュートシェーダー用バッファ作成関数
			static bool CreateStructuredBuffer(Graphics::IDevice* device, Bread::Graphics::BreadUsage usage, u32 bindFlags, u32 byteWidth, u32 structureByteStride, s32 miscFlags, void* initData, Graphics::IBuffer* buffer);

			// コンピュートシェーダー用テクスチャ作成関数
			static bool CreateBufferSRV(Graphics::IDevice* device, Graphics::IBuffer* buffer, Graphics::ITexture* shaderResouceView, Graphics::TextureFormatDx format = Graphics::TextureFormatDx::UNKNOWN, u32 byteWidth = 0, u32 structureByteStride = 0);
			static bool CreateBufferUAV(Graphics::IDevice* device, Graphics::IBuffer* buffer, Graphics::ITexture* shaderResouceView, Graphics::TextureFormatDx format = Graphics::TextureFormatDx::UNKNOWN, u32 byteWidth = 0, u32 structureByteStride = 0);

			// コンピュートシェーダー用データ抽出関数
			static void CreateAndCopyToBuffer(Graphics::IDevice* device, Graphics::IContext* context, Graphics::IBuffer* buffer, Graphics::IBuffer* copyBuffer);
		};

		//****************************************************************************
		// コンピュートシェーダー基底クラス
		//****************************************************************************
		class IComputeShader
		{
		public:
			IComputeShader() {}
			virtual ~IComputeShader() {}
		
		public:
			// 生成
			// static std::unique_ptr<IShader> Create(); // 各継承クラスで作成してください。
		
			// 初期化
			virtual bool Initialize(Graphics::IGraphicsDevice* graphicsDevice) = 0;
		
			// 終了化
			virtual void Finalize() = 0;

			// 実行
			virtual void Run(Graphics::IGraphicsDevice* graphicsDevice) = 0;
		};
	} // namespace FrameWork
} // namespace Bread