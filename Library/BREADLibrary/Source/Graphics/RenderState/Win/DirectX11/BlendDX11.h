#pragma once

#include <d3d11.h>
#include "Graphics/RenderState.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版ブレンド操作オブジェクト
		//****************************************************************************
		class BlendDX11 final : public IBlend
		{
		private:
			ID3D11BlendState* blendState = nullptr;

		public:
			// 初期化
			bool Initialize(IDevice* device, BlendState state) override;

			// 終了化
			void Finalize() override;

			// D3Dブレンドステート取得
			ID3D11BlendState* GetD3DBlendState() { return blendState; }
		};
	} // namespace Graphics
} // namespace Bread