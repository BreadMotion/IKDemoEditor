#include "FrameWork/Component/IKManager.h"
#include "Math/BreadMath.h"

using namespace Bread::Math;

namespace Bread {
	namespace FrameWork {

		void IKManager::Start()
		{

		}//Start

		void IKManager::Update(float elapsedTime)
		{
			//LookAt
			for (std::shared_ptr<LookAtSetup> lookAt : _registedLookAt)
			{
				if (*lookAt->flg)
					continue;
				HumanLookAt(lookAt);
			}

			//FootIK
			for (std::shared_ptr<FootIkSetUp> footIk : _registedFootIk)
			{
				FootIk(footIk);
			}

			//����q��IK�i�������j
			for (std::shared_ptr<HoldHandSetup> holdHand : _registedHoldHand)
			{
				HoldHand(holdHand);
			}

		}//Update

		void IKManager::HoldHand(std::shared_ptr<HoldHandSetup> holdHand)
		{
			Vector3 targetPos{ GetLocation(holdHand->_leaderHand->worldTransform) };
			targetPos += GetVector3ColX(holdHand->_leaderHand->worldTransform) * -13.0f;
			targetPos += GetVector3ColY(holdHand->_leaderHand->worldTransform) * -4.0f;
			targetPos += GetVector3ColZ(holdHand->_leaderHand->worldTransform) * -3.0f;

			for (int j = 0; j < 20; j++)
			{
				CCDIKParentSolver(holdHand->_armSetup._list, holdHand->_armSetup._elbow, targetPos);
				CCDIKParentSolver(holdHand->_armSetup._list, holdHand->_armSetup._shoulder, targetPos);
				CCDIKParentSolver(holdHand->_armSetup._list, holdHand->_armSetup._chest, targetPos);
			}
			HandCCDIKSolver(holdHand->_armSetup._list, holdHand->_leaderHand);


		}//HoldHand

		void IKManager::HumanLookAt(std::shared_ptr<LookAtSetup> lookAt)
		{

			LookAtNode(lookAt, *lookAt->_targetPos, 0.7f);

			LookAtParentRotation(lookAt, 0.3f);

			UpdateTransform(lookAt->_pSpine);

		}//HumanLookAt

		void IKManager::LookAtNode(std::shared_ptr<LookAtSetup> lookAt, const Vector3& targetPos, const float& rate)
		{
			// ���ڃW���C���g����̍��W�n�ɕϊ����邽�߂̍s��
			Matrix inverseMat{ lookAt->_pNeck->worldTransform };
			inverseMat._41 = 0; inverseMat._42 = 0; inverseMat._43 = 0;
			inverseMat = MatrixInverse(inverseMat);

			Vector3 translation{ targetPos - GetLocation(lookAt->_pHead->worldTransform) };
			Matrix mLocalTargetPos{ MatrixTranslation(translation.x,translation.y,translation.z) };

			mLocalTargetPos = MatrixMultiply(mLocalTargetPos, inverseMat);

			// �^�[�Q�b�g���W�����[�J�����W�n�ɕϊ�
			Vector3 localEffectorPos{ GetVector3ColZ(lookAt->_pNeck->localTransform) };
			Vector3 localTargetPos{ GetLocation(mLocalTargetPos) };

			// ��֐߁��G�t�F�N�^�ʒu�ւ̕����x�N�g��
			Vector3 basis2EffectDir{ Vector3Normalize(localEffectorPos) };
			// ��֐߁��ڕW�ʒu�ւ̕����x�N�g��
			Vector3 basis2TargetDir{ Vector3Normalize(localTargetPos) };

			lookAt->dot        = ACosF32(Vector3Dot(basis2TargetDir, basis2EffectDir));
			Vector3 axisRotate{ lookAt->axis = Vector3Normalize(Vector3Cross(basis2EffectDir, basis2TargetDir)) };

			Quaternion rotateQt{ QuaternionRotationAxis(axisRotate, lookAt->dot * rate) };

			Matrix mRotate = MatrixRotationQuaternion(rotateQt);
			Vector3 euler;
			ToEulerAngleZXY(euler, mRotate);

			euler.x = ToDegree(euler.x);
			euler.y = ToDegree(euler.y);
			euler.z = ToDegree(euler.z);

			// ���搧��
			euler = ClampVector(euler, lookAt->_pNeck->minRot, lookAt->_pNeck->maxRot);

			euler.x = ToRadian(euler.x);
			euler.y = ToRadian(euler.y);
			euler.z = ToRadian(euler.z);

			mRotate = MatrixRotationRollPitchYaw(euler.x, euler.y, euler.z);

			lookAt->_pNeck->localTransform = mRotate * lookAt->_pNeck->localTransform;

		}//LookAtNode

