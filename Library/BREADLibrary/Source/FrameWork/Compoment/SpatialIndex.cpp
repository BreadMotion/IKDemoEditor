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
			void SpatialIndexComponent::Initialize()
			{
				spatialIndex = { 0,0,0 };
				transform    = GetOwner()->GetComponent<Transform>();
			}

			// 終了化
			void SpatialIndexComponent::Finalize() {}

			//事前更新
			void SpatialIndexComponent::PreUpdate() {}

			// 更新
			void SpatialIndexComponent::Update()
			{
				//空間分割マネージャーとこのコンポーネントの所有者であるアクターのTransformコンポーネントを使って
				//どの空間にアクターが存在するのかを更新し保持する
				spatialIndex = Instance<SpatialDivisionManager>::instance
					.SpatialCurrent(Math::GetLocation(transform->GetWorldTransform()));
			}

			//事後更新
			void SpatialIndexComponent::NextUpdate() {}

			// 描画
			void SpatialIndexComponent::Draw(){}

			//imgui
			void SpatialIndexComponent::GUI()
			{
				if (ImGui::CollapsingHeader(u8"空間インデックス", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
				{
					ImGui::DragInt3(u8"空間番号", &spatialIndex.x);
				}
			}
	}//namespace Math
}//namespace Bread