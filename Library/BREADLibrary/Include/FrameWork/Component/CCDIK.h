#pragma once
#include "FrameWork/Component/Component.h"
#include "FrameWork/Object/Object.h"
#include "Math/BreadMath.h"
#include <vector>

enum PART
{
	ARM,
	FOOT,
	TOE,
	HEAD,
	NONE
};

class CyclicCoordinateDescent : public Bread::FrameWork::Component
{
public:
	struct ComputePart
	{
		const Bread::Math::Matrix* targetWorldTransform    = nullptr;
		Bread::FrameWork::ModelObject::Node* beginJoint    = nullptr;
		Bread::FrameWork::ModelObject::Node* endJoint      = nullptr;
		Bread::Math::Vector3                  targetPos      = Bread::Math::Vector3::Zero;
		Bread::Math::Vector3                  targetIKNormal = Bread::Math::Vector3::Zero;
		Bread::Math::Vector3                  rayStart       = Bread::Math::Vector3::Zero;
		Bread::Math::Vector3                  kDown          = Bread::Math::Vector3::Zero;
		Bread::f32                                                 ankleHeight      = 0.0f;
		Bread::s32                                                iterate                = 0;
		PART part;
		bool                                                         computeCCDIK = false;

		ComputePart(
			const Bread::Math::Matrix*           transform,
			Bread::FrameWork::ModelObject::Node* begin,
			Bread::FrameWork::ModelObject::Node* end,
			Bread::Math::Vector3                 target,
			Bread::Math::Vector3                 targetIKPos,
			Bread::s32                           it,
			Bread::f32                           ankle,
			PART                                 par) // TODO : ���O
		{
			targetWorldTransform = transform;
			beginJoint           = begin;
			endJoint             = end;
			targetPos            = target;
			targetIKNormal       = targetIKPos;
			iterate              = it;
			ankleHeight          = ankle;
			part                 = par;
		};

		//�G�t�F�N�^�̖ڕW�n�_�̐ݒ�
		void setTargetPos(const Bread::Math::Vector3& targetPos)       { this->targetPos = targetPos; }

		//�ڕW�n�_�̖@������ݒ�
		void setTargetIKNormal(const Bread::Math::Vector3& targetIKNormal) { this->targetIKNormal = targetIKNormal; };

		//���C�̎n�_���̐ݒ�
		void setRayStart(const Bread::Math::Vector3& v) { rayStart = v; }

		//���Ղ̉��x�N�g�����̐ݒ�
		void setKDown(const Bread::Math::Vector3& v)    { kDown = v; }
	};

public:
	explicit CyclicCoordinateDescent() { ID = GetOwner()->GetID(); }
	~CyclicCoordinateDescent() override {}

public:
	// �R���X�g���N�^
	void Construct()  override;

	// �f�X�g���N�^
	void Destruct()   override;

	// ������
	void Initialize() override;

	// �I����
	void Finalize()   override;

	// �X�V
	void Update()     override;

	//imgui
	void GUI()        override;

	//�����X�V
	void PartUpdate(const Bread::u32& index);

public:
	//���g��Transform�s��X�V��q�W���C���g�Ɍ����čċN����
	void updateAllPosition(Bread::FrameWork::ModelObject::Node& joint, const Bread::Math::Matrix* targetWorldTransform );
	void UpdatePosition(Bread::FrameWork::ModelObject::Node& joint, const Bread::Math::Vector3& euler, const Bread::Math::Matrix* targetWorldTransform );

	//�p�x����
	bool JointAngleLimit(Bread::Math::Vector3& euler, Bread::FrameWork::ModelObject::Node& joint, Bread::Math::Vector3& parentEuler);
	bool ZMinusJointAngleLimit(Bread::Math::Vector3& euler, Bread::FrameWork::ModelObject::Node& joint, Bread::Math::Vector3& parentEuler);

	//��]�l�����߂�
	float LocalRotateAngle(const Bread::Math::Vector3& jointToTarget, const Bread::Math::Vector3& jointToEffector);

	//��]�����O�ςŋ��߂�
	Bread::Math::Vector3 CreateLocalRotationAxis(const Bread::Math::Vector3& toEffector, const Bread::Math::Vector3& toTarget);

public:
	//����IK
	void FootCCDIK(
		Bread::FrameWork::ModelObject::Node* beginJoint,
		Bread::FrameWork::ModelObject::Node* endJoint,
		const Bread::Math::Vector3& targetPos,
		const Bread::Math::Vector3& normalVector,
		const Bread::Math::Vector3& rayStart,
		const Bread::Math::Vector3& kDown,
		const Bread::Math::Matrix* targetWorldTransform,
		const float                ankleHeight,
		int                        iteratNum);

public:
	//IK�������s��������ǉ�����
	void AddOrder(
		const Bread::Math::Matrix*           transform,
		Bread::FrameWork::ModelObject::Node* begin,
		Bread::FrameWork::ModelObject::Node* end,
		Bread::Math::Vector3                 target,
		const Bread::Math::Vector3           normalVector,
		const Bread::s32                     it,
		const Bread::f32                     ankle,
		const PART                           par)
	{
		order.emplace_back(
			std::make_shared<ComputePart>(transform, begin, end, target, normalVector,it, ankle, par));
	}
public:
	std::vector<std::shared_ptr<ComputePart>>             order;
};