		void IKManager::LookAtParentRotation(std::shared_ptr<LookAtSetup> lookAt, float rate)
		{
			Quaternion rotateQt = QuaternionRotationAxis(lookAt->axis, lookAt->dot * rate);

			Matrix mRotate = MatrixRotationQuaternion(rotateQt);
			Vector3 euler;
			ToEulerAngleZXY(euler.x, euler.y, euler.z, mRotate);

			euler.x = ToDegree(euler.x);
			euler.y = ToDegree(euler.y);
			euler.z = ToDegree(euler.z);

			// ���搧��
			euler = ClampVector(euler, lookAt->_pSpine->minRot, lookAt->_pSpine->maxRot);

			euler.x = ToRadian(euler.x);
			euler.y = ToRadian(euler.y);
			euler.z = ToRadian(euler.z);

			mRotate = MatrixRotationRollPitchYaw(euler.x, euler.y, euler.z);

			lookAt->_pSpine->localTransform = mRotate * lookAt->_pSpine->localTransform;
		}//LookAtParentRotation

		void IKManager::FootIk(std::shared_ptr<FootIkSetUp> footIk)
		{
			// ���Ƃ̐ڐG�_��T��
			RaycastLegs(footIk);

			// ����𐳂����ʒu�ɍX�V
			UpdateAnklesTarget(footIk);

			// ���Ղ𐳂����ʒu�ɍX�V
			UpdatePelvisOffset(footIk);

			for (int i = 0; i < FOOT_NUM; i++)
			{
				//���ɂ������ĂȂ�������X�L�b�v
				if (!footIk->_rayInfo[i]._hit)
				{
					continue;
				}

				//���ꂼ��̊֐߂��Ȃ��Ă����������ʒu�ɍX�V
				for (int j = 0; j < LOOP_MAX; j++)
				{
					CCDIKParentSolver(footIk->_legSetup[i]._pAnkle, footIk->_legSetup[i]._pKnee, footIk->_anklesTgtWs[i]);
					CCDIKParentSolver(footIk->_legSetup[i]._pAnkle, footIk->_legSetup[i]._pHip,  footIk->_anklesTgtWs[i]);

				}

				//�Ō�ɏ��ɑ���Y�킹��
				CCDIKSolver(footIk->_legSetup[i]._pAnkle, footIk->_rayInfo[i]._hitNormal, footIk->_anklesTgtWs[i]);
			}
		}//FootIk

		bool IKManager::RaycastLegs(std::shared_ptr<IKManager::FootIkSetUp> footIk)
		{
			Matrix root = GetRootTransform(footIk);

			for (int i = 0; i < FOOT_NUM; i++)
			{
				LegSetup& leg = footIk->_legSetup[i];
				RayInfo& ray  = footIk->_rayInfo[i];

				//����̍X�V�O�ʒu
				Vector4 colW{ GetColW(leg._pAnkle->worldTransform) };
				footIk->_anklesIniWs[i] = Vector3(colW.x, colW.y, colW.z);

				ray._start = footIk->_anklesIniWs[i] + footIk->kFootRayHeightOffset;
				ray._dir   = footIk->kDown;

				Vector3 rayend{ footIk->_anklesIniWs[i] };
				rayend.y = footIk->_anklesIniWs[i].y - footIk->_footHeight;
				Vector3 raystart{ ray._start };

				// ���ɑ����������Ă��邩���C�L���X�g���Ċm���߂�
				ray._hit = RayMeshIntersect(raystart, footIk->kDown, footIk->_mesh, true, &ray._hitPoint, &ray._hitNormal);
			}

			return true;
		}//RaycastLegs

