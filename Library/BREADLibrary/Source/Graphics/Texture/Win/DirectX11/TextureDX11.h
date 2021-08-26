#pragma once

#include <d3d11.h>
#include "Graphics/Texture.h"
#include "Graphics/Buffer.h"

namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版テクスチャ操作オブジェクト
		//****************************************************************************
		class TextureDX11 final : public ITexture
		{
		private:
			union
			{
				ID3D11Resource* resource = nullptr;
				ID3D11Texture1D* texture1d;
				ID3D11Texture2D* texture2d;
				ID3D11Texture3D* texture3d;
			};
			ID3D11ShaderResourceView* shaderResourceView = nullptr;
			ID3D11UnorderedAccessView* unorderedAccessView = nullptr;

		public:
			TextureDX11() {}
			~TextureDX11() override { Finalize(); }

			// 初期化
			bool Initialize(ID3D11Texture2D* texture, ID3D11ShaderResourceView* d3dShaderResourceView);

			// 初期化
			bool Initialize(IDevice* device, const TextureDesc& desc) override;

			// 初期化
			bool Initialize(IDevice* device, const ShaderResouceDesc& desc, IBuffer* buffer) override;

			// 初期化
			bool Initialize(IDevice* device, const UnorderedAccessViewDesc& desc, IBuffer* buffer) override;

			// 初期化
			bool Initialize(IDevice* device, const char* filename, MaterialType materialType, const Math::Color& color) override;

			// 終了化
			void Finalize() override;

			// ハンドル取得
			void* Handle() override { return shaderResourceView; }

			// テクスチャ設定の取得
			void GetTextureDesc(TextureDesc* desc) override;

			// テクスチャ設定の取得
			void GetShaderResourceViewDesc(TextureDescDx* desc) override;

			// D3Dテクスチャ取得
			ID3D11Texture2D* GetD3DTexture() const { return texture2d; }

			// D3Dシェーダーリソースビュー取得
			ID3D11ShaderResourceView* GetD3DShaderResourceView() const { return shaderResourceView; }

			// D3Dアンオーダーアクセスビュー取得
			ID3D11UnorderedAccessView* GetD3DUnorderedAccessView() const { return unorderedAccessView; }

			// D3Dシェーダーリソースビュー設定
			void SetD3DShaderResourceView(ID3D11ShaderResourceView* d3dShaderResourceView) { shaderResourceView = d3dShaderResourceView; }

		public:
			// テクスチャの読み込み
			static bool LoadTextureFromFile
			(
				ID3D11Device* device,
				const char* filename,
				ID3D11ShaderResourceView** shaderResourceView
			);

			// ダミーテクスチャの生成
			static bool CreateDummyTexture
			(
				ID3D11Device* device,
				ID3D11ShaderResourceView** shaderResourceView,
				unsigned int value/*0xAABBGGRR*/,
				unsigned int dimensions,
				bool forceSrgb,
				bool enableCaching
			);

			// DXGIフォーマット取得
			static DXGI_FORMAT GetDXGIFormat(TextureFormat format);

			// D3Dリソース次元取得
			static D3D11_RESOURCE_DIMENSION GetD3DResourceDimension(TextureDimension dimension);

			// フォーマット取得
			static TextureFormat GetFormat(DXGI_FORMAT format);

			// リソース次元取得
			static TextureDimension GetResourceDimension(D3D11_SRV_DIMENSION dimension);

			// シェーダーリソース次元取得
			static TextureDimensionDx GetShaderResourceDimension(D3D11_SRV_DIMENSION dimension);

			// 1ピクセルあたりのビット数取得
			static UINT BitsPerPixel(DXGI_FORMAT dxgiformat);

			// サーフェイス情報取得
			static void GetSurfaceInfo(UINT width, UINT height, DXGI_FORMAT dxgiformat, UINT* outNumBytes, UINT* outRowBytes, UINT* outNumRows);

			// SRGBフォーマットへ変換
			static DXGI_FORMAT MakeSRGB(DXGI_FORMAT dxgiFormat);

			// 初期データを設定
			static HRESULT FillInitData(
				UINT width,
				UINT height,
				UINT depth,
				UINT mipCount,
				UINT arraySize,
				DXGI_FORMAT dxgiFormat,
				UINT maxsize,
				UINT bitSize,
				const uint8_t* bitData,
				UINT& twidth,
				UINT& theight,
				UINT& tdepth,
				UINT& skipMip,
				D3D11_SUBRESOURCE_DATA* subresourceDatas);

			// D3Dリソース作成
			static HRESULT CreateD3DResources(
				ID3D11Device* device,
				D3D11_RESOURCE_DIMENSION resourceDimention,
				UINT width,
				UINT height,
				UINT depth,
				UINT mipCount,
				UINT arraySize,
				DXGI_FORMAT dxgiFormat,
				D3D11_USAGE d3dUsage,
				UINT bindFlags,
				UINT cpuAccessFlags,
				UINT miscFlags,
				bool forceSrgb,
				bool isCubeMap,
				D3D11_SUBRESOURCE_DATA* subresourceDatas,
				ID3D11Resource** resource,
				ID3D11ShaderResourceView** shaderResourceView);
		};
	} // namespace Graphics
} // namespace Bread