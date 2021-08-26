#pragma once

#include "NonCopyable.h"

namespace Bread
{
	namespace FND
	{
		//****************************************************************************
		// ���ׂẴN���X�̊��ƂȂ�N���X
		//****************************************************************************
		class Base : public NonCopyable
		{
		public:
			Base() {}
			virtual ~Base() {}
		};

		class CopyableBase
		{
		public:
			CopyableBase() {}
			virtual ~CopyableBase() {}
		};

	}	// namespace FND
}	// namespace Bread