		bool IKManager::UpdateAnklesTarget(std::shared_ptr<IKManager::FootIkSetUp> footIk) {
			for (size_t i = 0; i < 2; ++i) {
				const RayInfo& ray = footIk->_rayInfo[i];
				if (!ray._hit)
				{
					continue;
				}

				//�q�b�g�m�[�}����StartPos��HitPoint�Ƃ̃x�N�g��������
				//�q�b�g�m�[�}���Ƃ̓��ς����߂�
				const f32 ABl = Vector3Dot((ray._start - ray._hitPoint), ray._hitNormal);
				if (ABl == 0.f)
				{
					// ABl��0�������珈������
					continue;
				}

				// ���C��Ίp���Ƃ��钷���`�̒��_�����
				const Vector3 B{ ray._start - (ray._hitNormal * ABl) };

				// B�ƃq�b�g�|�C���g�̋��������߂�
				const Vector3 IB{ B - ray._hitPoint };
				const f32 IBl{ Vector3Length(IB) };

				if (IBl <= 0.0f)
				{
					// �������ʒu���Čv�Z
					footIk->_anklesTgtWs[i] = ray._hitPoint + (ray._hitNormal * footIk->_footHeight);
				}
				else
				{
					//  �^���X�̒藝��p���A�ʒu�����߂�
					const float IHl{  IBl * footIk->_footHeight / ABl };
					const Vector3 IH{ IB * (IHl / IBl) };
					const Vector3 H{  ray._hitPoint + IH };

					// �������A���N���ʒu�����߂�
					const Vector3 C{ H + (ray._hitNormal * footIk->_footHeight) };

					// �v�Z���ʂ��A���N���^�[�Q�b�g�ɓ����
					footIk->_anklesTgtWs[i] = C;
				}
			}

			return true;
		}//UpdateAnklesTarget

		bool IKManager::UpdatePelvisOffset(std::shared_ptr<IKManager::FootIkSetUp> footIk) {
			footIk->_pelvisOffset = Vector3::Zero;

			f32 maxDot = -FLT_MAX;
			if (footIk->_pelvisCorrection)
			{
				for (size_t i = 0; i < 2; ++i)
				{
					const RayInfo& ray = footIk->_rayInfo[i];
					if (!ray._hit)
					{
						continue;
					}

					// ���̑��񂪍X�V�O�̑���ɔ�ׂĒႢ�i�������j���ǂ������m�F���܂��B
					const Vector3 ankleToTgt{ footIk->_anklesTgtWs[i] - footIk->_anklesIniWs[i] };
					const f32     dot{ Vector3Dot(ankleToTgt, footIk->kDown) };
					if (dot > maxDot)
					{
						maxDot = dot;

						// �r���n�ʂɐڒn����܂ł̍ő�ψʂ�p���ăI�t�Z�b�g���v�Z����B
						footIk->_pelvisOffset = footIk->kDown * dot;
					}
				}
			}

			return true;
		}//UpdatePelvisOffset

		Matrix IKManager::GetRootTransform(std::shared_ptr<FootIkSetUp> footIk)
		{
			Matrix result{MatrixRotationRollPitchYaw(footIk->_rootYaw, 0, 0)
				* MatrixTranslation(footIk->_rootTrans->x,footIk->_rootTrans->y,footIk->_rootTrans->z) };

			result._13 *= -1;
			result._23 *= -1;
			result._33 *= -1;
			return result;

		}//GetRootTransform

