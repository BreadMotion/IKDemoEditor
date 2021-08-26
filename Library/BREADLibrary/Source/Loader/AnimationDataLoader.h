#pragma once

#include <fbxsdk.h>
#include "Graphics/Animation.h"


namespace Bread
{
	namespace Loader
	{
		//****************************************************************************
		// �A�j���[�V�����f�[�^���[�_�[�N���X
		//****************************************************************************
		class AnimationDataLoader
		{
		public:
			// �A�j���[�V�����f�[�^�ǂݍ���
			bool Load(FbxScene* fbxScene, Graphics::AnimationData& data);
		};

	} // namespace Loader
} // namespace Bread