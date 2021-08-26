#pragma once

#include "Types.h"


namespace Bread
{
	namespace FND
	{
		//****************************************************************************
		// ���O�o�̓��x��
		//****************************************************************************
		enum class LogLevel
		{
			Vervose,	// �ڍ׏��
			Info,		// ���
			Warning,	// ���[�j���O
			Error,		// �G���[
			All			// �K���o��
		};

		//****************************************************************************
		// ���O�o�̓C���^�[�t�F�[�X
		//****************************************************************************
		class ILogWriter
		{
		public:
			// ���O�o��
			virtual void Write(LogLevel level, const char* message) = 0;
			virtual void Write(LogLevel level, const wchar_t* message) = 0;
		};

		//****************************************************************************
		// ���O�o�͑���
		//****************************************************************************
		class Logger
		{
		private:
			static ILogWriter*  s_writer;
			static u32			s_type_mask;
			static LogLevel		s_level;

		public:
			// ���O�̏o�͐���̂��߂̃}�X�N�l��ݒ�
			static void SetTypeMask(u32 mask) { s_type_mask = 1 | mask; }

			// ���O�o�̓��x����ݒ�
			static void SetLevel(LogLevel level) { s_level = level; }

			// ���O�o�̓��x�����擾
			static LogLevel GetLevel() { return s_level; }

			// ���O���C�^�[��ݒ�
			static void SetWriter(ILogWriter* writer) { s_writer = writer; }

			// ���O���o��
			static void Print(u32 type, LogLevel level, const char* format, ...);
			static void Print(u32 type, LogLevel level, const wchar_t* format, ...);
		};

	} // namespace FND
} // namespace Bread

// �}�N��
#define BREAD_LOG_TYPE_ALL		    (1 << 0)
#define BREAD_LOG_TYPE_FND		    (1 << 1)
#define BREAD_LOG_TYPE_OS			(1 << 2)
#define BREAD_LOG_TYPE_MATH		    (1 << 3)
#define BREAD_LOG_TYPE_RHI		    (1 << 4)
#define BREAD_LOG_TYPE_SND		    (1 << 5)
#define BREAD_LOG_TYPE_FBX		    (1 << 6)
#define BREAD_LOG_TYPE_DEV		    (1 << 7)
#define BREAD_LOG_TYPE_FRAMEWORK	(1 << 8)
#define BREAD_LOG_TYPE_APP		    (1 << 9)

#if defined(BREAD_TARGET_RELEASE) && 0
#define	BREAD_LOG_IMPLE(type, level, ...)		{}
#else
#define	BREAD_LOG_IMPLE(type, level, ...)		{ Bread::FND::Logger::Print(type, level, __VA_ARGS__); }
#endif

#define	BREAD_LOG_VERVOSE(...)			    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_ALL, Bread::FND::LogLevel::Vervose,       __VA_ARGS__)
#define	BREAD_LOG(...)					    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_ALL, Bread::FND::LogLevel::Info,          __VA_ARGS__)
#define	BREAD_LOG_WARNING(...)			    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_ALL, Bread::FND::LogLevel::Warning,       __VA_ARGS__)
#define	BREAD_LOG_ERROR(...)			    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_ALL, Bread::FND::LogLevel::Error,         __VA_ARGS__)

#define	BREAD_LOG_FND_VERVOSE(...)		    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_FND, Bread::FND::LogLevel::Vervose,       __VA_ARGS__)
#define	BREAD_LOG_FND(...)				    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_FND, Bread::FND::LogLevel::Info,          __VA_ARGS__)
#define	BREAD_LOG_FND_WARNING(...)		    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_FND, Bread::FND::LogLevel::Warning,       __VA_ARGS__)
#define	BREAD_LOG_FND_ERROR(...)		    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_FND, Bread::FND::LogLevel::Error,         __VA_ARGS__)

#define	BREAD_LOG_OS_VERVOSE(...)			BREAD_LOG_IMPLE(BREAD_LOG_TYPE_OS, Bread::FND::LogLevel::Vervose,        __VA_ARGS__)
#define	BREAD_LOG_OS(...)					BREAD_LOG_IMPLE(BREAD_LOG_TYPE_OS, Bread::FND::LogLevel::Info,           __VA_ARGS__)
#define	BREAD_LOG_OS_WARNING(...)			BREAD_LOG_IMPLE(BREAD_LOG_TYPE_OS, Bread::FND::LogLevel::Warning,        __VA_ARGS__)
#define	BREAD_LOG_OS_ERROR(...)			    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_OS, Bread::FND::LogLevel::Error,          __VA_ARGS__)

