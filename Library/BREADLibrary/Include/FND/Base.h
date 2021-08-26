#pragma once

#include "NonCopyable.h"

namespace Bread
{
	namespace FND
	{
		//****************************************************************************
		// すべてのクラスの基底となるクラス
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