#include "pch.h"
#include "GraphicsDeviceDX11.h"
#include "Context/Win/DirectX11/ContextDX11.h"
#include "Device/Win/DirectX11/DeviceDX11.h"


namespace Bread
{
	namespace Graphics
	{
		// 生成
		std::shared_ptr<IGraphicsDevice> IGraphicsDevice::Create()
		{
			return std::make_shared<GraphicsDeviceDX11>();
		}

		// 初期化
		bool GraphicsDeviceDX11::Initialize(OS::IDisplay* display)
		{
			device = Bread::Graphics::IDevice::Create();
			if (!device->Initialize())
			{
				return false;
			}

			context = Bread::Graphics::IContext::Create();
			if (!context->Initialize(device.get()))
			{
				return false;
			}
			//if (!context->Initialize_DF(device.get()))
			//{
			//	return false;
			//}

			swapChain = Bread::Graphics::ISwapChain::Create();
			Bread::Graphics::SwapChainDesc desc = {};
			desc.width        = display->GetWidth();
			desc.heigth       = display->GetHeight();
			desc.windowHandle = reinterpret_cast<void*>(display->GetHandle());

			if (!swapChain->Initialize(device.get(), desc))
			{
				return false;
			}

			return true;
		}

		// 終了化
		void GraphicsDeviceDX11::Finalize()
		{
			swapChain->Finalize();
			context->Finalize();
			device->Finalize();
		}

		// レンダリングされたイメージを表示
		void GraphicsDeviceDX11::Present(int syncInterval)
		{
			swapChain->Present(syncInterval);
		}

		// 描画開始
		void GraphicsDeviceDX11::RenderBegin()
		{
			Bread::Graphics::IRenderTargetSurface* nullRTS[8]      = {};
			Bread::Graphics::IDepthStencilSurface* nullDSS[8]      = {};
			Bread::Graphics::ITexture*                      nullTexture[8] = {};
			context->SetRenderTargets(8, nullRTS, 0);
			context->SetShaderResources(ShaderType::Vertex, 0, 8, nullTexture);
			context->SetShaderResources(ShaderType::Pixel,  0, 8, nullTexture);

			ContextDX11* contextDX11 = static_cast<ContextDX11*>(context.get());

			ISampler* samplers[] =
			{
				contextDX11->GetSamplerState(SamplerState::LinearWrap)
			};
			context->SetSamplers(ShaderType::Vertex, 0, 1, samplers);
			context->SetSamplers(ShaderType::Pixel,   0, 1,  samplers);
			context->SetRasterizer(contextDX11->GetRasterizerState(rasterizerState));
			context->SetBlend(contextDX11->GetBlendState(BlendState::AlphaBlend), 0, 0xFFFFFFFF);

			float color[] = { 0.5f, 0.5f, 0.5f, 1.0f };
			Bread::Graphics::IRenderTargetSurface* rts = swapChain->GetRenderTargerSurface();
			Bread::Graphics::IDepthStencilSurface* dss = swapChain->GetDepthStencilSurface();

			context->ClearRenderTargetView(rts, color);
			context->ClearDepthStencilView(dss, 1.0f, 0);

			context->SetRenderTargets(1, &rts, dss);
			context->SetDepthStencil(contextDX11->GetDepthStencilState(DepthState::TestAndWrite), 1);
		}

		// 描画終了
		void GraphicsDeviceDX11::RenderEnd()
		{
			// No Data.
		}

		//TODO : とりあえずここに処理を追加しておく
		//描画命令を記録する
		void GraphicsDeviceDX11::RecordRenderCommand()
		{
			ID3D11DeviceContext* d3dDfContext = static_cast<DeviceDX11*>(device.get())->GetD3DDfContext();
			d3dDfContext->FinishCommandList(true, commandLists.GetAddressOf());
		}

		//TODO : とりあえずここに処理を追加しておく
		//描画命令を実行する
		void GraphicsDeviceDX11::RenderCommandRun()
		{
			ID3D11DeviceContext* d3dContext  = static_cast<DeviceDX11*>(device.get())->GetD3DContext();

			d3dContext->ExecuteCommandList(commandLists.Get(), false);
			commandLists.Reset();
		}
	} // namespace Graphics
} // namespace Bread
