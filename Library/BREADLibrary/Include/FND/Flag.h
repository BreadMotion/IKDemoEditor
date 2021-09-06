#pragma once

#include "Types.h"
#include "FND/NonCopyable.h"


namespace Bread
{
	namespace FND
	{
		//****************************************************************************
		// �t���O����
		//****************************************************************************
		template <class T>
		class Flag
		{
		private:
			T flag = 0;

		public:
			// ���Z�q�̃L���X�e�B���O
			operator T () const { return flag; }

			// ���
			void operator = (T f) { flag = f; }
			void operator = (const Flag<T>& f) { flag = f.flag; }

			// ������Z�q
			Flag<T>& operator |= (T f) { flag |= f; return *this; }
			Flag<T>& operator &= (T f) { flag &= f; return *this; }
			Flag<T>& operator |= (const Flag<T>& f) { flag |= f.flag; return *this; }
			Flag<T>& operator &= (const Flag<T>& f) { flag &= f.flag; return *this; }

		public:
			// �t���O�𗧂Ă�
			void Set(T f) { flag |= f; }

			// �t���O�𗎂Ƃ�
			void Reset(T f) { flag &= ~f; }

			// �t���O�������Ă��邩�e�X�g����
			bool Test(T f) const { return ((flag & f) != 0); }

			// �t���O�𔽓]����
			void Toggle(T f) { flag ^= f; }

			// �t���O���N���A����
			void Clear() { flag = (T)0; }

			// �t���O��ݒ肷��
			void SetAll(T f) { flag = flag; }

			// �t���O���擾����
			T GetAll() const { return flag; }

			// �}�X�N�����t���O���擾����
			T GetMask(T mask) const { (T)(flag & mask); }

			// �t���O�T�C�Y���擾����
			T GetSize() const { return (T)sizeof(T) * 8; }
		};


		// ��`
		using Flag8  = Flag<u8>;
		using Flag16 = Flag<u16>;
		using Flag32 = Flag<u32>;
		using Flag64 = Flag<u64>;

	}	// namespace fnd
}	// namespace ys

// �h���N���X�p�R���X�g���N�^(64bit)
#define		FLAG64_CONSTRUCT( name )	\
public:\
	name(){}\
	name(Bread::u64 flag){ SetAll( flag ); }

// �h���N���X�p�R���X�g���N�^(32bit)
#define		FLAG32_CONSTRUCT( name )	\
public:\
	name(){}\
	name(Bread::u32 flag){ SetAll( flag ); }

// �h���N���X�p�R���X�g���N�^(16bit)
#define		FLAG16_CONSTRUCT( name )	\
public:\
	name(){}\
	name(Bread::u16 flag){ SetAll( flag ); }

// �h���N���X�p�R���X�g���N�^(8bit)
#define		FLAG8_CONSTRUCT( name )	\
public:\
	name(){}\
	name(Bread::u8 flag){ SetAll( flag ); }