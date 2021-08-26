#pragma once

#include <memory>
#include "Types.h"
#include "FND/Base.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// メインループ操作インターフェース
		//****************************************************************************
		class IMain : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IMain> Create();

			// 初期化
			virtual bool Initialize(uintPtr instance) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// ループ処理
			virtual void Run() = 0;

			//// 終了要求
			//virtual void Exit() = 0;

			//// 終了要求状態か確認
			//virtual bool IsExit() = 0;

			//// 名前取得
			//virtual const char* GetName() = 0;

			//// 名前設定
			//virtual void SetName(const char* name) = 0;

			//// インスタンスハンドルを取得
			//virtual uintPtr Handle() = 0;
		};
	} // namespace OS
} // namespace Bread