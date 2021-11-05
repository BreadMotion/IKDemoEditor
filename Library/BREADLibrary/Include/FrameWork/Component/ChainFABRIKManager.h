#pragma once
#include <memory>
#include "Math/BreadMath.h"
#include "FrameWork/Object/Object.h"
#include "FrameWork/Object/BasicObjectElement/INode.h"
#include "FrameWork/Component/Transform.h"

namespace Bread
{
	namespace FrameWork
	{
		namespace SetUp
		{
			//�Z�b�g�A�b�v
			struct IKSetUp
			{
				//�^�[�Q�b�g�W���C���g�̎Q�ƕϐ�
				std::vector<IJoint>* setupJoint{ nullptr };

				//�K�v�R���|�[�l���g�ɃA�N�Z�X����p�ϐ�
				std::shared_ptr<Transform>   rootTransform;

				Math::Vector3* worldTargetPos{ nullptr };
			};
		}

		namespace InverseKinematics
		{
			class [[]] FARBIKManager final
			{
			private://humanIK������ɂ������Ċm�肳�ꂽ��`
				std::vector<std::shared_ptr<SetUp::IKSetUp>> registedIK;

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
				void _fastcall FABRIK(std::shared_ptr<SetUp::IKSetUp> setupIK);

			private: //Shared function
				//FARBIK��p���ċr�̍��W��ڕW�n�_�ɕς��Ă���
				void __fastcall FARBIKParentSolver(std::shared_ptr<SetUp::IKSetUp> setupIK);

				//�q����e�Ɍ������Ă̊e�W���C���g�̃��[���h���W���Z�o
				void __fastcall ForwardCuluculate(std::vector<Math::Vector3>& targetPosition, ITransform*                      pEffector,
					                              ITransform*                 pCurrent,       const std::shared_ptr<Transform> root = nullptr);

				//�e����q�Ɍ������Ă̊e�W���C���g�̃��[���h���W���Z�o
				void __fastcall BackwardCuluculate(std::vector<Math::Vector3>& anserPosition, const Math::Vector3& targetPosition,
					                               ITransform*                 pEffector,     ITransform*          pCurrent,        const std::shared_ptr<Transform> root = nullptr);

				//BackwardCuluculate�ŎZ�o�������W�܂ł̃x�N�g���ɓY���悤�ɃW���C���g����]������
				void __fastcall FABRIKSolver(const Math::Vector3& anserPosition, ITransform*                      pEffector,
					                         ITransform*          pCurrent,      const std::shared_ptr<Transform> root = nullptr);

				//�Ώۂ̐e�̃��[�J�����W���v�Z����
				void __fastcall CulculateParentLocal(const Math::Vector3& basis2EffectDir, const Math::Vector3& basis2TargetDir,
					                                 f32                  rotationAngle,  ITransform*           pCurrent, const std::shared_ptr<Transform> root);

				//�Ώۂ̊p�x���v�Z����
				void __fastcall CulculateAngle(ITransform*          ankle,           ITransform*    hip,
					                           const Math::Vector3& targetPos,       Math::Vector3& basis2EffectDir,
					                           Math::Vector3&       basis2TargetDir, f32&           rotateAngle, const std::shared_ptr<Transform> root);

				//�Ώۂ�Trasnform���X�V����
				void _fastcall UpdateTransform     (ITransform* node);
				void _fastcall UpdateChildTranslate(ITransform* node);

			public://Register, UnRegister Function
				//FootIK Register
				void _fastcall RegisterFABRIK  (std::vector<IJoint>*           model, std::shared_ptr<Transform> rootT, Math::Vector3* targetPos);
				void _fastcall UnRegisterFABRIK(std::shared_ptr<SetUp::IKSetUp> setupIK);

			public://FARBIK GUI
				void GUI();
			};
		}
	}
}