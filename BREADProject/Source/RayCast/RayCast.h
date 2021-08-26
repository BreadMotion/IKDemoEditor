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
			ModelObject*                     targetTarrain     = nullptr;
			Graphics::IGraphicsDevice* graphicsDevice = nullptr;
			Math::Vector3  start;
			Math::Vector3  end;

			bool                 hitFlag = false;

			bool               useFlag = false;

			f32 VariableLengthSearch = 100.0f;
			f32 minDot                        = 0.0f;
			std::vector<Bread::u32>* targetFaceIndex = nullptr;

		public:
			// ヒット結果（レイキャストで取り出す情報）
			struct HitResult
			{
				Math::Vector3   position        = { 0,0,0 }; // レイとポリゴンの交点
				Math::Vector3   normal          = { 0,0,0 };   // 衝突したポリゴンの法線ベクトル
				f32                    distance         = 0.0f;      // レイの始点から交点までの距離
				Math::Vector3   start               = { 0,0,0 };    // 衝突したレイの始点位置 ※MultiRayCastのみ
				Math::Vector3   end                = { 0,0,0 };      // 衝突したレイの終点位置 ※MultiRayCastのみ
			};
			HitResult hitResult;

			//std::vector<ModelObject::> targetIndex;

		public:
			explicit RayCastCom(Graphics::IGraphicsDevice* graphicDevice,ModelObject* terrain)
			{
				graphicsDevice = graphicDevice;
				targetTarrain  = terrain;
			}
			~RayCastCom()override {}

		public:
			//初期化
			void Initialize() override;

			//更新
			void Update(const f32& dt) override;

			//描画
			void Draw(const f32& dt)override;

			//GUI
			void GUI() override
			{
				using namespace ImGui;
				std::string guiName = "RayCast : " + ID;
				std::shared_ptr<Actor> terain = targetTarrain->GetOwner();

				if (ImGui::CollapsingHeader(u8"レイキャスト", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
				{
					char  name[128] = {};
					FND::StrCpy(name, sizeof(name), GetID().c_str());
					ImGui::Text(u8"名前"); ImGui::SameLine();
					ImGui::InputText(("##" + ID).c_str(), name, IM_ARRAYSIZE(name));
					SetID(name);

					ImGui::Separator();

					Text("targetStage : %s", targetTarrain->GetOwner()->GetID().c_str());
					Text(u8"最小角度 : %f", minDot);
					DragFloat("VariableLengthSearch", &VariableLengthSearch);
					Text("targetFaceindex : %d", targetFaceIndex->size());

					Checkbox("hitFlag", &hitFlag); ImGui::SameLine();
					Checkbox("useFlag", &useFlag);

					Separator();
					DragFloat3("start", start);
					RegisterWatchVal(("start" + terain->GetComponent<Transform>()->ID), &start);

					DragFloat3("end", end);
					RegisterWatchVal(("end" + terain->GetComponent<Transform>()->ID), &end);

					DragFloat3("hitPos",              hitResult.position);
					RegisterWatchVal(("hitPos" + terain->GetComponent<Transform>()->ID), &hitResult.position);

					DragFloat3("hitNormal",       hitResult.normal);
					RegisterWatchVal(("hitNormal" + terain->GetComponent<Transform>()->ID), &hitResult.normal);

					DragFloat("toHitDistance", &hitResult.distance);

					DragFloat3("hitStart",            hitResult.start);
					RegisterWatchVal(("hitStart" + terain->GetComponent<Transform>()->ID), &hitResult.start);

					DragFloat3("hitEnd",             hitResult.end);
					RegisterWatchVal(("hitEnd" + terain->GetComponent<Transform>()->ID), &hitResult.end);
				}
			}

		public:
			void SetStartPosition(const Math::Vector3& startPos)
			{
				start = startPos;
			}

			void SetEndPosition(const Math::Vector3& endPos)
			{
				end = endPos;
			}

			void SetDistance(const f32 distance)
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

			void SetTargetFaceIndex(std::vector<Bread::u32>& targetFaceIndex)
			{
				this->targetFaceIndex = &targetFaceIndex;
			}

		public:
			//rayCast
			bool IntersectRayVsModel();

			//判定を行うfaceの登録を行う
			void TargetFaceRegister();
		};
	};
}