#include "FrameWork/Component/HumanFARBIKManager.h"

using namespace Bread::Math;

#define DEBUGGUI_BEGIN ImGui::Begin("FARBIK Function Infomation")
#define DEBUGGUI_END   ImGui::End();

//TODO : FARBIKの実装
namespace Bread
{
	namespace FrameWork
	{
		namespace FootIK
		{
			//足の本数
			constexpr u32 FootNum{ 2 };
			s32 LoopNum{ 1 };
		}

		namespace InverseKinematics
		{
			//初回初期化
			void HumanFARBIKManager::Start()
			{

			}

			//初期化
			void HumanFARBIKManager::Initialize()
			{
			}

			//更新
			void HumanFARBIKManager::Update()
			{
				//登録されているFootIKを計算する
				for (auto footIK : registedFootIK)
				{
					FootIK(footIK);
					footIK->model->UpdateBoneTransform();
				}
			}

			//Updateでこれを呼ぶ
			void _fastcall HumanFARBIKManager::FootIK(std::shared_ptr<FootIK::FootIKSetUp> footIk)
			{
				//レイが当たっていなかった場合スキップ
				for (u32 i = 0; i < FootIK::FootNum; ++i)
				{
					FootIK::LegSetUp& leg{ footIk->legSetup[i] };
					footIk->anklesIniWs[i] = GetLocation(leg.pAnkle->worldTransform * (footIk->rootTransform->GetWorldTransform()));
				}

				//足首の座標を更新(まだ本体は更新しない)
				UpdateAnklesTarget(footIk);

				//骨盤を正しい位置に更新
				UpdatePelvisOffset(footIk);

				//足の数文ループする
				for (u32 i = 0; i < FootIK::FootNum; ++i)
				{
					//レイキャストが当たっていない場合更新しない
					if (!footIk->rayCast[i]->GetHItFlag())
					{
						continue;
					}

					//試行回数文ループする
					for (u32 j = 0; j < FootIK::LoopNum; j++)
					{
						//踝-膝 でFARBIK
						FARBIKParentSolver(footIk, i);
					}
				}
			}

			//踝の目標座標の更新
			void __fastcall HumanFARBIKManager::UpdateAnklesTarget(std::shared_ptr<FootIK::FootIKSetUp> footIk)
			{
				for (u32 i = 0; i < FootIK::FootNum; ++i)
				{
					const RayCastCom::HitResult& ray{ footIk->rayCast[i]->hitResult };
					if (!footIk->rayCast[i]->GetHItFlag())
					{
						//レイが当たっていなければスルーする
						continue;
					}

					//始点と目標座標とのベクトルを求め
					//目標座標のポリゴンの法線との内積を求める
					const f32 ABAngle{ Vector3Dot(ray.start - ray.position, ray.normal) };
					if (ABAngle == 0.0f)
					{
						//ベクトルの角度が無かったら抜ける
						continue;
					}

					//らいを対角線とする長方形の頂点を作る
					const Vector3 B{ ray.start - (ray.normal * ABAngle) };

					//Bと目標座標の距離を求める
					const Vector3 PB { B - ray.position  };
					const f32 PBAngle{ Vector3Length(PB) };

#if 0
					if (PBAngle <= 0.0f)
					{
						//正しい座標を再計算
						footIk->anklesTargetWs[i] = ray.position + ((ray.normal) * footIk->footHeight);
					}
					else
					{
						//タレスの定理を用いて、位置を求める
						footIk->anklesTargetWs[i] = ray.position + (PB * ((PBAngle * footIk->footHeight / ABAngle) / PBAngle));
					}
#else
					//正しい座標を再計算
					footIk->anklesTargetWs[i] = ray.position + ((ray.normal) * footIk->footHeight);
#endif
				}
			}

