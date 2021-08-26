#pragma once

#include <memory>
#include "Types.h"
#include "FND/Base.h"
#include "OS/Display.h"
#include "Graphics/Device.h"
#include "Graphics/Context.h"
#include "Graphics/SwapChain.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// グラフィックスデバイス操作インターフェース
		//****************************************************************************
		class IGraphicsDevice : public FND::Base
		{
		public:
			// 生成
			static std::shared_ptr<IGraphicsDevice> Create();

			// 初期化
			virtual bool Initialize(OS::IDisplay* display) = 0;

			// 終了化
			virtual void Finalize() = 0;

		public:
			virtual IDevice*               GetDevice()    = 0;
			virtual IContext*              GetContext()   = 0;
			virtual ISwapChain*            GetSwapChain() = 0;

			// レンダリングされたイメージを表示
			virtual void Present(int syncInterval) = 0;

			// 描画開始
			virtual void RenderBegin() = 0;

			// 描画終了
			virtual void RenderEnd()   = 0;

			//描画命令を記録する
			virtual void RecordRenderCommand() = 0;

			//描画命令を実行する
			virtual void RenderCommandRun()    = 0;
		};
	} // namespace Graphics
} // namespace Bread