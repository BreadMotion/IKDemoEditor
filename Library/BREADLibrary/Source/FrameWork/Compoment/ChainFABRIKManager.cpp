#include "FrameWork/Component/ChainFABRIKManager.h"

using namespace Bread::Math;

namespace Bread
{
	namespace FrameWork
	{
		namespace FABRIK
		{
			s32 LoopNum{ 1 };
		}

		namespace InverseKinematics
		{

			//初回初期化
			void FARBIKManager::Start()
			{

			}

			//初期化
			void FARBIKManager::Initialize()
			{

			}

			//更新
			void FARBIKManager::Update()
			{
				//登録されているFABRIKを計算する
				for (auto& IK : registedIK)
				{
					FABRIK(IK);

					//TODO : ModelObject->UpdateBoneTransform()を呼べ
					for (auto& joint : *IK->setupJoint)
					{
						//joint->;
					}

				}
			}

			//Updateでこれを呼ぶ
			void _fastcall FARBIKManager::FABRIK(std::shared_ptr<SetUp::IKSetUp> setupIK)
			{
				//試行回数分ループする
				for (u32 i = 0; i < FABRIK::LoopNum; ++i)
				{
					FARBIKParentSolver(setupIK);
				}
			}

			//TODO : 複数のエンドエフェクタに対応できるように設計を変える必要がある
			//TODO : 末端のエフェクタの目標座標の取って来かたを考えること
			//FARBIKを用いて脚の座標を目標地点に変えていく
			void __fastcall FARBIKManager::FARBIKParentSolver(std::shared_ptr<SetUp::IKSetUp> setupIK)
			{
				std::vector<Vector3> targetPosition; targetPosition.clear();
				std::vector<Vector3> anserPosition;  anserPosition.clear();
				targetPosition.emplace_back(*setupIK->worldTargetPos);
				anserPosition.emplace_back(GetLocation(setupIK->setupJoint->at(0).worldTransform  * setupIK->rootTransform->GetWorldTransform()));

				//backward
				for (u32 i = 0; i < setupIK->setupJoint->size() - 1; ++i)
				{
					const auto irev { setupIK->setupJoint->size() - 1 - i };
					auto       joint{ setupIK->setupJoint->at(irev)       };

					ForwardCuluculate(targetPosition, joint.parent, &joint, setupIK->rootTransform);
				}

				//forward
				for (u32 i = 0; i < setupIK->setupJoint->size() - 1; ++i)
				{
					auto joint{ setupIK->setupJoint->at(i) };

					BackwardCuluculate(anserPosition, targetPosition.at(i + 1), &joint, joint.child.at(0), setupIK->rootTransform);
				}

				//算出したanserPositionからForward方向にジョイントの角度を変えていく
				for (u32 i = 0; i < setupIK->setupJoint->size() - 1; ++i)
				{
					FABRIKSolver(anserPosition.at(i), &setupIK->setupJoint->at(i), setupIK->setupJoint->at(i).child.at(0), setupIK->rootTransform);
				}
			}

			//子から親に向かっての各ジョイントのワールド座標を算出
			void __fastcall FARBIKManager::ForwardCuluculate(std::vector<Math::Vector3>& targetPosition, ITransform* pEffector,
				ITransform* pCurrent, const std::shared_ptr<Transform> root)
			{
				//pCurrentが子ジョイント、pEffectorが注目ジョイント、rootはモデルを描画する座標、hitCoordinateが　踝の最終座標
				const Vector3 parentPosition        { GetLocation(pEffector->worldTransform * root->GetWorldTransform())                                };
				const f32     effectorToParentLength{ Vector3Length(parentPosition - GetLocation(pCurrent->worldTransform * root->GetWorldTransform())) };

				//親の目標座標を登録
				targetPosition.emplace_back((Vector3Normalize(parentPosition - targetPosition.back()) * effectorToParentLength) + targetPosition.back());
			}

