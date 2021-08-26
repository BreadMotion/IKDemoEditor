#include "FrameWork/Component/CCDIK.h"
#include "./../../ImGui/Include/imgui.h"
#include <algorithm>

/// <summary>
/// �X�J���[�l���͈͓������肷��B�͈͊O�̏ꍇnextAdd�ɒ��߂������̃X�J���[����������
/// </summary>
/// <param name="outm">    �X�J���[                                  </param>
/// <param name="clamp">   x = �ŏ��l�By = �ő�l�B     </param>
/// <param name="nextAdd"> ���߂������̃X�J���[             </param>
/// <returns>              true = ����O false = �����            </returns>
bool clampScalarVal(float& outm, const Bread::Math::Vector2 clamp, float& nextAdd) {
	if (Bread::Math::IsNanF32(outm)) {
		return false;
	}
	if (outm == 0.0f) {
		return false;
	}
	if (outm < clamp.x) {
		nextAdd = outm - clamp.x;
		outm = clamp.x;
		return true;
	}
	else if (outm > clamp.y) {
		nextAdd = outm - clamp.y;
		outm = clamp.y;
		return true;
	}

	return false;
}

/// <summary>
/// �X�J���[�l���͈͓������肷��B�͈͊O�̏ꍇnextAdd�ɒ��߂������̃X�J���[����������
/// </summary>
/// <param name="outm">    �X�J���[                                  </param>
/// <param name="clamp">   x = �ŏ��l�By = �ő�l�B     </param>
/// <param name="nextAdd"> ���߂������̃X�J���[             </param>
/// <returns>              true = ����O false = �����            </returns>
bool InverseClampScalarVal(float& outm, const Bread::Math::Vector2 clamp, float& nextAdd) {
	if (Bread::Math::IsNanF32(outm)) {
		return false;
	}
	if (outm == 0.0f) {
		return false;
	}
	if (outm > clamp.x) {
		nextAdd = outm - clamp.x;
		outm = clamp.x;
		return true;
	}
	else if (outm < clamp.y) {
		nextAdd = outm - clamp.y;
		outm = clamp.y;
		return true;
	}

	return false;
}

