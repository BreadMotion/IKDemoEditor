#pragma once

#define WIN32_WINNT 0x0510

#include <Windows.h>
#include <xaudio2.h>
#include <mmsystem.h>
#include <thread>
#include <mutex>
#include <vector>
#include <cassert>
#include <vector>
#include <list>

#include "WAVFileReader.h"
#include "Types.h"

#pragma comment(lib, "xaudio2.lib")


// TODO : re -> Audioクラスを作成
namespace Bread
{
	namespace FrameWork
	{
		class CXAudio2
		{
		public:
			//std::vector<SourceVoice> sourceVoice;
			std::vector<IXAudio2SourceVoice*> lpSourceVoice;
			std::unique_ptr<uint8_t[]> waveFile;
			wchar_t fName[256];
			WAVData waveData;
			XAUDIO2_BUFFER buffer = { 0 };
			bool playing;

			enum Type
			{
				normal,
				loop,
			};

		public:
			//CXAudio2(const wchar_t* fileName, int type);
			CXAudio2() {}
			~CXAudio2()
			{
				Release();
			}

		public:
			static std::unique_ptr<CXAudio2> Create();

			/// <summary>
			/// waveファイルのロード
			/// </summary>
			/// <param name="fileName"> waveファイルパス </param>
			/// <param name="type"> 再生形態 : 0 -> 通常再生、1 -> ループ再生 </param>
			void Load(const wchar_t* fileName, int type);
			void Release()
			{
				for (auto& it : lpSourceVoice)
				{
					it->DestroyVoice();
				}
				lpSourceVoice.clear();
			}

		public:
			bool PlayWAV(bool onCopy);		// 再生
			bool PauseWAV();				// 一時停止
			bool StopWAV();					// 停止
			bool Update();					// 更新
			bool Copy();					// 複製
			bool SetVolume(float volume);	// ボリューム調整
			bool SetPitch(float pitch);		// ピッチ調整
			Bread::f32 GetVolume();		// ボリューム取得
			Bread::f32 GetPitch();		// ピッチ取得
		};

		// TODO : re -> Factor化
		class CXAudio2Origin
		{
		public:
			IXAudio2* lpXAudio = nullptr;
			IXAudio2MasteringVoice* lpMasterVoice = nullptr;

		private:
			CXAudio2Origin();

		public:
			~CXAudio2Origin()
			{
				Release();
			}

			void Release()
			{
				if (lpMasterVoice) lpMasterVoice->DestroyVoice();

				if (lpXAudio)
				{
					lpXAudio->Release();
					lpXAudio = nullptr;
				}

				CoUninitialize();
			}

			static CXAudio2Origin* GetInstance()
			{
				static CXAudio2Origin instance;
				return &instance;
			}
		};

		#define pCXAudio2Origin  (CXAudio2Origin::GetInstance())
	}
}