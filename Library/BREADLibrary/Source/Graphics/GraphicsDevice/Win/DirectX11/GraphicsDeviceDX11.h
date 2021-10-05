#pragma once
#include "Graphics/GraphicsDevice.h"

//TODO : �쐬�ꏊ���l���邱��
//�e�X�g�p deferrdContext��Commandlist�ϐ������悤
#include <wrl/client.h>

namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11�ŃO���t�B�b�N�X�f�o�C�X����I�u�W�F�N�g
		//****************************************************************************
		class GraphicsDeviceDX11 final : public IGraphicsDevice
		{
		private:
			std::unique_ptr<IDevice>                  device;
			std::unique_ptr<IContext>                 context;
			std::unique_ptr<ISwapChain>               swapChain;
			Microsoft::WRL::ComPtr<ID3D11CommandList> commandLists;

		public:
			RasterizerState rasterizerState = RasterizerState::SolidCullNone;

		public:
			// ������
			bool Initialize(OS::IDisplay* display) override;

			// �I����
			void Finalize() override;

		public:
			IDevice*     GetDevice()      override { return device.get(); }
			IContext*    GetContext()     override { return context.get(); }
			ISwapChain*  GetSwapChain()   override { return swapChain.get(); }

			// �����_�����O���ꂽ�C���[�W��\��
			void Present(int syncInterval) override;

			// �`��J�n
			void RenderBegin()override;

			// �`��I��
			void RenderEnd()override;

			//�`�施�߂��L�^����
			void RecordRenderCommand()override;

			//�`�施�߂����s����
			void RenderCommandRun()override;
		};
	} // namespace Graphics
} // namespace Bread