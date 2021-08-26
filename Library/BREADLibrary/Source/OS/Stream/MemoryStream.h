#pragma once

#include "OS/Stream.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// メモリストリーム操作クラス
		//****************************************************************************
		class MemoryStream final : public IMemoryStream
		{
		private:
			u8* buffer = nullptr;
			u8* current = nullptr;
			s32 size;

		public:
			~MemoryStream() override { Finalize(); }

		public:
			// 初期化
			bool Initialize() override { return true; }

			// 終了化
			void Finalize() override { Close(); }

			// オープン
			bool Open(void* buffer, s32 size) override;

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
		};
	} // namespace OS
} // namespace Phoenix