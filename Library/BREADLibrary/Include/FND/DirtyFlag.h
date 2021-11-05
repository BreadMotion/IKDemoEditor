#pragma once
#include "Base.h"
#include "Types.h"
#include "Math/BreadMath.h"

namespace Bread
{
	namespace FND
	{
		class [[]] DirtyFlag //: public Base
		{
		private:
			bool dirtyFlag{ false };

		public:
			DirtyFlag()  {}
			~DirtyFlag() {}

		protected:
			//変更があったかを示す
			bool IsDirty()                  { return dirtyFlag; }

			//ダーティフラグを伏せる
			void CleanDirtyFlag()           { dirtyFlag = false; }

			//ダーティフラグを設定する
			bool SetDirty(const bool& flag) { return (dirtyFlag = flag); }

			//ダーティフラグの操作する関数
			virtual void ResearchDirty()    {}

		public:
			bool _fastcall operator == (const bool& flag) { return dirtyFlag == flag; }
			bool _fastcall operator != (const bool& flag) { return dirtyFlag != flag; }
			bool _fastcall operator  = (const bool& flag) { return dirtyFlag = flag; }
		};
	}//namespace FND
}//namespace Bread