			//骨盤の目標座標の更新
			void __fastcall HumanFARBIKManager::UpdatePelvisOffset(std::shared_ptr<FootIK::FootIKSetUp> footIk)
			{
				//骨盤の平行移動値を初期化する
				for (u32 i = 0; i < FootIK::FootNum; ++i)
				{
					footIk->pelvisOffset = Vector3::Zero;
				}

				f32 maxDot{ -FLT_MAX };
				if (footIk->pelvisOffset)
				{
					for (u32 i = 0; i < FootIK::FootNum; ++i)
					{
						const RayCastCom::HitResult& ray{ footIk->rayCast[i]->hitResult };
						if (footIk->rayCast[i]->GetHItFlag())
						{
							//レイが当たっていなければスルーする
							continue;
						}

						//今の足首が更新前の足首に比べて低い（下方向に存在するかドウかを確認します）
						const Vector3 ankleToTarget{ footIk->anklesTargetWs[i] - footIk->anklesIniWs[i] };
						const f32 dot{ Vector3Dot(ankleToTarget, FootIK::downVec) };

						if (dot > maxDot)
						{
							maxDot = dot;

							//脚が地面に設置するまでの最大変位を用いてオフセットを計算する
							footIk->pelvisOffset = FootIK::downVec * dot;

							Matrix scale
							{
								MatrixScaling(
									footIk->legSetup[i].pHip->parent->scale.x,
									footIk->legSetup[i].pHip->parent->scale.y,
									footIk->legSetup[i].pHip->parent->scale.z)
							};
							Matrix rotate
							{
								MatrixRotationQuaternion(footIk->legSetup[i].pHip->parent->rotate)
							};
							Matrix translate
							{
								MatrixTranslation(
								footIk->legSetup[i].pHip->parent->translate.x + footIk->pelvisOffset.x,
								footIk->legSetup[i].pHip->parent->translate.y + footIk->pelvisOffset.y,
								footIk->legSetup[i].pHip->parent->translate.z + footIk->pelvisOffset.z
								)
							};

							footIk->legSetup[i].pHip->parent->localTransform = scale * rotate * translate;
							footIk->legSetup[i].pHip->parent->worldTransform
								= footIk->legSetup[i].pHip->parent->localTransform
								* footIk->legSetup[i].pHip->parent->parent->worldTransform;

							UpdateChildTranslate(footIk->legSetup[i].pHip->parent);
						}

					}
				}
			}

			const Math::Matrix& _fastcall HumanFARBIKManager::GetRootTransform(std::shared_ptr<FootIK::FootIKSetUp> footIk)
			{
				const Vector3 rootTranslate{ GetLocation(footIk->rootTransform->GetWorldTransform()) };
				Matrix result
				{
					MatrixRotationRollPitchYaw(footIk->rootYaw,0.0f,0.0f)
					* MatrixTranslation(rootTranslate.x,rootTranslate.y,rootTranslate.z)
				};
#if 0
				result._13 *= -1.0f;
				result._23 *= -1.0f;
				result._33 *= -1.0f;
#endif

				return result;
			}

			//CCDIKを用いて足の座標を目標地点に変えていく
			void __fastcall HumanFARBIKManager::CCDIKSolver(IJoint* pEffector, const Math::Vector3& faceNormal, const Math::Vector3& hitCoordinate, const std::shared_ptr<Transform> root)
			{
				Matrix effectorWorld { pEffector->worldTransform * root->GetWorldTransform()     };
				Vector3 effectorFront{ Vector3Normalize(GetRotation(effectorWorld).LocalFront()) };
				Vector3 axisFront    { Vector3Normalize(Vector3Cross(faceNormal,effectorFront))  };

				f32 dot{ acos(Vector3Dot(effectorFront,faceNormal)) };

				if (std::abs(dot) > Epsilon && dot < PI)
				{
					//クォータニオン空買い移転行列を生成
					const Matrix rotationMatrix{ MatrixRotationQuaternion(pEffector->rotate)             };
					Vector3      euler         { ConvertToRollPitchYawFromRotationMatrix(rotationMatrix) },
						         originEuler   { Vector3::Zero                                           };
					originEuler = euler;

					euler.x = ToDegree(-dot);
					euler   = ClampVector(euler, pEffector->minRot, pEffector->maxRot);
					euler.x = ToRadian(euler.x);

					if (axisFront.y < 0.0f)
					{
						euler.x *= -1.0f;
					}

					//注目ジョイントの姿勢を更新
					pEffector->rotate = ConvertToQuaternionFromRollPitchYaw(euler.x, originEuler.y, originEuler.z);
					UpdateChildTranslate(pEffector);
				}
			}

