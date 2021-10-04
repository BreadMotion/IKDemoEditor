#include "../pch.h"
#include <stdio.h>
#include <assert.h>
#include "FND/assert.h"
#include "FND/logger.h"


namespace Bread
{
	namespace FND
	{
		// スタティック
		IAssertWriter* Assert::writer = nullptr;

		//****************************************************************************
		// アサート操作クラス
		//****************************************************************************
		// アサートライター設定
		void Assert::SetWriter(IAssertWriter* assertWriter)
		{
			writer = assertWriter;
		}

		// アサートブレイク
		void Assert::Break(bool expression, const char* file, s32 line, const char* function)
		{
			if (!expression)
			{
				char message[1024];
				::sprintf_s(message, sizeof(message), "file:%s\nline:%d\nfunction:%s", file, line, function);
				::MessageBoxA(NULL, message, "Assert", MB_OK);
			}
			assert(expression);
		}

		// アサート出力
		void Assert::Print(const char* file, s32 line, const char* format, ...)
		{
			char message[1024];
			va_list args;
			va_start(args, format);
			vsnprintf(message, sizeof(message), format, args);
			va_end(args);

#if		defined(BREAD_TARGET_WIN)
			::OutputDebugStringA(message);
			::puts(message);
#endif

			if (writer != nullptr)
			{
				writer->Write(file, line, message);
			}
		}
	}
}