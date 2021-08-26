#pragma once

#include <d3d11.h>
#include "Graphics/RenderState.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版サンプラーステート操作オブジェクト
		//****************************************************************************
		class SamplerDX11 final : public ISampler
		{
		private:
			ID3D11SamplerState* samplerState = nullptr;

		public:
			// 初期化
			bool Initialize(IDevice* device, SamplerState state, bool enableAlways = true, bool enableComparison = false) override;

			// 終了化
			void Finalize() override;

			// D3Dサンプラーステート取得
			ID3D11SamplerState* GetD3DSamplerState() const { return samplerState; }
		};
	} // namespace Graphics
} // namespace Bread