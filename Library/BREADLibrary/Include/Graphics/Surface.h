#pragma once

#include <memory>
#include "Types.h"
#include "FND/Base.h"
#include "Graphics/Device.h"
#include "Graphics/Texture.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// レンダーターゲットサーフェイス設定記述
		//****************************************************************************
		struct RenderTargetSurfaceDesc
		{
			u32 width;
			u32 height;
		};

		//****************************************************************************
		// 深度ステンシルサーフェイス設定記述
		//****************************************************************************
		struct DepthStencilSurfaceDesc
		{
			u32 width;
			u32 height;
		};

		//****************************************************************************
		// レンダーターゲットサーフェイス操作インターフェース
		//****************************************************************************
		class IRenderTargetSurface : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IRenderTargetSurface> Create();

			// 初期化
			virtual bool Initialize(IDevice* device, const RenderTargetSurfaceDesc& desc) = 0;
			
			// 終了化
			virtual void Finalize() = 0;

			// テクスチャ操作オブジェクト取得
			virtual ITexture* GetTexture() = 0;
		};

		//****************************************************************************
		// 深度ステンシルサーフェイス操作インターフェース
		//****************************************************************************
		class IDepthStencilSurface : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IDepthStencilSurface> Create();

			// 初期化
			virtual bool Initialize(IDevice* device, const DepthStencilSurfaceDesc& desc) = 0;
			
			// 終了化
			virtual void Finalize() = 0;

			// テクスチャ操作オブジェクト取得
			virtual ITexture* GetTexture() = 0;
		};
	} // namespace Graphics
} // namespace Bread