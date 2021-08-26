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
			bool Initialize(const char* basePath) override;

			// 終了化
			void Finalize() override { Close(); }

			// 存在確認
			bool Exists(const char* path) override;
			bool ExistsW(const wchar_t* path) override;

			// オープン
			bool Open(const char* path, FileAccess access) override;

			// クローズ
			void Close() override;

			// 読み込み
			s32 Read(void* buffer, s32 size) override;

			// 書き込み
			s32 Write(const void* buffer, s32 size) override;

			// ファイルポインタ位置設定
			bool Seek(s32 offset, StreamSeek origin) override;

			// ファイルポインタ位置取得
			s32 Tell() override;

			// ファイルサイズ取得
			s32 GetSize() override;

			// ファイルの削除
			bool Remove(const char* path) override;
		};
	} // namespace OS
} // namespace Phoenix