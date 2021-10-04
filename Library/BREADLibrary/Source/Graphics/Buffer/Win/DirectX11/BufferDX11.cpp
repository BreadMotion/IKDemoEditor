#include "pch.h"
#include "BufferDX11.h"
#include "FND/STD.h"
#include "FND/Util.h"
#include "FND/Logger.h"
#include "Device/Win/DirectX11/DeviceDX11.h"


namespace Bread
{
	namespace Graphics
	{
		std::unique_ptr<IBuffer> IBuffer::Create()
		{
			return std::make_unique<BufferDX11>();
		}

		// èâä˙âª
		bool BufferDX11::Initialize(IDevice* device, const BreadBufferDesc& desc)
		{
			HRESULT hr;

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			D3D11_BUFFER_DESC d3dDesc = {};
			FND::MemSet(&d3dDesc, 0, sizeof(d3dDesc));
			d3dDesc.Usage = static_cast<D3D11_USAGE>(desc.usage);
			d3dDesc.BindFlags = desc.bindFlags;
			d3dDesc.CPUAccessFlags = desc.cpuAccessFlags;
			d3dDesc.MiscFlags = desc.miscFlags;
			d3dDesc.ByteWidth = desc.byteWidth;
			d3dDesc.StructureByteStride = desc.structureByteStride;

			hr = d3dDevice->CreateBuffer(&d3dDesc, 0, &buffer);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateBuffer() : Failed!!\n");
				return false;
			}

			return true;
		}

		bool BufferDX11::Initialize(IDevice* device, const BreadBufferDesc& desc, const BreadSubresourceData& data)
		{
			HRESULT hr;

			ID3D11Device* d3dDevice = static_cast<DeviceDX11*>(device)->GetD3DDevice();

			D3D11_BUFFER_DESC d3dDesc = {};
			FND::MemSet(&d3dDesc, 0, sizeof(d3dDesc));
			d3dDesc.Usage = static_cast<D3D11_USAGE>(desc.usage);
			d3dDesc.BindFlags = desc.bindFlags;
			d3dDesc.CPUAccessFlags = desc.cpuAccessFlags;
			d3dDesc.MiscFlags = desc.miscFlags;
			d3dDesc.ByteWidth = desc.byteWidth;
			d3dDesc.StructureByteStride = desc.structureByteStride;

			D3D11_SUBRESOURCE_DATA d3dSubresourceData;
			d3dSubresourceData.pSysMem = data.sysMem;
			d3dSubresourceData.SysMemPitch = data.sysMemPitch;
			d3dSubresourceData.SysMemSlicePitch = data.sysMemSlicePitch;

			hr = d3dDevice->CreateBuffer(&d3dDesc, &d3dSubresourceData, &buffer);
			if (FAILED(hr))
			{
				BREAD_LOG_GRP_ERROR("ID3D11Device::CreateBuffer() : Failed!!\n");
				return false;
			}

			return true;
		}

		// èIóπâª
		void BufferDX11::Finalize()
		{
			FND::SafeRelease(buffer);
		}

		// èÓïÒéÊìæ
		void BufferDX11::GetDesc(BreadBufferDesc* desc)
		{
			D3D11_BUFFER_DESC d3dDesc = {};
			FND::MemSet(&d3dDesc, 0, sizeof(d3dDesc));
			buffer->GetDesc(&d3dDesc);

			desc->usage = static_cast<BreadUsage>(d3dDesc.Usage);
			desc->bindFlags = d3dDesc.BindFlags;
			desc->cpuAccessFlags = d3dDesc.CPUAccessFlags;
			desc->miscFlags = d3dDesc.MiscFlags;
			desc->byteWidth = d3dDesc.ByteWidth;
			desc->structureByteStride = d3dDesc.StructureByteStride;
		}
	} // namespace Graphics
} // namespace Bread