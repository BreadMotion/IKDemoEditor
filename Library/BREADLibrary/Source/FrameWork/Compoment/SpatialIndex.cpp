#include "FND/Instance.h"
#include "FrameWork/Component/SpatialIndex.h"
#include "FrameWork/SpatialDivisionManager/SpatialDivisionManager.h"

using Bread::FND::Instance;
using Bread::Math::Vector3S32;
using Bread::Math::Vector3;

namespace Bread
{
	namespace FrameWork
	{

			// ������
			void SpatialIndexComponent::Initialize() {}

			// �I����
			void SpatialIndexComponent::Finalize() {}

			//���O�X�V
			void SpatialIndexComponent::PreUpdate(const f32&) {}

			// �X�V
			void SpatialIndexComponent::Update(const f32&)
			{
				SpatialIndex = Instance<SpatialDivisionManager>::instance.SpatialCurrent(position);
			}

			//����X�V
			void SpatialIndexComponent::NextUpdate(const f32&) {}

			// �`��
			void SpatialIndexComponent::Draw(const f32&) {}

			//imgui
			void SpatialIndexComponent::GUI() {}
	}//namespace Math
}//namespace Bread