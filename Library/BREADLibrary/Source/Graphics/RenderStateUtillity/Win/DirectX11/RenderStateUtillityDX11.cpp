#include "pch.h"
#include <float.h>
#include "RenderStateUtillityDX11.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版レンダーステートユーティリティ
		//****************************************************************************
		// D3Dブレンド記述作成
		void RenderStateUtillityDX11::MakeD3DBlendDesc(BlendState blendState, D3D11_BLEND_DESC& desc)
		{
			desc.AlphaToCoverageEnable = false;

			// TRUEの場合、マルチレンダーターゲットで各レンダーターゲットのブレンドステートの設定を個別に設定できる
			// FALSEの場合、0番目のみが使用される
			desc.IndependentBlendEnable = false;

			switch (blendState)
			{
			case BlendState::Opaque:
				desc.AlphaToCoverageEnable = false;
				desc.IndependentBlendEnable = false;
				desc.RenderTarget[0].BlendEnable = false;
				desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
				desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
				desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				break;

			case BlendState::AlphaBlend:
				desc.AlphaToCoverageEnable = false;
				desc.IndependentBlendEnable = false;
				desc.RenderTarget[0].BlendEnable = true;
				desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
				desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				/*desc.AlphaToCoverageEnable = false;
				desc.RenderTarget[0].BlendEnable = true;
				desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;*/
				break;

			case BlendState::Additive:
				desc.AlphaToCoverageEnable = false;
				desc.IndependentBlendEnable = false;
				desc.RenderTarget[0].BlendEnable = true;
				desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
				desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
				desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
				desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				break;

			case BlendState::Subtraction:
				desc.AlphaToCoverageEnable = false;
				desc.IndependentBlendEnable = false;
				desc.RenderTarget[0].BlendEnable = true;
				desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
				desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
				desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
				desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				break;

			case BlendState::Multiply:
				desc.AlphaToCoverageEnable = false;
				desc.IndependentBlendEnable = false;
				desc.RenderTarget[0].BlendEnable = true;
				desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
				desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
				desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
				desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				break;

			case BlendState::AlphaToCoverageEnable:
				desc.AlphaToCoverageEnable = true;
				desc.IndependentBlendEnable = false;
				desc.RenderTarget[0].BlendEnable = false;
				desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
				desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
				desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
				desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
				desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				break;

			default: break;
			}
		}

		// D3D深度記述作成
		void RenderStateUtillityDX11::MakeD3DDepthDesc(DepthState depthState, D3D11_DEPTH_STENCIL_DESC& desc)
		{
			switch (depthState)
			{
			case DepthState::TestAndWrite:
				desc.DepthEnable = true;
				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
				desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
				break;

			case DepthState::TestOnly:
				desc.DepthEnable = true;
				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
				desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
				break;

			case DepthState::WriteOnly:
				desc.DepthEnable = false;
				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
				desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
				break;

			case DepthState::NoTestNoWrite:
				desc.DepthEnable = false;
				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
				desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
				break;

			default: break;
			}
		}

		// D3Dラスタライザ記述作成
		void RenderStateUtillityDX11::MakeD3DRasterizerDesc(RasterizerState rasterizerState, D3D11_RASTERIZER_DESC& desc, bool enableCull, bool enableDepth, bool enableMultisample, bool enableScissor)
		{
			desc.FrontCounterClockwise = enableCull;
			desc.DepthBias = 0;
			desc.DepthClipEnable = 0;
			desc.SlopeScaledDepthBias = 0;
			desc.DepthClipEnable = enableDepth;
			desc.MultisampleEnable = enableMultisample;
			desc.ScissorEnable = enableScissor;

			switch (rasterizerState)
			{
			case RasterizerState::SolidCullNone:
				desc.FillMode = D3D11_FILL_SOLID;
				desc.CullMode = D3D11_CULL_NONE;
				desc.AntialiasedLineEnable = false;
				break;

			case RasterizerState::SolidCullBack:
				desc.FillMode = D3D11_FILL_SOLID;
				desc.CullMode = D3D11_CULL_BACK;
				desc.AntialiasedLineEnable = false;
				break;

			case RasterizerState::SolidCullFront:
				desc.FillMode = D3D11_FILL_SOLID;
				desc.CullMode = D3D11_CULL_FRONT;
				desc.AntialiasedLineEnable = false;
				break;

			case RasterizerState::WireCullNone:
				desc.FillMode = D3D11_FILL_WIREFRAME;
				desc.CullMode = D3D11_CULL_NONE;
				desc.AntialiasedLineEnable = true;
				break;

			case RasterizerState::WireCullBack:
				desc.FillMode = D3D11_FILL_WIREFRAME;
				desc.CullMode = D3D11_CULL_BACK;
				desc.AntialiasedLineEnable = true;
				break;

			case RasterizerState::WireCullFront:
				desc.FillMode = D3D11_FILL_WIREFRAME;
				desc.CullMode = D3D11_CULL_FRONT;
				desc.AntialiasedLineEnable = true;
				break;

			default: break;
			}
		}

		// D3Dサンプラ記述作成
		void RenderStateUtillityDX11::MakeD3DSamplerDesc(SamplerState samplerState, D3D11_SAMPLER_DESC& desc, bool enableAlways, bool enableComparison)
		{
			desc.MipLODBias = 0.0f;
			//desc.MaxAnisotropy = 1;
			//desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			//desc.MinLOD = -FLT_MAX;
			//desc.MaxLOD = FLT_MAX;
			desc.MaxAnisotropy = 16;
			desc.ComparisonFunc = enableAlways ? D3D11_COMPARISON_ALWAYS : D3D11_COMPARISON_LESS_EQUAL;
			desc.MinLOD = 0;
			desc.MaxLOD = D3D11_FLOAT32_MAX;
			desc.BorderColor[0] = 0.0f;
			desc.BorderColor[1] = 0.0f;
			desc.BorderColor[2] = 0.0f;
			desc.BorderColor[3] = 1.0f;

			switch (samplerState)
			{
			case SamplerState::PointWrap:
			case SamplerState::LinearWrap:
			case SamplerState::AnisotropicWrap:
				desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				break;

			case SamplerState::PointClamp:
			case SamplerState::LinearClamp:
			case SamplerState::AnisotropicClamp:
				desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
				desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
				desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
				break;

			case SamplerState::PointBorder:
			case SamplerState::LinearBorder:
			case SamplerState::AnisotropicBorder:
				desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
				desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
				desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
				break;

			default: break;
			}

			switch (samplerState)
			{
			case SamplerState::PointWrap:
			case SamplerState::PointClamp:
			case SamplerState::PointBorder:
				desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
				break;

			case SamplerState::LinearWrap:
			case SamplerState::LinearClamp:
			case SamplerState::LinearBorder:
				desc.Filter = enableComparison ? D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				break;

			case SamplerState::AnisotropicWrap:
			case SamplerState::AnisotropicClamp:
			case SamplerState::AnisotropicBorder:
				desc.Filter = D3D11_FILTER_ANISOTROPIC;
				break;

			default: break;
			}
		}
	} // namespace Graphics
} // namespace Bread