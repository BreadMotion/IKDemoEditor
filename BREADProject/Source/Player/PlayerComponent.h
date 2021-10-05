#pragma once

#include "FrameWork/Component/Component.h"

#include "Math/BreadMath.h"

#include "FND/Util.h"
#include "FND/STD.h"

namespace Bread
{
	namespace FrameWork
	{
		class PlayerComponent : public Component
		{
		private:

		public:
			explicit PlayerComponent();
			~PlayerComponent()override;

		public:
			//初期化
			void Initialize()override;

			//事前更新
			void __fastcall PreUpdate()override;

			//更新
			void __fastcall Update()override;

			//事後更新
			void __fastcall NextUpdate()override;

			//GUI
			void GUI()override;
		};
	}
}