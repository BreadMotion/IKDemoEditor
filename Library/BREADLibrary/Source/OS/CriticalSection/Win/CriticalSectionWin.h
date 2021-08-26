#pragma once

#include <Windows.h>
#include "OS/CriticalSection.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// Windows��critical�Z�N�V��������N���X
		//****************************************************************************
		class CriticalSectionWin final : public ICriticalSection
		{
		private:
			CRITICAL_SECTION criticalSection = { 0 };
			bool initialize = false;

		public:
			~CriticalSectionWin() override { Finalize(); }

		public:
			// ������
			bool Initialize() override;

			// �I����
			void Finalize() override;

			// ���L���擾�҂�
			void Enter() override;

			// ���L�����
			void Leave() override;
		};
	}
}