		void IKManager::CCDIKSolver(ModelObject::Node* pEffector, const Vector3& faceNormal, const Vector3& hitCoordinate)
		{

			Vector3 effectorDirZ{ Vector3Normalize(GetVector3ColZ(pEffector->worldTransform)) };
			Vector3 effectorDirX{ Vector3Normalize(GetVector3ColX(pEffector->worldTransform)) };
			Vector3 axisFront{ Vector3Normalize(Vector3Cross(effectorDirX, faceNormal)) };

			f32 dot{ acos(Vector3Dot(effectorDirZ, axisFront)) };

			if (axisFront.y < 0)
			{
				dot *= -1;
			}

			if (dot > 1.0e-2f && dot < 3.141592)
			{

				Vector4 eulerX{ GetColX(pEffector->localTransform) };
				Matrix mEulerXMatrix{ MatrixRotationRollPitchYawFromVector(eulerX) };
				Vector3 rotationAxis{ ConvertToQuaternionFromRotationMatrix(mEulerXMatrix) };

				// ��]���Ɖ�]�p�x����N�H�[�^�j�I���𐶐�
				Quaternion rotationQt = QuaternionRotationAxis(rotationAxis, -dot);

				// �N�H�[�^�j�I�������]�s��𐶐�
				Matrix rotationMatrix = MatrixRotationQuaternion(rotationQt);
				Vector3 euler;
				ToEulerAngleZXY(euler.x, euler.y, euler.z, rotationMatrix);

				euler.x = ToDegree(euler.x);
				euler.y = ToDegree(euler.y);
				euler.z = ToDegree(euler.z);

				euler = ClampVector(euler, pEffector->minRot, pEffector->maxRot);

				euler.x = ToRadian(euler.x);
				euler.y = ToRadian(euler.y);
				euler.z = ToRadian(euler.z);

				rotationMatrix = MatrixRotationRollPitchYawFromVector(euler);

				// ���ڃW���C���g�̎p�����X�V
				pEffector->localTransform = rotationMatrix * pEffector->localTransform;

				UpdateChildTranslate(pEffector->parent);

			}
		}//CCDIKSolver

		void IKManager::HandCCDIKSolver(ModelObject::Node* folHand, ModelObject::Node* leadHand)
		{

			Vector3 leadDirX{ Vector3Normalize(GetVector3ColX(leadHand->worldTransform)) };
			Vector3 leadDirY{ Vector3Normalize(GetVector3ColY(leadHand->worldTransform)) };
			Vector3 leadDirZ{ Vector3Normalize(GetVector3ColZ(leadHand->worldTransform)) };

			Vector3 followDirX{ Vector3Normalize(GetVector3ColX(folHand->worldTransform)) };
			Vector3 followDirY{ Vector3Normalize(GetVector3ColY(folHand->worldTransform)) };
			Vector3 followDirZ{ Vector3Normalize(GetVector3ColZ(folHand->worldTransform)) };

			f32 dot = acos(Vector3Dot(followDirZ, leadDirX));
			if (dot > 1.0e-2f && dot < PI)
			{

				Vector3 eulerZ{ GetVector3ColZ(folHand->localTransform) };
				Vector3 eulerX{ GetVector3ColX(leadHand->localTransform) };

				// ��]���Ɖ�]�p�x����N�H�[�^�j�I���𐶐�
				Quaternion rotationQt = QuaternionRotationAxis(Vector3Cross(eulerX, eulerZ), dot);

				// �N�H�[�^�j�I�������]�s��𐶐�
				Matrix rotationMatrix = MatrixRotationQuaternion(rotationQt);
				Vector3 euler;
			    ToEulerAngleZXY(euler.x, euler.y, euler.z, rotationMatrix);

				euler.x = ToDegree(euler.x);
				euler.y = ToDegree(euler.y);
				euler.z = ToDegree(euler.z);

				euler = ClampVector(euler, folHand->minRot, folHand->maxRot);

				euler.x = ToRadian(euler.x);
				euler.y = ToRadian(euler.y);
				euler.z = ToRadian(euler.z);

				rotationMatrix = mathdx::MatrixRotationRollPitchYawFromVector(euler);

				// ���ڃW���C���g�̎p�����X�V
				folHand->localTransform = rotationMatrix * folHand->localTransform;
				UpdateChildTranslate(folHand->parent);

			}
		}//HandCCDIKSolver

