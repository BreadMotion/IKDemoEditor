#include "FrameWork/Component/IKManager.h"
#include "Math/BreadMath.h"

using namespace Bread::Math;

namespace Bread {
	namespace FrameWork {

		void IKManager::Start()
		{

		}//Start

		void IKManager::Update()
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
				footIk->pmodel->UpdateBoneTransform();
			}

			//手を繋ぐIK（実装中）
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
			// 注目ジョイントからの座標系に変換するための行列
			Matrix inverseMat{ lookAt->_pNeck->worldTransform };
			inverseMat._41 = 0; inverseMat._42 = 0; inverseMat._43 = 0;
			inverseMat = MatrixInverse(inverseMat);

			Vector3 translation{ targetPos - GetLocation(lookAt->_pHead->worldTransform) };
			Matrix mLocalTargetPos{ MatrixTranslation(translation.x,translation.y,translation.z) };

			mLocalTargetPos = MatrixMultiply(mLocalTargetPos, inverseMat);

			// ターゲット座標をローカル座標系に変換
			Vector3 localEffectorPos{ GetVector3ColZ(lookAt->_pNeck->localTransform) };
			Vector3 localTargetPos{ GetLocation(mLocalTargetPos) };

			// 基準関節→エフェクタ位置への方向ベクトル
			Vector3 basis2EffectDir{ Vector3Normalize(localEffectorPos) };
			// 基準関節→目標位置への方向ベクトル
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

			// 可動域制御
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

			// 可動域制御
			euler = ClampVector(euler, lookAt->_pSpine->minRot, lookAt->_pSpine->maxRot);

			euler.x = ToRadian(euler.x);
			euler.y = ToRadian(euler.y);
			euler.z = ToRadian(euler.z);

			mRotate = MatrixRotationRollPitchYaw(euler.x, euler.y, euler.z);

