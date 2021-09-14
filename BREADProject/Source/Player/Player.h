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
			std::weak_ptr<Graphics::IGraphicsDevice> graphicsDevice;

			std::weak_ptr<Graphics::Camera> cameraAct;
			std::weak_ptr<ModelObject>      stageModel;
			std::weak_ptr<IKTargetActor>    leftFootTargetActor;
			std::weak_ptr<IKTargetActor>    rightFootTargetActor;
			float* objMatrix = nullptr;

		public:
			//生成
			static std::shared_ptr<Actor> Create(std::shared_ptr<Graphics::IGraphicsDevice> graphicDevice, std::shared_ptr<Graphics::Camera> cam, std::shared_ptr<ModelObject> stage)
			{
				return std::make_shared<PlayerActor>(graphicDevice, cam, stage);
			}

			PlayerActor(std::shared_ptr<Graphics::IGraphicsDevice> graphicDevice, std::shared_ptr<Graphics::Camera> cam, std::shared_ptr<ModelObject> stage)
			{
				graphicsDevice = graphicDevice;
				cameraAct      = cam;
				stageModel     = stage;
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

		private:
			bool         isChangeAnimation = false;
			Bread::f32   animationSpeed    = 0.0f;

			Bread::Player::AnimationState                  animationState = Player::AnimationState::Idle;
			std::map<Bread::Player::LayerType, Bread::s32> layerIndexList;
			std::map<Bread::Player::StateType, Bread::s32> stateIndexList;

			std::vector<CollisionData> collisions;

			std::weak_ptr<ModelObject>             playerModel;
			std::weak_ptr<VelocityMap>             velmap;
			std::weak_ptr<Transform>               transform;
			std::weak_ptr<CyclicCoordinateDescent> ccdik;
			std::weak_ptr<GeometricPrimitive>      primitive;
			std::weak_ptr<CollisionCom>            collision;
			std::weak_ptr<RayCastCom>              rayCast;

			const float hipsHeight = 10.0f;
			std::vector<Bread::u32> targetFaceIndex;
		};
	};
}