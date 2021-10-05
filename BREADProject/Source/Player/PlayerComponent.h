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
			//������
			void Initialize()override;

			//���O�X�V
			void __fastcall PreUpdate()override;

			//�X�V
			void __fastcall Update()override;

			//����X�V
			void __fastcall NextUpdate()override;

			//GUI
			void GUI()override;
		};
	}
}