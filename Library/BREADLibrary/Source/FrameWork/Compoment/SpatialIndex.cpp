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
			// 初期化
			void SpatialIndexComponent::Initialize()
			{
				SpatialIndex = { 0,0,0 };
			}

			// 終了化
			void SpatialIndexComponent::Finalize() {}

			//事前更新
			void SpatialIndexComponent::PreUpdate() {}

			// 更新
			void SpatialIndexComponent::Update()
			{
				SpatialIndex = Instance<SpatialDivisionManager>::instance
					.SpatialCurrent(GetOwner()->GetComponent<Transform>()->GetTranslate());
			}

			//事後更新
			void SpatialIndexComponent::NextUpdate() {}

			// 描画
			void SpatialIndexComponent::Draw() {}

			//imgui
			void SpatialIndexComponent::GUI() {}
	}//namespace Math
}//namespace Bread