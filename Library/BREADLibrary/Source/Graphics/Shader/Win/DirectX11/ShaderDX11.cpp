#include "pch.h"
#include <list>
#include "ShaderDX11.h"
#include "OS/Path.h"
#include "FND/Util.h"
#include "../Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../Graphics/Context/Win/DirectX11/ContextDX11.h"
#include "../Source/Loader/ResourceManager.h"
#include "../Source/Graphics/Buffer/Win/DirectX11/BufferDX11.h"

namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版グラフィックスデバイス操作オブジェクト
		//****************************************************************************
		// 生成
		std::unique_ptr<IShader> IShader::Create()
		{
			return std::make_unique<ShaderDX11>();
		}

		// 終了化
		void ShaderDX11::Finalize()
		{
			FND::SafeRelease(inputLayout);
			FND::SafeRelease(pixelShader);
			FND::SafeRelease(geometryShader);
			FND::SafeRelease(vertexShader);
		}

		// 頂点シェーダー読み込み
		void ShaderDX11::LoadVS(IDevice* device, const char* csoNameOfVertexShader, PhoenixInputElementDesc* inputElementDesc, u32 numElements)
		{
#ifdef _WIN64
			const char* filename = Bread::OS::Path::Combine(toShaderPath, csoNameOfVertexShader);
#else
			const char* filename = Bread::OS::Path::Combine(toShaderPath, csoNameOfVertexShader);
#endif
			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			std::vector<D3D11_INPUT_ELEMENT_DESC> descList;
			for (u32 i = 0; i < numElements; i++)
			{
				D3D11_INPUT_ELEMENT_DESC desc = {};
				desc.SemanticName         = static_cast<LPCSTR>(inputElementDesc[i].semanticName);
				desc.Format               = static_cast<DXGI_FORMAT>(inputElementDesc[i].format);
				desc.InputSlotClass       = static_cast<D3D11_INPUT_CLASSIFICATION>(inputElementDesc[i].inputSlotClass);
				desc.SemanticIndex        = inputElementDesc[i].semanticIndex;
				desc.InputSlot            = inputElementDesc[i].inputSlot;
				desc.AlignedByteOffset    = inputElementDesc[i].alignedByteOffset;
				desc.InstanceDataStepRate = inputElementDesc[i].instanceDataStepRate;

				descList.emplace_back(desc);
			}

			std::unique_ptr<OS::IFileStream> file;
			file = OS::IFileStream::Create();
			file->Initialize(nullptr);

			const char* fullPass = OS::Path::GetFullPath(filename);
			if (file->Exists(fullPass))
				ResourceManager::CreateVertexShaderAndInputLayout(d3dDevice, fullPass, &vertexShader, &inputLayout, descList.data(), numElements);
		}

		// ジオメトリシェーダー読み込み
		void ShaderDX11::LoadGS(IDevice* device, const char* csoNameOfGeometryShader)
		{
#ifdef _WIN64
			const char* filename = Bread::OS::Path::Combine(toShaderPath, csoNameOfGeometryShader);
#else
			const char* filename = Bread::OS::Path::Combine(toShaderPath, csoNameOfGeometryShader);
#endif

			std::unique_ptr<OS::IFileStream> file;
			file = OS::IFileStream::Create();
			file->Initialize(nullptr);

			const char* fullPass    = OS::Path::GetFullPath(filename);

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();
			if (file->Exists(fullPass)) ResourceManager::CreateGeometryShader(d3dDevice, fullPass, &geometryShader);
		}

		// ピクセルシェーダー読み込み
		void ShaderDX11::LoadPS(IDevice* device, const char* csoNameOfPixelShader)
		{
#ifdef _WIN64
			const char* filename = Bread::OS::Path::Combine(toShaderPath, csoNameOfPixelShader);
#else
			const char* filename = Bread::OS::Path::Combine(toShaderPath, csoNameOfPixelShader);
#endif
			std::unique_ptr<OS::IFileStream> file;
			file = OS::IFileStream::Create();
			file->Initialize(nullptr);

			const char* fullPass = OS::Path::GetFullPath(filename);

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();
			if (file->Exists(fullPass)) ResourceManager::CreatePixelShader(d3dDevice, fullPass, &pixelShader);
		}

		// シェーダー開始
		void ShaderDX11::Activate(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			//ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DDfContext();

			d3dDeviceContext->IAGetInputLayout(&cachedInputLayout);
			d3dDeviceContext->VSGetShader(&cachedVertexShader, 0, 0);
			d3dDeviceContext->GSGetShader(&cachedGeometryShader, 0, 0);
			d3dDeviceContext->PSGetShader(&cachedPixelShader, 0, 0);

			if (vertexShader)
			{
				if (inputLayout)
				{
					d3dDeviceContext->IASetInputLayout(inputLayout);
				}
				d3dDeviceContext->VSSetShader(vertexShader, 0, 0);
			}
			if (geometryShader)
			{
				d3dDeviceContext->GSSetShader(geometryShader, 0, 0);
			}
			if (pixelShader)
			{
				d3dDeviceContext->PSSetShader(pixelShader, 0, 0);
			}
		}

		void ShaderDX11::ActivateVS(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			//ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DDfContext();

			d3dDeviceContext->IAGetInputLayout(&cachedInputLayout);
			d3dDeviceContext->VSGetShader(&cachedVertexShader, 0, 0);

			d3dDeviceContext->IASetInputLayout(inputLayout);
			d3dDeviceContext->VSSetShader(vertexShader, 0, 0);
		}

		void ShaderDX11::ActivateGS(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			//ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DDfContext();

			d3dDeviceContext->GSGetShader(&cachedGeometryShader, 0, 0);
			d3dDeviceContext->GSSetShader(geometryShader, 0, 0);
		}

		void ShaderDX11::ActivatePS(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			//ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DDfContext();

			d3dDeviceContext->PSGetShader(&cachedPixelShader, 0, 0);
			d3dDeviceContext->PSSetShader(pixelShader, 0, 0);
		}

		// シェーダー終了
		void ShaderDX11::Deactivate(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			//ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DDfContext();

			d3dDeviceContext->IASetInputLayout(cachedInputLayout);
			d3dDeviceContext->VSSetShader(cachedVertexShader, 0, 0);
			d3dDeviceContext->GSSetShader(cachedGeometryShader, 0, 0);
			d3dDeviceContext->PSSetShader(cachedPixelShader, 0, 0);
		}

		void ShaderDX11::DeactivateVS(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			//ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DDfContext();

			d3dDeviceContext->IASetInputLayout(cachedInputLayout);
			d3dDeviceContext->VSSetShader(cachedVertexShader, 0, 0);
		}

		void ShaderDX11::DeactivateGS(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			//ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DDfContext();

			d3dDeviceContext->GSSetShader(cachedGeometryShader, 0, 0);
		}

		void ShaderDX11::DeactivatePS(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			//ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DDfContext();

			d3dDeviceContext->PSSetShader(cachedPixelShader, 0, 0);
		}

		//****************************************************************************
		// DirectX11版コンピュートシェーダー操作オブジェクト
		//****************************************************************************
		// 生成
		std::unique_ptr<IComputeShader> IComputeShader::Create()
		{
			return std::make_unique<ComputeShaderDX11>();
		}

		// 終了化
		void ComputeShaderDX11::Finalize()
		{
			FND::SafeRelease(computeShader);
		}

		// コンピュートシェーダー読み込み
		void ComputeShaderDX11::Load(IDevice* device, const char* csoNameOfComputeShader)
		{
#ifdef _WIN64
			const char* filename = Bread::OS::Path::Combine(toShaderPath, csoNameOfComputeShader);
#else
			const char* filename = Bread::OS::Path::Combine(toShaderPath, csoNameOfComputeShader);
#endif
			std::unique_ptr<OS::IFileStream> file;
			file = OS::IFileStream::Create();
			file->Initialize(nullptr);

			const char* fullPass = OS::Path::GetFullPath(filename);

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();
			if (file->Exists(fullPass)) ResourceManager::CreateComputeShader(d3dDevice, fullPass, &computeShader);
		}

		// シェーダー開始
		void ComputeShaderDX11::Activate(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();

			d3dDeviceContext->CSGetShader(&cachedcomputeShader, 0, 0);
			d3dDeviceContext->CSSetShader(computeShader, 0, 0);
		}

		// シェーダー終了
		void ComputeShaderDX11::Deactivate(IDevice* device)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			d3dDeviceContext->CSSetShader(cachedcomputeShader, 0, 0);
		}

		// コンピュートシェーダー実行
		void ComputeShaderDX11::Dispatch(IDevice* device, u32 x, u32 y, u32 z)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			d3dDeviceContext->Dispatch(x, y, z);
		}

		// コンピュートシェーダー実行
		void ComputeShaderDX11::DispatchIndirect(IDevice* device, IBuffer* buffer, u32 offset)
		{
			ID3D11DeviceContext* d3dDeviceContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			ID3D11Buffer*        d3dBuffer        = static_cast<BufferDX11*>(buffer)->GetD3DBuffer();

			d3dDeviceContext->DispatchIndirect(d3dBuffer, offset);
		}
	} // namespace Graphics
} // namespace Bread