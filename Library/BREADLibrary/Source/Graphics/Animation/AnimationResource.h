#pragma once

#include "Graphics/Animation.h"
#include "Math/BreadMath.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// アニメーションリソースクラス
		//****************************************************************************
		class AnimationResource : public IAnimationResource
		{
		private:
			AnimationData data;

		public:
			// アニメーションデータ取得
			const AnimationData& GetAnimationData() override { return data; }

			// 初期化
			bool __fastcall Initialize(const char* filename);
		};

		//****************************************************************************
		// アニメーションリソースファクトリクラス
		//****************************************************************************
		class AnimationResourceFactory : public IAnimationResourceFactory
		{
		public:
			// リソース作成
			std::unique_ptr<OS::Resource> __fastcall CreateResource(u32 type) override;

			// リソース読み込み
			bool __fastcall LoadResource(OS::Resource* resource, OS::IFileStream* stream, const char* filename)  override;
		};
	} // namespace Graphics
} // namespace Bread