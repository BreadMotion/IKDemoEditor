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
			//������
			void Initialize() override;

			//���O�X�V
			void PreUpdate(const f32& dt)override {}

			//�X�V
			void Update(const f32& dt) override;

			//����X�V
			void NextUpdate(const f32& dt)override {}

			//�`��
			void Draw(const f32& dt)override;

			//GUI
			void GUI() override
			{
				std::string guiName = "Collision : " + GetID();
				if (ImGui::CollapsingHeader(u8"�R���W�����f�[�^", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
				{
					char  name[128] = {};
					FND::StrCpy(name, sizeof(name), GetID().c_str());
					ImGui::Text(u8"���O"); ImGui::SameLine();
					ImGui::InputText(("##" + GetID()).c_str(), name, IM_ARRAYSIZE(name));
					SetID(name);

					ImGui::Separator();
				}
			}

		public:
			// �����蔻��f�[�^�擾
			const std::vector<CollisionData> GetCollisionDatas()
			{
				return collisions;
			}
		private:
			std::vector<CollisionData> collisions;
		};
	};
}