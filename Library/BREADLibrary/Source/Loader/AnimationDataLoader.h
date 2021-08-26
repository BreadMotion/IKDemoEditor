#pragma once

#include <fbxsdk.h>
#include "Graphics/Animation.h"


namespace Bread
{
	namespace Loader
	{
		//****************************************************************************
		// アニメーションデータローダークラス
		//****************************************************************************
		class AnimationDataLoader
		{
		public:
			// アニメーションデータ読み込み
			bool Load(FbxScene* fbxScene, Graphics::AnimationData& data);
		};

	} // namespace Loader
} // namespace Bread