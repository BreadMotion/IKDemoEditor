#pragma once

#include <memory>
#include "Types.h"
#include "FND/Base.h"


namespace Bread
{
	namespace OS
	{
		// ウインドウハンドル
		typedef uintPtr WindowHandle;

#ifdef WIN32
		typedef HWND Handle;
#endif

		//****************************************************************************
		// ディスプレイ操作インターフェース
		//****************************************************************************
		class IDisplay : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IDisplay> Create();

			// 初期化
			virtual bool Initialize(const wchar_t* name, s32 width, s32 height, uintPtr instance) = 0;
			
			// 終了化
			virtual void Finalize() = 0;

			// ハンドル取得
			virtual Handle GetHWND() = 0;

			// 幅取得
			virtual s32 GetWidth() = 0;

			// 高さ取得
			virtual s32 GetHeight() = 0;

			// ウインドウハンドル取得
			virtual WindowHandle GetHandle() = 0;

			// タイマーチック
			virtual void TimerTick() = 0;

			// タイマーインターバル
			virtual f32 TimerInterval() = 0;

			// フレームレートの表示
			virtual void CalculateFrameStats() = 0;

			//インプットの更新
			virtual void InputUpdate() = 0;
		};
	} // namespace OS
} // namespace Bread