#define	BREAD_LOG_MATH_VERVOSE(...)		    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_MATH, Bread::FND::LogLevel::Vervose,      __VA_ARGS__)
#define	BREAD_LOG_MATH(...)				    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_MATH, Bread::FND::LogLevel::Info,         __VA_ARGS__)
#define	BREAD_LOG_MATH_WARNING(...)		    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_MATH, Bread::FND::LogLevel::Warning,      __VA_ARGS__)
#define	BREAD_LOG_MATH_ERROR(...)			BREAD_LOG_IMPLE(BREAD_LOG_TYPE_MATH, Bread::FND::LogLevel::Error,        __VA_ARGS__)

#define	BREAD_LOG_GRP_VERVOSE(...)		    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_RHI, Bread::FND::LogLevel::Vervose,       __VA_ARGS__)
#define	BREAD_LOG_GRP(...)				    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_RHI, Bread::FND::LogLevel::Info,          __VA_ARGS__)
#define	BREAD_LOG_GRP_WARNING(...)		    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_RHI, Bread::FND::LogLevel::Warning,       __VA_ARGS__)
#define	BREAD_LOG_GRP_ERROR(...)			BREAD_LOG_IMPLE(BREAD_LOG_TYPE_RHI, Bread::FND::LogLevel::Error,         __VA_ARGS__)

#define	BREAD_LOG_SND_VERVOSE(...)		    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_SND, Bread::FND::LogLevel::Vervose,       __VA_ARGS__)
#define	BREAD_LOG_SND(...)				    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_SND, Bread::FND::LogLevel::Info,          __VA_ARGS__)
#define	BREAD_LOG_SND_WARNING(...)		    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_SND, Bread::FND::LogLevel::Warning,       __VA_ARGS__)
#define	BREAD_LOG_SND_ERROR(...)			BREAD_LOG_IMPLE(BREAD_LOG_TYPE_SND, Bread::FND::LogLevel::Error,         __VA_ARGS__)

#define	BREAD_LOG_FBX_VERVOSE(...)		    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_FBX, Bread::FND::LogLevel::Vervose,       __VA_ARGS__)
#define	BREAD_LOG_FBX(...)				    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_FBX, Bread::FND::LogLevel::Info,          __VA_ARGS__)
#define	BREAD_LOG_FBX_WARNING(...)		    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_FBX, Bread::FND::LogLevel::Warning,       __VA_ARGS__)
#define	BREAD_LOG_FBX_ERROR(...)			BREAD_LOG_IMPLE(BREAD_LOG_TYPE_FBX, Bread::FND::LogLevel::Error,         __VA_ARGS__)

#define	BREAD_LOG_DEV_VERVOSE(...)		    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_DEV, Bread::FND::LogLevel::Vervose,       __VA_ARGS__)
#define	BREAD_LOG_DEV(...)				    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_DEV, Bread::FND::LogLevel::Info,          __VA_ARGS__)
#define	BREAD_LOG_DEV_WARNING(...)		    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_DEV, Bread::FND::LogLevel::Warning,       __VA_ARGS__)
#define	BREAD_LOG_DEV_ERROR(...)			BREAD_LOG_IMPLE(BREAD_LOG_TYPE_DEV, Bread::FND::LogLevel::Error,         __VA_ARGS__)

#define	BREAD_LOG_FRAMEWORK_VERVOSE(...)	BREAD_LOG_IMPLE(BREAD_LOG_TYPE_FRAMEWORK, Bread::FND::LogLevel::Vervose, __VA_ARGS__)
#define	BREAD_LOG_FRAMEWORK(...)			BREAD_LOG_IMPLE(BREAD_LOG_TYPE_FRAMEWORK, Bread::FND::LogLevel::Info,    __VA_ARGS__)
#define	BREAD_LOG_FRAMEWORK_WARNING(...)	BREAD_LOG_IMPLE(BREAD_LOG_TYPE_FRAMEWORK, Bread::FND::LogLevel::Warning, __VA_ARGS__)
#define	BREAD_LOG_FRAMEWORK_ERROR(...)	    BREAD_LOG_IMPLE(BREAD_LOG_TYPE_FRAMEWORK, Bread::FND::LogLevel::Error,   __VA_ARGS__)