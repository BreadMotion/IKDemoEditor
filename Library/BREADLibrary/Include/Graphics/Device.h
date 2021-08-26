#pragma once

#include <memory>
#include "Types.h"
#include "FND/Base.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// デバイス操作インターフェース
		//****************************************************************************
		class IDevice : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IDevice> Create();

			// 初期化
			virtual bool Initialize() = 0;

			// 終了化
			virtual void Finalize()   = 0;
		};
	} // namespace Graphics
} // namespace Bread