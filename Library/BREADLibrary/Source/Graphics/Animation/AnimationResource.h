#pragma once

#include "Graphics/Animation.h"
#include "Math/BreadMath.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// �A�j���[�V�������\�[�X�N���X
		//****************************************************************************
		class AnimationResource : public IAnimationResource
		{
		private:
			AnimationData data;

		public:
			// �A�j���[�V�����f�[�^�擾
			const AnimationData& GetAnimationData() override { return data; }

			// ������
			bool __fastcall Initialize(const char* filename);
		};

		//****************************************************************************
		// �A�j���[�V�������\�[�X�t�@�N�g���N���X
		//****************************************************************************
		class AnimationResourceFactory : public IAnimationResourceFactory
		{
		public:
			// ���\�[�X�쐬
			std::unique_ptr<OS::Resource> __fastcall CreateResource(u32 type) override;

			// ���\�[�X�ǂݍ���
			bool __fastcall LoadResource(OS::Resource* resource, OS::IFileStream* stream, const char* filename)  override;
		};
	} // namespace Graphics
} // namespace Bread