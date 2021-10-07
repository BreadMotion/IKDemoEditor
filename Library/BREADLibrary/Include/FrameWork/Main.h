#pragma once

#include <memory>
#include <string>
#include "OS/Main.h"
#include "OS/Display.h"
#include "Graphics/GraphicsDevice.h"
#include "FND/Instance.h"


namespace Bread
{
	namespace FrameWork
	{
		//****************************************************************************
		// ���C�����[�v
		//****************************************************************************
		class Main : public OS::IMain
		{
		public:
			Main() {}
			~Main() {}

		public:
			// ������
			bool __fastcall Initialize(uintPtr instance) override;

			// �I����
			void Finalize() override;

			// �I����
			void Run() override;

		protected:
			// �X�V
			virtual void __fastcall Update() = 0;

			// �`��
			virtual void __fastcall Render() = 0;
		};
	} // namespace FrameWork
} // namespace Bread