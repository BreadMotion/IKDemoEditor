#include "pch.h"
#include "MeshDX11.h"
#include "Device/Win/DirectX11/DeviceDX11.h"
#include "../Source/Graphics/Buffer/Win/DirectX11/BufferDX11.h"
#include "FND/Logger.h"


namespace Bread
{
	namespace Graphics
	{
		// D3Dプリミティブトポロジー取得
		inline D3D11_PRIMITIVE_TOPOLOGY GetD3DPrimitiveTopology(PrimitiveTopology primitiveTopology)
		{
			switch (primitiveTopology)
			{
			case PrimitiveTopology::PointList:
				return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;
			case PrimitiveTopology::LineList:
				return D3D_PRIMITIVE_TOPOLOGY_LINELIST;
			case PrimitiveTopology::LineStrip:
				return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;
			case PrimitiveTopology::TriangleList:
				return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			case PrimitiveTopology::TriangleStrip:
				return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			}

			return D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
		}

		// 生成
		std::unique_ptr<IMesh> IMesh::Create()
		{
			return std::make_unique<MeshDX11>();
		}

		// 初期化
		bool MeshDX11::Initialize(IDevice* device, const MeshDesc& desc)
		{
			BaseInitialize(desc);

			if (!BuildVertexBuffer(device, VertexBufferKind::Position, desc.positions, desc.vertexCount))
			{
				return false;
			}
			if (!BuildVertexBuffer(device, VertexBufferKind::Normal, desc.normals, desc.vertexCount))
			{
				return false;
			}
			if (!BuildVertexBuffer(device, VertexBufferKind::Tangent, desc.tangents, desc.vertexCount))
			{
				return false;
			}
			if (!BuildVertexBuffer(device, VertexBufferKind::Color, desc.colors, desc.vertexCount))
			{
				return false;
			}
			if (!BuildVertexBuffer(device, VertexBufferKind::TexCoord0, desc.texcoords0, desc.vertexCount))
			{
				return false;
			}
			if (!BuildVertexBuffer(device, VertexBufferKind::TexCoord1, desc.texcoords1, desc.vertexCount))
			{
				return false;
			}
			if (!BuildVertexBuffer(device, VertexBufferKind::TexCoord2, desc.texcoords2, desc.vertexCount))
			{
				return false;
			}
			if (!BuildVertexBuffer(device, VertexBufferKind::TexCoord3, desc.texcoords3, desc.vertexCount))
			{
				return false;
			}
			if (!BuildVertexBuffer(device, VertexBufferKind::BlendWeight0, desc.blendWeights0, desc.vertexCount))
			{
				return false;
			}
			if (!BuildVertexBuffer(device, VertexBufferKind::BlendWeight1, desc.blendWeights1, desc.vertexCount))
			{
				return false;
			}
			if (!BuildVertexBuffer(device, VertexBufferKind::BlendIndex0, desc.blendIndices0, desc.vertexCount))
			{
				return false;
			}
			if (!BuildVertexBuffer(device, VertexBufferKind::BlendIndex1, desc.blendIndices1, desc.vertexCount))
			{
				return false;
			}
			if (!BuildIndexBuffer(device, desc.indicesU16, desc.indexCount))
			{
				return false;
			}
			if (!BuildIndexBuffer(device, desc.indicesU32, desc.indexCount))
			{
				return false;
			}

			return true;
		}

		// 終了化
		void MeshDX11::Finalize()
		{
			for (VertexBuffer& vb : vertexBuffers)
			{
				vb.buffer.reset();
			}
			indexBuffers.buffer.reset();
		}

