#pragma once
#include <immintrin.h>

#include "Target/Target.h"

namespace Bread
{
#if defined(BREAD_TARGET_WIN)
	using s8   = char;
	using u8   = unsigned char;
	using s16  = short;
	using u16  = unsigned short;
	using s32  = int;
	using u32  = unsigned int;
	using s64  = long long;
	using u64  = unsigned long long;
	using f32  = float;
	using f64  = double;
	using uintPtr = uintptr_t;
	using sizeT   = size_t;

	using vector = __m128;
	// ��s8��s�́usigned�v�A8�́u8�r�b�g�v�������Ă���B
#else
	//�^�[�Q�b�g���錾����Ă��܂���B
#endif
}