			//親から子に向かっての各ジョイントのワールド座標を算出
			void __fastcall FARBIKManager::BackwardCuluculate(std::vector<Math::Vector3>& anserPosition, const Math::Vector3& targetPosition,
				ITransform* pEffector, ITransform* pCurrent, const std::shared_ptr<Transform> root)
			{
				//ここではpCurrentが子ジョイント、pEffectorが注目ジョイント、rootはモデルを描画する座標、hitCoordinateが踝の最終座標(←これは使う予定ない)
				const Vector3 effectorPosision     { GetLocation(pEffector->worldTransform * root->GetWorldTransform())                                  };
				const f32     effectorToChildLength{ Vector3Length(GetLocation(pCurrent->worldTransform * root->GetWorldTransform()) - effectorPosision) };

				//子供の最終目標座標を登録
				anserPosition.emplace_back((Vector3Normalize(targetPosition - anserPosition.back()) * effectorToChildLength) + anserPosition.back());
			}

			//BackwardCuluculateで算出した座標までのベクトルに添うようにジョイントを回転させる
			void __fastcall FARBIKManager::FABRIKSolver
			(
				const Math::Vector3& anserPosition, ITransform*                      pEffector,
				ITransform*          pCurrent,      const std::shared_ptr<Transform> root
			)
			{
				//TODO : 行列を更新する際,Rootの行列の影響をなくすこと
				// //ここでは
				// //pCurrentが子ジョイント、pEffectorが注目ジョイント、rootはモデルを描画する座標、hitCoordinateが踝の最終座標(←これは使う予定ない)
				// //Backwardで出した目標座標までのベクトルに向かってCCDIKの計算をする
				//ここではBackworkで登録したtargetPositionを後ろから順に使用していく（ここでは　iterate : u32　に見立てて使っていく）

				const Matrix effctorWorld    { pEffector->worldTransform   };
				const Matrix childWorld      { pCurrent->worldTransform    };
				const Matrix invEffectorWorld{ MatrixInverse(effctorWorld) };
				const Matrix invRootworld    { root->GetWorldTransform()   };

				const Vector3 childWorldT   { GetLocation(childWorld)   };
				const Vector3 effectorWorldT{ GetLocation(effctorWorld) };


				Vector3 EFtoChil{ GetLocation(childWorld) - GetLocation(effctorWorld) };
				//Vector3 localChildVec{ Vector3TransformCoord(GetLocation(childWorld) - GetLocation(effctorWorld) , invEffectorWorld) };
				Vector3 localChildVec{ Vector3TransformCoord(GetLocation(childWorld),invEffectorWorld) - Vector3TransformCoord(GetLocation(effctorWorld) , invEffectorWorld) };
				//Vector3 localChildVec{ GetLocation(childWorld) - GetLocation(effctorWorld) };

				Vector3 anserLocalPosition{ (anserPosition - GetLocation(root->GetWorldTransform())) };
				//Vector3 anserLocalPosition{ (Vector3TransformCoord(anserPosition,invRootworld) - GetLocation(root->GetWorldTransform())) };

				Vector3 EFtoAns{ anserLocalPosition - GetLocation(effctorWorld) };
				//Vector3 localTargetVec{ Vector3TransformCoord(EFtoAns, invEffectorWorld) };
				Vector3 localTargetVec{ Vector3TransformCoord(anserLocalPosition, invEffectorWorld) - Vector3TransformCoord(GetLocation(effctorWorld), invEffectorWorld) };


				const Vector3 basisLocalChildVec{ Vector3Normalize(localChildVec) };
				const Vector3 basisLocalTargetVec{ Vector3Normalize(localTargetVec) };

				//二つのローカルベクトルの回転軸を計算
				Vector3 rotationAxis{ /*Vector3Normalize*/(Vector3Cross(basisLocalChildVec ,basisLocalTargetVec)) };

				//回転量も出す
				f32 dot{ acos(Vector3Dot(basisLocalTargetVec,basisLocalChildVec)) };

				if (rotationAxis.y < 0.0f)
				{
					dot *= -1.0f;
				}

				//回転値が少ない場合計算しない
				if (std::abs(dot) > Epsilon && dot < PI)
				{
					//回転軸と回転角度からクォータニオンを生成
					const Vector3    rightVec  { pEffector->rotate.LocalRight()         };
					const Quaternion rotationQt{ QuaternionRotationAxis(rightVec, -dot) };

					//クォータニオン空買い移転行列を生成
					const Matrix rotationMatrix{ MatrixRotationQuaternion(pEffector->rotate * rotationQt) };
					Vector3      euler         { ConvertToRollPitchYawFromRotationMatrix(rotationMatrix)  };

					//注目ジョイントの姿勢を更新
					pEffector->rotate = ConvertToQuaternionFromRollPitchYaw(euler.x, euler.y, euler.z);
				}
				UpdateChildTranslate(pEffector);
			}