			//TODO : 実装中(後で整理すること)
			void __fastcall HumanFARBIKManager::FARBIKParentSolver(std::shared_ptr<FootIK::FootIKSetUp> footIk, const u32 iterate)
			{
				std::vector<Vector3> targetPosition; targetPosition.clear();
				std::vector<Vector3> anserPosition;  anserPosition.clear();
				targetPosition.emplace_back(footIk->anklesTargetWs[iterate]);
				anserPosition.emplace_back(GetLocation(footIk->legSetup[iterate].pHip->worldTransform * footIk->rootTransform->GetWorldTransform()));

				//足首が地面の高さに合うように計算
				ExceptionCuluculate_HingeJoint(targetPosition, anserPosition, footIk->legSetup[iterate], footIk->rootTransform);

				//足首を地面に添わせる
				//CCDIKSolver(footIk->legSetup[iterate].pAnkle, footIk->rayCast[iterate]->hitResult.normal, footIk->anklesTargetWs[iterate], footIk->rootTransform);
			}

			//末端からRootに向かってのジョイントの目標座標の登録
			void __fastcall HumanFARBIKManager::ForwardCuluculate(
				std::vector<Vector3>& targetPosition,
				IJoint* pEffector,
				IJoint* pCurrent,
				const std::shared_ptr<Transform> root)
			{
				//pCurrentが子ジョイント、pEffectorが注目ジョイント、rootはモデルを描画する座標、hitCoordinateが　踝の最終座標
				const Vector3 parentPosition        { GetLocation(pEffector->worldTransform * root->GetWorldTransform())                                };
				const f32     effectorToParentLength{ Vector3Length(parentPosition - GetLocation(pCurrent->worldTransform * root->GetWorldTransform())) };

				//親の目標座標を登録
				targetPosition.emplace_back((Vector3Normalize(parentPosition - targetPosition.back()) * effectorToParentLength) + targetPosition.back());
			}

			//TODO : 完成後この関数内のGUIを消したのち、ローカル変数を定数化
			//次更新ジョイントの自由度がヒンジだった場合使用するFABRIK
			void __fastcall HumanFARBIKManager::ExceptionCuluculate_HingeJoint(
				std::vector<Math::Vector3>& targetPosition,
				std::vector<Math::Vector3>& anserPosition,
				FootIK::LegSetUp& footIk,
				const std::shared_ptr<Transform>     root)
			{
				//ヘロンの公式を使って股関節と足首の回転角度を出す
				//使う3辺 = Hip->Knee , Knee->TargetPos , TargetPos->Hip
				//ワールド上の座標はそれぞれすでに出ているため、角度、面積、��(HIP,(Hip＋ΔTarget),Knee)成分が出る
				//股関節のY軸周りの回転角度は保持される（計算前の値を保持)
				//https://keisan.casio.jp/exec/system/1209543011

				//三角形を構成する各頂点の座標
				Vector3 TargetPosition    { targetPosition.back()                                                  };
				Vector3 AnkleWorldPosition{ GetLocation(footIk.pAnkle->worldTransform * root->GetWorldTransform()) };
				Vector3 HipWorldPosition  { GetLocation(footIk.pHip->worldTransform   * root->GetWorldTransform()) };
				Vector3 KneeWorldPosition { GetLocation(footIk.pKnee->worldTransform  * root->GetWorldTransform()) };

				// A辺の長さ(TargetToHipLength)	: B辺の長さ(KneeToAnkleLength) : C辺の長さ(HipToKneeLength)
				f32 A{ Vector3Length(TargetPosition    - HipWorldPosition)   };
				f32 B{ Vector3Length(KneeWorldPosition - AnkleWorldPosition) };
				f32 C{ Vector3Length(HipWorldPosition  - KneeWorldPosition)  };

				//目標座標までの長さが脚の長さを超えるか、０以下の場合処理はしない
				if (A > B + C || A <= 0.0f)
				{
					return;
				}

				//ヘロンの定理で各角度を算出
				//Angle( x :Hipの角度 , y : KneeのAangle , z : TargetのAngle
				Vector3 angle{ f32Angle_HeronFormula(A, B, C) };

				struct LocalFunction
				{
					static void  __fastcall RotateUpdate(IJoint* joint, const f32& angle, const s32& reverseFlag)
					{
						//オイラー(X成分)以外は弄らない
						Vector3 eulerForCompute, eulerForOrigin;
						eulerForCompute = ConvertToRollPitchYawFromRotationMatrix(MatrixRotationQuaternion(joint->rotate));
						eulerForOrigin  = eulerForCompute;//保存

						// 0 < val < 180 に変換
						switch (reverseFlag)
						{
						case 0:
							eulerForCompute.x += -std::fabsf(ToDegree(angle));
							break;
						case 1:
							eulerForCompute.x += 180.0f - std::fabsf(ToDegree(angle));
							break;
						case 2:
							eulerForCompute.x += std::fabsf(ToDegree(angle));
							break;
						}

						//角度制限（オイラー）
						eulerForCompute = ClampVector(eulerForCompute, joint->minRot, joint->maxRot);

						// 0 < val < π に変換
						eulerForCompute.x = ToRadian(eulerForCompute.x);

						//回転量X成分のみ反映、Y,Zは残す
						joint->rotate = ConvertToQuaternionFromRollPitchYaw(eulerForCompute.x, -eulerForOrigin.y, -eulerForOrigin.z);
					}
				};
				LocalFunction::RotateUpdate(footIk.pHip,  angle.x, 0);
				LocalFunction::RotateUpdate(footIk.pKnee, angle.y, 1);
				LocalFunction::RotateUpdate(footIk.pAnkle,angle.z, 0);
				UpdateChildTranslate(footIk.pHip);
			}

