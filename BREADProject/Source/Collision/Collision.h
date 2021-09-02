#pragma once
#include <string>
#include "FrameWork/Component/Component.h"
#include "Graphics/GraphicsDevice.h"
#include "Types.h"

#include "FrameWork/Object/Object.h"
#include "Graphics/Camera.h"
#include "FrameWork/Component/Transform.h"

#include "FND/STD.h"
#include "FND/Util.h"

namespace Bread
{
	namespace FrameWork
	{
		class CollisionCom : public Component
		{
		private:
			Graphics::IGraphicsDevice* graphicsDevice = nullptr;
			std::weak_ptr<Transform>   targetT;

		public:
			explicit CollisionCom(Graphics::IGraphicsDevice* graphicDevice)
			{
				graphicsDevice = graphicDevice;
			}
			~CollisionCom()override {}

		public:
			//初期化
			void Initialize() override;

			//事前更新
			void PreUpdate(const f32& dt)override {}

			//更新
			void Update(const f32& dt) override;

			//事後更新
			void NextUpdate(const f32& dt)override {}

			//描画
			void Draw(const f32& dt)override;

			//GUI
			void GUI() override
			{
				std::string guiName = "Collision : " + GetID();
				if (ImGui::CollapsingHeader(u8"コリジョンデータ", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
				{
					char  name[128] = {};
					FND::StrCpy(name, sizeof(name), GetID().c_str());
					ImGui::Text(u8"名前"); ImGui::SameLine();
					ImGui::InputText(("##" + GetID()).c_str(), name, IM_ARRAYSIZE(name));
					SetID(name);

					ImGui::Separator();
				}
			}

		public:
			// 当たり判定データ取得
			const std::vector<CollisionData> GetCollisionDatas()
			{
				return collisions;
			}
		private:
			std::vector<CollisionData> collisions;
		};
	};
}