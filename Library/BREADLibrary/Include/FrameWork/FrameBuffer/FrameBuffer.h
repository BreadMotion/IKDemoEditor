#pragma once

#include "Graphics/Surface.h"
#include "Graphics/Viewport.h"
#include "Graphics/GraphicsDevice.h"
#include "Graphics/Shader.h"
#include "../Source/Graphics/Texture/Win/DirectX11/TextureDX11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_win32.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_impl_dx11.h"
#include "../../ExternalLibrary/ImGui/Include/imgui_internal.h"

#include <d3d11.h>
#include <wrl.h>
#include <vector>


namespace Bread
{
	namespace FrameWork
	{
		class FrameBuffer
		{
		private:
			static const s32 ViewportCount     = 16;
			static const u32 RenderTargetCount = 6;

		public:
			std::unique_ptr<Graphics::IRenderTargetSurface> renderTargerSurface[RenderTargetCount]; // キューブマップ用に6枚用意
			std::unique_ptr<Graphics::IDepthStencilSurface> depthStencilSurface;

			std::unique_ptr<Graphics::IRenderTargetSurface> cachedRenderTargerSurface;
			std::unique_ptr<Graphics::IDepthStencilSurface> cachedDepthStencilSurface;

			u32 numberOfStoredViewports;
			
			Graphics::Viewport viewport = {};
			Graphics::Viewport* cachedViewports[ViewportCount];

			u32 arraySize = 0;

		public:
			FrameBuffer() {}
			~FrameBuffer() { Finalize(); }

		public:
			static std::unique_ptr<FrameBuffer> Create();

			bool Initialize
			(
				Graphics::IGraphicsDevice* graphicsDevice,
				u32                        width,
				u32                        height,
				bool                       enableMSAA                         = false,
				int                        subsamples                         = 1,
				Graphics::TextureFormatDx  renderTargetTexture2dFormat        = Graphics::TextureFormatDx::R8G8B8A8_UNORM,
				Graphics::TextureFormatDx  depthStencilTexture2dFormat        = Graphics::TextureFormatDx::R24G8_TYPELESS,
				bool                       needRenderTargetShaderResourceView = true,
				bool                       needDepthStencilShaderResourceView = true,
				bool                       generateMips                       = false,
				bool                       textureCube                        = false,
				u32                        arraySize                          = 1
			);

			void Finalize();

			void GUI();

			bool AddRenderTargetView
			(
				Graphics::IGraphicsDevice* graphicsDevice,
				u32 width,
				u32 height,
				Graphics::TextureFormatDx renderTargetTexture2dFormat = Graphics::TextureFormatDx::R8G8B8A8_UNORM,
				bool needRenderTargetShaderResourceView = true
			);

			void Clear(Graphics::IGraphicsDevice* graphicsDevice, u32 index = 0, float r = 0, float g = 0, float b = 0, float a = 1, float depth = 1, u8 stencil = 0);

			//clear only 'render_target_view'
			void ClearRenderTargetView(Graphics::IGraphicsDevice* graphicsDevice, u32 index = 0, float r = 0, float g = 0, float b = 0, float a = 1);

			//clear only 'depth_stencil_view'
			void ClearDepthStencilView(Graphics::IGraphicsDevice* graphicsDevice, FLOAT depth = 1, UINT8 stencil = 0);

			void Activate(Graphics::IGraphicsDevice* graphicsDevice, u32 index = 0);

			//activate only 'render_target_view'
			void ActivateRenderTargetView(Graphics::IGraphicsDevice* graphicsDevice, u32 index = 0);

			void ActivateAllRenderTargetView(Graphics::IGraphicsDevice* graphicsDevice);

			//activate only 'depth_stencil_view'
			void ActivateDepthStencilView(Graphics::IGraphicsDevice* graphicsDevice);

			void Deactivate(Graphics::IGraphicsDevice* graphicsDevice);

			Graphics::IRenderTargetSurface* GetRenderTargetSurface(u32 index = 0) { return renderTargerSurface[index].get(); }
			Graphics::IDepthStencilSurface* GetDepthStencilSurface() { return depthStencilSurface.get(); }

			Graphics::IRenderTargetSurface** GetAllRenderTargetSurface()
			{
				std::vector<Graphics::IRenderTargetSurface*> rts;
				for (u32 i = 0; i < arraySize; ++i)
				{
					rts.emplace_back(renderTargerSurface[i].get());
				}

				return rts.data();
			}

			u32 GetRenderTargetSurfaceArraySize() { return arraySize; }
		};

		class MSAAResolve
		{
		private:
			std::unique_ptr<Graphics::IShader>       embeddedShader;

			std::unique_ptr<Graphics::IRasterizer>   embeddedRasterizerState;
			std::unique_ptr<Graphics::IDepthStencil> embeddedDepthStencilState;

		public:
			MSAAResolve() {}
			~MSAAResolve() { Finalize(); }

		public:
			static std::unique_ptr<MSAAResolve> Create();

			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			void Finalize();

			void Resolve(Graphics::IGraphicsDevice* graphicsDevice, const FrameBuffer* source, FrameBuffer* destination);
		};
	}
}