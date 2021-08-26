#pragma once

#include <memory>
#include "Types.h"
#include "FND/Base.h"
#include "Graphics/Model.h"
#include "Graphics/Animation.h"


namespace Bread
{
	namespace Loader
	{
		//****************************************************************************
		// FBX読み込み操作インターフェース
		//****************************************************************************
		class ILoader : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<ILoader> Create();

			// 初期化
			virtual bool Initialize(const char* fileName) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// モデルデータ読み込み
			virtual bool Load(Graphics::ModelData& data, const char* dirname) = 0;

			// アニメーションデータ読み込み
			virtual bool Load(Graphics::AnimationData& data) = 0;
		};

	} // namespace Loader
} // namespace Bread