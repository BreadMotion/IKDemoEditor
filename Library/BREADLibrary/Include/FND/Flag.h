#pragma once

#include "Types.h"
#include "FND/NonCopyable.h"


namespace Bread
{
	namespace FND
	{
		//****************************************************************************
		// フラグ操作
		//****************************************************************************
		template <class T>
		class Flag
		{
		private:
			T flag = 0;

		public:
			// 演算子のキャスティング
			operator T () const { return flag; }

			// 代入
			void operator = (T f) { flag = f; }
			void operator = (const Flag<T>& f) { flag = f.flag; }

			// 代入演算子
			Flag<T>& operator |= (T f) { flag |= f; return *this; }
			Flag<T>& operator &= (T f) { flag &= f; return *this; }
			Flag<T>& operator |= (const Flag<T>& f) { flag |= f.flag; return *this; }
			Flag<T>& operator &= (const Flag<T>& f) { flag &= f.flag; return *this; }

		public:
			// フラグを立てる
			void Set(T f) { flag |= f; }

			// フラグを落とす
			void Reset(T f) { flag &= ~f; }

			// フラグが立っているかテストする
			bool Test(T f) const { return ((flag & f) != 0); }

			// フラグを反転する
			void Toggle(T f) { flag ^= f; }

			// フラグをクリアする
			void Clear() { flag = (T)0; }

			// フラグを設定する
			void SetAll(T f) { flag = flag; }

			// フラグを取得する
			T GetAll() const { return flag; }

			// マスクしたフラグを取得する
			T GetMask(T mask) const { (T)(flag & mask); }

			// フラグサイズを取得する
			T GetSize() const { return (T)sizeof(T) * 8; }
		};


		// 定義
		using Flag8  = Flag<u8>;
		using Flag16 = Flag<u16>;
		using Flag32 = Flag<u32>;
		using Flag64 = Flag<u64>;

	}	// namespace fnd
}	// namespace ys

// 派生クラス用コンストラクタ(64bit)
#define		FLAG64_CONSTRUCT( name )	\
public:\
	name(){}\
	name(Bread::u64 flag){ SetAll( flag ); }

// 派生クラス用コンストラクタ(32bit)
#define		FLAG32_CONSTRUCT( name )	\
public:\
	name(){}\
	name(Bread::u32 flag){ SetAll( flag ); }

// 派生クラス用コンストラクタ(16bit)
#define		FLAG16_CONSTRUCT( name )	\
public:\
	name(){}\
	name(Bread::u16 flag){ SetAll( flag ); }

// 派生クラス用コンストラクタ(8bit)
#define		FLAG8_CONSTRUCT( name )	\
public:\
	name(){}\
	name(Bread::u8 flag){ SetAll( flag ); }