#include "pch.h"
#include "BlendDX11.h"
#include "Device/Win/DirectX11/DeviceDX11.h"
#include "FND/Logger.h"
#include "FND/Util.h"
#include "FND/STD.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版ブレンドステート操作オブジェクト
		//****************************************************************************
		// 生成
		std::unique_ptr<IBlend> IBlend::Create()
		{
			return std::make_unique<BlendDX11>();
		}
		
		// 初期化
		bool BlendDX11::Initialize(IDevice* device, BlendState state)
		{
			HRESULT hr;

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			D3D11_BLEND_DESC desc = {};
			FND::MemSet(&desc, 0, sizeof(desc));
			RenderStateUtillityDX11::MakeD3DBlendDesc(state, desc);

			hr = d3dDevice->CreateBlendState(&desc, &blendState);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateBlendState() : Failed!!\n");
				return false;
			}

			return true;
		}
		
		// 終了化
		void BlendDX11::Finalize()
		{
			FND::SafeRelease(blendState);
		}
	} // namespace Graphics
} // namespace Bread