			//Rootから末端に向かってのジョイントの目標座標の登録
			void __fastcall HumanFARBIKManager::BackwardCuluculate(
				std::vector<Vector3>& anserPosition,
				const Vector3& targetPosition,
				IJoint* pEffector,
				IJoint* pCurrent,
				const std::shared_ptr<Transform> root)
			{
				//ここではpCurrentが子ジョイント、pEffectorが注目ジョイント、rootはモデルを描画する座標、hitCoordinateが踝の最終座標(←これは使う予定ない)
				const Vector3 effectorPosision     { GetLocation(pEffector->worldTransform * root->GetWorldTransform())                                  };
				const f32     effectorToChildLength{ Vector3Length(GetLocation(pCurrent->worldTransform * root->GetWorldTransform()) - effectorPosision) };

				//子供の最終目標座標を登録
				anserPosition.emplace_back((Vector3Normalize(targetPosition - anserPosition.back()) * effectorToChildLength) + anserPosition.back());
			}

			//TODO : 調整中(後でGUIけせ)
			void __fastcall HumanFARBIKManager::IKSolver(
				const Math::Vector3& anserPosition,
				IJoint* pEffector,
				IJoint* pCurrent,
				const std::shared_ptr<Transform> root)
			{
				//TODO : 行列を更新する際,Rootの行列の影響をなくすこと
				//ここでは
				//pCurrentが子ジョイント、pEffectorが注目ジョイント、rootはモデルを描画する座標、hitCoordinateが踝の最終座標(←これは使う予定ない)
				//Backwardで出した目標座標までのベクトルに向かってCCDIKの計算をする
				//ここではBackworkで登録したtargetPositionを後ろから順に使用していく（ここでは　iterate : u32　に見立てて使っていく）

				DEBUGGUI_BEGIN;
				ImGui::Separator();
				ImGui::Text((pEffector->name + "IKSolver").c_str());

				const Matrix effctorWorld{ pEffector->worldTransform };
				const Matrix childWorld{ pCurrent->worldTransform };
				const Matrix invEffectorWorld{ MatrixInverse(effctorWorld) };
				const Matrix invRootworld{ root->GetWorldTransform() };

				Vector3 childWorldT{ GetLocation(childWorld) };
				Vector3 effectorWorldT{ GetLocation(effctorWorld) };

				ImGui::DragFloat3("childLocalT", pCurrent->translate);
				ImGui::DragFloat3("childWorldT", childWorldT);
				ImGui::DragFloat3("effectorLocalT", pEffector->translate);
				ImGui::DragFloat3("effectorWorldT", effectorWorldT);

				Vector3 EFtoChil{ GetLocation(childWorld) - GetLocation(effctorWorld) };
				//Vector3 localChildVec{ Vector3TransformCoord(GetLocation(childWorld) - GetLocation(effctorWorld) , invEffectorWorld) };
				Vector3 localChildVec{ Vector3TransformCoord(GetLocation(childWorld),invEffectorWorld) - Vector3TransformCoord(GetLocation(effctorWorld) , invEffectorWorld) };
				//Vector3 localChildVec{ GetLocation(childWorld) - GetLocation(effctorWorld) };


				ImGui::DragFloat3("EFtoChil", EFtoChil);
				ImGui::DragFloat3("localChildVec", localChildVec);
				ImGui::Separator();

				Vector3 anserLocalPosition{ (anserPosition - GetLocation(root->GetWorldTransform())) };
				//Vector3 anserLocalPosition{ (Vector3TransformCoord(anserPosition,invRootworld) - GetLocation(root->GetWorldTransform())) };
				ImGui::DragFloat3("AnserLocalPosition", anserLocalPosition);
				ImGui::DragFloat3("effectorLocalT", pEffector->translate);
				ImGui::DragFloat3("effectorWorldT", effectorWorldT);

				Vector3 EFtoAns{ anserLocalPosition - GetLocation(effctorWorld) };
				//Vector3 localTargetVec{ Vector3TransformCoord(EFtoAns, invEffectorWorld) };
				Vector3 localTargetVec{ Vector3TransformCoord(anserLocalPosition, invEffectorWorld) - Vector3TransformCoord(GetLocation(effctorWorld), invEffectorWorld) };

				ImGui::DragFloat3("EFtoAns", EFtoAns);
				ImGui::DragFloat3("localTargetVec", localTargetVec);

				const Vector3 basisLocalChildVec{ Vector3Normalize(localChildVec) };
				const Vector3 basisLocalTargetVec{ Vector3Normalize(localTargetVec) };

				//二つのローカルベクトルの回転軸を計算
				Vector3 rotationAxis{ /*Vector3Normalize*/(Vector3Cross(basisLocalChildVec ,basisLocalTargetVec)) };

				ImGui::DragFloat3("rotationAxis", rotationAxis);

				//回転量も出す
				f32 dot{ acos(Vector3Dot(basisLocalTargetVec,basisLocalChildVec)) };

				if (rotationAxis.y < 0.0f)
				{
					dot *= -1.0f;
				}

				ImGui::DragFloat("dot", &dot);

				//回転値が少ない場合計算しない
				if (std::abs(dot) > Epsilon && dot < PI)
				{
					//回転軸と回転角度からクォータニオンを生成
					const Vector3 rightVec{ pEffector->rotate.LocalRight() };
					const Quaternion rotationQt{ QuaternionRotationAxis(rightVec, -dot) };

					//クォータニオン空買い移転行列を生成
					const Matrix rotationMatrix{ MatrixRotationQuaternion(pEffector->rotate * rotationQt) };
					Vector3 euler{ ConvertToRollPitchYawFromRotationMatrix(rotationMatrix) };

					//TODO : FABRIKは角度制限は円錐でしかできない(修正箇所)
#if 1
					euler.x = ToDegree(euler.x);
					euler.y = ToDegree(euler.y);
					euler.z = ToDegree(euler.z);

					euler.x = std::fabsf(euler.x);
					euler = ClampVector(euler, pEffector->minRot, pEffector->maxRot);

					euler.x = ToRadian(euler.x);
					euler.y = ToRadian(euler.y);
					euler.z = ToRadian(euler.z);
#endif

					ImGui::DragFloat3("euler", euler);

					//注目ジョイントの姿勢を更新
					pEffector->rotate = ConvertToQuaternionFromRollPitchYaw(euler.x, euler.y, euler.z);
				}
				UpdateChildTranslate(pEffector);
				ImGui::Separator();
				DEBUGGUI_END;
			}

