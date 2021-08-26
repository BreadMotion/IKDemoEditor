#include "pch.h"
#include "TextureResource.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// テクスチャリソースクラス
		//****************************************************************************
		// 初期化
		bool TextureResource::Initialize(Graphics::IDevice* device, const TextureDesc& desc)
		{
			texture = ITexture::Create();
			if (!texture->Initialize(device, desc))
			{
				return false;
			}

			return true;
		}

		//****************************************************************************
		// テクスチャリソースクラス
		//****************************************************************************
		// 生成
		std::unique_ptr<ITextureResourceFactory> ITextureResourceFactory::Create(Graphics::IDevice* device)
		{
			return std::make_unique<TextureResourceFactory>(device);
		}

		// リソース作成
		std::unique_ptr<OS::Resource> TextureResourceFactory::CreateResource(u32 type)
		{
			return std::make_unique<TextureResource>();
		}

		// リソース読み込み
		bool TextureResourceFactory::LoadResource(OS::Resource* resource, OS::IFileStream* stream, const char* filename)
		{
			return false;
		}

	} // namespace Graphics
} // namespace Bread