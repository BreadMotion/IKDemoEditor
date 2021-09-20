#pragma once
#include <algorithm>
#include <functional>

#include "Component.h"
#include "Math/BREADMath.h"

#include "../../ExternalLibrary/ImGui/Include/imgui.h"

#include "FND/Util.h"
#include "FND/STD.h"

namespace Bread
{
	namespace FrameWork
	{
		class VelocityMap : public Component
		{
		private:
			f32 mass = 0.1f;
			Math::Vector3 position     = Math::Vector3::Zero;
			Math::Vector3 velocity     = Math::Vector3::Zero;

			Math::Vector3 acceleration = Math::Vector3::Zero;
			Math::Vector3 resultant    = Math::Vector3::Zero;

			Math::Vector3 gravity      = Math::Vector3::Zero;

			bool onGravity     = false;

			const f32 minSpeed = 0.2f;
			const f32 maxSpeed = 2.0f;
			const f32 maxGravity = -10.0f;

		public:
			explicit  VelocityMap() {}
			~VelocityMap() override {}


		public:
			// 初期化
			void Initialize() override
			{
				SetID(GetOwner()->GetID());

				position  = Math::Vector3::Zero;
				velocity  = Math::Vector3::Zero;

				acceleration   = Math::Vector3::Zero;
				resultant      = Math::Vector3::Zero;

				gravity      = { 0.0f, -0.1f, 0.0f };
				onGravity = false;
			}

			//事前更新
			void __fastcall PreUpdate(const f32& dt)override {}

			// 更新
			void __fastcall Update(const f32& dt) override
			{
				using namespace Math;
				if (onGravity)
				{
					AddForce(mass * gravity);
				}
				else
				{
					SetVelocity(Vector3(velocity.x, 0.0f, velocity.z));
				}
				Integrate(dt);
			}

			//事後更新
			void __fastcall NextUpdate(const f32& dt)override {}

			// GUI
			void GUI()override
			{
				using namespace ImGui;
				std::string guiName = "VelocityMap : " + GetID();
				if (ImGui::CollapsingHeader(u8"速度マッピング", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
				{
					char  name[128] = {};
					FND::StrCpy(name, sizeof(name), GetID().c_str());
					ImGui::Text(u8"名前"); ImGui::SameLine();
					ImGui::InputText(("##" + GetID()).c_str(), name, IM_ARRAYSIZE(name));
					SetID(name);

					ImGui::Separator();

					DragFloat3("position",                      &position.x);
					RegisterWatchVal("position -" + GetID(), &position);
					DragFloat3("velocity",                      &velocity.x);
				    RegisterWatchVal("velocity -" + GetID(), &velocity);

					DragFloat3("acceleration",                      &acceleration.x);
					RegisterWatchVal("acceleration -" + GetID(), &acceleration);
					DragFloat3("resultant",                           &resultant.x);
					RegisterWatchVal("resultant -" + GetID(),       &resultant);

					DragFloat("friction",                         &mass);
				}
				Checkbox("onGravity", &onGravity);
			}

		public:
			//データを統合する
			void __fastcall Integrate(const f32& dt)
			{
				if (mass < 0.0f)return;

				acceleration = (resultant / mass);
				velocity     += acceleration;
				position    += velocity * (dt);

				MinSpeedReset();
				ResetResultant();
			}

			//外力を追加する
			void __fastcall AddForce(const Math::Vector3 &force)
			{
				resultant += force;
			}

			//衝突計算
			void __fastcall Collide(const Math::Vector3& normal, f32 restitution, f32 penetration)
			{
				if (std::fabsf(Math::Vector3Length(normal) - 1.0f) < 1e-3f) return;

				f32 v = Math::Vector3Dot(velocity, normal);
				if (v < 0)
				{
					velocity += -(restitution + 1.0f) * v * normal;
				}
				position += penetration * normal;
			}

		public:
			void ResetResultant()
			{
				using namespace Math;
				resultant = Vector3::Zero;
			}

			void MinSpeedReset()
			{
				const f32 noSpeed    = 0.0f;

				Math::Vector2 moveVel             = Math::Vector2(velocity.x, velocity.z);
 				Math::Vector2 moveVelNormal = Math::Vector2Normalize(moveVel);
				f32 length       = Math::Vector2Length(moveVel);
				length             = (length > minSpeed) ? length : noSpeed;
				f32 result         = (length < maxSpeed) ? length : maxSpeed;

				moveVel = moveVelNormal * result;
				velocity.x = moveVel.x;
				velocity.z = moveVel.y;
				velocity.y = (velocity.y > maxGravity) ? velocity.y : maxGravity;
			}

		public:
			//座標を取得する
			const Math::Vector3& const GetPosition()const
			{
				return position;
			}

			//座標を追加する
			void __fastcall AddPosition(const Math::Vector3& pos)
			{
				position += pos;
			}

			//座標を設定する
			void __fastcall SetPosition(const Math::Vector3& pos)
			{
				position = pos;
			}

			//速度を取得する
			const Math::Vector3& const GetVelocity()const
			{
				return velocity;
			}

			//速度を追加する
			void __fastcall AddVelocity(const Math::Vector3& vel)
			{
				velocity += vel;
			}

			//速度を設定する
			void __fastcall SetVelocity(const Math::Vector3& vel)
			{
				velocity = vel;
			}

			//グラビてぃの設定
			void _fastcall SetGrabityflag(const bool& flag)
			{
				onGravity = flag;
			}

			//加速度を取得する
			const Math::Vector3& GetAcceleration()const
			{
				return acceleration;
			}

			//質量を設定する
			void __fastcall SetMass(const f32& m)
			{
				mass = m;
			}

			const f32& GetMaxSpeed() const
			{
				return maxSpeed;
			}
		};
	}
}