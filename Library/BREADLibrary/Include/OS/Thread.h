#pragma once

#include <memory>
#include "Types.h"
#include "FND/Base.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// スレッド優先度
		//****************************************************************************
		enum class ThreadPriority
		{
			Normal,		// 標準プライオリティ
			Idle,  		// アイドリング
			Lowest,		// 標準-2
			Below, 		// 標準-1
			Above, 		// 標準+1
			Highest		// 標準+2
		};

		//****************************************************************************
		// スレッドワーカーインターフェース
		//****************************************************************************
		class IThreadWorker
		{
		public:
			// スレッド実行時に呼ばれる
			virtual void Execute() = 0;

			// スレッドを強制終了
			virtual void Exit() = 0;
		};
		//****************************************************************************
		// スレッド操作インターフェース
		//****************************************************************************
		class IThread : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IThread> Create();

			// 初期化
			virtual bool Initialize() = 0;

			// 終了化
			virtual void Finalize() = 0;

			// スレッド処理実行
			virtual bool __fastcall Execute(IThreadWorker* worker) = 0;

			// スレッド処理が実行中か判定
			virtual bool IsExecute() = 0;

			// スレッドが終了するまで待機
			virtual void Join() = 0;

			// 優先度の設定
			virtual void __fastcall SetPriorty(ThreadPriority priority) = 0;

			// カレントスレッドを指定時間停止
			static void __fastcall Sleep(u32 milliSec);
		};
	} // namespace OS
} // namespace Bread