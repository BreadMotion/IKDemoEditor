#include "pch.h"
#include "RasterizerDX11.h"
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
		// DirectX11版ラスタライザステート操作オブジェクト
		//****************************************************************************
		// 生成
		std::unique_ptr<IRasterizer> IRasterizer::Create()
		{
			return std::make_unique<RasterizerDX11>();
		}

		// 初期化
		bool RasterizerDX11::Initialize(IDevice* device, RasterizerState state, bool enableCull, bool enableDepth, bool enableMultisample, bool enableScissor)
		{
			HRESULT hr;

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			D3D11_RASTERIZER_DESC desc = {};
			FND::MemSet(&desc, 0, sizeof(desc));
			RenderStateUtillityDX11::MakeD3DRasterizerDesc(state, desc, enableCull, enableDepth, enableMultisample, enableScissor);

			hr = d3dDevice->CreateRasterizerState(&desc, &rasterizerState);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateRasterizerState() : Failed!!\n");
				return false;
			}

			return true;
		}

		// 終了化
		void RasterizerDX11::Finalize()
		{
			FND::SafeRelease(rasterizerState);
		}
	} // namespace Graphics
} // namespace Bread