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
			void PreUpdate(const f32& dt)override;

			//�X�V
			void Update(const f32& dt)override;

			//����X�V
			void NextUpdate(const f32& dt)override;

			//GUI
			void GUI()override;
		};
	}
}