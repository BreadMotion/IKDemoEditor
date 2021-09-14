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
		protected:
			std::unique_ptr<OS::IDisplay> display;
			float elapsedTime = 0.0f;

		public:
			Main() {}
			~Main() {}

		public:
			// ������
			bool Initialize(uintPtr instance) override;

			// �I����
			void Finalize() override;

			// �I����
			void Run() override;

		protected:
			// �X�V
			virtual void Update(Bread::f32 elapsedTime) = 0;

			// �`��
			virtual void Render(Bread::f32 elapsedTime) = 0;

		public:
			OS::IDisplay* getDisplay() { return display.get(); }
		};
	} // namespace FrameWork
} // namespace Bread