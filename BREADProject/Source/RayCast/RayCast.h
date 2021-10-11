#pragma once
#include <string>
#include "FrameWork/Component/Component.h"
#include "Types.h"

#include "Graphics/GraphicsDevice.h"

#include "FrameWork/Object/Object.h"
#include "FrameWork/Component/Transform.h"

namespace Bread
{
	namespace FrameWork
	{
		class RayCastCom : public Component
		{
		private:
			std::shared_ptr<ModelObject>   targetTarrain{ nullptr };
			Math::Vector3  start;
			Math::Vector3  end;

			bool hitFlag = false;

			bool useFlag = false;

			f32 VariableLengthSearch = 100.0f;
			f32 minDot               = 0.0f;
			std::vector<ModelObject::Face::VertexIndex> targetFace;

		public:
			// ヒット結果（レイキャストで取り出す情報）
			struct HitResult
			{
				Math::Vector3   position = { 0,0,0 }; // レイとポリゴンの交点
				Math::Vector3   normal   = { 0,0,0 }; // 衝突したポリゴンの法線ベクトル
				f32             distance = 0.0f;      // レイの始点から交点までの距離
				Math::Vector3   start    = { 0,0,0 }; // 衝突したレイの始点位置 ※MultiRayCastのみ
				Math::Vector3   end      = { 0,0,0 }; // 衝突したレイの終点位置 ※MultiRayCastのみ
			};
			HitResult hitResult;

			//std::vector<ModelObject::> targetIndex;

		public:
			explicit RayCastCom(std::shared_ptr<ModelObject> terrain)
			{
				targetTarrain  = terrain;
			}
			~RayCastCom()override {}

		public:
			//初期化
			void Initialize() override;

			//事前更新
			void __fastcall PreUpdate()override {}

			//更新
			void __fastcall Update() override;

			//事後更新
			void __fastcall NextUpdate()override {}

			//描画
			void __fastcall Draw()override;

			//GUI
			void GUI() override
			{
				using namespace ImGui;
				std::string guiName = "RayCast : " + GetID();
				std::shared_ptr<Actor> terain = targetTarrain->GetOwner();

				if (ImGui::CollapsingHeader(u8"レイキャスト", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
				{
					char  name[128] = {};
					FND::StrCpy(name, sizeof(name), GetID().c_str());
					ImGui::Text(u8"名前"); ImGui::SameLine();
					ImGui::InputText(("##" + GetID()).c_str(), name, IM_ARRAYSIZE(name));
					SetID(name);

					ImGui::Separator();

					Text("targetStage : %s", targetTarrain->GetOwner()->GetID().c_str());
					Text(u8"最小角度 : %f", minDot);
					DragFloat("VariableLengthSearch", &VariableLengthSearch);

					Checkbox("hitFlag", &hitFlag); ImGui::SameLine();
					Checkbox("useFlag", &useFlag);

					Separator();
					DragFloat3("start", start);
					//RegisterWatchVal(("start" + terain->GetComponent<Transform>()->GetID()), &start);

					DragFloat3("end", end);
					//RegisterWatchVal(("end" + terain->GetComponent<Transform>()->GetID()), &end);

					DragFloat3("hitPos", hitResult.position);
					//RegisterWatchVal(("hitPos" + terain->GetComponent<Transform>()->GetID()), &hitResult.position);

					DragFloat3("hitNormal", hitResult.normal);
					//RegisterWatchVal(("hitNormal" + terain->GetComponent<Transform>()->GetID()), &hitResult.normal);

					DragFloat("toHitDistance", &hitResult.distance);

					DragFloat3("hitStart", hitResult.start);
					//RegisterWatchVal(("hitStart" + terain->GetComponent<Transform>()->GetID()), &hitResult.start);

					DragFloat3("hitEnd", hitResult.end);
					//RegisterWatchVal(("hitEnd" + terain->GetComponent<Transform>()->GetID()), &hitResult.end);
				}
			}

		public:
			void __fastcall SetStartPosition(const Math::Vector3& startPos)
			{
				start = startPos;
			}

			void __fastcall SetEndPosition(const Math::Vector3& endPos)
			{
				end = endPos;
			}

			void __fastcall SetDistance(const f32 distance)
			{
				hitResult.distance = distance;
			}

			const bool& GetHItFlag()
			{
				return hitFlag;
			}

			const bool& GetUseFlag()
			{
				return useFlag;
			}

			void __fastcall SetTargetFaceIndex(const std::vector<ModelObject::Face::VertexIndex>& targetFace)
			{
				this->targetFace = targetFace;
			}

		public:
			//rayCast
			bool IntersectRayVsModel();

		};
	};
}