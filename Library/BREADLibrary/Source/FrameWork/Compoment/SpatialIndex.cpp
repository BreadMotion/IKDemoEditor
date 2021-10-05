#include "FND/Instance.h"
#include "FrameWork/Component/SpatialIndex.h"
#include "FrameWork/Component/Transform.h"
#include "FrameWork/SpatialDivisionManager/SpatialDivisionManager.h"

using Bread::FND::Instance;
using Bread::Math::Vector3S32;
using Bread::Math::Vector3;

namespace Bread
{
	namespace FrameWork
	{
			// ������
			void SpatialIndexComponent::Initialize()
			{
				SpatialIndex = { 0,0,0 };
			}

			// �I����
			void SpatialIndexComponent::Finalize() {}

			//���O�X�V
			void SpatialIndexComponent::PreUpdate() {}

			// �X�V
			void SpatialIndexComponent::Update()
			{
				SpatialIndex = Instance<SpatialDivisionManager>::instance
					.SpatialCurrent(GetOwner()->GetComponent<Transform>()->GetTranslate());
			}

			//����X�V
			void SpatialIndexComponent::NextUpdate() {}

			// �`��
			void SpatialIndexComponent::Draw() {}

			//imgui
			void SpatialIndexComponent::GUI() {}
	}//namespace Math
}//namespace Bread