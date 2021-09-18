#pragma once

#include "OS/Thread.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// Windows版スレッド操作クラス
		//****************************************************************************
		class ThreadWin : public IThread
		{
		private:
			enum ErrCode
			{
				ErrInit,	// 初期化されていない
				ErrRun,		// 実行していない
				ErrFail,	// 正常動作していない
				ErrExec,	// 実行中
				ErrExit,	// 終了した
				ErrEnd		// 終了中
			};

		private:
			IThreadWorker* worker = nullptr;
			HANDLE         handle = nullptr;
			unsigned id = 0;
			bool run = false;

		public:
			~ThreadWin() override { Finalize(); }

		public:
			// 初期化
			bool Initialize() override;

			// 終了化
			void Finalize() override;

			// スレッド処理実行
			bool __fastcall Execute(IThreadWorker* worker) override;

			// スレッド処理が実行中か判定
			bool IsExecute() override;

			// スレッドが終了するまで待機
			void Join() override;

			// 優先度の設定
			void __fastcall SetPriorty(ThreadPriority priority) override;

		private:
			// 動作チェック
			ErrCode CheakExec(bool block);

			// スレッド処理関数
			static unsigned __stdcall ThreadProc(void* param);
		};
	} // namespace OS
} // namespace Phoenix