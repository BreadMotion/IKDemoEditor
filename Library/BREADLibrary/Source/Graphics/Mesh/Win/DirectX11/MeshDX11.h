#pragma once

#include <d3d11.h>
#include "Graphics/Buffer.h"
#include "./Mesh/Win/Mesh.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版メッシュ操作オブジェクト
		//****************************************************************************
		class MeshDX11 final : public Mesh
		{
		private:
			struct VertexBuffer
			{
				std::unique_ptr<IBuffer> buffer = nullptr;
				UINT stride = 0;
			};
			struct IndexBuffer
			{
				std::unique_ptr<IBuffer> buffer = nullptr;
				DXGI_FORMAT dxgiFormat = {};
			};

		private:
			VertexBuffer vertexBuffers[(int)VertexBufferKind::TypeNum];
			IndexBuffer indexBuffers;

		public:
			MeshDX11() {}
			~MeshDX11() override { Finalize(); }

		public:
			// 初期化
			bool Initialize(IDevice* device, const MeshDesc& desc) override;

			// 終了化
			void Finalize() override;

			// 描画
			void Draw(IDevice* device, VertexBufferKind vbKind[], u32 kindNum, u32 start, u32 count, PrimitiveTopology primitiveTopology) override;
		
		private:
			template<typename T>
			bool BuildVertexBuffer(IDevice* device, VertexBufferKind kind, T* data, u32 vertexCount);

			template<typename T>
			bool BuildIndexBuffer(IDevice* device, T* data, u32 indexCount);
		};
	} // namespace Graphics
} // namespace Bread