			void __fastcall HumanFARBIKManager::IKSolver2(std::vector<Math::Vector3> anserPosition, std::shared_ptr<FootIK::FootIKSetUp> footIk, const u32 iterate)
			{
				//DEBUGGUI_BEGIN;
				//ImGui::Separator();
				//ImGui::Text((pEffector->name + "IKSolver").c_str());

				auto foot{ &footIk->legSetup[iterate] };

				const Matrix HipWorld{ foot->pHip->worldTransform };
				const Matrix KneeWorld{ foot->pKnee->worldTransform };
				const Matrix AnkleWorld{ foot->pAnkle->worldTransform };

				const Matrix invHipWorld{ MatrixInverse(HipWorld) };
				const Matrix invKneeWorld{ MatrixInverse(KneeWorld) };
				const Matrix invRootworld{ footIk->rootTransform->GetWorldTransform() };

				//Hip -> knee
				{
					Vector3 HipToKnee{ GetLocation(KneeWorld) - GetLocation(HipWorld) };
					//Vector3 localHipToKnee{ Vector3TransformCoord(HipToKnee ,invHipWorld)  };
					Vector3 localHipToKnee{ Vector3TransformCoord(GetLocation(KneeWorld) ,invHipWorld) - Vector3TransformCoord(GetLocation(HipWorld) ,invHipWorld) };
					//Vector3 localChildVec{ GetLocation(childWorld) - GetLocation(effctorWorld) };

					Vector3 HipToAns{
						(anserPosition[1] - GetLocation(footIk->rootTransform->GetWorldTransform())) - GetLocation(HipWorld) };

					//Vector3 localHipToAns{ Vector3TransformCoord(HipToAns,invHipWorld) };
					Vector3 localHipToAns{ Vector3TransformCoord(anserPosition[1] - GetLocation(footIk->rootTransform->GetWorldTransform()),invHipWorld)
						- Vector3TransformCoord(GetLocation(HipWorld) ,invHipWorld) };
					//Vector3 localTargetVec{ (anserPosition - GetLocation(root->GetWorldTransform())) - GetLocation(effctorWorld) };

					const Vector3 basisLocalChildVec{ Vector3Normalize(localHipToKnee) };
					const Vector3 basisLocalTargetVec{ Vector3Normalize(localHipToAns) };

					//二つのローカルベクトルの回転軸を計算
					Vector3 rotationAxis{ /*Vector3Normalize*/(Vector3Cross(basisLocalChildVec ,basisLocalTargetVec)) };

					//回転量も出す
					f32 dot{ acos(Vector3Dot(basisLocalTargetVec,basisLocalChildVec)) };

					//外積が逆向きだった場合acosの値の符号も反転する
					if (rotationAxis.y < 0.0f)
					{
						dot *= -1.0f;
					}

					//回転値が少ない場合計算しない
					if (std::fabsf(dot) > Epsilon && dot < PI)
					{
						//TODO : 余剰計算の仕組みをFABRIK1でもできるように実装しろ
						//Knee -> Ankleの横方向のずれの修正をHipが担う(CCDIKの時の余剰計算)
						//Knee->Ankle　と Knee->AnsTの外積と内積
						const Vector3 kneeToAnkleWorld{ (GetLocation(foot->pAnkle->worldTransform) - GetLocation(foot->pKnee->worldTransform)) };
						const Vector3 kneeToAnsWorld{ (anserPosition[2] - GetLocation(footIk->rootTransform->GetWorldTransform())) - GetLocation(foot->pKnee->worldTransform) };

						//回転軸と回転量を算出
						const Vector3 extraRotationAxis{ Vector3Cross(kneeToAnkleWorld,kneeToAnsWorld) };
						f32           extraDot{ acos(Vector3Dot(Vector3Normalize(kneeToAnsWorld),Vector3Normalize(kneeToAnkleWorld))) };

						//外積が逆向きだった場合acosの値の符号を反転する
						if (extraRotationAxis.y < 0.0f)
						{
							extraDot *= -1.0f;
						}

						//回転軸と回転角度からクォータニオンを生成
						const Vector3    rightVec{ foot->pHip->rotate.LocalRight() };
						const Quaternion rotationQt{ QuaternionRotationAxis(rightVec, -dot) };

						//クォータニオン空買い移転行列を生成
						const Matrix rotationMatrix{ MatrixRotationQuaternion(foot->pHip->rotate * rotationQt /** extraRotationQt*/) };
						Vector3 euler{ ConvertToRollPitchYawFromRotationMatrix(rotationMatrix) };

						//TODO : FABRIKは角度制限は円錐でしかできない(修正箇所)
#if 1
						euler.x = ToDegree(euler.x);
						euler.y = ToDegree(euler.y);
						euler.z = ToDegree(euler.z);

						euler.x = -std::fabsf(euler.x);
						euler = ClampVector(euler, foot->pHip->minRot, foot->pHip->maxRot);

						euler.x = ToRadian(euler.x);
						euler.y = ToRadian(euler.y);
						euler.z = ToRadian(euler.z);
#endif

						//注目ジョイントの姿勢を更新
						foot->pHip->rotate = ConvertToQuaternionFromRollPitchYaw(euler.x, euler.y, euler.z);
						UpdateChildTranslate(foot->pHip);
					}
				}
				//ImGui::Separator();
				//DEBUGGUI_END;
			}


