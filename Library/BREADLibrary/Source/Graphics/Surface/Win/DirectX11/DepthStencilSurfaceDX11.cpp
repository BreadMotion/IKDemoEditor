#include "pch.h"
#include "DepthStencilSurfaceDX11.h"
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
		// DirectX11版深度ステンシルサーフェイス操作オブジェクト
		//****************************************************************************
		// 生成
		std::unique_ptr<IDepthStencilSurface> IDepthStencilSurface::Create()
		{
			return std::make_unique<DepthStencilSurfaceDX11>();
		}

		// 初期化
		bool DepthStencilSurfaceDX11::Initialize(ID3D11Device* d3dDevice, const D3D11_TEXTURE2D_DESC& texDesc, const D3D11_DEPTH_STENCIL_VIEW_DESC* dsvDesc, const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc)
		{
			HRESULT hr;

			ID3D11Texture2D* d3dTexture = nullptr;
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

			hr = d3dDevice->CreateDepthStencilView(textureDX11->GetD3DTexture(), dsvDesc, &depthStencilView);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateDepthStencilView() : Failed!!\n");
				return false;
			}

			return true;
		}

		// 初期化
		bool DepthStencilSurfaceDX11::Initialize(IDevice* device, const DepthStencilSurfaceDesc& desc)
		{
			HRESULT hr;
			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			D3D11_TEXTURE2D_DESC d3dDesc = {};
			d3dDesc.Width = desc.width;
			d3dDesc.Height = desc.height;
			d3dDesc.MipLevels = 1;
			d3dDesc.ArraySize = 1;
			d3dDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
			d3dDesc.SampleDesc.Count = 1;
			d3dDesc.SampleDesc.Quality = 0;
			d3dDesc.Usage = D3D11_USAGE_DEFAULT;
			d3dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			d3dDesc.CPUAccessFlags = 0;
			d3dDesc.MiscFlags = 0;

			ID3D11Texture2D* d3dTexture = nullptr;
			hr = d3dDevice->CreateTexture2D(&d3dDesc, nullptr, &d3dTexture);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateTexture2D() : Failed!!\n");
				return false;
			}

			texture = ITexture::Create();
			TextureDX11* textureDX11 = static_cast<TextureDX11*>(texture.get());
			if (!textureDX11->Initialize(d3dTexture, nullptr))
			{
				return false;
			}

			D3D11_DEPTH_STENCIL_VIEW_DESC  d3dDSVDesc;
			ZeroMemory(&d3dDSVDesc, sizeof(d3dDSVDesc));
			d3dDSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			d3dDSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

			hr = d3dDevice->CreateDepthStencilView(textureDX11->GetD3DTexture(), &d3dDSVDesc, &depthStencilView);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateDepthStencilView() : Failed!!\n");
				return false;
			}

			return true;
		}

		// 終了化
		void DepthStencilSurfaceDX11::Finalize()
		{
			texture.reset();
			FND::SafeRelease(depthStencilView);
		}
	} // namespace Graphics
} // namespace Bread