		void IKManager::CCDIKParentSolver(ModelObject::Node* pEffector, ModelObject::Node* pCurrent, const Vector3& hitCoordinate)
		{
			float rotationAngle;
			Vector3 basis2EffectDir, basis2TargetDir;
			CulculateAngle(pEffector, pCurrent, hitCoordinate, basis2EffectDir, basis2TargetDir, rotationAngle);

			// �p�x�����������͏������Ȃ�
			if (rotationAngle > 1.0e-4f && rotationAngle < 3.141592)
			{
				CulculateParentLocal(basis2EffectDir, basis2TargetDir, rotationAngle, pCurrent);
			}
		}//CCDIKParentSolver

		void IKManager::CulculateParentLocal(const Vector3& basis2EffectDir, const Vector3& basis2TargetDir, f32 rotationAngle,ModelObject::Node* pCurrent)
		{
			// �O�ς���]��
			Vector3 rotationAxis = Vector3Normalize(Vector3Cross(basis2EffectDir, basis2TargetDir));

			// ��]���Ɖ�]�p�x����N�H�[�^�j�I���𐶐�
			Quaternion rotationQt{ QuaternionRotationAxis(rotationAxis, rotationAngle) };

			// �N�H�[�^�j�I�������]�s��𐶐�
			Matrix rotationMatrix{ MatrixRotationQuaternion(rotationQt) };

			Vector3 euler;
			ToEulerAngleZXY(euler.x, euler.y, euler.z, rotationMatrix);

			euler.x = ToDegree(euler.x);
			euler.y = ToDegree(euler.y);
			euler.z = ToDegree(euler.z);

			euler = ClampVector(euler, pCurrent->minRot, pCurrent->maxRot);

			euler.x = ToRadian(euler.x);
			euler.y = ToRadian(euler.y);
			euler.z = ToRadian(euler.z);

			rotationMatrix = MatrixRotationRollPitchYawFromVector(euler);

			// ���ڃW���C���g�̎p�����X�V
			pCurrent->localTransform = rotationMatrix * pCurrent->localTransform;

			// ���ڃW���C���g���炷�ׂĂ̎q�W���C���g�̈ʒu���X�V
			UpdateChildTranslate(pCurrent->parent);
		}//CulculateParentLocal

		void IKManager::CulculateAngle(ModelObject::Node* ankle, ModelObject::Node* hip, const Vector3& targetPos, Vector3& basis2EffectDir, Vector3& basis2TargetDir, f32& rotateAngle)
		{
			// �G�t�F�N�^�̃��[���h���W
			Vector3 effectPos{ GetLocation(ankle->worldTransform) };
			// ���ڃW���C���g�̃��[���h���W
			Vector3 focusJointPos{ GetLocation(hip->worldTransform) };

			// ���ڃW���C���g����̍��W�n�ɕϊ����邽�߂̍s��
			Matrix inverseMat{ hip->worldTransform };
			inverseMat._41 = 0; inverseMat._42 = 0; inverseMat._43 = 0;
			inverseMat = MatrixInverse(inverseMat);

			Vector3 focusToEffect{ effectPos - focusJointPos };
			Vector3 focusToTarget{ targetPos - focusJointPos };
			Matrix mLocalEffectPos{ MatrixTranslation(focusToEffect.x,focusToEffect.y,focusToEffect.z) };
			Matrix mLocalTargetPos{ MatrixTranslation(focusToEffect.x,focusToEffect.y,focusToEffect.z) };

			mLocalEffectPos = mLocalEffectPos * inverseMat;
			mLocalTargetPos = mLocalTargetPos * inverseMat;

			// �^�[�Q�b�g���W�����[�J�����W�n�ɕϊ�
			Vector3 localEffectorPos{ GetLocation(mLocalEffectPos) };
			Vector3 localTargetPos{ GetLocation(mLocalTargetPos) };

			// (1) ��֐߁��G�t�F�N�^�ʒu�ւ̕����x�N�g��
			basis2EffectDir = Vector3Normalize(localEffectorPos);
			// (2) ��֐߁��ڕW�ʒu�ւ̕����x�N�g��
			basis2TargetDir = Vector3Normalize(localTargetPos);

			// (1)��(2)�̈ʒu�Ɍ������]�s��̌v�Z
			f32 rotationDotProduct{ Vector3Dot(basis2TargetDir, basis2EffectDir) };
			rotateAngle = acos(rotationDotProduct);
		}//CulculateAngle

