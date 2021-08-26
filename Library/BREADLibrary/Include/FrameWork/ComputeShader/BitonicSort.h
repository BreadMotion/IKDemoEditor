#pragma once

#include <memory>
#include "Math/BreadMath.h"
#include "Graphics/Shader.h"
#include "Graphics/Buffer.h"
#include "FrameWork/ComputeShader/ComputeShader.h"


namespace Bread
{
	namespace FrameWork
	{
		struct SortData
		{
			f32 key;	// ソートしたい値
			u32 index;	// 一緒にソートされるIndex
		};

		class BitonicSort
		{
		public:
			static const u32 threadNumX = 64;

		private:
			struct ConstantBuffer
			{
				f32 inc;
				f32 dir;
				f32 dummy[2];
			};

		private:
			std::unique_ptr<Graphics::IComputeShader> shader;
			std::unique_ptr<Graphics::IBuffer> buffer;
			std::unique_ptr<Graphics::ITexture> bufferUAV;

			std::unique_ptr<Graphics::IBuffer> constantBuffer;

		public:
			BitonicSort() {}
			~BitonicSort() { Finalize(); }

		public:
			// 生成
			static std::unique_ptr<BitonicSort> Create();

			// 初期化
			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			// 終了化
			void Finalize();

			/// <summary>
			/// バイトニックソートの実行関数
			/// </summary>
			/// <param name="graphicsDevice"> グラフィックスデバイス </param>
			/// <param name="data"> データ配列の先頭アドレス </param>
			/// <param name="dataMaxSize"> データ配列の最大サイズ (注意 : 64以下) </param>
			void Run(Graphics::IGraphicsDevice* graphicsDevice, SortData* data, u32 dataMaxSize);

		private:
			/// <summary>
			/// バッファの作成関数
			/// </summary>
			/// <param name="device"> デバイス </param>
			/// <param name="data"> データ配列の先頭アドレス </param>
			/// <param name="dataMaxSize"> データ配列の最大サイズ </param>
			bool Set(Graphics::IDevice* device, SortData* data, u32 dataMaxSize);
		};
	}
}