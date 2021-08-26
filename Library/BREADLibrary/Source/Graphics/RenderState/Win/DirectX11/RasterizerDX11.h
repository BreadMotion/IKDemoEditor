#pragma once

#include <d3d11.h>
#include "Graphics/RenderState.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版ラスタライザステート操作オブジェクト
		//****************************************************************************
		class RasterizerDX11 final : public IRasterizer
		{
		private:
			ID3D11RasterizerState* rasterizerState = nullptr;

		public:
			// 初期化
			bool Initialize(IDevice* device, RasterizerState state, bool enableCull = true, bool enableDepth = true, bool enableMultisample = false, bool enableScissor = false) override;

			// 終了化
			void Finalize() override;

			// D3Dラスタライザステート設定
			void SetD3DRasterizerState(ID3D11RasterizerState* rasterizerState) { this->rasterizerState = rasterizerState; }

			// D3Dラスタライザステート取得
			ID3D11RasterizerState* GetD3DRasterizerState() const { return rasterizerState; }
		};
	} // namespace Graphics
} // namespace Bread