#pragma once

#include <Windows.h>
#include <tchar.h>
#include <string>


// ファイル名を指定するコモンダイアログを表示
BOOL GetFileName(HWND hWnd, TCHAR* fname, int sz, const wchar_t* initDir, const wchar_t* ext, bool isOpen = true);

// 実行可能ファイルが実行されている場所までディレクション （例："C:\\Desktop\\Project\\）
// DebugとReleaseのファイルパスは含まない。もし、含むのであればkeepをtrueにするとそのままの状態で返す。
std::string GetModuleFilePass(bool keep = false);

// 実行可能ファイルが実行されている場所までディレクション （例：L"C:\\Desktop\\Project\\）
// DebugとReleaseのファイルパスは含まない。もし、含むのであればkeepをtrueにするとそのままの状態で返す。
std::wstring GetModuleFilePassW(bool keep = false);

// stringをwstringへ変換する
std::wstring StringToWString(std::string oString);