#include "pch.h"
#include "DepthStencilDX11.h"
#include "Device/Win/DirectX11/DeviceDX11.h"
#include "FND/Assert.h"
#include "FND/Logger.h"
#include "FND/Util.h"
#include "FND/STD.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版深度ステンシルステート操作オブジェクト
		//****************************************************************************
		// 生成
		std::unique_ptr<IDepthStencil> IDepthStencil::Create()
		{
			return std::make_unique<DepthStencilDX11>();
		}

		// 初期化
		bool DepthStencilDX11::Initialize(IDevice* device, DepthState state)
		{
			HRESULT hr;

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			D3D11_DEPTH_STENCIL_DESC desc = {};
			FND::MemSet(&desc, 0, sizeof(desc));
			RenderStateUtillityDX11::MakeD3DDepthDesc(state, desc);

			hr = d3dDevice->CreateDepthStencilState(&desc, &depthStencilState);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateDepthStencilState() : Failed!!\n");
				return false;
			}

			return true;
		}

		// 終了化
		void DepthStencilDX11::Finalize()
		{
			FND::SafeRelease(depthStencilState);
		}
	} // namespace Graphics
} // namespace Bread