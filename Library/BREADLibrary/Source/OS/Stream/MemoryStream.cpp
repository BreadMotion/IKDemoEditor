#include "pch.h"
#include "FND/Logger.h"
#include "FND/STD.h"
#include "MemoryStream.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// メモリストリーム操作クラス
		//****************************************************************************
		// 生成
		std::unique_ptr<IMemoryStream> IMemoryStream::Create()
		{
			return std::make_unique<MemoryStream>();
		}

		bool MemoryStream::Open(void* buffer, s32 size)
		{
			buffer = current = reinterpret_cast<u8*>(buffer);
			size = size;
			return false;
		}

		// クローズ
		void MemoryStream::Close()
		{
			buffer = nullptr;
			current = nullptr;
			size = 0;
		}

		// 読み込み
		s32 MemoryStream::Read(void* buffer, s32 size)
		{
			s32 remain = size - static_cast<s32>(current - static_cast<u8*>(buffer));
			s32 read = (remain < size) ? remain : size;

			FND::MemCpy(buffer, current, read);
			current += read;

			return read;
		}

		// 書き込み
		s32 MemoryStream::Write(const void* buffer, s32 size)
		{
			s32 remain = size - static_cast<s32>(current - static_cast<const u8*>(buffer));
			s32 write = (remain < size) ? remain : size;

			FND::MemCpy(current, buffer, write);
			current += write;

			return write;
		}

		// ファイルポインタ位置設定
		bool MemoryStream::Seek(s32 offset, StreamSeek origin)
		{
			switch (origin)
			{
			case StreamSeek::Top:
			{
				s32	pos = offset;
				if (pos < 0 || pos > size)
				{
					BREAD_LOG_OS_ERROR("ファイルシークに失敗しました。\n");
					return false;
				}

				current = buffer + offset;
				break;
			}
			case StreamSeek::Current:
			{
				s32	pos = static_cast<s32>(current - buffer) + offset;
				if (pos < 0 || pos > size)
				{
					BREAD_LOG_OS_ERROR("ファイルシークに失敗しました。\n");
					return false;
				}

				current += offset;
				break;
			}
			case StreamSeek::End:
			{
				s32 pos = size + offset;
				if (pos < 0 || pos > size)
				{
					BREAD_LOG_OS_ERROR("ファイルシークに失敗しました。\n");
					return false;
				}

				current = buffer + pos;
				break;
			}
			}

			return true;
		}

		// ファイルポインタ位置取得
		s32 MemoryStream::Tell()
		{
			return static_cast<s32>(current - buffer);
		}

		// ファイルサイズ取得
		s32 MemoryStream::GetSize()
		{
			return size;
		}

	} // namespace OS
} // namespace Bread