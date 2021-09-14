#pragma once
#define BREAD_TARGET_WIN

#if defined(BREAD_TARGET_WIN)
	#include "Target/TargetWin.h"
#else
	//ターゲットが宣言されていません。
#endif