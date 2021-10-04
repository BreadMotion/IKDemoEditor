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

			// 初期化
			void SpatialIndexComponent::Initialize() {}

			// 終了化
			void SpatialIndexComponent::Finalize() {}

			//事前更新
			void SpatialIndexComponent::PreUpdate(const f32&) {}

			// 更新
			void SpatialIndexComponent::Update(const f32&)
			{
				SpatialIndex = Instance<SpatialDivisionManager>::instance.SpatialCurrent(position);
			}

			//事後更新
			void SpatialIndexComponent::NextUpdate(const f32&) {}

			// 描画
			void SpatialIndexComponent::Draw(const f32&) {}

			//imgui
			void SpatialIndexComponent::GUI() {}
	}//namespace Math
}//namespace Bread