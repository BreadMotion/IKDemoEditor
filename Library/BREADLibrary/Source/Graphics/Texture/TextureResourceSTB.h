#pragma once

#include "Graphics/Texture.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// STBテクスチャリソースファクトリクラス
		//****************************************************************************
		class StbTextureResourceFactory : public IStbTextureResourceFactory
		{
		private:
			Graphics::IDevice* device = nullptr;

		public:
			StbTextureResourceFactory(Graphics::IDevice* device) : device(device) {}

			// リソース作成
			std::unique_ptr<OS::Resource> CreateResource(u32 type) override;

			// リソース読み込み
			bool LoadResource(OS::Resource* resource, OS::IFileStream* stream, const char* filename)  override;
		};
	} // namespace Graphics
} // namespace Bread