			//対象の親のローカル座標を計算する
			void __fastcall HumanFARBIKManager::CulculateParentLocal(const Math::Vector3& basis2EffectDir, const Math::Vector3& basis2TargetDir, f32 rotationAngle, IJoint* pCurrent, const std::shared_ptr<Transform> root)
			{

			}

			//対象の角度を計算する
			void __fastcall HumanFARBIKManager::CulculateAngle(IJoint* ankle, IJoint* hip, const Math::Vector3& targetPos, Math::Vector3& basis2EffectDir, Math::Vector3& basis2TargetDir, f32& rotateAngle, const std::shared_ptr<Transform> root)
			{

			}

			//対象のTrasnformを更新する
			void _fastcall HumanFARBIKManager::UpdateTransform(ITransform* node)
			{
				const Matrix S{ MatrixScaling(node->scale.x,node->scale.y,node->scale.z)                 },
					         R{ MatrixRotationQuaternion(node->rotate)                                   },
					         T{ MatrixTranslation(node->translate.x,node->translate.y,node->translate.z) };

				node->localTransform = S * R * T;
				node->worldTransform = node->localTransform * node->parent->worldTransform;
			}
			void _fastcall HumanFARBIKManager::UpdateChildTranslate(ITransform* node)
			{
				const Matrix S{ MatrixScaling(node->scale.x,node->scale.y,node->scale.z)        },
					R{ MatrixRotationQuaternion(node->rotate)                                   },
					T{ MatrixTranslation(node->translate.x,node->translate.y,node->translate.z) };

				node->localTransform = S * R * T;
				node->worldTransform = node->localTransform * node->parent->worldTransform;

				for (auto& child : node->child)
				{
					UpdateChildTranslate(child);
				}
			}

