#pragma once
#include "Graphics/Shader.h"

namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版シェーダー操作オブジェクト
		//****************************************************************************
		class ShaderDX11 final : public IShader
		{
		private:
			ID3D11InputLayout*    inputLayout    = nullptr;
			ID3D11VertexShader*   vertexShader   = nullptr;
			ID3D11GeometryShader* geometryShader = nullptr;
			ID3D11PixelShader*    pixelShader    = nullptr;

		private:
			ID3D11InputLayout*    cachedInputLayout    = nullptr;
			ID3D11VertexShader*   cachedVertexShader   = nullptr;
			ID3D11GeometryShader* cachedGeometryShader = nullptr;
			ID3D11PixelShader*    cachedPixelShader    = nullptr;

			const char* toShaderPath = "..\\Data\\Shader\\";

		public:
			ShaderDX11() {}
			~ShaderDX11() { Finalize(); }

		public:
			// 初期化
			bool Initialize() override { return true; }

			// 終了化
			void Finalize() override;

			// 頂点シェーダー読み込み
			void LoadVS(IDevice* device, const char* csoNameOfVertexShader, PhoenixInputElementDesc* inputElementDesc, u32 numElements) override;

			// ジオメトリシェーダー読み込み
			void LoadGS(IDevice* device, const char* csoNameOfGeometryShader) override;

			// ピクセルシェーダー読み込み
			void LoadPS(IDevice* device, const char* csoNameOfPixelShader) override;

			// シェーダー開始
			void Activate(IDevice* device)  override;
			void ActivateVS(IDevice* device) override;
			void ActivateGS(IDevice* device) override;
			void ActivatePS(IDevice* device) override;

			// シェーダー終了
			void Deactivate(IDevice* device)  override;
			void DeactivateVS(IDevice* device) override;
			void DeactivateGS(IDevice* device) override;
			void DeactivatePS(IDevice* device) override;
		};

		//****************************************************************************
		// DirectX11版コンピュートシェーダー操作オブジェクト
		//****************************************************************************
		class ComputeShaderDX11 : public IComputeShader
		{
		private:
			ID3D11ComputeShader* computeShader       = nullptr;

		private:
			ID3D11ComputeShader* cachedcomputeShader = nullptr;
			const char* toShaderPath = "..\\Data\\Shader\\";

		public:
			ComputeShaderDX11() {}
			~ComputeShaderDX11() { Finalize(); }

		public:
			// 初期化
			bool Initialize() override { return true; }

			// 終了化
			void Finalize() override;

			// コンピュートシェーダー読み込み
			void Load(IDevice* device, const char* csoNameOfComputeShader) override;

			// シェーダー開始
			void Activate(IDevice* device) override;

			// シェーダー終了
			void Deactivate(IDevice* device) override;

			// コンピュートシェーダー実行
			void Dispatch(IDevice* device, u32 x, u32 y, u32 z) override;

			// コンピュートシェーダー実行
			void DispatchIndirect(IDevice* device, IBuffer* buffer, u32 offset) override;
		};
	} // namespace Graphics
} // namespace Bread