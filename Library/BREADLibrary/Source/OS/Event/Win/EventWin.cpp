#include "pch.h"
#include "FND/Logger.h"
#include "EventWin.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// Windows版イベント操作クラス
		//****************************************************************************
		// 生成
		std::unique_ptr<IEvent> IEvent::Create()
		{
			return std::make_unique<EventWIN>();
		}

		// 初期化
		bool EventWIN::Initialize(bool manual)
		{
			static int id = 0;
			handle = ::CreateEventA(nullptr, manual, false, nullptr);

			if (handle == nullptr)
			{
				BREAD_LOG_OS_ERROR("CreateEvent() : Failed!!\n");
				return false;
			}

			return true;
		}

		// 解放
		void EventWIN::Finalize()
		{
			if (handle != nullptr)
			{
				::CloseHandle(handle);
				handle = nullptr;
			}
		}

		// シグナル設定
		void EventWIN::Set()
		{
			::SetEvent(handle);
		}

		// シグナル解除
		void EventWIN::Reset()
		{
			::ResetEvent(handle);
		}

		// シグナル設定待ち
		void EventWIN::Wait(f32 time)
		{
			DWORD milliSeconds = (time > 0.0001f) ? (DWORD)(time * 1000.0f) : INFINITE;

			::WaitForSingleObject(handle, milliSeconds);
		}

	}	// namespace os
}	// namespace ys