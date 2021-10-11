#pragma once
#include <vector>
#include "FrameWork/Component/Component.h"

#include "Math/BreadMath.h"

#include "FND/Util.h"
#include "FND/STD.h"

#include "FrameWork/Object/Object.h"
#include "FrameWork/Component/Transform.h"
#include "FrameWork/Component/VelocityMap.h"
#include "FrameWork/Component/CCDIK.h"
#include "FrameWork/Component/IKManager.h"
#include "FrameWork/Component/SpatialIndex.h"

#include "../IKTarget/IKTargetComponent.h"
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
			Death
		};

		// アニメーションレイヤー
		enum class LayerType
		{
			Base,
			LowerBody
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
			Death
		};
		// レイヤー内のステート番号
	}

	namespace FrameWork
	{
		class PlayerComponent : public Component
		{
		private:

		public:
			explicit PlayerComponent();
			~PlayerComponent()override;

		public:
			//初期化
			void Initialize()override;

			//事前更新
			void __fastcall PreUpdate()override;

			//更新
			void __fastcall Update()override;

			//事後更新
			void __fastcall NextUpdate()override;

			//GUI
			void GUI()override;

		public:
			// 当たり判定データ取得
			const std::vector<CollisionData> GetCollisionDatas()
			{
				return collisions;
			}

		private:
			//生成したComponentの構築
			void ComponentConstruction();

			//各コンポーネントの構築関数
			void ModelObjectConstruction();
			void TransformConstruction();
			void VelocityMapConstruction();
			void CollisionConstruction();
			void RayCastConstruction();
			void GeometricConstruction();

		private:
			//操作
			void Controll();

			//アニメーションの変更
			void ChangeAnimation();

			//アニメーションステート変更
			void ChangeAnimationState(const Bread::Player::AnimationState& state, const f32& moveSpeed);

		private:
			bool  isChangeAnimation{ false };
			f32   animationSpeed{ 0.0f };

			Player::AnimationState animationState{ Player::AnimationState::Idle };
			std::map<Bread::Player::LayerType, Bread::s32> layerIndexList;
			std::map<Bread::Player::StateType, Bread::s32> stateIndexList;

			std::vector<CollisionData> collisions;

		private:
			std::shared_ptr<Transform>          transform{ nullptr };
			std::shared_ptr<RayCastCom>         rayCast{ nullptr };
			std::shared_ptr<ModelObject>        model{ nullptr };
			std::shared_ptr<VelocityMap>        velMap{ nullptr };
			std::shared_ptr<CollisionCom>       collision{ nullptr };
			std::shared_ptr<GeometricPrimitive> geometric{ nullptr };

		private://子アクターのデータ
			std::shared_ptr<Actor> leftIKTarget{ nullptr };
			std::shared_ptr<Actor> rightIKTarget{ nullptr };

			std::shared_ptr<IKTargetComponent> leftIKTargetComponent{ nullptr };
			std::shared_ptr<IKTargetComponent> rightIKTargetComponent{ nullptr };

			std::shared_ptr<Transform>  leftIKTargetTransform{ nullptr };
			std::shared_ptr<RayCastCom> leftIKTargetRayCast{ nullptr };
			std::shared_ptr<Transform>  rightIKTargetTransform{ nullptr };
			std::shared_ptr<RayCastCom> rightIKTargetRayCast{ nullptr };
		};
	}
}