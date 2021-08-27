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
			Graphics::IGraphicsDevice* graphicsDevice = nullptr;

			Graphics::Camera* cameraAct            = nullptr;
			ModelObject*      stageModel           = nullptr;
			IKTargetActor*    leftFootTargetActor  = nullptr;
			IKTargetActor*    rightFootTargetActor = nullptr;
			float*            objMatrix            = nullptr;

		public:
			//����
			static std::shared_ptr<Actor> Create(Graphics::IGraphicsDevice* graphicDevice, Graphics::Camera* cam, ModelObject* stage);

			PlayerActor(Graphics::IGraphicsDevice* graphicDevice, Graphics::Camera* cam,ModelObject* stage)
			{
				graphicsDevice = graphicDevice;
				cameraAct        = cam;
				stageModel      = stage;
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