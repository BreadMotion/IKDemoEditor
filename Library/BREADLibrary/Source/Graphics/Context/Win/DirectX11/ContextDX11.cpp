#include "pch.h"
#include "ContextDX11.h"
#include "FND/STD.h"
#include "FND/Logger.h"
#include "FND/Util.h"
#include "Device/Win/DirectX11/DeviceDX11.h"
#include "Buffer/Win/DirectX11/BufferDX11.h"
#include "Surface/Win/DirectX11/RenderTargetSurfaceDX11.h"
#include "Surface/Win/DirectX11/DepthStencilSurfaceDX11.h"
#include "Texture/Win/DirectX11/TextureDX11.h"
#include "RenderState/Win/DirectX11/BlendDX11.h"
#include "RenderState/Win/DirectX11/DepthStencilDX11.h"
#include "RenderState/Win/DirectX11/RasterizerDX11.h"
#include "RenderState/Win/DirectX11/SamplerDX11.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版コンテキスト操作オブジェクト
		//****************************************************************************
		// 生成
		std::unique_ptr<IContext> IContext::Create()
		{
			return std::make_unique<ContextDX11>();
		}

		// 初期化
		bool ContextDX11::Initialize(IDevice* device)
		{
			HRESULT hr;

			deviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			deviceContext->AddRef();

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			// 定数バッファ
			{
				BreadBufferDesc desc = {};
				FND::MemSet(&desc, 0, sizeof(desc));
				desc.usage               = BreadUsage::Default;
				desc.bindFlags           = static_cast<u32>(BreadBindFlag::ConstantBuffer);
				desc.cpuAccessFlags      = 0;
				desc.miscFlags           = 0;
				desc.byteWidth           = sizeof(CbScene);
				desc.structureByteStride = 0;

				cbScene = IBuffer::Create();
				if (!cbScene->Initialize(device, desc))
				{
					return false;
				}

				// メッシュ描画毎更新用バッファ
				desc.byteWidth = sizeof(CbMesh);

				cbMesh = IBuffer::Create();
				if (!cbMesh->Initialize(device, desc))
				{
					return false;
				}

				// メッシュ描画毎更新用バッファ
				desc.usage = BreadUsage::Dynamic;
				desc.cpuAccessFlags = static_cast<u32>(BreadCPUAccessFlag::CPUAccessWrite);
				desc.byteWidth = sizeof(CbBone);

				cbBone = IBuffer::Create();
				if (!cbBone->Initialize(device, desc))
				{
					return false;
				}
			}

			// ブレンドステート
			{
				for (s32 i = 0; i < FND::ArraySize(blendState); i++)
				{
					blendState[i] = IBlend::Create();
					if (!blendState[i]->Initialize(device, static_cast<BlendState>(i)))
					{
						return false;
					}
				}
			}

			// 深度ステンシルステート
			{
				for (s32 i = 0; i < FND::ArraySize(depthStencilState); i++)
				{
					depthStencilState[i] = IDepthStencil::Create();
					if (!depthStencilState[i]->Initialize(device, static_cast<DepthState>(i)))
					{
						return false;
					}
				}
			}

			// ラスタライザーステート
			{
				for (s32 i = 0; i < FND::ArraySize(rasterizerState); i++)
				{
					rasterizerState[i] = IRasterizer::Create();
					if (!rasterizerState[i]->Initialize(device, static_cast<RasterizerState>(i)))
					{
						return false;
					}
				}
			}

			// サンプラーステート
			{
				for (s32 i = 0; i < FND::ArraySize(samplerState); i++)
				{
					samplerState[i] = ISampler::Create();
					if (!samplerState[i]->Initialize(device, static_cast<SamplerState>(i)))
					{
						return false;
					}
				}
			}

			// パフォーマンスクエリ
			{
				for (s32 i = 0; i < FND::ArraySize(query); i++)
				{
					D3D11_QUERY_DESC desc = {};
					FND::MemSet(&desc, 0, sizeof(desc));
					desc.MiscFlags = 0;
					desc.Query = D3D11_QUERY_TIMESTAMP;

					hr = d3dDevice->CreateQuery(&desc, &query[i]);
					if (FAILED(hr))
					{
						BREAD_LOG_GRP_ERROR("ID3D11Device::CreateQuery() : Failed!!\n");
						return false;
					}
				}
			}

			return true;
		}

		// 初期化
		bool ContextDX11::Initialize_DF(IDevice* device)
		{
			deferredContext = static_cast<DeviceDX11*>(device)->GetD3DDfContext();
			deferredContext->AddRef();

			return true;
		}

		// 終了化
		void ContextDX11::Finalize()
		{
			FND::SafeRelease(deferredContext);
			FND::SafeRelease(deviceContext);

			cbScene->Finalize();
			cbMesh->Finalize();
			cbBone->Finalize();

			for (ID3D11Query*& d3dQuery : query)
			{
				FND::SafeRelease(d3dQuery);
			}

			for (auto& state : blendState)
			{
				state.reset();
			}
			for (auto& state : rasterizerState)
			{
				state.reset();
			}
			for (auto& state : depthStencilState)
			{
				state.reset();
			}
			for (auto& state : samplerState)
			{
				state.reset();
			}
		}


		// 描画
		void ContextDX11::Draw(u32 vertexCount, u32 startVertexLocation)
		{
			deviceContext->Draw(vertexCount, startVertexLocation);
		}
		void ContextDX11::DrawDf(u32 vertexCount, u32 startVertexLocation)
		{
			deferredContext->Draw(vertexCount, startVertexLocation);
		}
		void ContextDX11::DrawInstancedIndirect(IBuffer* buffer, u32 offset)
		{
			ID3D11Buffer* d3dBuffer = static_cast<BufferDX11*>(buffer)->GetD3DBuffer();
			deviceContext->DrawInstancedIndirect(d3dBuffer, offset);
		}
		void ContextDX11::DrawInstancedIndirectDf(IBuffer* buffer, u32 offset)
		{
			ID3D11Buffer* d3dBuffer = static_cast<BufferDX11*>(buffer)->GetD3DBuffer();
			deferredContext->DrawInstancedIndirect(d3dBuffer, offset);
		}


		// 描画開始
		void ContextDX11::Begin()
		{

		}

		// 描画終了
		void ContextDX11::End()
		{

		}

		// 描画同期待ち
		void ContextDX11::WaitSync()
		{

		}

		// レンダーターゲットクリア
		void ContextDX11::ClearRenderTargetView(IRenderTargetSurface* renderTargetSurface, const f32* color)
		{
			ID3D11RenderTargetView* renderTargetView = static_cast<RenderTargetSurfaceDX11*>(renderTargetSurface)->GetD3DRenderTargetView();
			if (renderTargetView)
			{
				deviceContext->ClearRenderTargetView(renderTargetView, color);
			}
		}
		void ContextDX11::ClearRenderTargetViewDf(IRenderTargetSurface* renderTargetSurface, const f32* color)
		{
			ID3D11RenderTargetView* renderTargetView = static_cast<RenderTargetSurfaceDX11*>(renderTargetSurface)->GetD3DRenderTargetView();
			if (renderTargetView)
			{
				deferredContext->ClearRenderTargetView(renderTargetView, color);
			}
		}

		// 深度ステンシルクリア
		void ContextDX11::ClearDepthStencilView(IDepthStencilSurface* depthStencilSurface, f32 depth, u8 stencil)
		{
			ID3D11DepthStencilView* depthStencilView = static_cast<DepthStencilSurfaceDX11*>(depthStencilSurface)->GetD3DDepthStencilView();
			if (depthStencilView)
			{
				deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
			}
		}
		void ContextDX11::ClearDepthStencilViewDf(IDepthStencilSurface* depthStencilSurface, f32 depth, u8 stencil)
		{
			ID3D11DepthStencilView* depthStencilView = static_cast<DepthStencilSurfaceDX11*>(depthStencilSurface)->GetD3DDepthStencilView();
			if (depthStencilView)
			{
				deferredContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
			}
		}

		// レンダーターゲット設定
		void ContextDX11::SetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface)
		{
			ID3D11RenderTargetView* renderTargetViews[8];
			u32 size = static_cast<u32>(sizeof(renderTargetViews));
			count = count < size ? count : size;

			for (u32  i = 0; i < count; ++i)
			{
				if (renderTargetSurface[i])
				{
					renderTargetViews[i] = static_cast<RenderTargetSurfaceDX11*>(renderTargetSurface[i])->GetD3DRenderTargetView();
				}
				else
				{
					renderTargetViews[i] = nullptr;
				}
			}

			ID3D11DepthStencilView* depthStencilView;
			if (depthStencilSurface)
			{
				depthStencilView = static_cast<DepthStencilSurfaceDX11*>(depthStencilSurface)->GetD3DDepthStencilView();
			}
			else
			{
				depthStencilView = nullptr;
			}

			deviceContext->OMSetRenderTargets(count, renderTargetViews, depthStencilView);
		}
		void ContextDX11::SetRenderTargetsDf(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface)
		{
			ID3D11RenderTargetView* renderTargetViews[8];
			u32 size = static_cast<u32>(sizeof(renderTargetViews));
			count = count < size ? count : size;

			for (u32 i = 0; i < count; ++i)
			{
				if (renderTargetSurface[i])
				{
					renderTargetViews[i] = static_cast<RenderTargetSurfaceDX11*>(renderTargetSurface[i])->GetD3DRenderTargetView();
				}
				else
				{
					renderTargetViews[i] = nullptr;
				}
			}

			ID3D11DepthStencilView* depthStencilView;
			if (depthStencilSurface)
			{
				depthStencilView = static_cast<DepthStencilSurfaceDX11*>(depthStencilSurface)->GetD3DDepthStencilView();
			}
			else
			{
				depthStencilView = nullptr;
			}

			deferredContext->OMSetRenderTargets(count, renderTargetViews, depthStencilView);
		}

		// レンダーターゲット取得
		void ContextDX11::GetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface)
		{
			RenderTargetSurfaceDX11* renderTargetSurfaceDX11[8];
			ID3D11RenderTargetView* renderTargetViews[8];
			u32 size = static_cast<u32>(sizeof(renderTargetViews));
			count = count < size ? count : size;

			for (u32 i = 0; i < count; ++i)
			{
				renderTargetSurfaceDX11[i] = static_cast<RenderTargetSurfaceDX11*>(renderTargetSurface[i]);
				renderTargetViews[i]       = renderTargetSurfaceDX11[i]->GetD3DRenderTargetView();
			}

			DepthStencilSurfaceDX11* depthStencilSurfaceDX11 = static_cast<DepthStencilSurfaceDX11*>(depthStencilSurface);
			ID3D11DepthStencilView* depthStencilView         = depthStencilSurfaceDX11->GetD3DDepthStencilView();

			deviceContext->OMGetRenderTargets(count, renderTargetViews, &depthStencilView);

			for (u32 i = 0; i < count; ++i)
			{
				renderTargetSurfaceDX11[i]->SetD3DRenderTargetView(renderTargetViews[i]);
			}
			depthStencilSurfaceDX11->SetDepthStencilView(depthStencilView);
		}
		void ContextDX11::GetRenderTargetsDf(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface)
		{
			RenderTargetSurfaceDX11* renderTargetSurfaceDX11[8];
			ID3D11RenderTargetView* renderTargetViews[8];
			u32 size = static_cast<u32>(sizeof(renderTargetViews));
			count = count < size ? count : size;

			for (u32 i = 0; i < count; ++i)
			{
				renderTargetSurfaceDX11[i] = static_cast<RenderTargetSurfaceDX11*>(renderTargetSurface[i]);
				renderTargetViews[i]       = renderTargetSurfaceDX11[i]->GetD3DRenderTargetView();
			}

			DepthStencilSurfaceDX11* depthStencilSurfaceDX11 = static_cast<DepthStencilSurfaceDX11*>(depthStencilSurface);
			ID3D11DepthStencilView* depthStencilView = depthStencilSurfaceDX11->GetD3DDepthStencilView();

			deferredContext->OMGetRenderTargets(count, renderTargetViews, &depthStencilView);

			for (u32 i = 0; i < count; ++i)
			{
				renderTargetSurfaceDX11[i]->SetD3DRenderTargetView(renderTargetViews[i]);
			}
			depthStencilSurfaceDX11->SetDepthStencilView(depthStencilView);
		}

		// ビューポート設定
		void ContextDX11::SetViewports(u32 count, const Viewport viewports[])
		{
			D3D11_VIEWPORT d3dViewports[8];
			u32 size = static_cast<u32>(sizeof(viewports));
			count = count < size ? count : size;

			if (count > 0)
			{
				for (u32 i = 0; i < count; ++i)
				{
					d3dViewports[i].TopLeftX = viewports[i].x;
					d3dViewports[i].TopLeftY = viewports[i].y;
					d3dViewports[i].Width    = viewports[i].width;
					d3dViewports[i].Height   = viewports[i].height;
					d3dViewports[i].MinDepth = viewports[i].minDepth;
					d3dViewports[i].MaxDepth = viewports[i].maxDepth;
				}
				deviceContext->RSSetViewports(count, d3dViewports);
			}
			else
			{
				deviceContext->RSSetViewports(0, nullptr);
			}
		}
		void ContextDX11::SetViewportsDf(u32 count, const Viewport viewports[])
		{
			D3D11_VIEWPORT d3dViewports[8];
			u32 size = static_cast<u32>(sizeof(viewports));
			count = count < size ? count : size;

			if (count > 0)
			{
				for (u32 i = 0; i < count; ++i)
				{
					d3dViewports[i].TopLeftX = viewports[i].x;
					d3dViewports[i].TopLeftY = viewports[i].y;
					d3dViewports[i].Width    = viewports[i].width;
					d3dViewports[i].Height   = viewports[i].height;
					d3dViewports[i].MinDepth = viewports[i].minDepth;
					d3dViewports[i].MaxDepth = viewports[i].maxDepth;
				}
				deferredContext->RSSetViewports(count, d3dViewports);
			}
			else
			{
				deferredContext->RSSetViewports(0, nullptr);
			}
		}

		// ビューポート取得
		void ContextDX11::GetViewports(u32 count, Viewport* viewports[])
		{
			D3D11_VIEWPORT d3dViewports[8];
			u32 size = static_cast<u32>(sizeof(viewports));
			count = count < size ? count : size;

			deviceContext->RSGetViewports(&count, d3dViewports);

			for (u32 i = 0; i < count; ++i)
			{
				viewports[i]->x        = d3dViewports[i].TopLeftX;
				viewports[i]->y        = d3dViewports[i].TopLeftY;
				viewports[i]->width    = d3dViewports[i].Width;
				viewports[i]->height   = d3dViewports[i].Height;
				viewports[i]->minDepth = d3dViewports[i].MinDepth;
				viewports[i]->maxDepth = d3dViewports[i].MaxDepth;
			}
		}
		void ContextDX11::GetViewportsDf(u32 count, Viewport* viewports[])
		{
			D3D11_VIEWPORT d3dViewports[8];
			u32 size = static_cast<u32>(sizeof(viewports));
			count = count < size ? count : size;

			deferredContext->RSGetViewports(&count, d3dViewports);

			for (u32 i = 0; i < count; ++i)
			{
				viewports[i]->x        = d3dViewports[i].TopLeftX;
				viewports[i]->y        = d3dViewports[i].TopLeftY;
				viewports[i]->width    = d3dViewports[i].Width;
				viewports[i]->height   = d3dViewports[i].Height;
				viewports[i]->minDepth = d3dViewports[i].MinDepth;
				viewports[i]->maxDepth = d3dViewports[i].MaxDepth;
			}
		}

		// シェーダーリソースビュー設定
		void ContextDX11::SetShaderResources(ShaderType shadowType, u32 startSlot, u32 numViews, ITexture* texture[])
		{
			ID3D11ShaderResourceView* d3dShaderResourceViews[8] = { nullptr };
			u32 size = static_cast<u32>(sizeof(texture));
			numViews = numViews < size ? numViews : size;

			for (u32 i = 0; i < numViews; i++)
			{
				TextureDX11* textureDX11 = static_cast<TextureDX11*>(texture[i]);
				if (textureDX11 != nullptr)
				{
					d3dShaderResourceViews[i] = textureDX11->GetD3DShaderResourceView();
				}
			}

			switch (shadowType)
			{
			case ShaderType::Vertex:
				deviceContext->VSSetShaderResources(startSlot, numViews, d3dShaderResourceViews);
				break;

			case ShaderType::Pixel:
				deviceContext->PSSetShaderResources(startSlot, numViews, d3dShaderResourceViews);
				break;

			case ShaderType::Compute:
				deviceContext->CSSetShaderResources(startSlot, numViews, d3dShaderResourceViews);
				break;

			default: break;
			}
		}
		void ContextDX11::SetShaderResourcesDf(ShaderType shadowType, u32 startSlot, u32 numViews, ITexture* texture[])
		{
			ID3D11ShaderResourceView* d3dShaderResourceViews[8] = { nullptr };
			u32 size = static_cast<u32>(sizeof(texture));
			numViews = numViews < size ? numViews : size;

			for (u32 i = 0; i < numViews; i++)
			{
				TextureDX11* textureDX11 = static_cast<TextureDX11*>(texture[i]);
				if (textureDX11 != nullptr)
				{
					d3dShaderResourceViews[i] = textureDX11->GetD3DShaderResourceView();
				}
			}

			switch (shadowType)
			{
			case ShaderType::Vertex:
				deferredContext->VSSetShaderResources(startSlot, numViews, d3dShaderResourceViews);
				break;

			case ShaderType::Pixel:
				deferredContext->PSSetShaderResources(startSlot, numViews, d3dShaderResourceViews);
				break;

			case ShaderType::Compute:
				deferredContext->CSSetShaderResources(startSlot, numViews, d3dShaderResourceViews);
				break;

			default: break;
			}
		}

		// アンオーダーアクセスビュー設定
		void ContextDX11::SetUnorderedAccess(u32 startSlot, u32 numViews, ITexture* texture[], u32* uavInitialCounts)
		{
			ID3D11UnorderedAccessView* d3dUnorderedAccessViews[8] = { nullptr };
			u32 size = static_cast<u32>(sizeof(texture));
			numViews = numViews < size ? numViews : size;

			for (u32 i = 0; i < numViews; i++)
			{
				TextureDX11* textureDX11 = static_cast<TextureDX11*>(texture[i]);
				if (textureDX11 != nullptr)
				{
					d3dUnorderedAccessViews[i] = textureDX11->GetD3DUnorderedAccessView();
				}
			}

			deviceContext->CSSetUnorderedAccessViews(startSlot, numViews, d3dUnorderedAccessViews, uavInitialCounts);
		}
		void ContextDX11::SetUnorderedAccessDf(u32 startSlot, u32 numViews, ITexture* texture[], u32* uavInitialCounts)
		{
			ID3D11UnorderedAccessView* d3dUnorderedAccessViews[8] = { nullptr };
			u32 size = static_cast<u32>(sizeof(texture));
			numViews = numViews < size ? numViews : size;

			for (u32 i = 0; i < numViews; i++)
			{
				TextureDX11* textureDX11 = static_cast<TextureDX11*>(texture[i]);
				if (textureDX11 != nullptr)
				{
					d3dUnorderedAccessViews[i] = textureDX11->GetD3DUnorderedAccessView();
				}
			}

			deferredContext->CSSetUnorderedAccessViews(startSlot, numViews, d3dUnorderedAccessViews, uavInitialCounts);
		}

		// ブレンドステート設定
		void ContextDX11::SetBlend(IBlend* blend, const f32* blendFactor, u32 samplerMask)
		{
			ID3D11BlendState* d3dBlend = static_cast<BlendDX11*>(blend)->GetD3DBlendState();
			deviceContext->OMSetBlendState(d3dBlend, blendFactor, samplerMask);
		}
		void ContextDX11::SetBlendDf(IBlend* blend, const f32* blendFactor, u32 samplerMask)
		{
			ID3D11BlendState* d3dBlend = static_cast<BlendDX11*>(blend)->GetD3DBlendState();
			deferredContext->OMSetBlendState(d3dBlend, blendFactor, samplerMask);
		}

		// 深度ステンシルステート設定
		void ContextDX11::SetDepthStencil(IDepthStencil* depthStencil, u32 stencilRef)
		{
			ID3D11DepthStencilState* d3dDepthStencil = static_cast<DepthStencilDX11*>(depthStencil)->GetD3DDepthStencilState();
			deviceContext->OMSetDepthStencilState(d3dDepthStencil, stencilRef);
		}
		void ContextDX11::SetDepthStencilDf(IDepthStencil* depthStencil, u32 stencilRef)
		{
			ID3D11DepthStencilState* d3dDepthStencil = static_cast<DepthStencilDX11*>(depthStencil)->GetD3DDepthStencilState();
			deferredContext->OMSetDepthStencilState(d3dDepthStencil, stencilRef);
		}

		// 深度ステンシルステート取得
		void ContextDX11::GetDepthStencil(IDepthStencil* depthStencil, u32 stencilRef)
		{
			ID3D11DepthStencilState* d3dDepthStencil;
			deviceContext->OMGetDepthStencilState(&d3dDepthStencil, &stencilRef);
			static_cast<DepthStencilDX11*>(depthStencil)->SetD3DDepthStencilState(d3dDepthStencil);
		}
		void ContextDX11::GetDepthStencilDf(IDepthStencil* depthStencil, u32 stencilRef)
		{
			ID3D11DepthStencilState* d3dDepthStencil;
			deferredContext->OMGetDepthStencilState(&d3dDepthStencil, &stencilRef);
			static_cast<DepthStencilDX11*>(depthStencil)->SetD3DDepthStencilState(d3dDepthStencil);
		}

		// ラスタライザーステート設定
		void ContextDX11::SetRasterizer(IRasterizer* rasterizer)
		{
			ID3D11RasterizerState* d3dRasterizer = static_cast<RasterizerDX11*>(rasterizer)->GetD3DRasterizerState();
			deviceContext->RSSetState(d3dRasterizer);
		}
		void ContextDX11::SetRasterizerDf(IRasterizer* rasterizer)
		{
			ID3D11RasterizerState* d3dRasterizer = static_cast<RasterizerDX11*>(rasterizer)->GetD3DRasterizerState();
			deferredContext->RSSetState(d3dRasterizer);
		}

		// ラスタライザーステート取得
		void ContextDX11::GetRasterizer(IRasterizer* rasterizer)
		{
			ID3D11RasterizerState* d3dRasterizer;
			deviceContext->RSGetState(&d3dRasterizer);
			static_cast<RasterizerDX11*>(rasterizer)->SetD3DRasterizerState(d3dRasterizer);
		}
		void ContextDX11::GetRasterizerDf(IRasterizer* rasterizer)
		{
			ID3D11RasterizerState* d3dRasterizer;
			deferredContext->RSGetState(&d3dRasterizer);
			static_cast<RasterizerDX11*>(rasterizer)->SetD3DRasterizerState(d3dRasterizer);
		}

		// サンプラーステート設定
		void ContextDX11::SetSamplers(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[])
		{
			ID3D11SamplerState* d3dSampler[8] = { nullptr };
			u32 size = static_cast<u32>(sizeof(sampler));
			numViews = numViews < size ? numViews : size;

			for (u32 i = 0; i < numViews; i++)
			{
				SamplerDX11* samplerDX11 = static_cast<SamplerDX11*>(sampler[i]);
				if (samplerDX11 != nullptr)
				{
					d3dSampler[i] = samplerDX11->GetD3DSamplerState();
				}
			}

			switch (shadowType)
			{
			case ShaderType::Vertex:
				deviceContext->VSSetSamplers(startSlot, numViews, d3dSampler);
				break;

			case ShaderType::Pixel:
				deviceContext->PSSetSamplers(startSlot, numViews, d3dSampler);
				break;

			default: break;
			}
		}
		void ContextDX11::SetSamplersDf(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[])
		{
			ID3D11SamplerState* d3dSampler[8] = { nullptr };
			u32 size = static_cast<u32>(sizeof(sampler));
			numViews = numViews < size ? numViews : size;

			for (u32 i = 0; i < numViews; i++)
			{
				SamplerDX11* samplerDX11 = static_cast<SamplerDX11*>(sampler[i]);
				if (samplerDX11 != nullptr)
				{
					d3dSampler[i] = samplerDX11->GetD3DSamplerState();
				}
			}

			switch (shadowType)
			{
			case ShaderType::Vertex:
				deferredContext->VSSetSamplers(startSlot, numViews, d3dSampler);
				break;

			case ShaderType::Pixel:
				deferredContext->PSSetSamplers(startSlot, numViews, d3dSampler);
				break;

			default: break;
			}
		}

		// サンプラーステート取得
		void ContextDX11::GetSamplers(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[])
		{
			ID3D11SamplerState* d3dSampler[8] = { nullptr };
			u32 size = static_cast<u32>(sizeof(sampler));
			numViews = numViews < size ? numViews : size;

			for (u32 i = 0; i < numViews; i++)
			{
				SamplerDX11* samplerDX11 = static_cast<SamplerDX11*>(sampler[i]);
				if (samplerDX11 != nullptr)
				{
					d3dSampler[i] = samplerDX11->GetD3DSamplerState();
				}
			}

			switch (shadowType)
			{
			case ShaderType::Vertex:
				deviceContext->VSGetSamplers(startSlot, numViews, d3dSampler);
				break;

			case ShaderType::Pixel:
				deviceContext->PSGetSamplers(startSlot, numViews, d3dSampler);
				break;

			default: break;
			}
		}
		void ContextDX11::GetSamplersDf(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[])
		{
			ID3D11SamplerState* d3dSampler[8] = { nullptr };
			u32 size = static_cast<u32>(sizeof(sampler));
			numViews = numViews < size ? numViews : size;

			for (u32 i = 0; i < numViews; i++)
			{
				SamplerDX11* samplerDX11 = static_cast<SamplerDX11*>(sampler[i]);
				if (samplerDX11 != nullptr)
				{
					d3dSampler[i] = samplerDX11->GetD3DSamplerState();
				}
			}

			switch (shadowType)
			{
			case ShaderType::Vertex:
				deferredContext->VSGetSamplers(startSlot, numViews, d3dSampler);
				break;

			case ShaderType::Pixel:
				deferredContext->PSGetSamplers(startSlot, numViews, d3dSampler);
				break;

			default: break;
			}
		}

		// バッファ更新
		void ContextDX11::UpdateSubresource(IBuffer* buffer, u32 dstSubresource, const BreadBox* dstBox, const void* drcData, u32 srcRowPitch, u32 srcDepthPitch)
		{
			ID3D11Buffer* d3dBuffer = static_cast<BufferDX11*>(buffer)->GetD3DBuffer();

			if (dstBox != nullptr) // Boxにデータある場合
			{
				D3D11_BOX box = {};
				box.left   = dstBox->left;
				box.top    = dstBox->top;
				box.front  = dstBox->front;
				box.right  = dstBox->right;
				box.bottom = dstBox->bottom;
				box.back   = dstBox->back;
				deviceContext->UpdateSubresource(d3dBuffer, dstSubresource, &box, drcData, srcRowPitch, srcDepthPitch);
			}
			else // Boxにデータがない場合
			{
				// Memo : D3D11_BOXを使わない場合、nullptrを指定しないと更新操作が実行されない。
				deviceContext->UpdateSubresource(d3dBuffer, dstSubresource, nullptr, drcData, srcRowPitch, srcDepthPitch);
			}
		}
		void ContextDX11::UpdateSubresourceDf(IBuffer* buffer, u32 dstSubresource, const BreadBox* dstBox, const void* drcData, u32 srcRowPitch, u32 srcDepthPitch)
		{
			ID3D11Buffer* d3dBuffer = static_cast<BufferDX11*>(buffer)->GetD3DBuffer();

			if (dstBox != nullptr) // Boxにデータある場合
			{
				D3D11_BOX box = {};
				box.left   = dstBox->left;
				box.top    = dstBox->top;
				box.front  = dstBox->front;
				box.right  = dstBox->right;
				box.bottom = dstBox->bottom;
				box.back   = dstBox->back;
				deviceContext->UpdateSubresource(d3dBuffer, dstSubresource, &box, drcData, srcRowPitch, srcDepthPitch);
			}
			else // Boxにデータがない場合
			{
				// Memo : D3D11_BOXを使わない場合、nullptrを指定しないと更新操作が実行されない。
				deferredContext->UpdateSubresource(d3dBuffer, dstSubresource, nullptr, drcData, srcRowPitch, srcDepthPitch);
			}
		}

		// バッファ更新開始
		void ContextDX11::Map(IBuffer* buffer, u32 subresource, BreadMap mapType, u32 mapFlags, BreadMappedSubresource* mappedSubresource)
		{
			HRESULT hr;

			ID3D11Buffer* d3dBuffer = static_cast<BufferDX11*>(buffer)->GetD3DBuffer();

			D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
			hr = deviceContext->Map(d3dBuffer, subresource, static_cast<D3D11_MAP>(mapType), mapFlags, &d3dMappedResource);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11DeviceContext::Map() : Failed!!\n");
				return;
			}

			mappedSubresource->data       = d3dMappedResource.pData;
			mappedSubresource->rowPitch   = d3dMappedResource.RowPitch;
			mappedSubresource->depthPitch = d3dMappedResource.DepthPitch;
		}
		void ContextDX11::MapDf(IBuffer* buffer, u32 subresource, BreadMap mapType, u32 mapFlags, BreadMappedSubresource* mappedSubresource)
		{
			HRESULT hr;

			ID3D11Buffer* d3dBuffer = static_cast<BufferDX11*>(buffer)->GetD3DBuffer();

			D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
			hr = deferredContext->Map(d3dBuffer, subresource, static_cast<D3D11_MAP>(mapType), mapFlags, &d3dMappedResource);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11DeviceContext::Map() : Failed!!\n");
				return;
			}

			mappedSubresource->data       = d3dMappedResource.pData;
			mappedSubresource->rowPitch   = d3dMappedResource.RowPitch;
			mappedSubresource->depthPitch = d3dMappedResource.DepthPitch;
		}

		// バッファ更新終了
		void ContextDX11::Unmap(IBuffer* buffer, u32 subresource)
		{
			ID3D11Buffer* d3dBuffer = static_cast<BufferDX11*>(buffer)->GetD3DBuffer();
			deviceContext->Unmap(d3dBuffer, subresource);
		}
		void ContextDX11::UnmapDf(IBuffer* buffer, u32 subresource)
		{
			ID3D11Buffer* d3dBuffer = static_cast<BufferDX11*>(buffer)->GetD3DBuffer();
			deferredContext->Unmap(d3dBuffer, subresource);
		}

		// バッファ設定
		void ContextDX11::SetConstantBuffers(ShaderType shadowType, u32 startSlot, u32 numViews, IBuffer* buffer[])
		{
			ID3D11Buffer* d3dBuffer[8] = { nullptr };
			u32 size = static_cast<u32>(sizeof(buffer));
			numViews = numViews < size ? numViews : size;

			for (u32 i = 0; i < numViews; i++)
			{
				BufferDX11* bufferDX11 = static_cast<BufferDX11*>(buffer[i]);
				if (bufferDX11 != nullptr)
				{
					d3dBuffer[i] = bufferDX11->GetD3DBuffer();
				}
			}

			switch (shadowType)
			{
			case ShaderType::Vertex:
				deviceContext->VSSetConstantBuffers(startSlot, numViews, d3dBuffer);
				break;

			case ShaderType::Pixel:
				deviceContext->PSSetConstantBuffers(startSlot, numViews, d3dBuffer);
				break;

			case ShaderType::Compute:
				deviceContext->CSSetConstantBuffers(startSlot, numViews, d3dBuffer);
				break;

			default: break;
			}
		}
		void ContextDX11::SetConstantBuffersDf(ShaderType shadowType, u32 startSlot, u32 numViews, IBuffer* buffer[])
		{
			ID3D11Buffer* d3dBuffer[8] = { nullptr };
			u32 size = static_cast<u32>(sizeof(buffer));
			numViews = numViews < size ? numViews : size;

			for (u32 i = 0; i < numViews; i++)
			{
				BufferDX11* bufferDX11 = static_cast<BufferDX11*>(buffer[i]);
				if (bufferDX11 != nullptr)
				{
					d3dBuffer[i] = bufferDX11->GetD3DBuffer();
				}
			}

			switch (shadowType)
			{
			case ShaderType::Vertex:
				deferredContext->VSSetConstantBuffers(startSlot, numViews, d3dBuffer);
				break;

			case ShaderType::Pixel:
				deferredContext->PSSetConstantBuffers(startSlot, numViews, d3dBuffer);
				break;

			case ShaderType::Compute:
				deferredContext->CSSetConstantBuffers(startSlot, numViews, d3dBuffer);
				break;

			default: break;
			}
		}

		// シーン定数バッファ更新
		void ContextDX11::UpdateConstantBufferScene(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform)
		{
			//CbScene constantBufferScene;
			cbSceneData.view           = (viewTransform);
			cbSceneData.projection     = (projectionTransform);
			cbSceneData.viewProjection = Math::MatrixMultiplyTranspose(viewTransform, projectionTransform);

			cbSceneData.viewInv           = Math::MatrixTranspose(Math::MatrixInverse(viewTransform));
			cbSceneData.projectionInv     = Math::MatrixTranspose(Math::MatrixInverse(projectionTransform));
			cbSceneData.viewProjectionInv = Math::MatrixTranspose(Math::MatrixInverse(Math::MatrixMultiply(viewTransform, projectionTransform)));

			u32 viewportCount     = 1;
			Viewport* viewports[] = { new Viewport() };
			GetViewports(viewportCount, viewports);
			cbSceneData.viewport = Math::Vector4(viewports[0]->x, viewports[0]->y, viewports[0]->width, viewports[0]->height);

			UpdateSubresource(cbScene.get(), 0, 0, &cbSceneData, 0, 0);

			for (u32 i = 0; i < viewportCount; ++i)
			{
				FND::SafeDelete(viewports[i]);
			}
		}
		void ContextDX11::UpdateConstantBufferSceneDf(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform)
		{
			//CbScene constantBufferScene;
			cbSceneData.view           = (viewTransform);
			cbSceneData.projection     = (projectionTransform);
			cbSceneData.viewProjection = Math::MatrixMultiplyTranspose(viewTransform, projectionTransform);

			cbSceneData.viewInv           = Math::MatrixTranspose(Math::MatrixInverse(viewTransform));
			cbSceneData.projectionInv     = Math::MatrixTranspose(Math::MatrixInverse(projectionTransform));
			cbSceneData.viewProjectionInv = Math::MatrixTranspose(Math::MatrixInverse(Math::MatrixMultiply(viewTransform, projectionTransform)));

			u32 viewportCount = 1;
			Viewport* viewports[] = { new Viewport() };
			GetViewports(viewportCount, viewports);
			cbSceneData.viewport  = Math::Vector4(viewports[0]->x, viewports[0]->y, viewports[0]->width, viewports[0]->height);

			UpdateSubresource(cbScene.get(), 0, 0, &cbSceneData, 0, 0);

			for (u32 i = 0; i < viewportCount; ++i)
			{
				FND::SafeDelete(viewports[i]);
			}
		}

		// 前フレームのシーン定数バッファ更新
		void ContextDX11::UpdateConstantBufferPrevScene(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform)
		{
			cbSceneData.prevView           = (viewTransform);
			cbSceneData.prevProjection     = (projectionTransform);
			cbSceneData.prevViewProjection = Math::MatrixMultiplyTranspose(viewTransform, projectionTransform);

			cbSceneData.prevViewInv           = Math::MatrixTranspose(Math::MatrixInverse(viewTransform));
			cbSceneData.prevProjectionInv     = Math::MatrixTranspose(Math::MatrixInverse(projectionTransform));
			cbSceneData.prevViewProjectionInv = Math::MatrixTranspose(Math::MatrixInverse(Math::MatrixMultiply(viewTransform, projectionTransform)));
		}
		void ContextDX11::UpdateConstantBufferPrevSceneDf(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform)
		{
			cbSceneData.prevView           = (viewTransform);
			cbSceneData.prevProjection     = (projectionTransform);
			cbSceneData.prevViewProjection = Math::MatrixMultiplyTranspose(viewTransform, projectionTransform);

			cbSceneData.prevViewInv           = Math::MatrixTranspose(Math::MatrixInverse(viewTransform));
			cbSceneData.prevProjectionInv     = Math::MatrixTranspose(Math::MatrixInverse(projectionTransform));
			cbSceneData.prevViewProjectionInv = Math::MatrixTranspose(Math::MatrixInverse(Math::MatrixMultiply(viewTransform, projectionTransform)));
		}

		// メッシュ定数バッファ更新
		void ContextDX11::UpdateConstantBufferMesh(const Math::Matrix& worldTransform)
		{
			CbMesh constantBufferMesh;

	/*		Math::Matrix systemUnitTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
			systemUnitTransform._11          = systemUnitTransform._22 = systemUnitTransform._33 = 0.01f;*/

			constantBufferMesh.world        = Math::MatrixTranspose(worldTransform);
			constantBufferMesh.worldInverse = Math::MatrixInverse(worldTransform);
			constantBufferMesh.texture      = Math::MatrixIdentity();

			UpdateSubresource(cbMesh.get(), 0, 0, &constantBufferMesh, 0, 0);
		}
		void ContextDX11::UpdateConstantBufferMeshDf(const Math::Matrix& worldTransform)
		{
			CbMesh constantBufferMesh;

			//Math::Matrix systemUnitTransform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
			//systemUnitTransform._11          = systemUnitTransform._22 = systemUnitTransform._33 = 0.01f;

			constantBufferMesh.world            = Math::MatrixTranspose(worldTransform);
			constantBufferMesh.worldInverse = Math::MatrixInverse(worldTransform);
			constantBufferMesh.texture          = Math::MatrixIdentity();

			UpdateSubresourceDf(cbMesh.get(), 0, 0, &constantBufferMesh, 0, 0);
		}

		// ボーン定数バッファ更新
		void ContextDX11::UpdateConstantBufferBone(const Math::Matrix boneTransforms[], s32 boneTransformCount)
		{
			/*
			BreadMappedSubresource mappedResource;
			Map(cbBone.get(), 0, BreadMap::WriteDiscard, 0, &mappedResource);

			CbBone* constantBufferBone = reinterpret_cast<CbBone*>(mappedResource.data);
			for (s32 i = 0; i < boneTransformCount; ++i)
			{
				constantBufferBone->bones[i] = Math::MatrixTranspose(boneTransforms[i]);
			}

			Unmap(cbBone.get(), 0);
			*/

			ID3D11Buffer* d3dBuffer = static_cast<BufferDX11*>(cbBone.get())->GetD3DBuffer();

			D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
			HRESULT hr = deviceContext->Map(d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11DeviceContext::Map() : Failed!!\n");
				return;
			}

			CbBone* cbBone = reinterpret_cast<CbBone*>(d3dMappedResource.pData);
			for (s32 i = 0; i < boneTransformCount; ++i)
			{
				//cbBone->bones[i] = boneTransforms[i];
				cbBone->bones[i] = Math::MatrixTranspose(boneTransforms[i]);
			}

			deviceContext->Unmap(d3dBuffer, 0);
		}
		void ContextDX11::UpdateConstantBufferBoneDf(const Math::Matrix boneTransforms[], s32 boneTransformCount)
		{
			/*
			BreadMappedSubresource mappedResource;
			Map(cbBone.get(), 0, BreadMap::WriteDiscard, 0, &mappedResource);

			CbBone* constantBufferBone = reinterpret_cast<CbBone*>(mappedResource.data);
			for (s32 i = 0; i < boneTransformCount; ++i)
			{
				constantBufferBone->bones[i] = Math::MatrixTranspose(boneTransforms[i]);
			}

			Unmap(cbBone.get(), 0);
			*/

			ID3D11Buffer* d3dBuffer = static_cast<BufferDX11*>(cbBone.get())->GetD3DBuffer();

			D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
			HRESULT hr = deferredContext->Map(d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11DeviceContext::Map() : Failed!!\n");
				return;
			}

			CbBone* cbBone = reinterpret_cast<CbBone*>(d3dMappedResource.pData);
			for (s32 i = 0; i < boneTransformCount; ++i)
			{
				//cbBone->bones[i] = boneTransforms[i];
				cbBone->bones[i] = Math::MatrixTranspose(boneTransforms[i]);
			}

			deferredContext->Unmap(d3dBuffer, 0);

			//CbBone cb;
			//for (s32 i = 0; i < boneTransformCount; ++i)
			//{
			//	//cbBone->bones[i] = boneTransforms[i];
			//	cb.bones[i] = Math::MatrixTranspose(boneTransforms[i]);
			//}

			//UpdateSubresource(cbBone.get(), 0, 0, &cb, 0, 0);
		}

		// バッファコピー
		void ContextDX11::CopyResource(IBuffer* destinationBuffer, IBuffer* sourceBuffer)
		{
			ID3D11Buffer* d3dDestinationBuffer = static_cast<BufferDX11*>(destinationBuffer)->GetD3DBuffer();
			ID3D11Buffer* d3dSourceBuffer      = static_cast<BufferDX11*>(sourceBuffer)->GetD3DBuffer();

			deviceContext->CopyResource(d3dDestinationBuffer, d3dSourceBuffer);
		}
		void ContextDX11::CopyResourceDf(IBuffer* destinationBuffer, IBuffer* sourceBuffer)
		{
			ID3D11Buffer* d3dDestinationBuffer = static_cast<BufferDX11*>(destinationBuffer)->GetD3DBuffer();
			ID3D11Buffer* d3dSourceBuffer      = static_cast<BufferDX11*>(sourceBuffer)->GetD3DBuffer();

			deferredContext->CopyResource(d3dDestinationBuffer, d3dSourceBuffer);
		}

		// ミップマップ作成
		void ContextDX11::GenerateMips(ITexture* texture)
		{
			if (!texture) return;

			ID3D11ShaderResourceView* d3dShaderResourceViews = nullptr;
			TextureDX11* textureDX11 = static_cast<TextureDX11*>(texture);
			if (textureDX11 != nullptr)
			{
				d3dShaderResourceViews = textureDX11->GetD3DShaderResourceView();
			}

			deviceContext->GenerateMips(d3dShaderResourceViews);
		}
		void ContextDX11::GenerateMipsDf(ITexture* texture)
		{
			if (!texture) return;

			ID3D11ShaderResourceView* d3dShaderResourceViews = nullptr;
			TextureDX11*              textureDX11            = static_cast<TextureDX11*>(texture);
			if (textureDX11 != nullptr)
			{
				d3dShaderResourceViews = textureDX11->GetD3DShaderResourceView();
			}

			deferredContext->GenerateMips(d3dShaderResourceViews);
		}
	} // namespace Graphics
} // namespace Bread