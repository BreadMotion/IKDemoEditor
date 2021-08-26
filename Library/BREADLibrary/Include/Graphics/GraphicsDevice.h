#pragma once

#include <memory>
#include "Types.h"
#include "FND/Base.h"
#include "OS/Display.h"
#include "Graphics/Device.h"
#include "Graphics/Context.h"
#include "Graphics/SwapChain.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// �O���t�B�b�N�X�f�o�C�X����C���^�[�t�F�[�X
		//****************************************************************************
		class IGraphicsDevice : public FND::Base
		{
		public:
			// ����
			static std::shared_ptr<IGraphicsDevice> Create();

			// ������
			virtual bool Initialize(OS::IDisplay* display) = 0;

			// �I����
			virtual void Finalize() = 0;

		public:
			virtual IDevice*               GetDevice()    = 0;
			virtual IContext*              GetContext()   = 0;
			virtual ISwapChain*            GetSwapChain() = 0;

			// �����_�����O���ꂽ�C���[�W��\��
			virtual void Present(int syncInterval) = 0;

			// �`��J�n
			virtual void RenderBegin() = 0;

			// �`��I��
			virtual void RenderEnd()   = 0;

			//�`�施�߂��L�^����
			virtual void RecordRenderCommand() = 0;

			//�`�施�߂����s����
			virtual void RenderCommandRun()    = 0;
		};
	} // namespace Graphics
} // namespace Bread