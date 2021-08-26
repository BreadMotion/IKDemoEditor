#include "pch.h"
#include "FrameWork/Quad/Quad.h"
#include "FND/Util.h"
#include "FND/STD.h"
#include "../Source/Graphics/Texture/Win/DirectX11/TextureDX11.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"
#include "../Source/Graphics/Buffer/Win/DirectX11/BufferDX11.h"


namespace Bread
{
	namespace FrameWork
	{
		std::unique_ptr<Quad> Quad::Create()
		{
			return std::make_unique<Quad>();
		}

		bool Quad::Initialize
		(
			Graphics::IGraphicsDevice* graphicsDevice,
			Graphics::SamplerState     samplerState,
			Math::Color                samplerBoarderColor
		)
		{
			Graphics::IDevice* device    = graphicsDevice->GetDevice();
			ID3D11Device*      d3dDevice = static_cast<Graphics::DeviceDX11*>(device)->GetD3DDevice();

			vertex vertices[] =
			{
				{ Math::Vector3(0, 0, 0), Math::Vector2(0, 0), Math::Vector4(1, 1, 1, 1) },
				{ Math::Vector3(0, 0, 0), Math::Vector2(0, 0), Math::Vector4(1, 1, 1, 1) },
				{ Math::Vector3(0, 0, 0), Math::Vector2(0, 0), Math::Vector4(1, 1, 1, 1) },
				{ Math::Vector3(0, 0, 0), Math::Vector2(0, 0), Math::Vector4(1, 1, 1, 1) },
			};

			HRESULT hr = S_OK;
			D3D11_BUFFER_DESC buffer_desc = {};
			buffer_desc.ByteWidth           = sizeof(vertices);
			buffer_desc.Usage               = D3D11_USAGE_DYNAMIC;
			buffer_desc.BindFlags           = D3D11_BIND_VERTEX_BUFFER;
			buffer_desc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
			buffer_desc.MiscFlags           = 0;
			buffer_desc.StructureByteStride = 0;
			D3D11_SUBRESOURCE_DATA subresource_data = {};
			subresource_data.pSysMem          = vertices;
			subresource_data.SysMemPitch      = 0;
			subresource_data.SysMemSlicePitch = 0;
			hr = d3dDevice->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.GetAddressOf());
			//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

			Graphics::PhoenixBufferDesc bufferDesc = {};
			bufferDesc.byteWidth           = sizeof(vertices);
			bufferDesc.usage               = Graphics::PhoenixUsage::Dynamic;
			bufferDesc.bindFlags           = static_cast<u32>(Graphics::PhoenixBindFlag::VertexBuffer);
			bufferDesc.cpuAccessFlags      = static_cast<u32>(Graphics::PhoenixCPUAccessFlag::CPUAccessWrite);
			bufferDesc.miscFlags           = 0;
			bufferDesc.structureByteStride = 0;

			Graphics::PhoenixSubresourceData subresourceData = {};
			subresourceData.sysMem           = vertices;
			subresourceData.sysMemPitch      = 0;
			subresourceData.sysMemSlicePitch = 0;

			vertexBuffer = Graphics::IBuffer::Create();
			if (!vertexBuffer->Initialize(device, bufferDesc, subresourceData))
			{
				return false;
			}

			Graphics::PhoenixInputElementDesc inputElementDesc[] =
			{
				// SemanticName	 SemanticIndex	Format													InputSlot	AlignedByteOffset					InputSlotClass										InstanceDataStepRate
				{"POSITION",	 0,				Bread::Graphics::PHOENIX_FORMAT_R32G32B32_FLOAT,		0,			PHOENIX_APPEND_ALIGNED_ELEMENT,		Bread::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"TEXCOORD",	 0,				Bread::Graphics::PHOENIX_FORMAT_R32G32_FLOAT,			0,			PHOENIX_APPEND_ALIGNED_ELEMENT,		Bread::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
				{"COLOR",		 0,				Bread::Graphics::PHOENIX_FORMAT_R32G32B32A32_FLOAT,  	0,			PHOENIX_APPEND_ALIGNED_ELEMENT,		Bread::Graphics::PHOENIX_INPUT_PER_VERTEX_DATA,	0 },
			};

			embeddedVertexShader = Graphics::IShader::Create();
			embeddedVertexShader->LoadVS
			(
				device,
				"QuadVS.cso",
				inputElementDesc,
				Bread::FND::ArraySize(inputElementDesc)
			);


			embeddedPixelShader[0] = Graphics::IShader::Create();
			embeddedPixelShader[0]->LoadPS
			(
				device,
				"QuadPS.cso"
			);

			embeddedPixelShader[1] = Graphics::IShader::Create();
			embeddedPixelShader[1]->LoadPS
			(
				device,
				"QuadPSMS.cso"
			);

			embeddedFilterPixelShader = Graphics::IShader::Create();
			embeddedFilterPixelShader->LoadPS
			(
				device,
				"QuadFilterPS.cso"
			);

			embeddedAlphaCutOffPixelShader = Graphics::IShader::Create();
			embeddedAlphaCutOffPixelShader->LoadPS
			(
				device,
				"QuadAlphaCutOffPS.cso"
			);

			embeddedDissolvePixelShader = Graphics::IShader::Create();
			embeddedDissolvePixelShader->LoadPS
			(
				device,
				"QuadDissolvePS.cso"
			);

			embeddedSamplerState = Graphics::ISampler::Create();
			if (!embeddedSamplerState->Initialize(device, samplerState))
			{
				return false;
			}

			embeddedRasterizerState = Graphics::IRasterizer::Create();
			if (!embeddedRasterizerState->Initialize(device, Graphics::RasterizerState::SolidCullNone))
			{
				return false;
			}

			embeddedDepthStencilState = Graphics::IDepthStencil::Create();
			if (!embeddedDepthStencilState->Initialize(device, Graphics::DepthState::NoTestNoWrite))
			{
				return false;
			}

			filterCB = Bread::Graphics::IBuffer::Create();
			{
				Bread::Graphics::PhoenixBufferDesc desc = {};
				Bread::FND::MemSet(&desc, 0, sizeof(desc));
				desc.usage               = Bread::Graphics::PhoenixUsage::Default;
				desc.bindFlags           = static_cast<Bread::s32>(Bread::Graphics::PhoenixBindFlag::ConstantBuffer);
				desc.cpuAccessFlags      = 0;
				desc.miscFlags           = 0;
				desc.byteWidth           = sizeof(FilterCB);
				desc.structureByteStride = 0;
				if (!filterCB->Initialize(device, desc))
				{
					return false;
				}
			}

			dissolveCB = Bread::Graphics::IBuffer::Create();
			{
				Bread::Graphics::PhoenixBufferDesc desc = {};
				Bread::FND::MemSet(&desc, 0, sizeof(desc));
				desc.usage               = Bread::Graphics::PhoenixUsage::Default;
				desc.bindFlags           = static_cast<Bread::s32>(Bread::Graphics::PhoenixBindFlag::ConstantBuffer);
				desc.cpuAccessFlags      = 0;
				desc.miscFlags           = 0;
				desc.byteWidth           = sizeof(DissolveCB);
				desc.structureByteStride = 0;
				if (!dissolveCB->Initialize(device, desc))
				{
					return false;
				}
			}

			dissolveTexture   = Bread::Graphics::ITexture::Create();
			dissolveTexture02 = Bread::Graphics::ITexture::Create();
			emissiveTexture   = Bread::Graphics::ITexture::Create();

			return true;
		}

