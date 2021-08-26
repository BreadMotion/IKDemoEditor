#pragma once

#include <memory>
#include "Types.h"
#include "FND/Base.h"
#include "Graphics/Device.h"
#include "Graphics/Texture.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// �����_�[�^�[�Q�b�g�T�[�t�F�C�X�ݒ�L�q
		//****************************************************************************
		struct RenderTargetSurfaceDesc
		{
			u32 width;
			u32 height;
		};

		//****************************************************************************
		// �[�x�X�e���V���T�[�t�F�C�X�ݒ�L�q
		//****************************************************************************
		struct DepthStencilSurfaceDesc
		{
			u32 width;
			u32 height;
		};

		//****************************************************************************
		// �����_�[�^�[�Q�b�g�T�[�t�F�C�X����C���^�[�t�F�[�X
		//****************************************************************************
		class IRenderTargetSurface : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IRenderTargetSurface> Create();

			// ������
			virtual bool Initialize(IDevice* device, const RenderTargetSurfaceDesc& desc) = 0;
			
			// �I����
			virtual void Finalize() = 0;

			// �e�N�X�`������I�u�W�F�N�g�擾
			virtual ITexture* GetTexture() = 0;
		};

		//****************************************************************************
		// �[�x�X�e���V���T�[�t�F�C�X����C���^�[�t�F�[�X
		//****************************************************************************
		class IDepthStencilSurface : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IDepthStencilSurface> Create();

			// ������
			virtual bool Initialize(IDevice* device, const DepthStencilSurfaceDesc& desc) = 0;
			
			// �I����
			virtual void Finalize() = 0;

			// �e�N�X�`������I�u�W�F�N�g�擾
			virtual ITexture* GetTexture() = 0;
		};
	} // namespace Graphics
} // namespace Bread