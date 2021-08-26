#pragma once

#include <d3d11.h>
#include "Graphics/Buffer.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版バッファー操作オブジェクト
		//****************************************************************************
		class BufferDX11 final : public IBuffer
		{
		private:
			ID3D11Buffer* buffer = nullptr;

		public:
			BufferDX11() {}
			~BufferDX11() { Finalize(); }

		public:
			// 初期化
			bool Initialize(IDevice* device, const PhoenixBufferDesc& desc) override;
			bool Initialize(IDevice* device, const PhoenixBufferDesc& desc, const PhoenixSubresourceData& data) override;

			// 終了化
			void Finalize() override;

			// 情報取得
			void GetDesc(PhoenixBufferDesc* desc) override;

			// D3Dバッファー取得
			ID3D11Buffer* GetD3DBuffer() { return buffer; }
		};
	} // namespace Graphics
} // namespace Bread