		void Quad::Finalize()
		{
			emissiveTexture.reset();
			dissolveTexture.reset();

			dissolveCB.reset();

			embeddedSamplerState.reset();
			embeddedDepthStencilState.reset();
			embeddedRasterizerState.reset();

			embeddedPixelShader[1].reset();
			embeddedPixelShader[0].reset();
			embeddedVertexShader.reset();

			vertexBuffer.reset();
		}

		void Quad::Draw
		(
			Graphics::IGraphicsDevice* graphicsDevice,
			Graphics::ITexture* shaderResourceView,
			float dx, float dy, float dw, float dh,
			float sx, float sy, float sw, float sh,
			float angle/*degree*/,
			float r, float g, float b, float a,
			bool useEmbeddedVertexShader,
			bool useEmbeddedPixelShader,
			bool useEmbeddedRasterizerState,
			bool useEmbeddedDepthStencilState,
			bool useEmbeddedSamplerState,
			bool useEmbeddedFileter,
			bool useEmbeddedDissolve,
			bool useEmbeddedDissolveEmissive
		) const
		{
			HRESULT hr = S_OK;

			Graphics::IDevice*  device      = graphicsDevice->GetDevice();
			Graphics::IContext* context     = graphicsDevice->GetContext();
			ID3D11DeviceContext* d3dContext = static_cast<Graphics::DeviceDX11*>(device)->GetD3DContext();

			Graphics::TextureDescDx srvDesc = {};
			shaderResourceView->GetShaderResourceViewDesc(&srvDesc);

			bool multisampled = srvDesc.dimension == Graphics::TextureDimensionDx::SRV_DIMENSION_TEXTURE2DMS;

			Graphics::TextureDesc texDesc = {};
			shaderResourceView->GetTextureDesc(&texDesc);

			Graphics::Viewport* viewport[] = { new Graphics::Viewport() };
			context->GetViewports(1, viewport);

			float screenWidth  = viewport[0]->width;
			float screenHeight = viewport[0]->height;

			FND::SafeDelete(viewport[0]);

			// Set each sprite's vertices coordinate to screen spaceenum BLEND_STATE
			// left-top
			float x0 = dx;
			float y0 = dy;
			// right-top
			float x1 = dx + dw;
			float y1 = dy;
			// left-bottom
			float x2 = dx;
			float y2 = dy + dh;
			// right-bottom
			float x3 = dx + dw;
			float y3 = dy + dh;

			// Translate sprite's centre to origin (rotate centre)
			float mx = dx + dw * 0.5f;
			float my = dy + dh * 0.5f;
			x0 -= mx;
			y0 -= my;
			x1 -= mx;
			y1 -= my;
			x2 -= mx;
			y2 -= my;
			x3 -= mx;
			y3 -= my;

			// Rotate each sprite's vertices by angle
			float rx, ry;
			float cos = cosf(angle * 0.01745f);
			float sin = sinf(angle * 0.01745f);
			rx = x0;
			ry = y0;
			x0 = cos * rx + -sin * ry;
			y0 = sin * rx + cos * ry;
			rx = x1;
			ry = y1;
			x1 = cos * rx + -sin * ry;
			y1 = sin * rx + cos * ry;
			rx = x2;
			ry = y2;
			x2 = cos * rx + -sin * ry;
			y2 = sin * rx + cos * ry;
			rx = x3;
			ry = y3;
			x3 = cos * rx + -sin * ry;
			y3 = sin * rx + cos * ry;

			// Translate sprite's centre to original position
			x0 += mx;
			y0 += my;
			x1 += mx;
			y1 += my;
			x2 += mx;
			y2 += my;
			x3 += mx;
			y3 += my;

			// Convert to NDC space
			x0 = 2.0f * x0        / screenWidth - 1.0f;
			y0 = 1.0f - 2.0f * y0 / screenHeight;
			x1 = 2.0f * x1        / screenWidth - 1.0f;
			y1 = 1.0f - 2.0f * y1 / screenHeight;
			x2 = 2.0f * x2        / screenWidth - 1.0f;
			y2 = 1.0f - 2.0f * y2 / screenHeight;
			x3 = 2.0f * x3        / screenWidth - 1.0f;
			y3 = 1.0f - 2.0f * y3 / screenHeight;

			Graphics::PhoenixMap map = Graphics::PhoenixMap::WriteDiscard;
			Graphics::PhoenixMappedSubresource mapedBuffer;
			context->Map(vertexBuffer.get(), 0, map, 0, &mapedBuffer);

			vertex* vertices = static_cast<vertex*>(mapedBuffer.data);
			vertices[0].position.x = x0;
			vertices[0].position.y = y0;
			vertices[1].position.x = x1;
			vertices[1].position.y = y1;
			vertices[2].position.x = x2;
			vertices[2].position.y = y2;
			vertices[3].position.x = x3;
			vertices[3].position.y = y3;
			vertices[0].position.z = vertices[1].position.z = vertices[2].position.z = vertices[3].position.z = 0.0f;

			Math::Vector4 color(r, g, b, a);
			vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = color;

			vertices[0].texcoord.x = (sx)      / texDesc.width;
			vertices[0].texcoord.y = (sy)      / texDesc.height;
			vertices[1].texcoord.x = (sx + sw) / texDesc.width;
			vertices[1].texcoord.y = (sy)      / texDesc.height;
			vertices[2].texcoord.x = (sx)      / texDesc.width;
			vertices[2].texcoord.y = (sy + sh) / texDesc.height;
			vertices[3].texcoord.x = (sx + sw) / texDesc.width;
			vertices[3].texcoord.y = (sy + sh) / texDesc.height;

			context->Unmap(vertexBuffer.get(), 0);

			Graphics::BufferDX11* bufferDX11 = static_cast<Graphics::BufferDX11*>(vertexBuffer.get());

			UINT stride = sizeof(vertex);
			UINT offset = 0;
			ID3D11Buffer* buffers[] = { bufferDX11->GetD3DBuffer() };

			d3dContext->IASetVertexBuffers(0, 1, buffers, &stride, &offset);
			d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			if (useEmbeddedVertexShader)
			{
				embeddedVertexShader->Activate(device);
			}
			if (useEmbeddedPixelShader)
			{
				if (useEmbeddedFileter)
				{
					FilterCB cb = {};
					{
						cb.bright      = bright;
						cb.contrast    = contrast;
						cb.saturate    = saturate;
						cb.option      = 0.0f;
						cb.screenColor = screenColor;
					}

					Bread::Graphics::IBuffer* psCBuffer[] =
					{
						filterCB.get()
					};
					context->UpdateSubresource(filterCB.get(), 0, 0, &cb, 0, 0);
					context->SetConstantBuffers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(psCBuffer), psCBuffer);

					embeddedFilterPixelShader->Activate(device);
				}
				else if (useEmbeddedDissolve)
				{
					DissolveCB cb = {};
					{
						cb.dissolveThreshold     = dissolveThreshold;
						cb.dissolveEmissiveWidth = dissolveEmissiveWidth;
						cb.dummy[0] = 0.0f;
						cb.dummy[1] = 0.0f;
					}

					Bread::Graphics::IBuffer* psCBuffer[] =
					{
						dissolveCB.get()
					};
					context->UpdateSubresource(dissolveCB.get(), 0, 0, &cb, 0, 0);
					context->SetConstantBuffers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(psCBuffer), psCBuffer);

					Graphics::ITexture* texture[] =
					{
						dissolveTexture.get(),
						dissolveTexture02.get(),
						emissiveTexture.get()
					};
					context->SetShaderResources(Graphics::ShaderType::Pixel, 1, 3, texture);

					if (!useEmbeddedDissolveEmissive)
					{
						embeddedAlphaCutOffPixelShader->Activate(device);
					}
					else
					{
						embeddedDissolvePixelShader->Activate(device);
					}
				}
				else
				{
					u32 index = multisampled ? 1 : 0;
					embeddedPixelShader[index]->Activate(device);
				}
			}
			Graphics::ITexture* texture[] = { shaderResourceView };
			context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, texture);

			std::unique_ptr<Graphics::IRasterizer> cachedRasterizerState = Graphics::IRasterizer::Create();
			if (useEmbeddedRasterizerState)
			{
				context->GetRasterizer(cachedRasterizerState.get());
				context->SetRasterizer(embeddedRasterizerState.get());
			}

			std::unique_ptr<Graphics::IDepthStencil> cachedDepthStencilState = Graphics::IDepthStencil::Create();
			if (useEmbeddedDepthStencilState)
			{
				context->GetDepthStencil(cachedDepthStencilState.get(), 0);
				context->SetDepthStencil(embeddedDepthStencilState.get(), 1);
			}

			std::unique_ptr<Graphics::ISampler> cachedSamplerState = Graphics::ISampler::Create();
			if (useEmbeddedSamplerState)
			{
				Graphics::ISampler* cachedSamplerStates[] = { cachedSamplerState.get() };
				Graphics::ISampler* embeddedSamplerStates[] = { embeddedSamplerState.get() };

				context->GetSamplers(Graphics::ShaderType::Pixel, 0, 1, cachedSamplerStates);
				context->SetSamplers(Graphics::ShaderType::Pixel, 0, 1, embeddedSamplerStates);
			}
			context->Draw(4, 0);


			if (useEmbeddedVertexShader)
			{
				embeddedVertexShader->Deactivate(device);
			}
			if (useEmbeddedPixelShader)
			{
				if (useEmbeddedFileter)
				{
					Bread::Graphics::IBuffer* psCBuffer[] =
					{
						nullptr
					};
					context->SetConstantBuffers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(psCBuffer), psCBuffer);

					embeddedFilterPixelShader->Deactivate(device);
				}
				else if (useEmbeddedDissolve)
				{
					Bread::Graphics::IBuffer* psCBuffer[] =
					{
						nullptr
					};
					context->SetConstantBuffers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(psCBuffer), psCBuffer);

					Graphics::ITexture* texture[] =
					{
						nullptr,
						nullptr,
						nullptr
					};
					context->SetShaderResources(Graphics::ShaderType::Pixel, 1, 3, texture);

					if (!useEmbeddedDissolveEmissive)
					{
						embeddedAlphaCutOffPixelShader->Deactivate(device);
					}
					else
					{
						embeddedDissolvePixelShader->Deactivate(device);
					}
				}
				else
				{
					u32 index = multisampled ? 1 : 0;
					embeddedPixelShader[index]->Deactivate(device);
				}
			}

			Graphics::ITexture* nullTexture[] = { nullptr };
			context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, nullTexture);

			if (cachedRasterizerState.get())
			{
				context->SetRasterizer(cachedRasterizerState.get());
			}
			if (cachedDepthStencilState.get())
			{
				context->SetDepthStencil(cachedDepthStencilState.get(), 1);
			}
			if (cachedSamplerState.get())
			{
				Graphics::ISampler* cachedSamplerStates[] = { cachedSamplerState.get() };
				context->SetSamplers(Graphics::ShaderType::Pixel, 0, 1, cachedSamplerStates);
			}
		}

		void Quad::Draw
		(
			Graphics::IGraphicsDevice* graphicsDevice,
			Graphics::ITexture* shaderResourceView,
			float dx, float dy, float dw, float dh,
			float angle/*degree*/,
			float r, float g, float b, float a,
			bool useEmbeddedVertexShader,
			bool useEmbeddedPixelShader,
			bool useEmbeddedRasterizerState,
			bool useEmbeddedDepthStencilState,
			bool useEmbeddedSamplerState,
			bool useEmbeddedFileter,
			bool useEmbeddedDissolve,
			bool useEmbeddedDissolveEmissive
		) const
		{
			Graphics::TextureDesc texDesc = {};
			shaderResourceView->GetTextureDesc(&texDesc);

			Draw(graphicsDevice, shaderResourceView, dx, dy, dw, dh, 0.0f, 0.0f, static_cast<float>(texDesc.width), static_cast<float>(texDesc.height), angle, r, g, b, a,
				useEmbeddedVertexShader, useEmbeddedPixelShader, useEmbeddedRasterizerState, useEmbeddedDepthStencilState, useEmbeddedSamplerState, useEmbeddedFileter, useEmbeddedDissolve, useEmbeddedDissolveEmissive);
		}

		void Quad::Draw
		(
			Graphics::IGraphicsDevice* graphicsDevice,
			Graphics::ITexture* shaderResourceView,
			Bread::Math::Vector2 pos, Bread::Math::Vector2 size,
			Bread::Math::Vector2 texPos, Bread::Math::Vector2 texSize,
			float angle/*degree*/,
			float r, float g, float b, float a,
			bool useEmbeddedVertexShader,
			bool useEmbeddedPixelShader,
			bool useEmbeddedRasterizerState,
			bool useEmbeddedDepthStencilState,
			bool useEmbeddedSamplerState,
			bool useEmbeddedFileter,
			bool useEmbeddedDissolve,
			bool useEmbeddedDissolveEmissive
		) const
		{
			Graphics::TextureDesc texDesc = {};
			shaderResourceView->GetTextureDesc(&texDesc);

			Draw(graphicsDevice, shaderResourceView, pos.x, pos.y, size.x, size.y, texPos.x, texPos.y, texSize.x, texSize.y, angle, r, g, b, a,
				useEmbeddedVertexShader, useEmbeddedPixelShader, useEmbeddedRasterizerState, useEmbeddedDepthStencilState, useEmbeddedSamplerState, useEmbeddedFileter, useEmbeddedDissolve, useEmbeddedDissolveEmissive);
		}


		std::unique_ptr<FullScreenQuad> FullScreenQuad::Create()
		{
			return std::make_unique<FullScreenQuad>();
		}

		bool FullScreenQuad::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();

			embeddedVertexShader = Graphics::IShader::Create();
			embeddedVertexShader->LoadVS
			(
				device,
				"FullScreenQuadVS.cso",
				0,
				0
			);

			embeddedPixelShader[0] = Graphics::IShader::Create();
			embeddedPixelShader[0]->LoadPS
			(
				device,
				"FullScreenQuadPS.cso"
			);

			embeddedPixelShader[1] = Graphics::IShader::Create();
			embeddedPixelShader[1]->LoadPS
			(
				device,
				"FullScreenQuadPSMS.cso"
			);

			embeddedRasterizerState = Graphics::IRasterizer::Create();
			if (!embeddedRasterizerState->Initialize(device, Graphics::RasterizerState::SolidCullBack, false, false, false, false))
			{
				return false;
			}

			embeddedDepthStencilState = Graphics::IDepthStencil::Create();
			if (!embeddedDepthStencilState->Initialize(device, Graphics::DepthState::NoTestNoWrite))
			{
				return false;
			}

			return true;
		}

		void FullScreenQuad::Finalize()
		{
			embeddedDepthStencilState.reset();
			embeddedRasterizerState.reset();
			embeddedPixelShader[1].reset();
			embeddedPixelShader[0].reset();
			embeddedVertexShader.reset();
		}

		void FullScreenQuad::Draw
		(
			Graphics::IGraphicsDevice* graphicsDevice,
			bool useEmbeddedRasterizerState,
			bool useEmbeddedDepthStencilState,
			bool useEmbeddedPixelShader,
			bool enableMSAA
		)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();
			Graphics::IContext* context = graphicsDevice->GetContext();
			ID3D11DeviceContext* d3dContext = static_cast<Graphics::DeviceDX11*>(device)->GetD3DContext();

			std::unique_ptr<Graphics::IRasterizer> cachedRasterizerState = Graphics::IRasterizer::Create();
			if (useEmbeddedRasterizerState)
			{
				context->GetRasterizer(cachedRasterizerState.get());
				context->SetRasterizer(embeddedRasterizerState.get());
			}

			std::unique_ptr<Graphics::IDepthStencil> cachedDepthStencilState = Graphics::IDepthStencil::Create();
			if (useEmbeddedDepthStencilState)
			{
				context->GetDepthStencil(cachedDepthStencilState.get(), 0);
				context->SetDepthStencil(embeddedDepthStencilState.get(), 1);
			}

			if (useEmbeddedPixelShader)
			{
				u32 index = enableMSAA ? 1 : 0;
				embeddedPixelShader[index]->Activate(device);
			}

			d3dContext->IASetVertexBuffers(0, 0, 0, 0, 0);
			d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
			d3dContext->IASetInputLayout(0);

			embeddedVertexShader->Activate(device);
			context->Draw(4, 0);
			embeddedVertexShader->Deactivate(device);

			if (useEmbeddedPixelShader)
			{
				u32 index = enableMSAA ? 1 : 0;
				embeddedPixelShader[index]->Deactivate(device);
			}

			if (cachedRasterizerState.get())
			{
				context->SetRasterizer(cachedRasterizerState.get());
			}
			if (cachedDepthStencilState.get())
			{
				context->SetDepthStencil(cachedDepthStencilState.get(), 1);
			}
		}


		std::unique_ptr<Bloom> Bloom::Create()
		{
			return std::make_unique<Bloom>();
		}

		bool Bloom::Initialize(Graphics::IGraphicsDevice* graphicsDevice, u32 width, u32 height)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();

			FullScreenQuad::Initialize(graphicsDevice);
			//Quad::Initialize(graphicsDevice);

			//fullScreenQuad = FullScreenQuad::FullScreenQuad::Create();
			//fullScreenQuad->Initialize(graphicsDevice);

			// 定数バッファ作成
			{
				Graphics::PhoenixBufferDesc bufferDesc = {};
				Bread::FND::MemSet(&bufferDesc, 0, sizeof(bufferDesc));
				bufferDesc.usage               = Bread::Graphics::PhoenixUsage::Dynamic;
				bufferDesc.bindFlags           = static_cast<Bread::s32>(Bread::Graphics::PhoenixBindFlag::ConstantBuffer);
				bufferDesc.cpuAccessFlags      = static_cast<Bread::s32>(Graphics::PhoenixCPUAccessFlag::CPUAccessWrite);
				bufferDesc.miscFlags           = 0;
				bufferDesc.byteWidth           = sizeof(ShaderConstants);
				bufferDesc.structureByteStride = 0;

				constantBuffer = Graphics::IBuffer::Create();
				if (!constantBuffer->Initialize(device, bufferDesc))
				{
					return false;
				}
			}

			glowExtraction = Bread::FrameWork::FrameBuffer::Create();
			if (!glowExtraction->Initialize
			(
				graphicsDevice,
				width, height,
				false, 1,
				Bread::Graphics::TextureFormatDx::R16G16B16A16_FLOAT,
				Bread::Graphics::TextureFormatDx::UNKNOWN,
				true,
				false,
				false
			))
			{
				return false;
			}

			for (size_t indexOfDownsampled = 0; indexOfDownsampled < numberOfDownsampled; ++indexOfDownsampled)
			{
				gaussianBlur[indexOfDownsampled][0] = Bread::FrameWork::FrameBuffer::Create();
				if (!gaussianBlur[indexOfDownsampled][0]->Initialize
				(
					graphicsDevice,
					width >> indexOfDownsampled, height >> indexOfDownsampled,
					false, 1,
					Bread::Graphics::TextureFormatDx::R16G16B16A16_FLOAT,
					Bread::Graphics::TextureFormatDx::UNKNOWN,
					true,
					false,
					false
				))
				{
					return false;
				}

				gaussianBlur[indexOfDownsampled][1] = Bread::FrameWork::FrameBuffer::Create();
				if (!gaussianBlur[indexOfDownsampled][1]->Initialize
				(
					graphicsDevice,
					width  >> indexOfDownsampled,
					height >> indexOfDownsampled,
					false, 1,
					Bread::Graphics::TextureFormatDx::R16G16B16A16_FLOAT,
					Bread::Graphics::TextureFormatDx::UNKNOWN,
					true,
					false,
					false
				))
				{
					return false;
				}
			}

			glowExtractionPS = Graphics::IShader::Create();
			glowExtractionPS->LoadPS(device, "GlowExtractionPS.cso");

			gaussianBlurHorizontalPS = Graphics::IShader::Create();
			gaussianBlurHorizontalPS->LoadPS(device, "GaussianBlurHorizontalPS.cso");

			gaussianBlurVerticalPS = Graphics::IShader::Create();
			gaussianBlurVerticalPS->LoadPS(device, "GaussianBlurVerticalPS.cso");

			gaussianBlurConvolutionPS = Graphics::IShader::Create();
			gaussianBlurConvolutionPS->LoadPS(device, "GaussianBlurConvolutionPS.cso");

			gaussianBlurDownsamplingPS = Graphics::IShader::Create();
			gaussianBlurDownsamplingPS->LoadPS(device, "GaussianBlurDownsamplingPS.cso");

			bloomBlendPS = Graphics::IShader::Create();
			bloomBlendPS->LoadPS(device, "BloomBlendPS.cso");

			lensFlare = FrameBuffer::Create();
			if (!lensFlare->Initialize
			(
				graphicsDevice,
				width >> 2, height >> 2,
				false, 1,
				Bread::Graphics::TextureFormatDx::R16G16B16A16_FLOAT,
				Bread::Graphics::TextureFormatDx::UNKNOWN,
				true,
				false,
				false
			))
			{
				return false;
			}

			lensFlarePS = Graphics::IShader::Create();
			lensFlarePS->LoadPS(device, "LensFlarePS.cso");

			gradientMap = Graphics::ITexture::Create();
			//gradientMap->Initialize(device, ".\\resources\\color_gradient.02.png", Graphics::MaterialType::Diffuse, Math::Color(1, 1, 1, 1));

			noiseMap = Graphics::ITexture::Create();
			//noiseMap->Initialize(device, ".\\resources\\noise.png", Graphics::MaterialType::Diffuse, Math::Color(1, 1, 1, 1));
		
			return true;
		}

		void Bloom::Finalize()
		{

		}

		void Bloom::Generate(Graphics::IGraphicsDevice* graphicsDevice, Graphics::ITexture* hdrTexture, bool enableLensFlare)
		{
			Graphics::IDevice*  device  = graphicsDevice->GetDevice();
			Graphics::IContext* context = graphicsDevice->GetContext();

			// データセット
			{
				Graphics::ISampler* samplers[] =
				{
					context->GetSamplerState(Graphics::SamplerState::PointWrap),
					context->GetSamplerState(Graphics::SamplerState::LinearWrap),
					context->GetSamplerState(Graphics::SamplerState::AnisotropicWrap),
				};
				context->SetSamplers(Graphics::ShaderType::Pixel, 0, 3, samplers);

				Graphics::PhoenixMap map = Graphics::PhoenixMap::WriteDiscard;
				Graphics::PhoenixMappedSubresource mapedBuffer;
				{
					context->Map(constantBuffer.get(), 0, map, 0, &mapedBuffer);
					FND::MemCpy(mapedBuffer.data, &shaderContants, sizeof(ShaderConstants));
					context->Unmap(constantBuffer.get(), 0);
				}

				Graphics::IBuffer* buffers[] =
				{
					constantBuffer.get()
				};
				context->SetConstantBuffers(Graphics::ShaderType::Pixel, 0, 1, buffers);
			}

			// 輝き部分の抽出
			{
				Graphics::ITexture* texture[] = { hdrTexture };
				Graphics::ITexture* nullTexture[] = { nullptr };

				glowExtraction->Clear(graphicsDevice);
				glowExtraction->Activate(graphicsDevice);
				{
					glowExtractionPS->Activate(device);
					{
						context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, texture);
						FullScreenQuad::Draw(graphicsDevice);
						/*Quad::Draw(graphicsDevice, hdrTexture, 1280.0f * 0, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, false, false, true, true, false);*/
						//fullScreenQuad->Draw(graphicsDevice);
						context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, nullTexture);
					}
					glowExtractionPS->Deactivate(device);
				}
				glowExtraction->Deactivate(graphicsDevice);
			}

			////Lens flare
			////http://john-chapman-graphics.blogspot.com/2013/02/pseudo-lens-flare.html
			//if (enableLensFlare)
			//{
			//	Graphics::ITexture* glowExtractionTex[] = { glowExtraction->GetRenderTargetSurface()->GetTexture(), gradientMap.get(), noiseMap.get() };
			//	Graphics::ITexture* nullGlowExtractionTex[] = { nullptr, nullptr, nullptr };

			//	lensFlare->Clear(graphicsDevice);
			//	lensFlare->Activate(graphicsDevice);
			//	{
			//		lensFlarePS->Activate(device);
			//		{
			//			context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 3, glowExtractionTex);
			//			FullScreenQuad::Draw(graphicsDevice);
			//			context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 3, nullGlowExtractionTex);
			//		}
			//		lensFlarePS->Deactivate(device);
			//	}
			//	lensFlare->Deactivate(graphicsDevice);

			//	Graphics::ITexture* lensFlareTex[] = { lensFlare->GetRenderTargetSurface()->GetTexture() };
			//	Graphics::ITexture* nullLensFlareTex[] = { nullptr, nullptr, nullptr };

			//	glowExtraction->Activate(graphicsDevice);
			//	{
			//		context->SetBlend(context->GetBlendState(Graphics::BlendState::Additive), 0, 0xFFFFFFFF);
			//		context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, lensFlareTex);
			//		FullScreenQuad::Draw(graphicsDevice);
			//		context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, nullLensFlareTex);
			//		context->SetBlend(context->GetBlendState(Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
			//	}
			//	glowExtraction->Deactivate(graphicsDevice);
			//}

			//Gaussian blur
			//Efficient Gaussian blur with linear sampling
			//http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
			{
				Graphics::ISampler* samplers[] =
				{
					context->GetSamplerState(Graphics::SamplerState::LinearBorder)
				};
				context->SetSamplers(Graphics::ShaderType::Pixel, 0, 1, samplers);

				// downsampling
				{
					Graphics::ITexture* texture[] = { glowExtraction->GetRenderTargetSurface()->GetTexture() };
					context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, texture);

					gaussianBlur[0][0]->Clear(graphicsDevice);
					gaussianBlur[0][0]->Activate(graphicsDevice);
					{
						gaussianBlurDownsamplingPS->Activate(device);
						{
							FullScreenQuad::Draw(graphicsDevice);
						}
						gaussianBlurDownsamplingPS->Deactivate(device);
					}
					gaussianBlur[0][0]->Deactivate(graphicsDevice);
				}

				// ping-pong gaussian blur
				{
					// 横方向にブラー加工
					{
						Graphics::ITexture* texture[] = { gaussianBlur[0][0]->GetRenderTargetSurface()->GetTexture() };
						context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, texture);

						gaussianBlur[0][1]->Clear(graphicsDevice);
						gaussianBlur[0][1]->Activate(graphicsDevice);
						{
							gaussianBlurHorizontalPS->Activate(device);
							{
								FullScreenQuad::Draw(graphicsDevice);
							}
							gaussianBlurHorizontalPS->Deactivate(device);
						}
						gaussianBlur[0][1]->Deactivate(graphicsDevice);
					}

					// 縦方向にブラー加工
					{
						Graphics::ITexture* texture[] = { gaussianBlur[0][1]->GetRenderTargetSurface()->GetTexture() };
						context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, texture);

						gaussianBlur[0][0]->Clear(graphicsDevice);
						gaussianBlur[0][0]->Activate(graphicsDevice);
						{
							gaussianBlurVerticalPS->Activate(device);
							{
								FullScreenQuad::Draw(graphicsDevice);
							}
							gaussianBlurVerticalPS->Deactivate(device);
						}
						gaussianBlur[0][0]->Deactivate(graphicsDevice);
					}
				}

				// 指定回数分だけダウンサンプリング＆ブラー加工
				for (size_t indexOfDownsampled = 1; indexOfDownsampled < numberOfDownsampled; ++indexOfDownsampled)
				{
					// downsampling
					{
						Graphics::ITexture* texture[] = { gaussianBlur[indexOfDownsampled - 1][0]->GetRenderTargetSurface()->GetTexture() };
						context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, texture);

						gaussianBlur[indexOfDownsampled][0]->Clear(graphicsDevice);
						gaussianBlur[indexOfDownsampled][0]->Activate(graphicsDevice);
						{
							gaussianBlurDownsamplingPS->Activate(device);
							{
								FullScreenQuad::Draw(graphicsDevice);
							}
							gaussianBlurDownsamplingPS->Deactivate(device);
						}
						gaussianBlur[indexOfDownsampled][0]->Deactivate(graphicsDevice);
					}

					// ping-pong gaussian blur
					{
						// 横方向にブラー加工
						{
							Graphics::ITexture* texture[] = { gaussianBlur[indexOfDownsampled][0]->GetRenderTargetSurface()->GetTexture() };
							context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, texture);

							gaussianBlur[indexOfDownsampled][1]->Clear(graphicsDevice);
							gaussianBlur[indexOfDownsampled][1]->Activate(graphicsDevice);
							{
								gaussianBlurHorizontalPS->Activate(device);
								{
									FullScreenQuad::Draw(graphicsDevice);
								}
								gaussianBlurHorizontalPS->Deactivate(device);
							}
							gaussianBlur[indexOfDownsampled][1]->Deactivate(graphicsDevice);
						}

						// 縦方向にブラー加工
						{
							Graphics::ITexture* texture[] = { gaussianBlur[indexOfDownsampled][1]->GetRenderTargetSurface()->GetTexture() };
							context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, texture);

							gaussianBlur[indexOfDownsampled][0]->Clear(graphicsDevice);
							gaussianBlur[indexOfDownsampled][0]->Activate(graphicsDevice);
							{
								gaussianBlurVerticalPS->Activate(device);
								{
									FullScreenQuad::Draw(graphicsDevice);
								}
								gaussianBlurVerticalPS->Deactivate(device);
							}
							gaussianBlur[indexOfDownsampled][0]->Deactivate(graphicsDevice);
						}
					}
				}
			}
		}

		void Bloom::Draw(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();
			Graphics::IContext* context = graphicsDevice->GetContext();

			// データセット
			{
				Graphics::ISampler* samplers[] =
				{
					context->GetSamplerState(Graphics::SamplerState::PointWrap),
					context->GetSamplerState(Graphics::SamplerState::LinearWrap),
					context->GetSamplerState(Graphics::SamplerState::AnisotropicWrap),
				};
				context->SetSamplers(Graphics::ShaderType::Pixel, 0, 3, samplers);

				Graphics::PhoenixMap map = Graphics::PhoenixMap::WriteDiscard;
				Graphics::PhoenixMappedSubresource mapedBuffer;
				{
					context->Map(constantBuffer.get(), 0, map, 0, &mapedBuffer);
					FND::MemCpy(mapedBuffer.data, &shaderContants, sizeof(ShaderConstants));
					context->Unmap(constantBuffer.get(), 0);
				}

				Graphics::IBuffer* buffers[] =
				{
					constantBuffer.get()
				};
				context->SetConstantBuffers(Graphics::ShaderType::Pixel, 0, 1, buffers);
			}

			// 描画
			gaussianBlurConvolutionPS->Activate(device);
			{
				context->SetBlend(context->GetBlendState(Graphics::BlendState::Additive), 0, 0xFFFFFFFF);

				std::vector<Graphics::ITexture*> shaderResourceViews;
				for (size_t indexOfDownsampled = 0; indexOfDownsampled < numberOfDownsampled; ++indexOfDownsampled)
				{
					shaderResourceViews.emplace_back(gaussianBlur[indexOfDownsampled][0]->GetRenderTargetSurface()->GetTexture());
				}
				context->SetShaderResources(Graphics::ShaderType::Pixel, 0, numberOfDownsampled, &shaderResourceViews.at(0));

				FullScreenQuad::Draw(graphicsDevice);
				//Quad::Draw(graphicsDevice, gaussianBlur[0][0]->GetRenderTargetSurface()->GetTexture(), 1280.0f * 0, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, false, false, false, false, false);

				Bread::Graphics::ITexture* nullTexture[8] = { nullptr };
				context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 8, nullTexture);

				context->SetBlend(context->GetBlendState(Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
			}
			gaussianBlurConvolutionPS->Deactivate(device);
		}

		void Bloom::Blend(Graphics::IGraphicsDevice* graphicsDevice, Graphics::ITexture* originTexture, Graphics::ITexture* bloomTexture)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();
			Graphics::IContext* context = graphicsDevice->GetContext();

			// データセット
			{
				Graphics::ISampler* samplers[] =
				{
					context->GetSamplerState(Graphics::SamplerState::PointWrap),
					context->GetSamplerState(Graphics::SamplerState::LinearWrap),
					context->GetSamplerState(Graphics::SamplerState::AnisotropicWrap),
				};
				context->SetSamplers(Graphics::ShaderType::Pixel, 0, 3, samplers);

				Graphics::PhoenixMap map = Graphics::PhoenixMap::WriteDiscard;
				Graphics::PhoenixMappedSubresource mapedBuffer;
				{
					context->Map(constantBuffer.get(), 0, map, 0, &mapedBuffer);
					FND::MemCpy(mapedBuffer.data, &shaderContants, sizeof(ShaderConstants));
					context->Unmap(constantBuffer.get(), 0);
				}

				Graphics::IBuffer* buffers[] =
				{
					constantBuffer.get()
				};
				context->SetConstantBuffers(Graphics::ShaderType::Pixel, 0, 1, buffers);
			}

			// 描画
			bloomBlendPS->Activate(device);
			{
				//context->SetBlend(context->GetBlendState(Graphics::BlendState::Additive), 0, 0xFFFFFFFF);

				Graphics::ITexture* texture[] = { originTexture, bloomTexture };
				context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 2, texture);

				FullScreenQuad::Draw(graphicsDevice);

				Bread::Graphics::ITexture* nullTexture[8] = { nullptr };
				context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 8, nullTexture);

				//context->SetBlend(context->GetBlendState(Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
			}
			bloomBlendPS->Deactivate(device);
		}


		std::unique_ptr<PostProcessingEffects> PostProcessingEffects::Create()
		{
			return std::make_unique<PostProcessingEffects>();
		}

		bool PostProcessingEffects::Initialize(Graphics::IGraphicsDevice* graphicsDevice)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();

			FullScreenQuad::Initialize(graphicsDevice);

			postProcessingEffectsPS = Graphics::IShader::Create();
			postProcessingEffectsPS->LoadPS(device, "PostProcessingEffectsPS.cso");

			// 定数バッファ作成
			{
				Graphics::PhoenixBufferDesc bufferDesc = {};
				Bread::FND::MemSet(&bufferDesc, 0, sizeof(bufferDesc));
				bufferDesc.usage               = Bread::Graphics::PhoenixUsage::Dynamic;
				bufferDesc.bindFlags           = static_cast<Bread::s32>(Bread::Graphics::PhoenixBindFlag::ConstantBuffer);
				bufferDesc.cpuAccessFlags      = static_cast<Bread::s32>(Graphics::PhoenixCPUAccessFlag::CPUAccessWrite);
				bufferDesc.miscFlags           = 0;
				bufferDesc.byteWidth           = sizeof(ShaderConstants);
				bufferDesc.structureByteStride = 0;

				shaderConstantsBuffer = Graphics::IBuffer::Create();
				if (!shaderConstantsBuffer->Initialize(device, bufferDesc))
				{
					return false;
				}
			}

			// サンプラー作成
			{
				comparisonSamplerState = Graphics::ISampler::Create();
				comparisonSamplerState->Initialize(device, Graphics::SamplerState::LinearBorder, false, true);
			}

			return true;
		}

		void PostProcessingEffects::Finalize()
		{

		}

		void PostProcessingEffects::Draw(Graphics::IGraphicsDevice* graphicsDevice, Graphics::ITexture* colorTexture, Graphics::ITexture* depthTexture, Graphics::ITexture* shadowTexture, const Math::Matrix& lightViewProjection, const Math::Matrix& inverseViewProjection)
		{
			Graphics::IDevice*  device  = graphicsDevice->GetDevice();
			Graphics::IContext* context = graphicsDevice->GetContext();

			//Math::Matrix t = Math::MatrixTranspose(lightViewProjection);

			// データセット
			{
				Graphics::ISampler* samplers[] =
				{
					context->GetSamplerState(Graphics::SamplerState::PointWrap),
					context->GetSamplerState(Graphics::SamplerState::LinearWrap),
					context->GetSamplerState(Graphics::SamplerState::AnisotropicWrap),
					comparisonSamplerState.get()
				};
				context->SetSamplers(Graphics::ShaderType::Pixel, 0, 4, samplers);

				shaderContexts.lightViewProjection   = lightViewProjection;
				shaderContexts.inverseViewProjection = inverseViewProjection;
				Graphics::PhoenixMap map = Graphics::PhoenixMap::WriteDiscard;
				Graphics::PhoenixMappedSubresource mapedBuffer;
				{
					context->Map(shaderConstantsBuffer.get(), 0, map, 0, &mapedBuffer);
					FND::MemCpy(mapedBuffer.data, &shaderContexts, sizeof(ShaderConstants));
					context->Unmap(shaderConstantsBuffer.get(), 0);
				}

				Graphics::IBuffer* buffers[] =
				{
					shaderConstantsBuffer.get()
				};
				context->SetConstantBuffers(Graphics::ShaderType::Pixel, 0, 1, buffers);
			}

			// 描画
			postProcessingEffectsPS->Activate(device);
			{
				/*{
					Graphics::ITexture* texture[] = { colorTexture };
					context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, texture);

					FullScreenQuad::Draw(graphicsDevice, true, true, true);
				}

				{
					context->SetBlend(context->GetBlendState(Graphics::BlendState::Multiply), 0, 0xFFFFFFFF);

					Graphics::ITexture* texture[] = { shadowTexture };
					context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, texture);

					FullScreenQuad::Draw(graphicsDevice, true, true, true);

					context->SetBlend(context->GetBlendState(Graphics::BlendState::AlphaBlend), 0, 0xFFFFFFFF);
				}*/

				Graphics::ITexture* texture[] = { colorTexture, depthTexture, shadowTexture };
				context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 3, texture);

				FullScreenQuad::Draw(graphicsDevice);

				Bread::Graphics::ITexture* nullTexture[8] = { nullptr };
				context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 8, nullTexture);
			}
			postProcessingEffectsPS->Deactivate(device);
		}

		std::unique_ptr<ToneMap> ToneMap::Create()
		{
			return std::make_unique<ToneMap>();
		}

		bool ToneMap::Initialize(Graphics::IGraphicsDevice* graphicsDevice, u32 width, u32 height)
		{
			FullScreenQuad::Initialize(graphicsDevice);

			averageLuminance = FrameBuffer::Create();
			if (!averageLuminance->Initialize(graphicsDevice, 512, 512, false, 1, Bread::Graphics::TextureFormatDx::R16_FLOAT, Bread::Graphics::TextureFormatDx::UNKNOWN, true, false, true))
			{
				return false;
			}

			// 定数バッファ作成
			{
				Graphics::PhoenixBufferDesc bufferDesc = {};
				Bread::FND::MemSet(&bufferDesc, 0, sizeof(bufferDesc));
				bufferDesc.usage               = Bread::Graphics::PhoenixUsage::Default;
				bufferDesc.bindFlags           = static_cast<Bread::s32>(Bread::Graphics::PhoenixBindFlag::ConstantBuffer);
				bufferDesc.cpuAccessFlags      = 0;
				bufferDesc.miscFlags           = 0;
				bufferDesc.byteWidth           = sizeof(ShaderConstants);
				bufferDesc.structureByteStride = 0;

				shaderConstantsBuffer = Graphics::IBuffer::Create();
				if (!shaderConstantsBuffer->Initialize(graphicsDevice->GetDevice(), bufferDesc))
				{
					return false;
				}
			}

			averageLuminancePS = Graphics::IShader::Create();
			averageLuminancePS->LoadPS(graphicsDevice->GetDevice(), "AverageLuminancePS.cso");

			toneMapPS = Graphics::IShader::Create();
			toneMapPS->LoadPS(graphicsDevice->GetDevice(), "ToneMapPS.cso");

			viewport.width    = static_cast<float>(width);
			viewport.height   = static_cast<float>(height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			viewport.x        = 0.0f;
			viewport.y        = 0.0f;

			return true;
		}

		void ToneMap::Finalize()
		{
			toneMapPS.reset();
			averageLuminancePS.reset();

			shaderConstantsBuffer.reset();

			averageLuminance.reset();
		}

		void ToneMap::Draw(Graphics::IGraphicsDevice* graphicsDevice, Graphics::ITexture* colorTexture, f32 elapsedTime)
		{
			Graphics::IDevice*  device  = graphicsDevice->GetDevice();
			Graphics::IContext* context = graphicsDevice->GetContext();

			// データセット
			{
				Graphics::ITexture* texture[] = { colorTexture };
				context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 1, texture);

				Bread::Graphics::ISampler* sampler[] =
				{
					context->GetSamplerState(Bread::Graphics::SamplerState::PointWrap),
					context->GetSamplerState(Bread::Graphics::SamplerState::LinearWrap),
					context->GetSamplerState(Bread::Graphics::SamplerState::AnisotropicWrap)
				};
				context->SetSamplers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(sampler), sampler);
			}

			// 平均輝度
			{
				const float intervalTime = 1.0f / 5.0f; // 1.0f / 5.0f
				cumulativeTime += elapsedTime;
				//if (cumulativeTime > intervalTime)
				{
					averageLuminance->Clear(graphicsDevice);
					averageLuminance->Activate(graphicsDevice);
					averageLuminancePS->Activate(device);
					FullScreenQuad::Draw(graphicsDevice);
					averageLuminancePS->Deactivate(device);
					averageLuminance->Deactivate(graphicsDevice);
					context->GenerateMips(averageLuminance->GetRenderTargetSurface()->GetTexture());
					cumulativeTime -= intervalTime;
				}
			}

			// データセット
			{
				Bread::Graphics::IBuffer* psCBuffer[] =
				{
					shaderConstantsBuffer.get()
				};
				context->UpdateSubresource(shaderConstantsBuffer.get(), 0, 0, &shaderConstant, 0, 0);
				context->SetConstantBuffers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(psCBuffer), psCBuffer);

				Graphics::ITexture* texture[] = { averageLuminance->GetRenderTargetSurface()->GetTexture() };
				context->SetShaderResources(Graphics::ShaderType::Pixel, 1, 1, texture);

				Graphics::Viewport* v = { &viewport };
				context->SetViewports(1, v);
			}

			// 描画
			{
				toneMapPS->Activate(device);
				FullScreenQuad::Draw(graphicsDevice);
				toneMapPS->Deactivate(device);
			}

			// データリセット
			{
				Bread::Graphics::ISampler* sampler[] =
				{
					context->GetSamplerState(Bread::Graphics::SamplerState::LinearWrap)
				};
				context->SetSamplers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(sampler), sampler);

				Graphics::ITexture* texture[] = { nullptr, nullptr };
				context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 2, texture);
			}
		}


		std::unique_ptr<MotionBlur> MotionBlur::Create()
		{
			return std::make_unique<MotionBlur>();
		}

		bool MotionBlur::Initialize(Graphics::IGraphicsDevice* graphicsDevice, u32 width, u32 height)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();

			FullScreenQuad::Initialize(graphicsDevice);

			int	kBlockK = 20;

			velocity = Bread::FrameWork::FrameBuffer::Create();
			if (!velocity->Initialize
			(
				graphicsDevice,
				width, height,
				false, 1,
				Bread::Graphics::TextureFormatDx::R8G8_UNORM,
				Bread::Graphics::TextureFormatDx::UNKNOWN,
				true,
				false,
				false
			))
			{
				return false;
			}

			titleMax = Bread::FrameWork::FrameBuffer::Create();
			if (!titleMax->Initialize
			(
				graphicsDevice,
				width / kBlockK, height / kBlockK,
				false, 1,
				Bread::Graphics::TextureFormatDx::R8G8B8A8_UNORM,
				Bread::Graphics::TextureFormatDx::UNKNOWN,
				true,
				false,
				false
			))
			{
				return false;
			}

			neighborMax = Bread::FrameWork::FrameBuffer::Create();
			if (!neighborMax->Initialize
			(
				graphicsDevice,
				width / kBlockK, height / kBlockK,
				false, 1,
				Bread::Graphics::TextureFormatDx::R8G8B8A8_UNORM,
				Bread::Graphics::TextureFormatDx::UNKNOWN,
				true,
				false,
				false
			))
			{
				return false;
			}

			neighborMax = Bread::FrameWork::FrameBuffer::Create();
			if (!neighborMax->Initialize
			(
				graphicsDevice,
				width, height,
				false, 1,
				Bread::Graphics::TextureFormatDx::R16G16B16A16_FLOAT,
				Bread::Graphics::TextureFormatDx::UNKNOWN,
				true,
				false,
				false
			))
			{
				return false;
			}

			// 定数バッファ作成
			{
				Graphics::PhoenixBufferDesc bufferDesc = {};
				Bread::FND::MemSet(&bufferDesc, 0, sizeof(bufferDesc));
				bufferDesc.usage               = Bread::Graphics::PhoenixUsage::Default;
				bufferDesc.bindFlags           = static_cast<Bread::s32>(Bread::Graphics::PhoenixBindFlag::ConstantBuffer);
				bufferDesc.cpuAccessFlags      = 0;
				bufferDesc.miscFlags           = 0;
				bufferDesc.byteWidth           = sizeof(VelocityConstants);
				bufferDesc.structureByteStride = 0;

				velocityConstantsBuffer = Graphics::IBuffer::Create();
				if (!velocityConstantsBuffer->Initialize(graphicsDevice->GetDevice(), bufferDesc))
				{
					return false;
				}

				bufferDesc.byteWidth = sizeof(BlurConstants);

				blurConstantsBuffer = Graphics::IBuffer::Create();
				if (!blurConstantsBuffer->Initialize(graphicsDevice->GetDevice(), bufferDesc))
				{
					return false;
				}
			}

			velocityPS = Graphics::IShader::Create();
			velocityPS->LoadPS(device, "VelocityMapPS.cso");

			normalBlurPS = Graphics::IShader::Create();
			normalBlurPS->LoadPS(device, "NormalBlurPS.cso");

			/*tileMaxPS = Graphics::IShader::Create();
			tileMaxPS->LoadPS(device, "GaussianBlurHorizontalPS.cso");

			neighborMaxPS = Graphics::IShader::Create();
			neighborMaxPS->LoadPS(device, "GlowExtractionPS.cso");

			reconstructionBlurPS = Graphics::IShader::Create();
			reconstructionBlurPS->LoadPS(device, "GaussianBlurHorizontalPS.cso");*/

			return true;
		}

		void MotionBlur::Finalize()
		{

		}

		void MotionBlur::ActivateVelocity(Graphics::IGraphicsDevice* graphicsDevice)
		{
			velocity->Clear(graphicsDevice, 0, 0.5f, 0.5f, 1.0f, 1.0f);
			velocity->Activate(graphicsDevice);
		}

		void MotionBlur::DeactivateVelocity(Graphics::IGraphicsDevice* graphicsDevice)
		{
			velocity->Deactivate(graphicsDevice);
		}
		
		void MotionBlur::ActivateVelocityPS(Graphics::IGraphicsDevice* graphicsDevice)
		{
			// データセット
			{
				Bread::Graphics::IContext* context = graphicsDevice->GetContext();
				Bread::Graphics::IBuffer* psCBuffer[] =
				{
					context->GetConstantBufferScene(),
					velocityConstantsBuffer.get()
				};
				context->UpdateSubresource(velocityConstantsBuffer.get(), 0, 0, &velocityConstants, 0, 0);
				context->SetConstantBuffers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(psCBuffer), psCBuffer);
			}

			// ピクセルシェーダーセット
			{
				velocityPS->ActivatePS(graphicsDevice->GetDevice());
			}
		}

		void MotionBlur::DeactivateVelocityPS(Graphics::IGraphicsDevice* graphicsDevice)
		{
			velocityPS->DeactivatePS(graphicsDevice->GetDevice());
		}

		void MotionBlur::Draw(Graphics::IGraphicsDevice* graphicsDevice, Graphics::ITexture* originTexture, const Graphics::Camera& camera, bool isNormal)
		{
			Graphics::IDevice* device = graphicsDevice->GetDevice();
			Graphics::IContext* context = graphicsDevice->GetContext();

			// データセット
			{
				Graphics::ISampler* samplers[] =
				{
					context->GetSamplerState(Graphics::SamplerState::PointWrap),
					context->GetSamplerState(Graphics::SamplerState::LinearWrap),
					context->GetSamplerState(Graphics::SamplerState::AnisotropicWrap),
				};
				context->SetSamplers(Graphics::ShaderType::Pixel, 0, 3, samplers);

				Bread::Graphics::IBuffer* psCBuffer[] =
				{
					blurConstantsBuffer.get()
				};
				context->UpdateSubresource(blurConstantsBuffer.get(), 0, 0, &blurConstants, 0, 0);
				context->SetConstantBuffers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(psCBuffer), psCBuffer);

				/*Bread::Graphics::IBuffer* vsCBuffer[] =
				{
					context->GetConstantBufferScene()
				};
				Bread::Graphics::IBuffer* psCBuffer[] =
				{
					context->GetConstantBufferScene()
				};
				context->SetConstantBuffers(Bread::Graphics::ShaderType::Vertex, 0, Bread::FND::ArraySize(vsCBuffer), vsCBuffer);
				context->SetConstantBuffers(Bread::Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(psCBuffer), psCBuffer);*/

				// シーン定数バッファ更新
				//context->UpdateConstantBufferScene(camera.GetView(), camera.GetProjection());
			}

			// 通常速度ブラー
			if (isNormal)
			{
				Graphics::ITexture* texture[] = 
				{
					originTexture,
					velocity->renderTargerSurface[0]->GetTexture()
				};
				context->SetShaderResources(Graphics::ShaderType::Pixel, 0, Bread::FND::ArraySize(texture), texture);

				normalBlurPS->Activate(device);
				{
					FullScreenQuad::Draw(graphicsDevice);
				}
				normalBlurPS->Deactivate(device);

				Bread::Graphics::ITexture* nullTexture[8] = { nullptr };
				context->SetShaderResources(Graphics::ShaderType::Pixel, 0, 8, nullTexture);
			}
			// 再構築ブラー
			else
			{

			}
		}
	}
}