#pragma once
#include "Types.h"


namespace Bread
{
	namespace FND
	{
		// strcmp
		extern s32 StrCmp(const char* lhs, const char* rhs);

		// strncmp
		extern s32 StrNCmp(const char* lhs, const char* rhs, s32 max_count);

		// strcpy
		extern void StrCpy(char* dst, s32 size_in_bytes, const char* src);

		// strncpy
		extern void StrNCpy(char* dst, s32 size_in_bytes, const char* src, s32 count);

		// strcat
		extern void StrCat(char* dst, s32 size_in_bytes, const char* src);

		// strlen
		extern s32 StrLen(const char* str);

		// wcslen
		extern s32 WcsLen(const wchar_t* str);

		// memcpy
		extern void* MemCpy(void* dst, const void* src, s32 size);

		// memset
		extern void* MemSet(void* dst, s32 val, s32 size);

		// memcmp
		s32 MemCmp(const void* buf1, const void* buf2, s32 size);

		// wcstombs
		extern s32 WcsToMbs(char* mbstr, s32 size_in_bytes, const wchar_t* wcstr, s32 count);

		// mbstowcs
		extern s32 MbsToWcs(wchar_t* wcstr, s32 size_in_words, const char* mbstr, s32 count);

		// _splitpath
		extern void SplitPath(const char* full_path, char* drive, s32 drive_count, char* dir, s32 dir_count, char* filename, s32 filename_count, char* ext, s32 ext_count);

		// _wsplitpath
		extern void WSplitPath(const wchar_t* full_path, wchar_t* drive, s32 drive_count, wchar_t* dir, s32 dir_count, wchar_t* filename, s32 filename_count, wchar_t* ext, s32 ext_count);

		// _makepath
		extern void MakePath(char* path, s32 size_in_bytes, const char* drive, const char* dir, const char* filename, const char* ext);

		// _wmakepath
		extern void WMakePath(wchar_t* path, s32 size_in_bytes, const wchar_t* drive, const wchar_t* dir, const wchar_t* filename, const wchar_t* ext);

		// fabsf
		extern f32 FAbs(f32 x);

		// logf
		extern f32 Log(f32 x, f32 y);

		// log10f
		extern f32 Log10(f32 x);

		// tolower
		extern s32 ToLower(s32 c);
	} // namespace FND
} // namespace Bread