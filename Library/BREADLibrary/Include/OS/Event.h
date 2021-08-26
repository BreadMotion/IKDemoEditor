#pragma once

#include <memory>
#include "Types.h"
#include "FND/Base.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// イベント操作インターフェース
		//****************************************************************************
		class IEvent : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IEvent> Create();

			// 初期化
			virtual bool Initialize(bool manual = false) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// シグナル設定
			virtual void Set() = 0;

			// シグナル解除
			virtual void Reset() = 0;

			// シグナル設定待ち
			virtual void Wait(f32 time = 0.0f) = 0;
		};
	}
}