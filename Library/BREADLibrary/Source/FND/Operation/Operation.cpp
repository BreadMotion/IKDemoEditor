#include "../pch.h"
#include "FND/Operation.h"


BOOL GetFileName(HWND hWnd, TCHAR* fname, int sz, const wchar_t* initDir, const wchar_t* ext, bool isOpen)
{
	if (isOpen)
	{
		OPENFILENAME o;
		fname[0] = _T('\0');
		ZeroMemory(&o, sizeof(o));
		o.lStructSize = sizeof(o);		// 構造体サイズ
		o.hwndOwner = hWnd;				// 親ウィンドウのハンドル
		o.lpstrInitialDir = initDir;	// 初期フォルダー
		o.lpstrFile = fname;			// 取得したファイル名を保存するバッファ
		o.nMaxFile = sz;				// 取得したファイル名を保存するバッファサイズ
		if (wcscmp(ext, L".bin") == 0)
		{
			o.lpstrFilter = _TEXT("binファイル(*.bin)\0*.bin\0") _TEXT("全てのファイル(*.*)\0*.*\0");
			o.lpstrDefExt = _TEXT("bin");
		}
		else if (wcscmp(ext, L".json") == 0)
		{
			o.lpstrFilter = _TEXT("jsonファイル(*.json)\0*.json\0") _TEXT("全てのファイル(*.*)\0*.*\0");
			o.lpstrDefExt = _TEXT("json");
		}
		else if (wcscmp(ext, L".fbx") == 0)
		{
			o.lpstrFilter = _TEXT("fbxファイル(*.fbx)\0*.fbx\0") _TEXT("全てのファイル(*.*)\0*.*\0");
			o.lpstrDefExt = _TEXT("fbx");
		}
		else
		{
			o.lpstrFilter = _TEXT("全てのファイル(*.*)\0*.*\0");
			o.lpstrDefExt = _TEXT("");
		}
		o.lpstrTitle = _TEXT("ファイルを指定");
		o.nFilterIndex = 1;
#if 0 // 複数ファイルのパスを取得
		o.Flags |= OFN_ALLOWMULTISELECT;
		o.Flags |= OFN_EXPLORER;
#endif
		return GetOpenFileName(&o);
	}
	else
	{
		OPENFILENAME s;
		fname[0] = _T('\0');
		ZeroMemory(&s, sizeof(s));
		s.lStructSize = sizeof(s);		// 構造体サイズ
		s.hwndOwner = hWnd;				// 親ウィンドウのハンドル
		s.lpstrInitialDir = initDir;	// 初期フォルダー
		s.lpstrFile = fname;			// 取得したファイル名を保存するバッファ
		s.nMaxFile = sz;				// 取得したファイル名を保存するバッファサイズ
		if (wcscmp(ext, L".bin") == 0)
		{
			s.lpstrFilter = _TEXT("binファイル(*.bin)\0*.bin\0") _TEXT("全てのファイル(*.*)\0*.*\0");
			s.lpstrDefExt = _TEXT("bin");
		}
		else if (wcscmp(ext, L".json") == 0)
		{
			s.lpstrFilter = _TEXT("jsonファイル(*.json)\0*.json\0") _TEXT("全てのファイル(*.*)\0*.*\0");
			s.lpstrDefExt = _TEXT("json");
		}
		else
		{
			s.lpstrFilter = _TEXT("全てのファイル(*.*)\0*.*\0");
			s.lpstrDefExt = _TEXT("");
		}
		s.lpstrTitle = _TEXT("ファイルを保存");
		s.nFilterIndex = 1;
		return GetSaveFileName(&s);
	}

	return FALSE;
}


std::string GetModuleFilePass(bool keep)
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	std::string result = std::string(buffer).substr(0, pos);

	if (!keep)
	{
		std::string::size_type pos2 = result.find_last_of("\\/");
		std::string result2 = result.substr(pos2 + 1, pos);
		if (result2 == std::string("Debug") || result2 == std::string("Release"))
		{
			result = std::string(buffer).substr(0, pos2);
		}
	}

	result += std::string("\\");

	return result;
}

std::wstring GetModuleFilePassW(bool keep)
{
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	std::string result = std::string(buffer).substr(0, pos);

	if (!keep)
	{
		std::string::size_type pos2 = result.find_last_of("\\/");
		std::string result2 = result.substr(pos2 + 1, pos);
		if (result2 == std::string("Debug") || result2 == std::string("Release"))
		{
			result = std::string(buffer).substr(0, pos2);
		}
	}

	result += std::string("\\");

	std::wstring reconvert = StringToWString(result);

	return reconvert;
}

std::wstring StringToWString(std::string oString)
{
	// SJIS → wstring
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, (wchar_t*)NULL, 0);

	// バッファの取得
	wchar_t* cpUCS2 = new wchar_t[iBufferSize];

	// SJIS → wstring
	MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, cpUCS2, iBufferSize);

	// stringの生成
	std::wstring oRet(cpUCS2, cpUCS2 + iBufferSize - 1);

	// バッファの破棄
	delete[] cpUCS2;

	// 変換結果を返す
	return(oRet);
}