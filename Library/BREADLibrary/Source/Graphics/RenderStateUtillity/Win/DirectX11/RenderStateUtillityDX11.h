#pragma once

#include <d3d11.h>


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// ブレンドステート
		//****************************************************************************
		enum class BlendState
		{
			Opaque,					// 標準ブレンド
			AlphaBlend,				// アルファブレンド
			Additive,				// 加算ブレンド
			Subtraction,			// 減算ブレンド
			Multiply,				// 乗算ブレンド
			AlphaToCoverageEnable,	// アルファ透過

			TypeNum
		};

		//****************************************************************************
		// 深度ステート
		//****************************************************************************
		enum class DepthState
		{
			TestAndWrite,		// 深度テスト＆深度書き込み
			TestOnly,			// 深度テストのみ
			WriteOnly,			// 深度書き込みのみ
			NoTestNoWrite,		// 深度テスト＆深度書き込みなし

			TypeNum
		};

		//****************************************************************************
		// ラスタライザステート
		//****************************************************************************
		enum class RasterizerState
		{
			SolidCullNone,		// 三角形塗りつぶしカリングなし
			SolidCullBack,		// 三角形塗りつぶし裏面カリング
			SolidCullFront,		// 三角形塗りつぶし表面カリング
			WireCullNone,		// ワイヤーフレームカリングなし
			WireCullBack,		// ワイヤーフレーム裏面カリング
			WireCullFront,		// ワイヤーフレーム表面カリング

			TypeNum
		};

		//****************************************************************************
		// サンプラステート
		//****************************************************************************
		enum class SamplerState
		{
			PointWrap,
			PointClamp,
			PointBorder,
			LinearWrap,
			LinearClamp,
			LinearBorder,
			AnisotropicWrap,
			AnisotropicClamp,
			AnisotropicBorder,

			TypeNum
		};

		//****************************************************************************
		// DirectX11版レンダーステートユーティリティ
		//****************************************************************************
		class RenderStateUtillityDX11
		{
		public:
			// D3Dブレンド記述作成
			static void MakeD3DBlendDesc(BlendState blendState, D3D11_BLEND_DESC& desc);

			// D3D深度記述作成
			static void MakeD3DDepthDesc(DepthState depthState, D3D11_DEPTH_STENCIL_DESC& desc);

			// D3Dラスタライザ記述作成
			static void MakeD3DRasterizerDesc(RasterizerState rasterizerState, D3D11_RASTERIZER_DESC& desc, bool enableCull = true, bool enableDepth = true, bool enableMultisample = false, bool enableScissor = false);

			// D3Dサンプラ記述作成
			static void MakeD3DSamplerDesc(SamplerState samplerState, D3D11_SAMPLER_DESC& desc, bool enableAlways = true, bool enableComparison = false);
		};
	} // namespace Graphics
} // namespace Bread