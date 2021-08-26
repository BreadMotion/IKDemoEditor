#include "pch.h"
#include "FND/Logger.h"
#include "CriticalSectionWin.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// Windows版criticalセクション操作クラス
		//****************************************************************************
		// 生成
		std::unique_ptr<ICriticalSection> ICriticalSection::Create()
		{
			return std::make_unique<CriticalSectionWin>();
		}

		// 初期化
		bool CriticalSectionWin::Initialize()
		{
			try
			{
				// クリティカルセクション登録
				::InitializeCriticalSection(&criticalSection);
			}
			catch (...)
			{
				// クリティカルセクション登録での例外エラーは、ここでcatchする
				BREAD_LOG_OS_ERROR("InitializeCriticalSection() : Failed!!\n");
				return false;
			}
			initialize = true;

			return true;
		}

		// 終了化
		void CriticalSectionWin::Finalize()
		{
			if (!initialize)
			{
				return;
			}
			initialize = false;

			::DeleteCriticalSection(&criticalSection);
		}

		// 所有権取得待ち
		void CriticalSectionWin::Enter()
		{
			try {
				// クリティカルセクション所有権取得
				::EnterCriticalSection(&criticalSection);
			}
			catch (...) {
				// クリティカルセクション所有での例外エラーは、ここでcatchする
				BREAD_LOG_OS_ERROR("EnterCriticalSection() : Failed!!\n");
				return;
			}
		}

		// 所有権解放
		void CriticalSectionWin::Leave()
		{
			try {
				// クリティカルセクション所有権解放
				::LeaveCriticalSection(&criticalSection);
			}
			catch (...) {
				// クリティカルセクション所有での例外エラーは、ここでcatchする
				BREAD_LOG_OS_ERROR("LeaveCriticalSection() : Failed!!\n");
				return;
			}
		}
	}
}