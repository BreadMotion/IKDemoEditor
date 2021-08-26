#pragma once

#include <d3d11.h>
#include <vector>

#include "Graphics/SwapChain.h"
#include "Graphics/Surface.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版スワップチェーン操作オブジェクト
		//****************************************************************************
		class SwapChainDX11 final : public ISwapChain
		{
		private:
			IDXGISwapChain* swapChain = nullptr;
			std::unique_ptr<IRenderTargetSurface> renderTargerSurface;
			std::unique_ptr<IDepthStencilSurface> depthStencilSurface;

		public:
			SwapChainDX11() {}
			~SwapChainDX11() override { Finalize(); }

		public:
			// 初期化
			bool Initialize(IDevice* device, const SwapChainDesc& desc) override;

			// 終了化
			void Finalize() override;

			// 画面表示
			void Present(int syncInterval) override;

			// レンダーターゲットサーフェイス取得
			IRenderTargetSurface* GetRenderTargerSurface() override { return renderTargerSurface.get(); }

			// 深度ステンシルサーフェイス取得
			IDepthStencilSurface* GetDepthStencilSurface() override { return depthStencilSurface.get(); }

			// スワップチェーン取得
			IDXGISwapChain* GetDXGISwapChain() const { return swapChain; }

		private:
			// スワップチェーン作成
			bool CreateSwapChain(ID3D11Device* device, IDXGIFactory* dxgiFactory, HWND hwnd, unsigned int width, unsigned int height, bool enableMsaa = false, int subSamples = 1);
			
			// バックバッファ作成
			bool CreateBackBuffer(IDevice* device, unsigned int width, unsigned int height);
		};
	} // namespace Graphics
} // namespace Bread