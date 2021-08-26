#pragma once

#include <memory>
#include "FND/Base.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// クリティカルセクション操作インターフェース
		//****************************************************************************
		class ICriticalSection : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<ICriticalSection> Create();

			// 初期化
			virtual bool Initialize() = 0;

			// 終了化
			virtual void Finalize() = 0;

			// 所有権取得待ち
			virtual void Enter() = 0;

			// 所有権解放
			virtual void Leave() = 0;
		};

		//****************************************************************************
		// クリティカルセクションロック
		//****************************************************************************
		class CriticalSectionLock
		{
		private:
			ICriticalSection* obj;

		public:
			CriticalSectionLock(ICriticalSection* obj) : obj(obj)
			{
				obj->Enter();
			}
			~CriticalSectionLock()
			{
				obj->Leave();
			}
		};
	}
}