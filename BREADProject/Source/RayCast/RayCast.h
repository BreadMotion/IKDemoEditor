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
		//レイキャストを行い、結果を持つクラス
		class RayCastCom : public Component
		{
		private://レイキャストの内部データ

			Math::Vector3  start{ Math::Vector3::Zero };           //レイキャストの始点
			Math::Vector3  end  { Math::Vector3::Zero };           //レイキャストの終点

			bool hitFlag{ false };                                 //レイキャストの判定結果
			bool useFlag{ false };                                 //レイキャストの実行

			f32 minDot    { 0.0f   };                              //レイキャストが反応する斜角の最小値

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

		public://constractor
			explicit RayCastCom()
			{
			}
			~RayCastCom()override {}

		public://Component Override

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
				std::string            guiName{ "RayCast : " + GetID() };

				if (ImGui::CollapsingHeader(u8"レイキャスト", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
				{
					char  name[128] = {};
					FND::StrCpy(name, sizeof(name), GetID().c_str());

					ImGui::Text(u8"名前"); ImGui::SameLine();
					ImGui::InputText(("##" + GetID()).c_str(), name, IM_ARRAYSIZE(name));
					SetID(name);

					ImGui::Separator();

					Text(u8"最小角度  : %f", minDot);

					Checkbox("hitFlag", &hitFlag); ImGui::SameLine();
					Checkbox("useFlag", &useFlag);

					Separator();

					DragFloat3("start"       , start              );
					DragFloat3("end"         , end                );
					DragFloat3("hitPos"      , hitResult.position );
					DragFloat3("hitNormal"   , hitResult.normal   );
					DragFloat("toHitDistance", &hitResult.distance);
					DragFloat3("hitStart"    , hitResult.start    );
					DragFloat3("hitEnd"      , hitResult.end      );
				}
			}

		public://RayCast Interface

			//レイキャストの始点を設定
			void __fastcall SetStartPosition(const Math::Vector3& startPos)
			{
				start = startPos;
			}

			//レイキャストの終点を設定
			void __fastcall SetEndPosition(const Math::Vector3& endPos)
			{
				end   = endPos;
			}

			//レイキャストの距離を設定
			void __fastcall SetDistance(const f32 distance)
			{
				hitResult.distance = distance;
			}

			//レイキャストが反応しているか知らせる
			const bool& GetHItFlag()
			{
				return hitFlag;
			}

			//レイキャストが行われているかを知らせる
			const bool& GetUseFlag()
			{
				return useFlag;
			}

		public:
			//Targetのモデルと線（レイ）との当たり判定を行う
			bool IntersectRayVsModel();

		};
	};
}