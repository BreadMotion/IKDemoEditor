#pragma once

#include <memory>
#include "Types.h"
#include "FND/Base.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// �f�o�C�X����C���^�[�t�F�[�X
		//****************************************************************************
		class IDevice : public FND::Base
		{
		public:
			// ����
			static std::unique_ptr<IDevice> Create();

			// ������
			virtual bool Initialize() = 0;

			// �I����
			virtual void Finalize()   = 0;
		};
	} // namespace Graphics
} // namespace Bread