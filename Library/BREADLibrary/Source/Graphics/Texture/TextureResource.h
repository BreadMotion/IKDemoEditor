#pragma once

#include "Graphics/Texture.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// テクスチャリソースクラス
		//****************************************************************************
		class TextureResource : public ITextureResource
		{
		private:
			std::unique_ptr<Graphics::ITexture> texture = nullptr;

		public:
			// 初期化
			bool Initialize(Graphics::IDevice* device, const TextureDesc& desc);

			// テクスチャ取得
			Graphics::ITexture* GetTexture() override { return texture.get(); }
		};

		//****************************************************************************
		// テクスチャリソースクラス
		//****************************************************************************
		class TextureResourceFactory : public ITextureResourceFactory
		{
		private:
			Graphics::IDevice* device = nullptr;

		public:
			TextureResourceFactory(Graphics::IDevice* device) : device(device) {}

			// リソース作成
			std::unique_ptr<OS::Resource> CreateResource(u32 type) override;

			// リソース読み込み
			bool LoadResource(OS::Resource* resource, OS::IFileStream* stream, const char* filename)  override;
		};
	} // namespace Graphics
} // namespace Bread