#pragma once

#include "Types.h"


namespace Bread
{
	namespace FND
	{
		// 4bit切り上げ
		template<class T>
		inline T RoundUp4(const T& v)    { return (((v)+(const T)3) & ~(const T)3); }
		// 4bit切り捨て
		template<class T>
		inline T RoundDown4(const T& v)  { return ((v) & ~(const T)3); }
		// 8bit切り上げ
		template<class T>
		inline T RoundUp8(const T& v)    { return (((v)+(const T)7) & ~(const T)7); }
		// 8bit切り捨て
		template<class T>
		inline T RoundDown8(const T& v)  { return ((v) & ~(const T)7); }
		// 16bit切り上げ
		template<class T>
		inline T RoundUp16(const T& v)   { return (((v)+(const T)15) & ~(const T)15); }
		// 16bit切り捨て
		template<class T>
		inline T RoundDown16(const T& v) { return ((v) & ~(const T)15); }
		// 32bit切り上げ
		template<class T>
		inline T RoundUp32(const T& v)   { return (((v)+(const T)31) & ~(const T)31); }
		// 32bit切り捨て
		template<class T>
		inline T RoundDown32(const T& v) { return ((v) & ~(const T)31); }
		// 64bit切り上げ
		template<class T>
		inline T RoundUp64(const T& v)   { return (((v)+(const T)63) & ~(const T)63); }
		// 64bit切り捨て
		template<class T>
		inline T RoundDown64(const T& v) { return ((v) & ~(const T)63); }
		// 128bit切り上げ
		template<class T>
		inline T RoundUp128(const T& v)  { return (((v)+(const T)127) & ~(const T)127); }
		// 128bit切り捨て
		template<class T>
		inline T RoundDown128(const T& v) { return ((v) & ~(const T)127); }
		// 256切り上げ
		template<class T>
		inline T RoundUp256(const T& v)   { return (((v)+(const T)255) & ~(const T)255); }
		// 256bit切り捨て
		template<class T>
		inline T RoundDown256(const T& v) { return ((v) & ~(const T)255); }
		// 512切り上げ
		template<class T>
		inline T RoundUp512(const T& v)   { return (((v)+(const T)511) & ~(const T)511); }
		// 512bit切り捨て
		template<class T>
		inline T RoundDown512(const T& v) { return ((v) & ~(const T)511); }
		// 1024切り上げ
		template<class T>
		inline T RoundUp1024(const T& v)  { return (((v)+(const T)1023) & ~(const T)1023); }
		// 1024bit切り捨て
		template<class T>
		inline T RoundDown1024(const T& v) { return ((v) & ~(const T)1023); }
		// 2048切り上げ
		template<class T>
		inline T RoundUp2048(const T& v)   { return (((v)+(const T)2047) & ~(const T)2047); }
		// 2048bit切り捨て
		template<class T>
		inline T RoundDown2048(const T& v) { return ((v) & ~(const T)2047); }
		// 4096切り上げ
		template<class T>
		inline T RoundUp4096(const T& v)   { return (((v)+(const T)4095) & ~(const T)4095); }
		// 4096bit切り捨て
		template<class T>
		inline T RoundDown4096(const T& v) { return ((v) & ~(const T)4095); }
		// 8192切り上げ
		template<class T>
		inline T RoundUp8192(const T& v)   { return (((v)+(const T)8191) & ~(const T)8191); }
		// 8192切り捨て
		template<class T>
		inline T RoundDown8192(const T& v) { return ((v) & ~(const T)8191); }

		// 低速 n 切り上げ
		template<class T>
		inline T RoundUp(const T& n, const T& v)
		{
			T	v = n * (((v)+n) % n);
			return v;
		}

		// 低速 n 切り捨て
		template<class T>
		inline T RoundDown(const T& n, const T& v)
		{
			T	v = n * ((v) % n);
			return v;
		}

		// 大選択
		template<class T>
		inline T Max(const T& a, const T& b)
		{
			return ((a) > (b)) ? (a) : (b);
		}

		// 小選択
		template<class T>
		inline T Min(const T& a, const T& b)
		{
			return ((a) < (b)) ? (a) : (b);
		}

		// クランプ
		template<class T>
		inline T Clamp(const T& x, const T& min, const T& max)
		{
			return Min(max, Max(min, x));
		}

		// 配列要素数取得
		template<class T, s32 n>
		inline s32 ArraySize(T(&)[n])
		{
			return n;
		}

		// 安全解放
		template<class T>
		inline void SafeRelease(T*& p)
		{
			if (p != nullptr)
			{
				p->Release();
				p = nullptr;
			}
		}

		// 安全終了化
		template<class T>
		inline void SafeFinalize(T& p)
		{
			if (p != nullptr)
			{
				p->Finalize();
				p = nullptr;
			}
		}

		// 安全解放
		template<class T>
		inline void SafeDelete(T*& p)
		{
			if (p != nullptr)
			{
				delete p;
				p = nullptr;
			}
		}

		// 安全解放
		template<class T>
		inline void SafeDeleteArray(T*& p)
		{
			if (p != nullptr)
			{
				delete[] p;
				p = nullptr;
			}
		}

	} // namespace FND
} // namespace Bread