			//対象の親のローカル座標を計算する
			void __fastcall FARBIKManager::CulculateParentLocal(const Math::Vector3& basis2EffectDir, const Math::Vector3& basis2TargetDir,
				f32 rotationAngle, ITransform* pCurrent, const std::shared_ptr<Transform> root)
			{

			}

			//対象の角度を計算する
			void __fastcall FARBIKManager::CulculateAngle(ITransform* ankle, ITransform* hip,
				const Math::Vector3& targetPos, Math::Vector3& basis2EffectDir,
				Math::Vector3& basis2TargetDir, f32& rotateAngle, const std::shared_ptr<Transform> root)
			{

			}

			//対象のTrasnformを更新する
			void _fastcall FARBIKManager::UpdateTransform(ITransform* node)
			{
				const Matrix S{ MatrixScaling(node->scale.x,node->scale.y,node->scale.z) },
					R{ MatrixRotationQuaternion(node->rotate) },
					T{ MatrixTranslation(node->translate.x,node->translate.y,node->translate.z) };

				node->localTransform = S * R * T;
				node->worldTransform = node->localTransform * node->parent->worldTransform;
			}
			void _fastcall FARBIKManager::UpdateChildTranslate(ITransform* node)
			{
				const Matrix S{ MatrixScaling(node->scale.x,node->scale.y,node->scale.z) },
					R{ MatrixRotationQuaternion(node->rotate) },
					T{ MatrixTranslation(node->translate.x,node->translate.y,node->translate.z) };

				node->localTransform = S * R * T;
				node->worldTransform = node->localTransform * node->parent->worldTransform;

				for (auto& child : node->child)
				{
					UpdateChildTranslate(child);
				}

			}

			//FootIK Register
			void _fastcall FARBIKManager::RegisterFABRIK(std::vector<IJoint>* model, std::shared_ptr<Transform> rootT, Math::Vector3* targetPos)
			{
				std::shared_ptr<SetUp::IKSetUp> fabrIK{ std::make_shared<SetUp::IKSetUp>() };
				fabrIK->setupJoint     = model;
				fabrIK->rootTransform  = rootT;
				fabrIK->worldTargetPos = targetPos;

				//FootIKの登録
				registedIK.emplace_back(fabrIK);
			}
			void _fastcall FARBIKManager::UnRegisterFABRIK(std::shared_ptr<SetUp::IKSetUp> setupIK)
			{
				std::vector<std::shared_ptr<SetUp::IKSetUp>>::iterator it{ registedIK.begin() };
				while (it != registedIK.end())
				{
					if (*it == setupIK)
					{
						it = registedIK.erase(it);
					}
					else
					{
						++it;
					}
				}
			}

			void FARBIKManager::GUI()
			{
#if 0
				ImGui::Begin("IKManager");
				for (const auto& it : registedIK)
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
#endif
			}
		};
	}
}