inline DirectX::XMFLOAT4 Clamp(DirectX::XMFLOAT4 v, DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
{
	DirectX::XMFLOAT4 result;

	DirectX::XMVECTOR vA = DirectX::XMLoadFloat3(&a);
	DirectX::XMVECTOR vV = DirectX::XMLoadFloat4(&v);
	DirectX::XMVECTOR vB = DirectX::XMLoadFloat3(&b);

	DirectX::XMStoreFloat4(&result,
		DirectX::XMVectorClamp(vV, vA, vB));

	return result;
}

inline Bread::Math::Vector3 ClampVector(Bread::Math::Vector3 clamper, Bread::Math::Vector3 min, Bread::Math::Vector3 max)
{
	Bread::Math::Vector3 result;
	if (min.x > max.x)
	{
		min.x = max.x;
	}
	else if (min.y > max.y)
	{
		min.y = max.y;

	}
	else if (min.z > max.z)
	{
		min.z = max.z;

	}

	if (clamper.x >= 180.f)
	{
		clamper.x -= 360.f;
	}
	else if (clamper.x <= -180.f)
	{
		clamper.x += 360.f;
	}
	if (clamper.y >= 180.f)
	{
		clamper.y -= 360.f;
	}
	else if (clamper.y <= -180.f)
	{
		clamper.y += 360.f;
	}
	if (clamper.z >= 180.f)
	{
		clamper.z -= 360.f;
	}
	else if (clamper.z <= -180.f)
	{
		clamper.z += 360.f;
	}

	DirectX::XMFLOAT4 newClamper = { clamper.x, clamper.y ,clamper.z ,0.0f };
	DirectX::XMFLOAT3
		newmin = { min.x,min.y ,min.z },
		newmax = { max.x,max.y ,max.z };
	DirectX::XMFLOAT4 f4Result = Clamp(newClamper, newmin, newmax);
	result = { f4Result.x,f4Result.y ,f4Result.z };

	return result;
}

namespace Bread {
	namespace FrameWork {

		/// <summary>
		/// �Q�̃x�N�g���̊O�ςŉ�]����Ԃ�
		/// </summary>
		/// <param name="toEffector">�@�W���C���g����G�t�F�N�^�[�܂ł̃x�N�g��        </param>
		/// <param name="toTarget">�@�@�W���C���g����ڕW�n�_�܂ł̃x�N�g���@�@</param>
		/// <returns>��]��</returns>
		Bread::Math::Vector3 CyclicCoordinateDescent::CreateLocalRotationAxis(const Bread::Math::Vector3& toEffector, const Bread::Math::Vector3& toTarget)
		{
			using namespace Bread;
			using namespace Bread::Math;

			Vector3 rotationAxis = Vector3Cross(toEffector, toTarget);
			rotationAxis.z = abs(rotationAxis.z);
			return Vector3Normalize(rotationAxis);
		}

		/// <summary>
		/// �Q�̃x�N�g���̓��p�ŉ�]�p�x��Ԃ�
		/// </summary>
		/// <param name="jointToTarget">�@�@�W���C���g����G�t�F�N�^�[�܂ł̃x�N�g��</param>
		/// <param name="jointToEffector">�@�W���C���g����ڕW�n�_�܂ł̃x�N�g��</param>
		/// <returns>��]�p�x</returns>
		float CyclicCoordinateDescent::LocalRotateAngle(const Bread::Math::Vector3& jointToTarget, const Bread::Math::Vector3& jointToEffector)
		{
			float rotationDotProduct = Vector3Dot(jointToTarget, jointToEffector);
			return acos(rotationDotProduct);
		}

		/// <summary>
		/// �W���C���g�̊p�x�i�I�C���[�p�j���N�����v�y�ђ��߂�������e�̃W���C���g�̊p�x�i�I�C���[�p�j�ɉ��Z���A�e��Tranform�s��̂�������̖��߂��o��
		/// </summary>
		/// <param name="euler">�@          �W���C���g�̃I�C���[�p</param>
		/// <param name="joint">�@           �X�V����W���C���g    </param>
		/// <param name="parentEuler">�@���߂����p�x          </param>
		/// <returns>joint�̐e�̍X�V����</returns>
		bool CyclicCoordinateDescent::JointAngleLimit(Bread::Math::Vector3& euler, Bread::FrameWork::ModelObject::Node& joint, Bread::Math::Vector3& parentEuler)
		{
			using namespace Bread;
			using namespace Bread::Math;
			bool orderToParent = false;

#if 0
			if (clampScalarVal(euler.x, { joint.minRot.x,joint.maxRot.x }, parentEuler.x))
			{
				if (joint.Movable[FrameWork::MovableShaft::X])
				{
					orderToParent = true;
				}
			}

			if (clampScalarVal(euler.y, { joint.minRot.y,joint.maxRot.y }, parentEuler.y))
			{
				if (joint.Movable[FrameWork::MovableShaft::Y])
				{
					orderToParent = true;
				}
			}

			if (clampScalarVal(euler.z, { joint.minRot.z,joint.maxRot.z }, parentEuler.z))
			{
				if (joint.Movable[FrameWork::MovableShaft::Z])
				{
					orderToParent = true;
				}
			}
#else
			if (joint.Movable[FrameWork::MovableShaft::X])
			{
				if (clampScalarVal(euler.x, { joint.minRot.x,joint.maxRot.x }, parentEuler.x))
				{
					orderToParent = true;
				}
			}

			if (joint.Movable[FrameWork::MovableShaft::Y])
			{
				if (clampScalarVal(euler.y, { joint.minRot.y,joint.maxRot.y }, parentEuler.y))
				{
					orderToParent = true;
				}
			}

			if (joint.Movable[FrameWork::MovableShaft::Z])
			{
				if (clampScalarVal(euler.z, { joint.minRot.z,joint.maxRot.z }, parentEuler.z))
				{
					orderToParent = true;
				}
			}
#endif

			return orderToParent;
		}

		/// <summary>
		/// �W���C���g�̊p�x�i�I�C���[�p�j���N�����v�y�ђ��߂�������e�̃W���C���g�̊p�x�i�I�C���[�p�j�ɉ��Z���A�e��Tranform�s��̂�������̖��߂��o��
		/// </summary>
		/// <param name="euler">�@          �W���C���g�̃I�C���[�p</param>
		/// <param name="joint">�@           �X�V����W���C���g    </param>
		/// <param name="parentEuler">�@���߂����p�x          </param>
		/// <returns>joint�̐e�̍X�V����</returns>
		bool CyclicCoordinateDescent::ZMinusJointAngleLimit(Bread::Math::Vector3& euler, Bread::FrameWork::ModelObject::Node& joint, Bread::Math::Vector3& parentEuler)
		{
			using namespace Bread;
			using namespace Bread::Math;
			bool orderToParent = false;

#if 0
			if (clampScalarVal(euler.x, { joint.minRot.x,joint.maxRot.x }, parentEuler.x))
			{
				if (joint.Movable[FrameWork::MovableShaft::X])
				{
					orderToParent = true;
				}
			}

			if (clampScalarVal(euler.y, { joint.minRot.y,joint.maxRot.y }, parentEuler.y))
			{
				if (joint.Movable[FrameWork::MovableShaft::Y])
				{
					orderToParent = true;
				}
			}

			if (clampScalarVal(euler.z, { joint.minRot.z,joint.maxRot.z }, parentEuler.z))
			{
				if (joint.Movable[FrameWork::MovableShaft::Z])
				{
					orderToParent = true;
				}
			}
#else
			if (joint.Movable[FrameWork::MovableShaft::X])
			{
				if (InverseClampScalarVal(euler.x, { joint.maxRot.x,joint.minRot.x }, parentEuler.x))
				{
					orderToParent = true;
				}
			}

			if (joint.Movable[FrameWork::MovableShaft::Y])
			{
				if (InverseClampScalarVal(euler.y, { joint.maxRot.y, joint.minRot.y }, parentEuler.y))
				{
					orderToParent = true;
				}
			}

			if (joint.Movable[FrameWork::MovableShaft::Z])
			{
				if (InverseClampScalarVal(euler.z, { joint.maxRot.z ,joint.minRot.z }, parentEuler.z))
				{
					orderToParent = true;
				}
			}
#endif

			return orderToParent;
		}

		/// <summary>
		///����̃W���C���g��Transform���X�V����
		/// </summary>
		/// <param name="joint">�@�X�V����W���C���g                     </param>
		/// <param name="euler">�@CCD�Ōv�Z���I������I�C���[�p</param>
		void CyclicCoordinateDescent::UpdatePosition(Bread::FrameWork::ModelObject::Node& joint,
			const Bread::Math::Vector3& euler, const Bread::Math::Matrix* targetWorldTransform)
		{
			using namespace DirectX;
			using namespace Bread;
			using namespace Bread::Math;

			Matrix scale, rotate, translate;
			joint.rotate = ConvertToQuaternionFromYawPitchRoll(euler.x, euler.y, euler.z);
			scale = MatrixScaling(joint.scale.x, joint.scale.y, joint.scale.z);
			rotate = MatrixRotationQuaternion(&joint.rotate);
			translate = MatrixTranslation(joint.translate.x, joint.translate.y, joint.translate.z);

			joint.localTransform = scale * rotate * translate;
			joint.worldTransform = joint.localTransform * joint.parent->worldTransform;
		}

		/// <summary>
		/// ���[�Ɍ�����Transform�̍X�V���s��(FK)
		/// </summary>
		/// <param name="joint">�X�V����W���C���g</param>
		void CyclicCoordinateDescent::updateAllPosition(Bread::FrameWork::ModelObject::Node& joint, const Bread::Math::Matrix* targetWorldTransform)
		{
			joint.worldTransform = joint.localTransform * joint.parent->worldTransform/* * (*targetWorldTransform)*/;
			for (auto* t : joint.child)
				updateAllPosition(*t, targetWorldTransform);
		}

		// �R���X�g���N�^
		void CyclicCoordinateDescent::Construct()
		{

		}

		// �f�X�g���N�^
		void CyclicCoordinateDescent::Destruct()
		{

		}

		// ������
		void CyclicCoordinateDescent::Initialize()
		{
			ID = GetOwner()->GetID();
		}

		// �I����
		void CyclicCoordinateDescent::Finalize()
		{

		}

		// �X�V
		void CyclicCoordinateDescent::Update(const f32& dt)
		{
		}

		//imgui
		void CyclicCoordinateDescent::GUI()
		{
			using namespace Bread::Math;

			if (ImGui::CollapsingHeader(u8"CCDIK", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
			{
				char  name[128] = {};
				FND::StrCpy(name, sizeof(name), GetID().c_str());
				ImGui::Text(u8"���O"); ImGui::SameLine();
				ImGui::InputText(("##" + ID).c_str(), name, IM_ARRAYSIZE(name));
				SetID(name);

				u32 orderIndex = 0;
				for (auto& it : order)
				{
					std::string jointName = it->beginJoint->name;
					std::string guiName = "CCDIK" + jointName;

					Vector3
						worldPos = GetLocation(*it->targetWorldTransform),
						worldScale = GetScale(*it->targetWorldTransform);
					Quaternion worldRotate = GetRotation(*it->targetWorldTransform);
					ImGui::DragFloat3(("target WorldPos - " + std::to_string(orderIndex)).c_str(), &worldPos.x);
					ImGui::DragFloat3(("target WorldRotate - " + std::to_string(orderIndex)).c_str(), &worldRotate.x);
					ImGui::DragFloat3(("target WorldScale - " + std::to_string(orderIndex)).c_str(), &worldScale.x);

					ImGui::DragFloat3(("targetPos - " + std::to_string(orderIndex)).c_str(), &it->targetPos.x);
					RegisterWatchVal("targetPos -" + std::to_string(orderIndex), &it->targetPos);
					ImGui::DragFloat(("ankleHeight - " + std::to_string(orderIndex)).c_str(), &it->ankleHeight);
					ImGui::DragInt(("iterate - " + std::to_string(orderIndex)).c_str(), &it->iterate);
					ImGui::Checkbox(("computeCCDIK - " + std::to_string(orderIndex)).c_str(), &it->computeCCDIK);

					orderIndex++;
				}
			}
		}

		void CyclicCoordinateDescent::PartUpdate(const Bread::u32& index)
		{
			auto* route = order.at(index).get();

			if (!route->computeCCDIK)return;
			switch (route->part)
			{
			case ARM:
				break;
			case FOOT:
				FootCCDIK(route->beginJoint, route->endJoint, route->targetPos, route->targetIKNormal, route->rayStart, route->kDown, route->targetWorldTransform, route->ankleHeight, route->iterate);
				break;

			case TOE:
				//ToeAimIK();
				break;

			case HEAD:
				break;

			case NONE:
				break;
			}
		}

#if 1
		/// <summary>
		/// nodes��effector����RootNode��parent�ɂ���W���C���g�Ɍ�����CCDIK���s���i�����ȊO�̊p�x��������j
		/// ��]��������o���ۂ̊O�ς����]���Ȃ��悤�ɂ����͍l�����Ȃ����̂Ƃ���
		/// </summary>
		/// <param name="beginJoint">      �q���̎n�_�W���C���g                       </param>
		/// <param name="endJoint">      �@�e���̏I�_�W���C���g                      </param>
		/// <param name="targetPos">        �ڕW���W                                      </param>
		/// <param name="normalVector"> �ڕW�n�_�̖@�����                      </param>
		/// <param name="ankleHeight">     ��̍����@�@�@�@�@�@�@              </param>
		/// <param name="iteratNum">CCDIK�̎��s��                                </param>
		void CyclicCoordinateDescent::FootCCDIK(
			Bread::FrameWork::ModelObject::Node* beginJoint,
			Bread::FrameWork::ModelObject::Node* endJoint,
			const Bread::Math::Vector3& targetPos,
			const Bread::Math::Vector3& normalVector,
			const Bread::Math::Vector3& rayStart,
			const Bread::Math::Vector3& kDown,
			const Bread::Math::Matrix* targetWorldTransform,
			const float                                               ankleHeight,
			int                                                           iteratNum)
		{
			using namespace Bread;
			using namespace Bread::Math;
			using namespace Bread::FrameWork;
			Vector3 ankleTargetPos = Vector3::Zero;
			const Vector3 normal = Vector3Normalize(normalVector);
			//����̖ڕW�n�_�̒���
			{
				//���b�V���@���ƃ��C�̃x�N�g���̓��ς����߂�
				const float ABLength =
					Vector3Dot(
						Vector3Subtract(rayStart, targetPos),
						normal);
				if (ABLength == 0.0f)
				{
					//ABLength��0��������IK��������
					return;
				}
				//���C��Ίp���Ƃ��钷���`�̒��_�����
				const Vector3 B = Vector3Subtract(rayStart, normal * ABLength);

				const Vector3 HitPB = Vector3Subtract(B, targetPos);
				const float HitPBLength = Vector3Length(HitPB);

				if (HitPBLength <= 0.0f)
				{
					ankleTargetPos = targetPos + (normal * ankleHeight);
				}
				else
				{
					const float      HitPHLength = HitPBLength * ankleHeight / ABLength;
					const Vector3 HitP = HitPB * (HitPHLength / HitPBLength);
					const Vector3 H = targetPos + HitP;

					//�v�Z���ʂ���̃^�[�Q�b�g�ɂ���
					ankleTargetPos = HitP + (normal * ankleHeight);
				}
			}

			Vector3 pelvisOffset = Vector3::Zero;
			float maxDot = -FLT_MAX;
			//���Ղ̈ʒu������̖ڕW�n�_���璲��
			{
				const Vector3 ankleToTarget =
					Vector3Subtract(ankleTargetPos, GetLocation(beginJoint->worldTransform * (*targetWorldTransform)));
				const float dot = Vector3Dot(ankleToTarget, kDown);
				if (dot > maxDot)
				{
					maxDot = dot;

					//�����n�ʂɐݒu����܂ł̍ő�ψʂ�p���ăI�t�Z�b�g���v�Z����
					pelvisOffset = kDown * dot;
				}
			}

			for (s32 it = 0; it < iteratNum; ++it)
			{
				for (ModelObject::Node* joint = beginJoint->parent; joint != endJoint->parent; joint = joint->parent)
				{
					float rotationAngle = 0.0f;
					Vector3 basis2EffectDir = Vector3::Zero;
					Vector3 basis2TargetDir = Vector3::Zero;

					{
						//�G�t�F�N�^�̃��[���h���W
						Vector3 worldEffectPos = GetLocation(beginJoint->worldTransform * (*targetWorldTransform));
						//���ڃW���C���g�̃��[���h���W
						Vector3 worldFocusJointPos = GetLocation(joint->worldTransform * (*targetWorldTransform));

						//���ڃW���C���g����̍��W�n�ɕϊ����邽�߂̍s��
						Matrix inverseMat = joint->worldTransform * (*targetWorldTransform);
						inverseMat._41 = 0.0f; inverseMat._42 = 0.0f; inverseMat._43 = 0.0f;
						inverseMat = MatrixInverse(inverseMat);

						Vector3 effectOffset = Vector3Subtract(worldEffectPos, worldFocusJointPos);
						Vector3 targetOffset = Vector3Subtract(targetPos, worldFocusJointPos);
						Matrix   mLocalEffectPos = MatrixTranslation(effectOffset.x, effectOffset.y, effectOffset.z);
						Matrix   mLocalTargetPos = MatrixTranslation(targetOffset.x, targetOffset.y, targetOffset.z);

						mLocalEffectPos = MatrixMultiply(mLocalEffectPos, inverseMat);
						mLocalTargetPos = MatrixMultiply(mLocalTargetPos, inverseMat);

						Vector3 localEffectorPos = GetLocation(mLocalEffectPos);
						Vector3 localTargetPos = GetLocation(mLocalTargetPos);

						basis2EffectDir = Vector3Normalize(localEffectorPos);
						basis2TargetDir = Vector3Normalize(localTargetPos);

						float rotationDotProduct = Vector3Dot(basis2TargetDir, basis2EffectDir);
						rotationAngle = static_cast<float>(ACosF32(rotationDotProduct));
					}

					//�p�x���������Ƃ��͏������Ȃ�
					if (rotationAngle > 1.0e-4f && rotationAngle < PI)
					{
						//�O�ς���]��
						Vector3 rotationAxis = Vector3Normalize(Vector3Cross(basis2EffectDir, basis2TargetDir));

						//��]���Ɖ�]�p�x����N�I�[�^�j�I���𐶐�
						Quaternion rotationQt = QuaternionRotationAxis(rotationAxis, rotationAngle);

						//�I�C���[�p�ɕϊ�
						Vector3 euler = ConvertToYawPitchRollFromQuaternion(rotationQt);

						//�p�x�������s��
						{
							euler.x = ToDegree(euler.x); euler.y = ToDegree(euler.y); euler.z = ToDegree(euler.z);
							euler   = ClampVector(euler, joint->minRot, joint->maxRot);
							euler.x = ToRadian(euler.x); euler.y = ToRadian(euler.y); euler.z = ToRadian(euler.z);
						}
						UpdatePosition(*joint, euler, targetWorldTransform);
					}

					Matrix* trash = nullptr;
					for (auto& t : joint->child)
						updateAllPosition(*t, trash);
				}//endJoint�܂ł̃��[�v
			}//iteration�񐔂̃��[�v
		}
#endif

	}
}