		void IKManager::UpdateTransform(ModelObject::Node* _node)
		{
			Matrix mChildLocal{ _node->localTransform };
			//Matrix mChildModel{ _node->_modelTransform };
			//Matrix mParentModel{ _node->parent->_modelTransform };
			Matrix mParentWorld{ _node->parent->worldTransform };
			_node->localTransform  = mChildLocal;
			//_node->_modelTransform = mChildLocal * mParentModel;
			_node->worldTransform  = mChildLocal * mParentWorld;

			UpdateChildTranslate(_node);

		}//UpdateTransform


		void IKManager::UpdateChildTranslate(ModelObject::Node* _pParent)
		{
			for (auto& child : _pParent->child)
			{
				Matrix mChildLocal{ child->localTransform };
				//Matrix mChildModel{ child->_modelTransform };
				//Matrix mParentModel{ child->parent->_modelTransform };
				Matrix mParentWorld{ child->parent->worldTransform };
				child->localTransform = mChildLocal;
				//child->_modelTransform = mChildLocal * mParentModel;
				child->worldTransform = mChildLocal * mParentWorld;

				UpdateChildTranslate(child);
			}
		}//UpdateChildTranslate

		void IKManager::RegisterHoldHand(std::shared_ptr<ModelObject> follower, std::shared_ptr<ModelObject> leader)
		{

			if (!follower)
			{
				return;
			}

			std::shared_ptr<HoldHandSetup> holdHand{ std::make_shared<HoldHandSetup>() };

			holdHand->_armSetup._chest    = follower->GetNodeFromName("LeftShoulder");
			holdHand->_armSetup._shoulder = follower->GetNodeFromName("LeftArm");
			holdHand->_armSetup._elbow    = follower->GetNodeFromName("LeftForeArm");
			holdHand->_armSetup._list     = follower->GetNodeFromName("LeftHand");

			holdHand->_leaderHand = leader->GetNodeFromName("RightHand");

			if (holdHand->_armSetup._chest->minRot.x == 0)
			{

				holdHand->_armSetup._chest->minRot = { 0.f,	-45.f,	0.f };
				holdHand->_armSetup._chest->maxRot = { 0.f,	45.f,	0.f };
				holdHand->_armSetup._shoulder->minRot = { 0.f,	-90.f,	0.f };
				holdHand->_armSetup._shoulder->maxRot = { 0.f,	90.f,	0.f };
				holdHand->_armSetup._elbow->minRot = { 0.f,	-90.f, -25.f };
				holdHand->_armSetup._elbow->maxRot = { 0.f,	90.f,	25.f };
				holdHand->_armSetup._list->minRot = { -90.f,	-25.f,	0 };
				holdHand->_armSetup._list->maxRot = { 90.f,	25.f,	0 };
			}

			_registedHoldHand.emplace_back(holdHand);

		}//RegisterHoldHand

