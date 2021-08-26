#include "pch.h"
#include "SamplerDX11.h"
#include "Device/Win/DirectX11/DeviceDX11.h"
#include "FND/Logger.h"
#include "FND/Util.h"
#include "FND/STD.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版サンプラーステート操作オブジェクト
		//****************************************************************************
		// 生成
		std::unique_ptr<ISampler> ISampler::Create()
		{
			return std::make_unique<SamplerDX11>();
		}
		
		// 初期化
		bool SamplerDX11::Initialize(IDevice* device, SamplerState state, bool enableAlways, bool enableComparison)
		{
			HRESULT hr;

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			D3D11_SAMPLER_DESC desc = {};
			FND::MemSet(&desc, 0, sizeof(desc));
			RenderStateUtillityDX11::MakeD3DSamplerDesc(state, desc, enableAlways, enableComparison);

			hr = d3dDevice->CreateSamplerState(&desc, &samplerState);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateSamplerState() : Failed!!\n");
				return false;
			}

			return true;
		}
		
		// 終了化
		void SamplerDX11::Finalize()
		{
			FND::SafeRelease(samplerState);
		}
	} // namespace Graphics
} // namespace Bread