		// 描画
		void MeshDX11::Draw(IDevice* device, VertexBufferKind vbKind[], u32 kindNum, u32 start, u32 count, PrimitiveTopology primitiveTopology)
		{
			ID3D11DeviceContext* d3dContext = static_cast<DeviceDX11*>(device)->GetD3DContext();
			//ID3D11DeviceContext* d3dDfContext = static_cast<DeviceDX11*>(device)->GetD3DDfContext();

			ID3D11Buffer* d3dVbs[(int)VertexBufferKind::TypeNum];
			UINT          strides[(int)VertexBufferKind::TypeNum];
			UINT          offsets[(int)VertexBufferKind::TypeNum] = { 0 };

			bool check = false;
			for (UINT i = 0; i < kindNum; ++i)
			{
				const VertexBuffer& vb = vertexBuffers[(int)vbKind[i]];
				if (!vb.buffer)
				{
					BREAD_LOG_FRAMEWORK_WARNING("WARNING : シェーダーとの互換性がありません。それにより描画をスキップしました。\n");
					return;
				}
				d3dVbs[i]  = static_cast<BufferDX11*>(vb.buffer.get())->GetD3DBuffer();
				strides[i] = vb.stride;

				check |= vb.buffer.get() == nullptr;
			}
			if (check)
			{
				return;
			}

			D3D11_PRIMITIVE_TOPOLOGY d3dPrimitiveTopology = GetD3DPrimitiveTopology(primitiveTopology);

			d3dContext->IASetPrimitiveTopology(d3dPrimitiveTopology);
			d3dContext->IASetVertexBuffers(0, kindNum, d3dVbs, strides, offsets);

			const MeshDesc& desc = GetDesc();
			if (indexBuffers.buffer.get() != nullptr)
			{
				if (count == 0)
				{
					count = desc.indexCount;
				}
				d3dContext->IASetIndexBuffer(static_cast<BufferDX11*>(indexBuffers.buffer.get())->GetD3DBuffer(), indexBuffers.dxgiFormat, 0);
				d3dContext->DrawIndexed(count, start, 0);
			}
			else
			{
				if (count == 0)
				{
					count = desc.vertexCount;
				}
				d3dContext->Draw(count, start);
			}
		}

		// 頂点バッファ構築
		template<typename T>
		bool MeshDX11::BuildVertexBuffer(IDevice* device, VertexBufferKind kind, T* data, u32 vertexCount)
		{
			if (data == nullptr)
			{
				return true;
			}
			VertexBuffer& vb = vertexBuffers[(int)kind];
			vb.buffer = IBuffer::Create();
			vb.stride = sizeof(T);

			PhoenixBufferDesc desc = {};
			desc.byteWidth           = static_cast<u32>(sizeof(T) * vertexCount);
			desc.usage               = PhoenixUsage::Default;
			desc.bindFlags           = static_cast<u32>(PhoenixBindFlag::VertexBuffer);
			desc.cpuAccessFlags      = 0;
			desc.miscFlags           = 0;
			desc.structureByteStride = sizeof(T);

			PhoenixSubresourceData subresourceData = {};
			subresourceData.sysMem           = data;
			subresourceData.sysMemPitch      = 0;
			subresourceData.sysMemSlicePitch = 0;

			if (!vb.buffer->Initialize(device, desc, subresourceData))
			{
				return false;
			}

			return true;
		}

		// インデックスバッファ構築
		template<typename T>
		bool MeshDX11::BuildIndexBuffer(IDevice* device, T* data, u32 indexCount)
		{
			if (data == 0)
			{
				return true;
			}
			indexBuffers.buffer     = IBuffer::Create();
			indexBuffers.dxgiFormat = sizeof(T) == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;

			PhoenixBufferDesc desc = {};
			desc.byteWidth           = static_cast<u32>(sizeof(T) * indexCount);
			desc.usage               = PhoenixUsage::Default;
			desc.bindFlags           = static_cast<u32>(PhoenixBindFlag::IndexBuffer);
			desc.cpuAccessFlags      = 0;
			desc.miscFlags           = 0;
			desc.structureByteStride = sizeof(T);

			PhoenixSubresourceData subresourceData = {};
			subresourceData.sysMem           = data;
			subresourceData.sysMemPitch      = 0;
			subresourceData.sysMemSlicePitch = 0;

			if (!indexBuffers.buffer->Initialize(device, desc, subresourceData))
			{
				return false;
			}

			return true;
		}
	} // namespace Graphics
} // namespace Bread