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

		// �A�j���[�V�������C���[
		enum class LayerType
		{
			Base,
			LowerBody,
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
			Death,
		};
		// ���C���[���̃X�e�[�g�ԍ�

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
			//����
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
			//������
			void Initialize() override;

			//���O�X�V
			void PreUpdate(const f32& dt) override;

			//�X�V
			void Update(const f32& dt) override;

			//����X�V
			void NextUpdate(const f32& dt) override;

			//�`��
			void Draw(const f32& dt) override;

		public:
			//����
			void Controll(const f32& dt);

			//�A�j���[�V�����ύX
			void ChangeAnimation();

			//�A�j���[�V�����X�e�[�g�ύX
			void ChangeAnimationState(const Bread::Player::AnimationState& state, const f32& moveSpeed);

		public:
			void SetObjMatrix(float* m)
			{
				objMatrix = m;
			}

			// �����蔻��f�[�^�擾
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