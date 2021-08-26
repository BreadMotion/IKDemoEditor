#include "pch.h"
#include "DeviceDX11.h"
#include "FND/Assert.h"
#include "FND/Logger.h"
#include "FND/Util.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版デバイス操作オブジェクト
		//****************************************************************************
		// 生成
		std::unique_ptr<IDevice> IDevice::Create()
		{
			return std::make_unique<DeviceDX11>();
		}

		// 初期化
		bool DeviceDX11::Initialize()
		{
			HRESULT hr = S_OK;

			UINT flags = 0;

			hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("IDXGIFactoryの生成に失敗しました。\n");
				return false;
			}

			// Adapter
			IDXGIAdapter* dxgiAdapter;
			std::vector<IDXGIAdapter*> dxgiAdapters;

			for (UINT i = 0; dxgiFactory->EnumAdapters(i, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND; i++)
			{
				dxgiAdapters.push_back(dxgiAdapter);
			}

			dxgiAdapter = dxgiAdapters[0];

			DXGI_ADAPTER_DESC dxgiAdapterDesc;
			dxgiAdapter->GetDesc(&dxgiAdapterDesc);


			// Driver Type
			D3D_DRIVER_TYPE driverTypes[] =
			{
				D3D_DRIVER_TYPE_UNKNOWN,
				D3D_DRIVER_TYPE_HARDWARE,
				D3D_DRIVER_TYPE_WARP,
				D3D_DRIVER_TYPE_REFERENCE,
			};
			UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);


			// Feature Level
			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0
			};
			UINT numFeatureLevels = sizeof(featureLevels) / sizeof(featureLevels[0]);

			D3D_FEATURE_LEVEL featureLevel;


			// Create Device
			for (UINT idx = 0; idx < numDriverTypes; ++idx)
			{
				D3D_DRIVER_TYPE driverType = driverTypes[idx];
				hr = D3D11CreateDevice(
					dxgiAdapter,
					driverType,
					0,
					flags,
					featureLevels,
					_countof(featureLevels),
					D3D11_SDK_VERSION,
					&device,
					&featureLevel,
					&immediateContext
				);
				if (SUCCEEDED(hr))
				{
					break;
				}
			}
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("D3D11CreateDevice() : Failed!!\n");
				return false;
			}

			//create DefferdContext
			hr = device->CreateDeferredContext(0, &dfImmediateContext);
			if (FAILED(hr))
			{
				return false;
			}

			return true;
		}

		// 終了化
		void DeviceDX11::Finalize()
		{
			if (dfImmediateContext)
			{
				dfImmediateContext->ClearState();
				dfImmediateContext->Flush();
			}
			if (immediateContext)
			{
				immediateContext->ClearState();
				immediateContext->Flush();
			}
			FND::SafeRelease(dfImmediateContext);
			FND::SafeRelease(immediateContext);
			FND::SafeRelease(device);
			FND::SafeRelease(dxgiFactory);
		}
	} // namespace Graphics
} // namespace Bread