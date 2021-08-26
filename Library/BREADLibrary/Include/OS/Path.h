#pragma once

namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// ファイルパス操作ユーティリティ
		//****************************************************************************
		class Path
		{
		public:
			// カレントディレクトリを取得
			static const char* GetCurrentDirectory(char* buffer = nullptr, int bufferSize = 0);

			// フルパスを取得
			static const char* GetFullPath(const char* filePath, char* buffer = nullptr, int bufferSize = 0);
			static const wchar_t* GetFullPathW(const wchar_t* filePath, wchar_t* buffer = nullptr, int bufferSize = 0);

			// フルパスからファイル名を取得
			static const char* GetFileName(const char* filePath);

			// フルパスからファイル拡張子を取得
			static const char* GetFileExtension(const char* filePath);

			// フルパスのファイル拡張子を変更
			static const char* ChangeFileExtension(const char* filePath, const char* extension, char* buffer = nullptr, int bufferSize = 0);

			// フルパスからファイル拡張子をチェック
			static bool CheckFileExtension(const char* filePath, const char* extension);

			// フルパスからファイル名を除いたディレクトリパスを取得
			static const char* GetDirectoryName(const char* filePath, char* buffer = nullptr, int bufferSize = 0);

			// ファイルパスを小文字に変換し'\\'を'/'変換
			static const char* ToLower(const char* filePath, char* buffer = nullptr, int bufferSize = 0);

			// ../を含むパスを最適化
			static const char* Clean(const char* filePath, char* buffer = nullptr, int bufferSize = 0);

			// 相対ファイルパスを生成
			static const char* GetRelativePath(const char* basePath, const char* targetPath, char* buffer = nullptr, int bufferSize = 0);

			// 相対ファイルパスから絶対ファイルパスを生成
			static const char* GetAbsolutePath(const char* basePath, const char* relativePath, char* buffer = nullptr, int bufferSize = 0);

			// ファイルパスから拡張子なしファイル名を取得
			static const char* GetFileNameWithoutExtension(const char* filePath, char* buffer = nullptr, int bufferSize = 0);

			// ファイルパスを結合
			static const char* Combine(const char* basePath, const char* targetPath, char* buffer = nullptr, int bufferSize = 0);

			// ワイルドカードマッチング
			// '*' は、複数文字何でもマッチングする
			// '?' は、一文字何でもマッチングする
			// 複数ワイルドカードはスペースまたはタブで区切ること
			static bool IsMatchWildcard(const char* filePath, const char* wildcard);

			// パスが同一か比較
			static bool Equal(const char* filePth1, const char* filePath2);

			// 実行可能ファイルが実行されている場所までディレクション （例："C:\\Desktop\\Project\\）
			// DebugとReleaseのファイルパスは含まない。もし、含むのであればkeepをtrueにするとそのままの状態で返す。
			static const char* GetModuleFilePass(bool keep = false);

			// 実行可能ファイルが実行されている場所までディレクション （例："C:\\Desktop\\Project\\）
			// backCountの数分ファイル階層を遡る。
			static const char* GetModuleFilePass(int backCount);
		};

	} // namespace OS
} // namespace Bread