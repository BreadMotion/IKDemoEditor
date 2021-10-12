#pragma once
#include "Base.h"
#include "Types.h"
#include "Math/BreadMath.h"

namespace Bread
{
	namespace FND
	{
		class [[]] DirtyFlag : public Base
		{
		private:
			bool dirtyFlag{ false };

		public:
			DirtyFlag() {}
			~DirtyFlag() {}

		public:
			virtual bool IsDirty(){}

		};
	}//namespace FND
}//namespace Bread