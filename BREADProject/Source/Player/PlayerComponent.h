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

		// �A�j���[�V�������C���[
		enum class LayerType
		{
			Base,
			LowerBody
		};

		// �X�e�[�g���
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
		// ���C���[���̃X�e�[�g�ԍ�
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
			//������
			void Initialize()override;

			//���O�X�V
			void __fastcall PreUpdate()override;

			//�X�V
			void __fastcall Update()override;

			//����X�V
			void __fastcall NextUpdate()override;

			//GUI
			void GUI()override;

		public:
			// �����蔻��f�[�^�擾
			const std::vector<CollisionData> GetCollisionDatas()
			{
				return collisions;
			}

		private:
			//��������Component�̍\�z
			void ComponentConstruction();

			//�e�R���|�[�l���g�̍\�z�֐�
			void ModelObjectConstruction();
			void TransformConstruction();
			void VelocityMapConstruction();
			void CollisionConstruction();
			void RayCastConstruction();
			void GeometricConstruction();

		private:
			//����
			void Controll();

			//�A�j���[�V�����̕ύX
			void ChangeAnimation();

			//�A�j���[�V�����X�e�[�g�ύX
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

		private://�q�A�N�^�[�̃f�[�^
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