			//FootIK Register
			void _fastcall HumanFARBIKManager::RegisterFootIK(std::shared_ptr<ModelObject> model, std::shared_ptr<Transform> rootT, const std::shared_ptr<RayCastCom> rayCast[2])
			{
				//ModelComponentかTransformComponentが無い場合早期リターン
				if (!model && rootT)
				{
					return;
				}

				std::shared_ptr<FootIK::FootIKSetUp> footIk{ std::make_shared<FootIK::FootIKSetUp>() };
				footIk->model = model;
				footIk->rootTransform = rootT;

#if 0
				footIk->legSetup[0].pHip   = model->GetNodeFromName("Zombie:LeftUpLeg");
				footIk->legSetup[0].pKnee  = model->GetNodeFromName("Zombie:LeftLeg");
				footIk->legSetup[0].pAnkle = model->GetNodeFromName("Zombie:LeftFoot");

				footIk->legSetup[1].pHip   = model->GetNodeFromName("Zombie:RightUpLeg");
				footIk->legSetup[1].pKnee  = model->GetNodeFromName("Zombie:RightLeg");
				footIk->legSetup[1].pAnkle = model->GetNodeFromName("Zombie:RightFoot");

#else
				footIk->legSetup[0].pHip   = model->GetNodeFromName("LeftUpLeg");
				footIk->legSetup[0].pKnee  = model->GetNodeFromName("LeftLeg");
				footIk->legSetup[0].pAnkle = model->GetNodeFromName("LeftFoot");

				footIk->legSetup[1].pHip   = model->GetNodeFromName("RightUpLeg");
				footIk->legSetup[1].pKnee  = model->GetNodeFromName("RightLeg");
				footIk->legSetup[1].pAnkle = model->GetNodeFromName("RightFoot");

#endif
				footIk->rayCast[0] = rayCast[0];
				footIk->rayCast[1] = rayCast[1];


				for (u32 i = 0; i < FootIK::FootNum; i++)
				{
					if (i == 0)
					{
						footIk->legSetup[i].pHip->minRot = { -110.0f, -0.0f, 0.0f  };
						footIk->legSetup[i].pHip->maxRot = { 110.0f,    0.0f, 0.0f };
					}
					else
					{
						footIk->legSetup[i].pHip->minRot = { -110.0f, -0.0f,0.0f };
						footIk->legSetup[i].pHip->maxRot = { 110.0f,  0.0f, 0.0f };
					}

					footIk->legSetup[i].pKnee->minRot  = { 0.0f ,   0.0f, 0.0f };
					footIk->legSetup[i].pKnee->maxRot  = { 150.0f,  0.0f, 0.0f };

					footIk->legSetup[i].pAnkle->minRot = { -45.0f, 0.0f, 0.0f };
					footIk->legSetup[i].pAnkle->maxRot = { 45.0f, 0.0f, 0.0f  };
				}

				//FootIKの登録
				registedFootIK.emplace_back(footIk);
			}
			void _fastcall HumanFARBIKManager::UnRegisterFootIK(std::shared_ptr<FootIK::FootIKSetUp> footIk)
			{

				std::vector<std::shared_ptr < FootIK::FootIKSetUp>>::iterator it{ registedFootIK.begin() };
				while (it != registedFootIK.end())
				{
					if (*it == footIk)
					{
						it = registedFootIK.erase(it);
					}
					else
					{
						++it;
					}
				}
			}

