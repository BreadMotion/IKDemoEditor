#pragma once
#include <string>
#include <vector>
#include "FrameWork/Component/Component.h"
#include "Graphics/GraphicsDevice.h"
#include "Types.h"

#include "Graphics/Camera.h"
#include "FrameWork/Input/InputDevice.h"
#include "FrameWork/Object/Object.h"
#include "FrameWork/Component/Transform.h"
#include "FrameWork/Component/VelocityMap.h"
#include "FrameWork/Component/CCDIK.h"

#include "../IKTarget/IKTargetCom.h"
#include "../Primitive/GeometricPrimitive.h"
#include "../Collision/Collision.h"
#include "../RayCast/RayCast.h"

namespace Bread
{
	namespace Player
	{
		enum class AnimationState
		{
			Idle,
			Walk,
			Run,
			SlowRun,
			Roll,
			Attack,
			Damage,
			Dedge,
			Guard,
			Death,
		};

		// アニメーションレイヤー
		enum class LayerType
		{
			Base,
			LowerBody,
		};

		// ステート種類
		enum class StateType
		{
			Idle,
			BattleIdle,
			DamageSmall,
			DamageBig,
			ForwardDedge,
			BackDedge,
			RightDedge,
			LeftDedge,
			Death,
		};
		// レイヤー内のステート番号

	}

	namespace FrameWork
	{
		class PlayerActor : public Actor
		{
		private:
			Graphics::IGraphicsDevice* graphicsDevice = nullptr;

			Graphics::Camera* cameraAct            = nullptr;
			ModelObject*      stageModel           = nullptr;
			IKTargetActor*    leftFootTargetActor  = nullptr;
			IKTargetActor*    rightFootTargetActor = nullptr;
			float*            objMatrix            = nullptr;

		public:
			//生成
			static std::shared_ptr<Actor> Create(Graphics::IGraphicsDevice* graphicDevice, Graphics::Camera* cam, ModelObject* stage);

			PlayerActor(Graphics::IGraphicsDevice* graphicDevice, Graphics::Camera* cam,ModelObject* stage)
			{
				graphicsDevice = graphicDevice;
				cameraAct        = cam;
				stageModel      = stage;
			}
			~PlayerActor()override {}

		public:
			//初期化
			void Initialize() override;

			//事前更新
			void PreUpdate(const f32& dt) override;

			//更新
			void Update(const f32& dt) override;

			//事後更新
			void NextUpdate(const f32& dt) override;

			//描画
			void Draw(const f32& dt) override;

		public:
			//操作
			void Controll(const f32& dt);

			//アニメーション変更
			void ChangeAnimation();

			//アニメーションステート変更
			void ChangeAnimationState(const Bread::Player::AnimationState& state, const f32& moveSpeed);

		public:
			void SetObjMatrix(float* m)
			{
				objMatrix = m;
			}

			// 当たり判定データ取得
			const std::vector<CollisionData> GetCollisionDatas()
			{
				return collisions;
			}

			Graphics::IGraphicsDevice* GetGraphicsDevice()
			{
				return graphicsDevice;
			}

		private:
			bool         isChangeAnimation = false;
			Bread::f32   animationSpeed    = 0.0f;

			Bread::Player::AnimationState                  animationState = Player::AnimationState::Idle;
			std::map<Bread::Player::LayerType, Bread::s32> layerIndexList;
			std::map<Bread::Player::StateType, Bread::s32> stateIndexList;

			std::vector<CollisionData> collisions;

			ModelObject*                    playerModel        = nullptr;
			VelocityMap*                    velmap             = nullptr;
			Transform*                      transform          = nullptr;
			CyclicCoordinateDescent*        ccdik              = nullptr;
			GeometricPrimitive*             primitive          = nullptr;
			CollisionCom*                   collision          = nullptr;
			RayCastCom*                     rayCast            = nullptr;

			const float hipsHeight = 10.0f;
			std::vector<Bread::u32> targetFaceIndex;
		};
	};
}