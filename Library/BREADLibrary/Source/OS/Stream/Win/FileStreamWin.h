#pragma once

#include <stdio.h>
#include "OS/Stream.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// Windows版ファイルストリーム操作クラス
		//****************************************************************************
		class FileStreamWin final : public IFileStream
		{
		private:
			FILE* fp = nullptr;
			char basePath[256] = { 0 };

		public:
			~FileStreamWin() override { Finalize(); }

		public:
			// 初期化
			bool __fastcall Initialize(const char* basePath) override;

			// 終了化
			void Finalize() override { Close(); }

			// 存在確認
			bool __fastcall Exists(const char* path) override;
			bool __fastcall ExistsW(const wchar_t* path) override;

			// オープン
			bool __fastcall Open(const char* path, FileAccess access) override;

			// クローズ
			void Close() override;

			// 読み込み
			s32 __fastcall Read(void* buffer, s32 size) override;

			// 書き込み
			s32 __fastcall Write(const void* buffer, s32 size) override;

			// ファイルポインタ位置設定
			bool __fastcall Seek(s32 offset, StreamSeek origin) override;

			// ファイルポインタ位置取得
			s32 Tell() override;

			// ファイルサイズ取得
			s32 GetSize() override;

			// ファイルの削除
			bool __fastcall Remove(const char* path) override;
		};
	} // namespace OS
} // namespace Phoenix