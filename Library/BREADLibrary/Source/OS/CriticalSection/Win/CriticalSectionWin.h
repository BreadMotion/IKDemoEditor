#pragma once

#include <Windows.h>
#include "OS/CriticalSection.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// Windows版criticalセクション操作クラス
		//****************************************************************************
		class CriticalSectionWin final : public ICriticalSection
		{
		private:
			CRITICAL_SECTION criticalSection = { 0 };
			bool initialize = false;

		public:
			~CriticalSectionWin() override { Finalize(); }

		public:
			// 初期化
			bool Initialize() override;

			// 終了化
			void Finalize() override;

			// 所有権取得待ち
			void Enter() override;

			// 所有権解放
			void Leave() override;
		};
	}
}