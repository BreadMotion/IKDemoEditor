#pragma once

#include "Types.h"
#define BREAD_TARGET_DEBUG
namespace Bread
{
	namespace FND
	{
		//****************************************************************************
		// アサート出力インターフェース
		//****************************************************************************
		class IAssertWriter
		{
		public:
			// アサート出力
			virtual void Write(const char* file, s32 line, const char* message) = 0;
		};

		//****************************************************************************
		// アサート操作
		//****************************************************************************
		class Assert
		{
		private:
			static IAssertWriter* writer;

		public:
			// アサートライター設定
			static void SetWriter(IAssertWriter* assertWriter);

			// アサートブレイク
			static void Break(bool expression, const char* file, s32 line, const char* function);

			// アサート出力
			static void Print(const char* file, s32 line, const char* format, ...);
		};

	} // namespace FND
} // namespace Bread

// マクロ
#if 	defined(BREAD_TARGET_RELEASE)
	#undef	BREAD_ASSERT
	#define	BREAD_ASSERT(...)				{}
	#undef	BREAD_ASSERT_MSG
	#define	BREAD_ASSERT_MSG(...)			{}		///< アサートチェック
#elif	defined(BREAD_TARGET_DEBUG)
	#undef	BREAD_ASSERT
	#define	BREAD_ASSERT(test)			{ Bread::FND::Assert::Break(test, __FILE__,__LINE__, __FUNCTION__); }
	#undef	BREAD_ASSERT_MSG
	#define	BREAD_ASSERT_MSG(test, ...)	{ if(!(test)){ Bread::FND::Assert::Print(__FILE__,__LINE__,__VA_ARGS__); BREAD_ASSERT(test); } }
#endif