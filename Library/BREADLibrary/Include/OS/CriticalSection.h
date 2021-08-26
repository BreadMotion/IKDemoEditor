#pragma once

#include <memory>
#include "FND/Base.h"


namespace Bread
{
	namespace OS
	{
		//****************************************************************************
		// �N���e�B�J���Z�N�V��������C���^�[�t�F�[�X
		//****************************************************************************
		class ICriticalSection : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<ICriticalSection> Create();

			// ������
			virtual bool Initialize() = 0;

			// �I����
			virtual void Finalize() = 0;

			// ���L���擾�҂�
			virtual void Enter() = 0;

			// ���L�����
			virtual void Leave() = 0;
		};

		//****************************************************************************
		// �N���e�B�J���Z�N�V�������b�N
		//****************************************************************************
		class CriticalSectionLock
		{
		private:
			ICriticalSection* obj;

		public:
			CriticalSectionLock(ICriticalSection* obj) : obj(obj)
			{
				obj->Enter();
			}
			~CriticalSectionLock()
			{
				obj->Leave();
			}
		};
	}
}