			void HumanFARBIKManager::GUI()
			{
				ImGui::Begin("IKManager");
				for (const auto& it : registedFootIK)
				{
					//if (ImGui::BeginPopup("bone info"))
					//{

					Vector3 euler{ };
					{//left
						{
							ImGui::DragFloat4(it->legSetup[0].pHip->name.c_str(), it->legSetup[0].pHip->rotate);
							euler = ConvertToRollPitchYawFromQuaternion(it->legSetup[0].pHip->rotate);
							ImGui::DragFloat3(it->legSetup[0].pHip->name.c_str(), euler);
							ImGui::Separator();
						}
						{
							ImGui::DragFloat4(it->legSetup[0].pKnee->name.c_str(), it->legSetup[0].pKnee->rotate);
							euler = ConvertToRollPitchYawFromQuaternion(it->legSetup[0].pKnee->rotate);
							ImGui::DragFloat3(it->legSetup[0].pKnee->name.c_str(), euler);
							ImGui::Separator();
						}
						{
							ImGui::DragFloat4(it->legSetup[0].pAnkle->name.c_str(), it->legSetup[0].pAnkle->rotate);
							euler = ConvertToRollPitchYawFromQuaternion(it->legSetup[0].pAnkle->rotate);
							ImGui::DragFloat3(it->legSetup[0].pAnkle->name.c_str(), euler);
							ImGui::Separator();
						}
					}
					{//right
						{
							ImGui::DragFloat4("rotate", it->legSetup[1].pHip->rotate);
							euler = ConvertToRollPitchYawFromQuaternion(it->legSetup[1].pHip->rotate);
							ImGui::DragFloat3(it->legSetup[1].pHip->name.c_str(), euler);
							ImGui::Separator();
						}
						{
							ImGui::DragFloat4("rotate", it->legSetup[1].pKnee->rotate);
							euler = ConvertToRollPitchYawFromQuaternion(it->legSetup[1].pKnee->rotate);
							ImGui::DragFloat3(it->legSetup[1].pKnee->name.c_str(), euler);
							ImGui::Separator();
						}
						{
							ImGui::DragFloat4("rotate", it->legSetup[1].pAnkle->rotate);
							euler = ConvertToRollPitchYawFromQuaternion(it->legSetup[1].pAnkle->rotate);
							ImGui::DragFloat3(it->legSetup[1].pAnkle->name.c_str(), euler);
							ImGui::Separator();
						}
					}
					//ImGui::EndPopup();
				//}

					ImGui::DragFloat3("raycast point     right", it->rayCast[1]->hitResult.position);
					ImGui::DragFloat3("raycast normal    right", it->rayCast[1]->hitResult.normal);
					ImGui::DragFloat3("raycast start     right", it->rayCast[1]->hitResult.start);
					ImGui::DragFloat3("raycast end       right", it->rayCast[1]->hitResult.end);
					ImGui::DragFloat("raycast distance   right", &it->rayCast[1]->hitResult.distance);
					ImGui::DragFloat3("raycast point     left", it->rayCast[0]->hitResult.position);
					ImGui::DragFloat3("raycast normal    left", it->rayCast[0]->hitResult.normal);
					ImGui::DragFloat3("raycast start     left", it->rayCast[0]->hitResult.start);
					ImGui::DragFloat3("raycast end       left", it->rayCast[0]->hitResult.end);
					ImGui::DragFloat("raycast distance   left", &it->rayCast[0]->hitResult.distance);

					ImGui::Separator();

					bool hitCheck{ it->rayCast[0]->GetHItFlag() };
					ImGui::Checkbox("0", &hitCheck); ImGui::SameLine();
					ImGui::Text("ankle0 : x.%f,y.%f,z.%f", it->anklesTargetWs[0].x, it->anklesTargetWs[0].y, it->anklesTargetWs[0].z);

					hitCheck = it->rayCast[1]->GetHItFlag();
					ImGui::Checkbox("1", &hitCheck); ImGui::SameLine();
					ImGui::Text("ankle1 : x.%f,y.%f,z.%f", it->anklesTargetWs[1].x, it->anklesTargetWs[1].y, it->anklesTargetWs[1].z);

					ImGui::DragInt("iterate", &FootIK::LoopNum, 1.0f, 1, 100);
				}
				ImGui::End();
			}
		}//namespace InverseKinematics
	}//namespace FrameWork
}//namespace Bread