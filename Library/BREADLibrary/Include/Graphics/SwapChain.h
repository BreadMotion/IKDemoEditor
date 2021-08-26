#pragma once

#include <memory>
#include "Types.h"
#include "FND/Base.h"
#include "Graphics/Device.h"
#include "Graphics/Surface.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// スワップチェーン設定記述
		//****************************************************************************
		struct SwapChainDesc
		{
			u32 width;
			u32 heigth;
			void* windowHandle;
		};

		//****************************************************************************
		// スワップチェーン操作インターフェース
		//****************************************************************************
		class ISwapChain : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<ISwapChain> Create();

			// 初期化
			virtual bool Initialize(IDevice* device, const SwapChainDesc& desc) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// レンダリングされたイメージを表示
			virtual void Present(int syncInterval) = 0;

			// レンダーターゲットサーフェイス操作オブジェクト取得
			virtual IRenderTargetSurface* GetRenderTargerSurface() = 0;

			// 深度ステンシルサーフェイス操作オブジェクト取得
			virtual IDepthStencilSurface* GetDepthStencilSurface() = 0;
		};
	} // namespace Graphics
} // namespace Bread