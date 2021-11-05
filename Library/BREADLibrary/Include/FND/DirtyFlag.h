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
			//�ύX����������������
			bool IsDirty()                  { return dirtyFlag; }

			//�_�[�e�B�t���O�𕚂���
			void CleanDirtyFlag()           { dirtyFlag = false; }

			//�_�[�e�B�t���O��ݒ肷��
			bool SetDirty(const bool& flag) { return (dirtyFlag = flag); }

			//�_�[�e�B�t���O�̑��삷��֐�
			virtual void ResearchDirty()    {}

		public:
			bool _fastcall operator == (const bool& flag) { return dirtyFlag == flag; }
			bool _fastcall operator != (const bool& flag) { return dirtyFlag != flag; }
			bool _fastcall operator  = (const bool& flag) { return dirtyFlag = flag; }
		};
	}//namespace FND
}//namespace Bread