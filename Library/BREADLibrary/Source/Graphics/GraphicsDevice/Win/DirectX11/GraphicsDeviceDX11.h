#pragma once
#include "Graphics/GraphicsDevice.h"

//TODO : 作成場所を考えること
//テスト用 deferrdContextのCommandlist変数を作るよう
#include <wrl/client.h>

namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版グラフィックスデバイス操作オブジェクト
		//****************************************************************************
		class GraphicsDeviceDX11 final : public IGraphicsDevice
		{
		private:
			std::unique_ptr<IDevice>                  device;
			std::unique_ptr<IContext>                 context;
			std::unique_ptr<ISwapChain>               swapChain;
			Microsoft::WRL::ComPtr<ID3D11CommandList> commandLists;

		public:
			RasterizerState rasterizerState = RasterizerState::SolidCullNone;

		public:
			// 初期化
			bool Initialize(OS::IDisplay* display) override;

			// 終了化
			void Finalize() override;

		public:
			IDevice*     GetDevice()      override { return device.get(); }
			IContext*    GetContext()     override { return context.get(); }
			ISwapChain*  GetSwapChain()   override { return swapChain.get(); }

			// レンダリングされたイメージを表示
			void Present(int syncInterval) override;

			// 描画開始
			void RenderBegin()override;

			// 描画終了
			void RenderEnd()override;

			//描画命令を記録する
			void RecordRenderCommand()override;

			//描画命令を実行する
			void RenderCommandRun()override;
		};
	} // namespace Graphics
} // namespace Bread