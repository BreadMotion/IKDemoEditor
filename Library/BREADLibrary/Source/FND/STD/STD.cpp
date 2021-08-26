#include "pch.h"
#include <locale.h>
#include <math.h>
#include "FND/STD.h"


namespace Bread
{
	namespace FND
	{
		// strcmp
		s32 StrCmp(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs);
		}

		// strncmp
		s32 StrNCmp(const char* lhs, const char* rhs, s32 max_count)
		{
			return strncmp(lhs, rhs, max_count);
		}

		// strcpy
		void StrCpy(char* dst, s32 size_in_bytes, const char* src)
		{
			strcpy_s(dst, size_in_bytes, src);
		}

		// strncpy
		void StrNCpy(char* dst, s32 size_in_bytes, const char* src, s32 count)
		{
			strncpy_s(dst, size_in_bytes, src, count);
		}

		// strcat
		void StrCat(char* dst, s32 size_in_bytes, const char* src)
		{
			strcat_s(dst, size_in_bytes, src);
		}

		// strlen
		s32 StrLen(const char* str)
		{
			return static_cast<s32>(strlen(str));
		}

		// wcslen
		s32 WcsLen(const wchar_t* str)
		{
			return static_cast<s32>(wcslen(str));
		}

		// memcpy
		void* MemCpy(void* dst, const void* src, s32 size)
		{
			return memcpy(dst, src, size);
		}

		// memset
		void* MemSet(void* dst, s32 val, s32 size)
		{
			return memset(dst, val, size);
		}

		// memset
		s32 MemCmp(const void* buf1, const void* buf2, s32 size)
		{
			return memcmp(buf1, buf2, size);
		}

		// wcstombs
		s32 WcsToMbs(char* mbstr, s32 size_in_bytes, const wchar_t* wcstr, s32 count)
		{
			size_t ret;
			wcstombs_s(&ret, mbstr, size_in_bytes, wcstr, count);
			return static_cast<s32>(ret);
		}

		// mbstowcs
		s32 MbsToWcs(wchar_t* wcstr, s32 size_in_words, const char* mbstr, s32 count)
		{
			size_t ret;
			mbstowcs_s(&ret, wcstr, size_in_words, mbstr, count);
			return static_cast<s32>(ret);
		}

		// _splitpath
		void SplitPath(const char* full_path, char* drive, s32 drive_count, char* dir, s32 dir_count, char* filename, s32 filename_count, char* ext, s32 ext_count)
		{
			_splitpath_s(full_path, drive, drive_count, dir, dir_count, filename, filename_count, ext, ext_count);
		}

		// _wsplitpath
		void WSplitPath(const wchar_t* full_path, wchar_t* drive, s32 drive_count, wchar_t* dir, s32 dir_count, wchar_t* filename, s32 filename_count, wchar_t* ext, s32 ext_count)
		{
			_wsplitpath_s(full_path, drive, drive_count, dir, dir_count, filename, filename_count, ext, ext_count);
		}

		// _makepath
		void MakePath(char* path, s32 size_in_bytes, const char* drive, const char* dir, const char* filename, const char* ext)
		{
			_makepath_s(path, size_in_bytes, drive, dir, filename, ext);
		}

		// _wmakepath
		void WMakePath(wchar_t* path, s32 size_in_bytes, const wchar_t* drive, const wchar_t* dir, const wchar_t* filename, const wchar_t* ext)
		{
			_wmakepath_s(path, size_in_bytes, drive, dir, filename, ext);
		}

		// fabsf
		f32 FAbs(f32 x)
		{
			return fabsf(x);
		}

		// logf
		f32 Log(f32 x, f32 y)
		{
			return logf(x) / logf(y);
		}

		// log10f
		f32 Log10(f32 x)
		{
			return log10f(x);
		}

		// tolower
		s32 ToLower(s32 c)
		{
			return tolower(c);
		}
	} // namespace FND
} // namespace Bread