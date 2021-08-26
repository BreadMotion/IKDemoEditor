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
		// FBX�ǂݍ��ݑ���C���^�[�t�F�[�X
		//****************************************************************************
		class ILoader : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<ILoader> Create();

			// ������
			virtual bool Initialize(const char* fileName) = 0;

			// �I����
			virtual void Finalize() = 0;

			// ���f���f�[�^�ǂݍ���
			virtual bool Load(Graphics::ModelData& data, const char* dirname) = 0;

			// �A�j���[�V�����f�[�^�ǂݍ���
			virtual bool Load(Graphics::AnimationData& data) = 0;
		};

	} // namespace Loader
} // namespace Bread