#include "pch.h"
#include "ThreadWin.h"
#include <process.h>
#include "FND/Logger.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// Windows版スレッド操作クラス
		//****************************************************************************
		// 生成
		std::unique_ptr<IThread> IThread::Create()
		{
			return std::make_unique<ThreadWin>();
		}

		// カレントスレッドを指定時間停止
		void IThread::Sleep(u32 milliSec)
		{
			::Sleep(milliSec);
		}

		// 初期化
		bool ThreadWin::Initialize()
		{
			return true;
		}

		// 終了化
		void ThreadWin::Finalize()
		{
			if (CheakExec(false) == ErrExec)
			{
				worker->Exit();
			}

			if (handle)
			{
				if (run)
				{
					DWORD ret;
					do
					{ // 終了が確認できるまで、終了コマンドを発行
						if (CheakExec(false) == ErrExec)
						{
							worker->Exit();	// ワーカープログラムの終了
						}

						ret = ::WaitForSingleObject(handle, 10);
					} while (ret == WAIT_TIMEOUT);
				}

				::CloseHandle(handle);
				handle = NULL;
			}
		}

		// スレッド処理実行
		bool ThreadWin::Execute(IThreadWorker* worker)
		{
			this->worker = worker;

			unsigned stackSize = 0;

			if (CheakExec(false) == ErrExec)
			{
				BREAD_LOG_OS_ERROR("スレッドが多重実行しています。\n");
				return false;
			}

			handle = (HANDLE)_beginthreadex
			(
				LPSECURITY_ATTRIBUTES(0), 	// セキュリティアトリビュート
				stackSize,				  	// スタックサイズ
				ThreadProc,				  	// プログラム開始
				this,					  	// パラメータ
				CREATE_SUSPENDED,		  	// 中断状態での起動
				&id						  	// スレッドID格納
			);

			if (!handle)
			{
				BREAD_LOG_OS_ERROR("スレッドを作成できませんでした。\n");
				return false;
			}

			if (::ResumeThread(handle) > 1)
			{
				BREAD_LOG_OS_ERROR("スレッドが実行されませんでした。\n");
				return false;
			}

			run = true;

			return true;
		}

		// スレッド処理が実行中か判定
		bool ThreadWin::IsExecute()
		{
			switch (CheakExec(false))
			{
			case ErrInit:
			case ErrRun:
			case ErrFail:
			case ErrExec:
			case ErrEnd:
				return false;
			case ErrExit:
				return true;
			}

			return false;
		}

		// スレッドが終了するまで待機
		void ThreadWin::Join()
		{
			CheakExec(true);
		}

		// 優先度の設定
		void ThreadWin::SetPriorty(ThreadPriority priority)
		{
			HANDLE dupHandle;
			if (!::DuplicateHandle(GetCurrentProcess(), handle, GetCurrentProcess(), &dupHandle, THREAD_SET_INFORMATION, true, 0))
			{
				return;
			}

			int nPriority = 0;
			switch (priority)
			{
			case ThreadPriority::Normal:	nPriority = THREAD_PRIORITY_NORMAL;			break;	// 標準プライオリティ
			case ThreadPriority::Idle:		nPriority = THREAD_PRIORITY_IDLE;			break;	// アイドリング
			case ThreadPriority::Lowest:	nPriority = THREAD_PRIORITY_LOWEST;			break;	// 標準-2
			case ThreadPriority::Below:		nPriority = THREAD_PRIORITY_BELOW_NORMAL;	break;	// 標準-1
			case ThreadPriority::Above:		nPriority = THREAD_PRIORITY_ABOVE_NORMAL;	break;	// 標準+1
			case ThreadPriority::Highest:	nPriority = THREAD_PRIORITY_HIGHEST;		break;	// 標準+2
			}

			::SetThreadPriority(dupHandle, nPriority);
			::CloseHandle(dupHandle);
		}
		
		// 動作チェック
		ThreadWin::ErrCode ThreadWin::CheakExec(bool block)
		{
			if (worker == nullptr)
			{
				return ErrInit;			// 初期化されていません
			}
			if (!run)
			{
				return ErrRun;			// 初期化されていません
			}

			DWORD exitCode;
			if (!::GetExitCodeThread(handle, &exitCode))
			{
				run = false;
				return ErrFail;			// 正常動作していません
			}
			if (exitCode == STILL_ACTIVE)
			{
				// ブロッキング
				if (block)
				{
					// 終了待ち
					::WaitForSingleObject(handle, INFINITE);
					run = false;
					return ErrExit;		// 終了した
				}
				return ErrExec;			// 実行中
			}
			run = false;

			return ErrEnd;				// 終了してます
		}

		// スレッド処理関数
		unsigned __stdcall ThreadWin::ThreadProc(void* param)
		{
			ThreadWin* thread = reinterpret_cast<ThreadWin*>(param);

			thread->worker->Execute();

			_endthreadex(0);

			return 0;
		}
	} // namespace OS
} // namespace Bread