			lookAt->_pSpine->localTransform = mRotate * lookAt->_pSpine->localTransform;
		}//LookAtParentRotation

		void IKManager::FootIk(std::shared_ptr<FootIkSetUp> footIk)
		{
			//床にあたってなかったらスキップ
			if (!footIk->rayCast[0]->GetHItFlag() && !footIk->rayCast[1]->GetHItFlag())
			{
				return;
			}

			for (u32 i =0;i < FOOT_NUM ; i++)
			{
				LegSetup& leg = footIk->_legSetup[i];
				footIk->_anklesIniWs[i] = GetLocation(leg._pAnkle->worldTransform * (*footIk->_rootTrans));
			}

			// 足首を正しい位置に更新
			UpdateAnklesTarget(footIk);

			// 骨盤を正しい位置に更新
			UpdatePelvisOffset(footIk);

			for (u32 i = 0; i < FOOT_NUM; i++)
			{
				//それぞれの関節を曲げていき正しい位置に更新
				for (u32 j = 0; j < LOOP_MAX; j++)
				{
					CCDIKParentSolver(footIk->_legSetup[i]._pAnkle, footIk->_legSetup[i]._pKnee, footIk->_anklesTgtWs[i], footIk->_rootTrans);
					CCDIKParentSolver(footIk->_legSetup[i]._pAnkle, footIk->_legSetup[i]._pHip,  footIk->_anklesTgtWs[i], footIk->_rootTrans);
				}

				//最後に床に足を添わせる
				CCDIKSolver(footIk->_legSetup[i]._pAnkle, footIk->rayCast[i]->hitResult.normal, footIk->_anklesTgtWs[i], footIk->_rootTrans);
			}
		}//FootIk

		bool IKManager::UpdateAnklesTarget(std::shared_ptr<IKManager::FootIkSetUp> footIk) {
			for (u32 i = 0; i < 2; i++) {
				const RayCastCom::HitResult& ray = footIk->rayCast[i]->hitResult;
				if (!footIk->rayCast[i]->GetHItFlag())
				{
					continue;
				}

				//ヒットノーマルとStartPosとHitPointとのベクトルを求め
				//ヒットノーマルとの内積を求める
				const f32 ABl = Vector3Dot((ray.start - ray.position), ray.normal);
				if (ABl == 0.f)
				{
					// ABlが0だったら処理抜け
					continue;
				}

				// レイを対角線とする長方形の頂点を作る
				const Vector3 B{ ray.start - (ray.normal * ABl) };

				// Bとヒットポイントの距離を求める
				const Vector3 IB{ B - ray.position };
				const f32 IBl{ Vector3Length(IB) };

				if (IBl <= 0.0f)
				{
					// 正しい位置を再計算
					footIk->_anklesTgtWs[i] = ray.position + (ray.normal * footIk->_footHeight);
				}
				else
				{
					//  タレスの定理を用い、位置を求める
					const float IHl{  IBl * footIk->_footHeight / ABl };
					const Vector3 IH{ IB * (IHl / IBl) };
					const Vector3 H{  ray.position + IH };

					// 正しいアンクル位置を求める
					const Vector3 C{ H + (ray.normal * footIk->_footHeight) };

					// 計算結果をアンクルターゲットに入れる
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
				for (u32 i = 0; i < 2; i++)
				{
					const RayCastCom::HitResult& ray = footIk->rayCast[i]->hitResult;
					if (!footIk->rayCast[i]->GetHItFlag())
					{
						continue;
					}

					// 今の足首が更新前の足首に比べて低い（下方向）かどうかを確認します。
					const Vector3 ankleToTgt{ footIk->_anklesTgtWs[i] - footIk->_anklesIniWs[i] };
					const f32     dot{ Vector3Dot(ankleToTgt, footIk->kDown) };
					if (dot > maxDot)
					{
						maxDot = dot;

						// 脚が地面に接地するまでの最大変位を用いてオフセットを計算する。
						footIk->_pelvisOffset = footIk->kDown * dot;
					}
				}
			}

			return true;
		}//UpdatePelvisOffset

		Matrix IKManager::GetRootTransform(std::shared_ptr<FootIkSetUp> footIk)
		{
			Vector3 rootTranslate = GetLocation((*footIk->_rootTrans));
			Matrix result{MatrixRotationRollPitchYaw(footIk->_rootYaw, 0, 0)
				* MatrixTranslation(rootTranslate.x,rootTranslate.y,rootTranslate.z) };

			result._13 *= -1;
			result._23 *= -1;
			result._33 *= -1;
			return result;

		}//GetRootTransform

		void IKManager::CCDIKSolver(ModelObject::Node* pEffector, const Vector3& faceNormal, const Vector3& hitCoordinate, const Matrix* root)
		{
			Vector3 effectorDirZ{ Vector3Normalize(GetVector3ColZ(pEffector->worldTransform * (*root))) };
			Vector3 effectorDirX{ Vector3Normalize(GetVector3ColX(pEffector->worldTransform * (*root))) };
			Vector3 axisFront{ Vector3Normalize(Vector3Cross(effectorDirX, faceNormal)) };

			f32 dot{ acos(Vector3Dot(effectorDirZ, axisFront)) };

			if (axisFront.y < 0)
			{
				dot *= -1.0f;
			}

			if (dot > 1.0e-2f && dot < PI)
			{
				Vector4 eulerX{ GetColX(pEffector->localTransform) };
				Matrix mEulerXMatrix{ MatrixRotationRollPitchYaw(eulerX.x, eulerX.y, eulerX.z) };
				Vector3 rotationAxis{ ConvertToQuaternionFromRotationMatrix(mEulerXMatrix) };

				// 回転軸と回転角度からクォータニオンを生成
				Quaternion rotationQt = QuaternionRotationAxis(rotationAxis, -dot);

				// クォータニオンから回転行列を生成
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

				//rotationMatrix = MatrixRotationRollPitchYaw(euler.x, euler.y, euler.z);

				// 注目ジョイントの姿勢を更新
				Matrix scale, rotate, translate;
				pEffector->rotate = ConvertToQuaternionFromRollPitchYaw(euler.x, euler.y, euler.z);
				scale     = MatrixScaling(pEffector->scale.x, pEffector->scale.y, pEffector->scale.z);
				rotate    = MatrixRotationQuaternion(pEffector->rotate);
				translate = MatrixTranslation(pEffector->translate.x, pEffector->translate.y, pEffector->translate.z);
				pEffector->localTransform = scale * rotate * translate;
				//pEffector->worldTransform = pEffector->localTransform * pEffector->parent->worldTransform;

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

				// 回転軸と回転角度からクォータニオンを生成
				Quaternion rotationQt = QuaternionRotationAxis(Vector3Cross(eulerX, eulerZ), dot);

				// クォータニオンから回転行列を生成
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

				rotationMatrix = MatrixRotationRollPitchYaw(euler.x, euler.y, euler.z);

				// 注目ジョイントの姿勢を更新
				folHand->localTransform = rotationMatrix * folHand->localTransform;
				UpdateChildTranslate(folHand->parent);

			}
		}//HandCCDIKSolver

		void IKManager::CCDIKParentSolver(ModelObject::Node* pEffector, ModelObject::Node* pCurrent, const Vector3& hitCoordinate, const Matrix* root)
		{
			float rotationAngle;
			Vector3 basis2EffectDir, basis2TargetDir;
			CulculateAngle(pEffector, pCurrent, hitCoordinate, basis2EffectDir, basis2TargetDir, rotationAngle, root);

			// 角度が小さい時は処理しない
			if (rotationAngle > 1.0e-4f && rotationAngle < PI)
			{
				CulculateParentLocal(basis2EffectDir, basis2TargetDir, rotationAngle, pCurrent, root);
			}
		}//CCDIKParentSolver

		void IKManager::CulculateParentLocal(const Vector3& basis2EffectDir, const Vector3& basis2TargetDir, f32 rotationAngle,ModelObject::Node* pCurrent,const Matrix* root)
		{
			// 外積が回転軸
			Vector3 Cross = Vector3Cross(basis2EffectDir, basis2TargetDir);
			Vector3 rotationAxis = Vector3Normalize(Cross);

			// 回転軸と回転角度からクォータニオンを生成
			Quaternion rotationQt{ QuaternionRotationAxis(rotationAxis, rotationAngle) };

			// クォータニオンから回転行列を生成
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

			// 注目ジョイントの姿勢を更新
			//rotationMatrix = MatrixRotationRollPitchYaw(euler.x, euler.y, euler.z);

			Matrix scale, rotate, translate;
			pCurrent->rotate = ConvertToQuaternionFromRollPitchYaw(euler.x, euler.y, euler.z);
			scale     = MatrixScaling(pCurrent->scale.x, pCurrent->scale.y, pCurrent->scale.z);
			rotate    = MatrixRotationQuaternion(pCurrent->rotate);
			translate = MatrixTranslation(pCurrent->translate.x, pCurrent->translate.y, pCurrent->translate.z);
			pCurrent->localTransform = scale * rotate * translate;
			//pCurrent->worldTransform = pCurrent->localTransform * pCurrent->parent->worldTransform;

			// 注目ジョイントからすべての子ジョイントの位置を更新
			UpdateChildTranslate(pCurrent->parent);
		}//CulculateParentLocal

		void IKManager::CulculateAngle(ModelObject::Node* ankle, ModelObject::Node* hip, const Vector3& targetPos, Vector3& basis2EffectDir, Vector3& basis2TargetDir, f32& rotateAngle, const Matrix* root)
		{
			// エフェクタのワールド座標
			Vector3 effectPos{ GetLocation(ankle->worldTransform * (*root)) };
			// 注目ジョイントのワールド座標
			Vector3 focusJointPos{ GetLocation(hip->worldTransform * (*root)) };

			// 注目ジョイントからの座標系に変換するための行列
			Matrix inverseMat{ hip->worldTransform * (*root) };
			inverseMat._41 = 0; inverseMat._42 = 0; inverseMat._43 = 0;
			inverseMat = MatrixInverse(inverseMat);

			Vector3 focusToEffect{ effectPos - focusJointPos };
			Vector3 focusToTarget{ targetPos - focusJointPos };
			Matrix mLocalEffectPos{ MatrixTranslation(focusToEffect.x,focusToEffect.y,focusToEffect.z) };
			Matrix mLocalTargetPos{ MatrixTranslation(focusToTarget.x,focusToTarget.y,focusToTarget.z) };

			mLocalEffectPos = mLocalEffectPos * inverseMat;
			mLocalTargetPos = mLocalTargetPos * inverseMat;

			// ターゲット座標をローカル座標系に変換
			Vector3 localEffectorPos{ GetLocation(mLocalEffectPos) };
			Vector3 localTargetPos{ GetLocation(mLocalTargetPos) };

			// (1) 基準関節→エフェクタ位置への方向ベクトル
			basis2EffectDir = Vector3Normalize(localEffectorPos);
			// (2) 基準関節→目標位置への方向ベクトル
			basis2TargetDir = Vector3Normalize(localTargetPos);

			// (1)を(2)の位置に向ける回転行列の計算
			f32 rotationDotProduct{ Vector3Dot(basis2TargetDir, basis2EffectDir) };
			rotateAngle = acos(rotationDotProduct);
		}//CulculateAngle

		void IKManager::UpdateTransform(ModelObject::Node* _node)
		{
			Matrix scale, rotate, translate;
			scale = MatrixScaling(_node->scale.x, _node->scale.y, _node->scale.z);
			rotate = MatrixRotationQuaternion(_node->rotate);
			translate = MatrixTranslation(_node->translate.x, _node->translate.y, _node->translate.z);
			_node->localTransform = scale * rotate * translate;
			Matrix mChildLocal{ _node->localTransform };
			//Matrix mChildModel{ _node->_modelTransform };
			//Matrix mParentModel{ _node->parent->_modelTransform };
			Matrix mParentWorld{ _node->parent->worldTransform };

			//_node->_modelTransform = mChildLocal * mParentModel;
			_node->worldTransform  = mChildLocal * mParentWorld;

			UpdateChildTranslate(_node);

		}//UpdateTransform


		void IKManager::UpdateChildTranslate(ModelObject::Node* _pParent)
		{
			for (auto& child : _pParent->child)
			{
				Matrix scale, rotate, translate;
				scale = MatrixScaling(child->scale.x, child->scale.y, child->scale.z);
				rotate = MatrixRotationQuaternion(child->rotate);
				translate = MatrixTranslation(child->translate.x, child->translate.y, child->translate.z);
				child->localTransform = scale * rotate * translate;

				//Matrix mChildModel{ child->_modelTransform };
				//Matrix mParentModel{ child->parent->_modelTransform };
				//child->_modelTransform = mChildLocal * mParentModel;
				child->worldTransform = child->localTransform * child->parent->worldTransform;

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

		void IKManager::RegisterFootIk(std::shared_ptr<ModelObject> model, std::shared_ptr<Transform> rootT, const std::shared_ptr<RayCastCom> rayCast[2])
		{
			if (!model)
				return;

			std::shared_ptr<FootIkSetUp> footIk = std::make_shared<FootIkSetUp>();
			footIk->pmodel = model;
			footIk->_rootTrans = &rootT->GetWorldTransform();

			footIk->_legSetup[0]._pHip   = model->GetNodeFromName("LeftUpLeg");
			footIk->_legSetup[0]._pKnee  = model->GetNodeFromName("LeftLeg");
			footIk->_legSetup[0]._pAnkle = model->GetNodeFromName("LeftFoot");
			footIk->_legSetup[1]._pHip   = model->GetNodeFromName("RightUpLeg");
			footIk->_legSetup[1]._pKnee  = model->GetNodeFromName("RightLeg");
			footIk->_legSetup[1]._pAnkle = model->GetNodeFromName("RightFoot");

			footIk->rayCast[0] = rayCast[0];
			footIk->rayCast[1] = rayCast[1];

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


		void IKManager::Gui()
		{
			ImGui::Begin("IKManager");
			for (const auto& it: _registedFootIk)
			{
				ImGui::Text("bone name  : %s", it->_legSetup[0]._pHip->name.c_str());
				ImGui::Text("bone name  : %s", it->_legSetup[0]._pKnee->name.c_str());
				ImGui::Text("bone name  : %s", it->_legSetup[0]._pAnkle->name.c_str());
				ImGui::Separator();
				ImGui::Text("bone name  : %s", it->_legSetup[1]._pHip->name.c_str());
				ImGui::Text("bone name  : %s", it->_legSetup[1]._pKnee->name.c_str());
				ImGui::Text("bone name  : %s", it->_legSetup[1]._pAnkle->name.c_str());
				ImGui::Separator();
				ImGui::Text("raycast  : %f",it->rayCast[0]->hitResult.position.y);
				ImGui::Text("raycast  : %f", it->rayCast[1]->hitResult.position.y);
				ImGui::Separator();
				ImGui::Text("ankle0     : x.%f,y.%f,z.%f", it->_anklesTgtWs[0].x, it->_anklesTgtWs[0].y, it->_anklesTgtWs[0].z);
				ImGui::Text("ankle1     : x.%f,y.%f,z.%f", it->_anklesTgtWs[1].x, it->_anklesTgtWs[1].y, it->_anklesTgtWs[1].z);
			}
			ImGui::End();
		}
	}//ani
}//nn