#pragma once

#include "Types.h"
#define BREAD_TARGET_DEBUG
namespace Bread
{
	namespace FND
	{
		//****************************************************************************
		// �A�T�[�g�o�̓C���^�[�t�F�[�X
		//****************************************************************************
		class IAssertWriter
		{
		public:
			// �A�T�[�g�o��
			virtual void Write(const char* file, s32 line, const char* message) = 0;
		};

		//****************************************************************************
		// �A�T�[�g����
		//****************************************************************************
		class Assert
		{
		private:
			static IAssertWriter* writer;

		public:
			// �A�T�[�g���C�^�[�ݒ�
			static void SetWriter(IAssertWriter* assertWriter);

			// �A�T�[�g�u���C�N
			static void Break(bool expression, const char* file, s32 line, const char* function);

			// �A�T�[�g�o��
			static void Print(const char* file, s32 line, const char* format, ...);
		};

	} // namespace FND
} // namespace Bread

// �}�N��
#if 	defined(BREAD_TARGET_RELEASE)
	#undef	BREAD_ASSERT
	#define	BREAD_ASSERT(...)				{}
	#undef	BREAD_ASSERT_MSG
	#define	BREAD_ASSERT_MSG(...)			{}		///< �A�T�[�g�`�F�b�N
#elif	defined(BREAD_TARGET_DEBUG)
	#undef	BREAD_ASSERT
	#define	BREAD_ASSERT(test)			{ Bread::FND::Assert::Break(test, __FILE__,__LINE__, __FUNCTION__); }
	#undef	BREAD_ASSERT_MSG
	#define	BREAD_ASSERT_MSG(test, ...)	{ if(!(test)){ Bread::FND::Assert::Print(__FILE__,__LINE__,__VA_ARGS__); BREAD_ASSERT(test); } }
#endif