		void IKManager::UnRegisterHoldHand(std::shared_ptr<IKManager::HoldHandSetup> follower)
		{
			std::vector<std::shared_ptr<HoldHandSetup>>::iterator it = _registedHoldHand.begin();
			while (it != _registedHoldHand.end())
			{
				if (*it == follower)
				{
					it = _registedHoldHand.erase(it);
				}
				else ++it;
			}

		}//UnRegisterHoldHand

		void IKManager::RegisterLookAt(std::shared_ptr<ModelObject> model, Vector3* targetPos, bool* flg)
		{
			if (!model)
			{
				return;
			}

			std::shared_ptr<LookAtSetup> lookAt{ std::make_shared<LookAtSetup>() };

			lookAt->flg = flg;
			lookAt->_targetPos = targetPos;

			lookAt->_pHead = model->GetNodeFromName("Head");
			lookAt->_pNeck = model->GetNodeFromName("Neck");
			lookAt->_pSpine = model->GetNodeFromName("Spine1");

			if (lookAt->_pHead->minRot.x == 0)
			{
				lookAt->_pNeck->minRot = { -75.0f, -75.0f, -75.0f };
				lookAt->_pNeck->maxRot = { 75.0f, 75.0f, 75.0f };
				lookAt->_pSpine->minRot = { -15.0f, -25.0f, -15.0f };
				lookAt->_pSpine->maxRot = { 15.0f, 25.0f, 15.0f };
			}
			_registedLookAt.emplace_back(lookAt);

		}//RegisterLookAt

		void IKManager::UnRegisterLookAt(std::shared_ptr<LookAtSetup> lookAt)
		{

			std::vector<std::shared_ptr<LookAtSetup>>::iterator it = _registedLookAt.begin();
			while (it != _registedLookAt.end()) {
				if (*it == lookAt) {
					it = _registedLookAt.erase(it);
				}
				else ++it;
			}
		}//UnRegisterLookAt

		void IKManager::RegisterFootIk(std::shared_ptr<ModelObject> model, std::shared_ptr<Transform> rootT)
		{
			if (!model)
				return;

			std::shared_ptr<FootIkSetUp> footIk = std::make_shared<FootIkSetUp>();
			footIk->_rootTrans = &rootT->GetTranslate();

			footIk->_legSetup[0]._pHip   = model->GetNodeFromName("LeftUpLeg");
			footIk->_legSetup[0]._pKnee  = model->GetNodeFromName("LeftLeg");
			footIk->_legSetup[0]._pAnkle = model->GetNodeFromName("LeftFoot");
			footIk->_legSetup[1]._pHip   = model->GetNodeFromName("RightUpLeg");
			footIk->_legSetup[1]._pKnee  = model->GetNodeFromName("RightLeg");
			footIk->_legSetup[1]._pAnkle = model->GetNodeFromName("RightFoot");

			if (footIk->_legSetup[0]._pHip->minRot.x == 0)
			{
				for (unsigned int i = 0; i < 2; i++)
				{
					footIk->_legSetup[i]._pHip->minRot = { -90.f, 0.0f, 0.f };
					footIk->_legSetup[i]._pHip->maxRot = { 0.f, 0.f, 0.f };
					footIk->_legSetup[i]._pKnee->minRot = { 0.f, 0.f, 0.f };
					footIk->_legSetup[i]._pKnee->maxRot = { 90.f,0.f,0.f };
					footIk->_legSetup[i]._pAnkle->minRot = { -45.f, 0.f,0.f };
					footIk->_legSetup[i]._pAnkle->maxRot = { 45.f, 0.f,0.f };
				}
			}

			_registedFootIk.emplace_back(footIk);

		}//RegisterFootIk

		void IKManager::UnRegisterFootIk(std::shared_ptr<FootIkSetUp> footIk)
		{

			std::vector<std::shared_ptr<FootIkSetUp>>::iterator it = _registedFootIk.begin();
			while (it != _registedFootIk.end())
			{
				if (*it == footIk)
				{
					it = _registedFootIk.erase(it);
				}
				else ++it;
			}
		}//UnRegisterFootIk
	}//ani
}//nn