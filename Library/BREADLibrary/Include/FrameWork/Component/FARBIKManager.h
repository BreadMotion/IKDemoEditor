#pragma once
#include <memory>
#include "Math/BreadMath.h"
#include "FrameWork/Object/Object.h"
#include "FrameWork/Component/Transform.h"
#include "../../../BREADProject/Source/RayCast/RayCast.h"

namespace Bread
{
	namespace FrameWork
	{
		namespace FootIK
		{
			//���̔���Ɏg���ϐ�
			const Math::Vector3 rayHeightOffset      { 0.0f, 0.1f , 0.0f };
			const Math::Vector3 footRayHeightOffset  { 0.0f, 50.0f, 0.0f };
			const Math::Vector3 downVec              { 0.0f, -1.0f, 0.0f };
			const Math::Vector3 upVec                { 0.0f,1.0f,0.0f    };

			//���W���C���g�̐ݒ�
			struct LegSetUp
			                             {
				ModelObject::Node* pHip  { nullptr };
				ModelObject::Node* pKnee { nullptr };
				ModelObject::Node* pAnkle{ nullptr };
			};

			//FootIK�̃Z�b�g�A�b�v
			struct FootIKSetUp
			{
				//���Q�{�Q�ƕϐ�
				LegSetUp legSetup[2];

				//�K�v�R���|�[�l���g�ɃA�N�Z�X����p�ϐ�
				std::shared_ptr<RayCastCom>  rayCast[2];
				std::shared_ptr<ModelObject> model;
				std::shared_ptr<Transform>   rootTransform;

				//�W���C���g�̕K�v�f�[�^�̃L���b�V���ϐ�
				Math::Vector3 anklesIniWs   [2];
				Math::Vector3 anklesTargetWs[2];
				Math::Vector3 pelvisOffset;

				//IK�V�X�e���̐ݒ�ϐ� (GUI�ɂē��I�ύX���ł���悤�ɒ萔�ɂ��Ȃ�)
				bool pelbisCorrection{ true };
				bool heightFlag      { true };
				bool twoBoneFlag     { true };
				bool  aimIK          { true };
				f32 rootYaw   { 2.0f  };
				f32 footHeight{ 20.0f };
				f32 weight    { 1.0f  };
				f32 soften    { 1.0f  };
			};

		}

		class [[]] FARBIKManager final
		{
		private://humanIK������ɂ������Ċm�肳�ꂽ��`
			std::vector<std::shared_ptr<FootIK::FootIKSetUp>> registedFootIK;

		public:
			explicit FARBIKManager() {};
			~FARBIKManager() {};

		public:
			//���񏉊���
			void Start();

			//������
			void Initialize();

			//�X�V
			void Update();

		private://FootIK Functions
			//Update�ł�����Ă�
			void _fastcall FootIK(std::shared_ptr<FootIK::FootIKSetUp> footIk);

			//��̖ڕW���W�̍X�V
			void __fastcall UpdateAnklesTarget(std::shared_ptr<FootIK::FootIKSetUp> footIk);

			//���Ղ̖ڕW���W�̍X�V
			void __fastcall UpdatePelvisOffset(std::shared_ptr<FootIK::FootIKSetUp> footIk);

		private://FootIK Suport Function
			const Math::Matrix& _fastcall GetRootTransform(std::shared_ptr<FootIK::FootIKSetUp> footIk);

			//FootIK�p - �q����e�Ɍ������Ă̊p�W���C���g�̃��[���h���W���Z�o
			//��AnkleTargetWorld���Ҋ֐߂���G�Ɍ����Ẵx�N�g���̏�ɂ����ꍇ�̗�O���� (�G���t�����ɋȂ���v�Z�ɂȂ��Ă��܂�����)
			void __fastcall ExceptionCuluculate_HingeJoint(std::vector<Math::Vector3>& targetPosition, std::vector<Math::Vector3>&      anserPosition,
				                                           FootIK::LegSetUp&           footIk,         const std::shared_ptr<Transform> root);


		private: //Shared function
			//CCDIK��p���đ��̍��W��ڕW�n�_�ɕς��Ă���
			void __fastcall CCDIKSolver       (ModelObject::Node* pEffector, const Math::Vector3& faceNormal, const Math::Vector3& hitCoordinate, const std::shared_ptr<Transform> root = nullptr);

			//FARBIK��p���ċr�̍��W��ڕW�n�_�ɕς��Ă���
			void __fastcall FARBIKParentSolver(std::shared_ptr<FootIK::FootIKSetUp> footIk, const u32 iterate);

			//�q����e�Ɍ������Ă̊e�W���C���g�̃��[���h���W���Z�o
			void __fastcall ForwardCuluculate (std::vector<Math::Vector3>& targetPosition, ModelObject::Node*               pEffector,
				                               ModelObject::Node*          pCurrent,       const std::shared_ptr<Transform> root = nullptr);

			//�e����q�Ɍ������Ă̊e�W���C���g�̃��[���h���W���Z�o
			void __fastcall BackwardCuluculate(std::vector<Math::Vector3>& anserPosition, const Math::Vector3& targetPosition,
				                               ModelObject::Node*          pEffector,     ModelObject::Node*    pCurrent, const std::shared_ptr<Transform> root = nullptr);

			//BackwardCuluculate�ŎZ�o�������W�܂ł̃x�N�g���ɓY���悤�ɃW���C���g����]������
			void __fastcall IKSolver(const Math::Vector3& anserPosition, ModelObject::Node*   pEffector,
				                     ModelObject::Node*   pCurrent, const std::shared_ptr<Transform> root = nullptr);

			//BackwardCuluculate�ŎZ�o�������W�܂ł̃x�N�g���ɓY���悤�ɃW���C���g����]������
			void __fastcall IKSolver2(std::vector<Math::Vector3> anserPosition, std::shared_ptr<FootIK::FootIKSetUp> footIk, const u32 iterate);


			//�Ώۂ̐e�̃��[�J�����W���v�Z����
			void __fastcall CulculateParentLocal(const Math::Vector3& basis2EffectDir, const Math::Vector3& basis2TargetDir,
				                                 f32                  rotationAngle,   ModelObject::Node*   pCurrent,          const std::shared_ptr<Transform> root);

			//�Ώۂ̊p�x���v�Z����
			void __fastcall CulculateAngle(ModelObject::Node*   ankle,           ModelObject::Node* hip,
				                           const Math::Vector3& targetPos,       Math::Vector3&     basis2EffectDir,
				                           Math::Vector3&       basis2TargetDir, f32&               rotateAngle,       const std::shared_ptr<Transform> root);

			//�Ώۂ�Trasnform���X�V����
			void _fastcall UpdateTransform     (ModelObject::Node* node);
			void _fastcall UpdateChildTranslate(ModelObject::Node* node);

		public://Register, UnRegister Function
			//FootIK Register
			void _fastcall RegisterFootIK  (std::shared_ptr<ModelObject> model, std::shared_ptr<Transform> rootT, const std::shared_ptr<RayCastCom> rayCast[2]);
			void _fastcall UnRegisterFootIK(std::shared_ptr<FootIK::FootIKSetUp> footIk);

		public://FARBIK GUI
			void GUI();
		};
	}
}