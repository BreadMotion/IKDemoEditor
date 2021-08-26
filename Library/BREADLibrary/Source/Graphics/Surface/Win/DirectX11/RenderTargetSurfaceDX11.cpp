#include "pch.h"
#include "RenderTargetSurfaceDX11.h"
#include "FND/Assert.h"
#include "FND/Logger.h"
#include "FND/Util.h"
#include "Device/Win/DirectX11/DeviceDX11.h"
#include "Texture/Win/DirectX11/TextureDX11.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版レンダーターゲットサーフェイス操作オブジェクト
		//****************************************************************************
		// 生成
		std::unique_ptr<IRenderTargetSurface> IRenderTargetSurface::Create()
		{
			return std::make_unique<RenderTargetSurfaceDX11>();
		}

		// 初期化
		bool RenderTargetSurfaceDX11::Initialize(ID3D11Device* d3dDevice, ID3D11Texture2D* d3dTexture2D, ID3D11ShaderResourceView* d3dShaderResourceView)
		{
			texture = ITexture::Create();
			auto textureDX11 = static_cast<TextureDX11*>(texture.get());
			if (!textureDX11->Initialize(d3dTexture2D, d3dShaderResourceView))
			{
				return false;
			}

			HRESULT hr = d3dDevice->CreateRenderTargetView(textureDX11->GetD3DTexture(), nullptr, &renderTargetView);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateRenderTargetView() : Failed!!\n");
				return false;
			}

			return true;
		}

		// 初期化
		bool RenderTargetSurfaceDX11::Initialize(ID3D11Device* d3dDevice, const D3D11_TEXTURE2D_DESC& texDesc, const D3D11_RENDER_TARGET_VIEW_DESC* rtvDesc, const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
		{
			HRESULT hr;

			ID3D11Texture2D* d3dTexture;
			hr = d3dDevice->CreateTexture2D(&texDesc, nullptr, &d3dTexture);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateTexture2D() : Failed!!\n");
				return false;
			}

			ID3D11ShaderResourceView* d3dShaderResourceView;
			hr = d3dDevice->CreateShaderResourceView(d3dTexture, srvDesc, &d3dShaderResourceView);
			if (FAILED(hr))
			{
				d3dTexture->Release();
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateShaderResourceView() : Failed!!\n");
				return false;
			}

			texture = ITexture::Create();
			auto textureDX11 = static_cast<TextureDX11*>(texture.get());
			if (!textureDX11->Initialize(d3dTexture, d3dShaderResourceView))
			{
				return false;
			}

			hr = d3dDevice->CreateRenderTargetView(textureDX11->GetD3DTexture(), rtvDesc, &renderTargetView);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateRenderTargetView() : Failed!!\n");
				return false;
			}

			return true;
		}

		// 初期化
		bool RenderTargetSurfaceDX11::Initialize(IDevice* device, const RenderTargetSurfaceDesc& desc)
		{
			HRESULT hr;
			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			D3D11_TEXTURE2D_DESC d3dDesc = {};
			d3dDesc.Width              = desc.width;
			d3dDesc.Height             = desc.height;
			d3dDesc.MipLevels          = 1;
			d3dDesc.ArraySize          = 1;
			d3dDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
			d3dDesc.SampleDesc.Count   = 1;
			d3dDesc.SampleDesc.Quality = 0;
			d3dDesc.Usage              = D3D11_USAGE_DEFAULT;
			d3dDesc.BindFlags          = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			d3dDesc.CPUAccessFlags     = 0;
			d3dDesc.MiscFlags          = 0;

			ID3D11Texture2D* d3dTexture;
			hr = d3dDevice->CreateTexture2D(&d3dDesc, nullptr, &d3dTexture);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateTexture2D() : Failed!!\n");
				return false;
			}

			ID3D11ShaderResourceView* d3dShaderResourceView;
			hr = d3dDevice->CreateShaderResourceView(d3dTexture, nullptr, &d3dShaderResourceView);
			if (FAILED(hr))
			{
				d3dTexture->Release();
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateShaderResourceView() : Failed!!\n");
				return false;
			}

			texture = ITexture::Create();
			auto textureDX11 = static_cast<TextureDX11*>(texture.get());
			if (!textureDX11->Initialize(d3dTexture, d3dShaderResourceView))
			{
				return false;
			}

			hr = d3dDevice->CreateRenderTargetView(textureDX11->GetD3DTexture(), nullptr, &renderTargetView);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateRenderTargetView() : Failed!!\n");
				return false;
			}

			return true;
		}

		// 終了化
		void RenderTargetSurfaceDX11::Finalize()
		{
			texture.reset();
			FND::SafeRelease(renderTargetView);
		}
	} // namespace Graphics
} // namespace Bread