#include "FrameWork/Component/HumanFARBIKManager.h"

using namespace Bread::Math;

#define DEBUGGUI_BEGIN ImGui::Begin("FARBIK Function Infomation")
#define DEBUGGUI_END   ImGui::End();

//TODO : FARBIK�̎���
namespace Bread
{
	namespace FrameWork
	{
		namespace FootIK
		{
			//���̖{��
			constexpr u32 FootNum{ 2 };
			s32 LoopNum{ 1 };
		}

		namespace InverseKinematics
		{
			//���񏉊���
			void HumanFARBIKManager::Start()
			{

			}

			//������
			void HumanFARBIKManager::Initialize()
			{
			}

			//�X�V
			void HumanFARBIKManager::Update()
			{
				//�o�^����Ă���FootIK���v�Z����
				for (auto footIK : registedFootIK)
				{
					FootIK(footIK);
					footIK->model->UpdateBoneTransform();
				}
			}

			//Update�ł�����Ă�
			void _fastcall HumanFARBIKManager::FootIK(std::shared_ptr<FootIK::FootIKSetUp> footIk)
			{
				//���C���������Ă��Ȃ������ꍇ�X�L�b�v
				for (u32 i = 0; i < FootIK::FootNum; ++i)
				{
					FootIK::LegSetUp& leg{ footIk->legSetup[i] };
					footIk->anklesIniWs[i] = GetLocation(leg.pAnkle->worldTransform * (footIk->rootTransform->GetWorldTransform()));
				}

				//����̍��W���X�V(�܂��{�͍̂X�V���Ȃ�)
				UpdateAnklesTarget(footIk);

				//���Ղ𐳂����ʒu�ɍX�V
				UpdatePelvisOffset(footIk);

				//���̐������[�v����
				for (u32 i = 0; i < FootIK::FootNum; ++i)
				{
					//���C�L���X�g���������Ă��Ȃ��ꍇ�X�V���Ȃ�
					if (!footIk->rayCast[i]->GetHItFlag())
					{
						continue;
					}

					//���s�񐔕����[�v����
					for (u32 j = 0; j < FootIK::LoopNum; j++)
					{
						//��-�G ��FARBIK
						FARBIKParentSolver(footIk, i);
					}
				}
			}

			//��̖ڕW���W�̍X�V
			void __fastcall HumanFARBIKManager::UpdateAnklesTarget(std::shared_ptr<FootIK::FootIKSetUp> footIk)
			{
				for (u32 i = 0; i < FootIK::FootNum; ++i)
				{
					const RayCastCom::HitResult& ray{ footIk->rayCast[i]->hitResult };
					if (!footIk->rayCast[i]->GetHItFlag())
					{
						//���C���������Ă��Ȃ���΃X���[����
						continue;
					}

					//�n�_�ƖڕW���W�Ƃ̃x�N�g��������
					//�ڕW���W�̃|���S���̖@���Ƃ̓��ς����߂�
					const f32 ABAngle{ Vector3Dot(ray.start - ray.position, ray.normal) };
					if (ABAngle == 0.0f)
					{
						//�x�N�g���̊p�x�����������甲����
						continue;
					}

					//�炢��Ίp���Ƃ��钷���`�̒��_�����
					const Vector3 B{ ray.start - (ray.normal * ABAngle) };

					//B�ƖڕW���W�̋��������߂�
					const Vector3 PB { B - ray.position  };
					const f32 PBAngle{ Vector3Length(PB) };

#if 0
					if (PBAngle <= 0.0f)
					{
						//���������W���Čv�Z
						footIk->anklesTargetWs[i] = ray.position + ((ray.normal) * footIk->footHeight);
					}
					else
					{
						//�^���X�̒藝��p���āA�ʒu�����߂�
						footIk->anklesTargetWs[i] = ray.position + (PB * ((PBAngle * footIk->footHeight / ABAngle) / PBAngle));
					}
#else
					//���������W���Čv�Z
					footIk->anklesTargetWs[i] = ray.position + ((ray.normal) * footIk->footHeight);
#endif
				}
			}

			//���Ղ̖ڕW���W�̍X�V
			void __fastcall HumanFARBIKManager::UpdatePelvisOffset(std::shared_ptr<FootIK::FootIKSetUp> footIk)
			{
				//���Ղ̕��s�ړ��l������������
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
							//���C���������Ă��Ȃ���΃X���[����
							continue;
						}

						//���̑��񂪍X�V�O�̑���ɔ�ׂĒႢ�i�������ɑ��݂��邩�h�E�����m�F���܂��j
						const Vector3 ankleToTarget{ footIk->anklesTargetWs[i] - footIk->anklesIniWs[i] };
						const f32 dot{ Vector3Dot(ankleToTarget, FootIK::downVec) };

						if (dot > maxDot)
						{
							maxDot = dot;

							//�r���n�ʂɐݒu����܂ł̍ő�ψʂ�p���ăI�t�Z�b�g���v�Z����
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

			//CCDIK��p���đ��̍��W��ڕW�n�_�ɕς��Ă���
			void __fastcall HumanFARBIKManager::CCDIKSolver(ModelObject::Node* pEffector, const Math::Vector3& faceNormal, const Math::Vector3& hitCoordinate, const std::shared_ptr<Transform> root)
			{
				Matrix effectorWorld { pEffector->worldTransform * root->GetWorldTransform()     };
				Vector3 effectorFront{ Vector3Normalize(GetRotation(effectorWorld).LocalFront()) };
				Vector3 axisFront    { Vector3Normalize(Vector3Cross(faceNormal,effectorFront))  };

				f32 dot{ acos(Vector3Dot(effectorFront,faceNormal)) };

				if (std::abs(dot) > Epsilon && dot < PI)
				{
					//�N�H�[�^�j�I���󔃂��ړ]�s��𐶐�
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

					//���ڃW���C���g�̎p�����X�V
					pEffector->rotate = ConvertToQuaternionFromRollPitchYaw(euler.x, originEuler.y, originEuler.z);
					UpdateChildTranslate(pEffector);
				}
			}

			//TODO : ������(��Ő������邱��)
			void __fastcall HumanFARBIKManager::FARBIKParentSolver(std::shared_ptr<FootIK::FootIKSetUp> footIk, const u32 iterate)
			{
				std::vector<Vector3> targetPosition; targetPosition.clear();
				std::vector<Vector3> anserPosition;  anserPosition.clear();
				targetPosition.emplace_back(footIk->anklesTargetWs[iterate]);
				anserPosition.emplace_back(GetLocation(footIk->legSetup[iterate].pHip->worldTransform * footIk->rootTransform->GetWorldTransform()));

				//���񂪒n�ʂ̍����ɍ����悤�Ɍv�Z
				ExceptionCuluculate_HingeJoint(targetPosition, anserPosition, footIk->legSetup[iterate], footIk->rootTransform);

				//�����n�ʂɓY�킹��
				//CCDIKSolver(footIk->legSetup[iterate].pAnkle, footIk->rayCast[iterate]->hitResult.normal, footIk->anklesTargetWs[iterate], footIk->rootTransform);
			}

			//���[����Root�Ɍ������ẴW���C���g�̖ڕW���W�̓o�^
			void __fastcall HumanFARBIKManager::ForwardCuluculate(
				std::vector<Vector3>& targetPosition,
				ModelObject::Node* pEffector,
				ModelObject::Node* pCurrent,
				const std::shared_ptr<Transform> root)
			{
				//pCurrent���q�W���C���g�ApEffector�����ڃW���C���g�Aroot�̓��f����`�悷����W�AhitCoordinate���@��̍ŏI���W
				const Vector3 parentPosition{ GetLocation(pEffector->worldTransform * root->GetWorldTransform()) };
				const f32     effectorToParentLength{ Vector3Length(parentPosition - GetLocation(pCurrent->worldTransform * root->GetWorldTransform())) };

				//�e�̖ڕW���W��o�^
				targetPosition.emplace_back((Vector3Normalize(parentPosition - targetPosition.back()) * effectorToParentLength) + targetPosition.back());
			}

			//TODO : �����ケ�̊֐�����GUI���������̂��A���[�J���ϐ���萔��
			//���X�V�W���C���g�̎��R�x���q���W�������ꍇ�g�p����FABRIK
			void __fastcall HumanFARBIKManager::ExceptionCuluculate_HingeJoint(
				std::vector<Math::Vector3>& targetPosition,
				std::vector<Math::Vector3>& anserPosition,
				FootIK::LegSetUp& footIk,
				const std::shared_ptr<Transform>     root)
			{
				//�w�����̌������g���ČҊ֐߂Ƒ���̉�]�p�x���o��
				//�g��3�� = Hip->Knee , Knee->TargetPos , TargetPos->Hip
				//���[���h��̍��W�͂��ꂼ�ꂷ�łɏo�Ă��邽�߁A�p�x�A�ʐρA��(HIP,(Hip�{��Target),Knee)�������o��
				//�Ҋ֐߂�Y������̉�]�p�x�͕ێ������i�v�Z�O�̒l��ێ�)
				//https://keisan.casio.jp/exec/system/1209543011

				//�O�p�`���\������e���_�̍��W
				Vector3 TargetPosition    { targetPosition.back()                                                  };
				Vector3 AnkleWorldPosition{ GetLocation(footIk.pAnkle->worldTransform * root->GetWorldTransform()) };
				Vector3 HipWorldPosition  { GetLocation(footIk.pHip->worldTransform   * root->GetWorldTransform()) };
				Vector3 KneeWorldPosition { GetLocation(footIk.pKnee->worldTransform  * root->GetWorldTransform()) };

				// A�ӂ̒���(TargetToHipLength)	: B�ӂ̒���(KneeToAnkleLength) : C�ӂ̒���(HipToKneeLength)
				f32 A{ Vector3Length(TargetPosition    - HipWorldPosition)   };
				f32 B{ Vector3Length(KneeWorldPosition - AnkleWorldPosition) };
				f32 C{ Vector3Length(HipWorldPosition  - KneeWorldPosition)  };

				//�ڕW���W�܂ł̒������r�̒����𒴂��邩�A�O�ȉ��̏ꍇ�����͂��Ȃ�
				if (A > B + C || A <= 0.0f)
				{
					return;
				}

				//�w�����̒藝�Ŋe�p�x���Z�o
				//Angle( x :Hip�̊p�x , y : Knee��Aangle , z : Target��Angle
				Vector3 angle{ f32Angle_HeronFormula(A, B, C) };

				struct LocalFunction
				{
					static void  __fastcall RotateUpdate(ModelObject::Node* joint, const f32& angle, const s32& reverseFlag)
					{
						//�I�C���[(X����)�ȊO�͘M��Ȃ�
						Vector3 eulerForCompute, eulerForOrigin;
						eulerForCompute = ConvertToRollPitchYawFromRotationMatrix(MatrixRotationQuaternion(joint->rotate));
						eulerForOrigin  = eulerForCompute;//�ۑ�

						// 0 < val < 180 �ɕϊ�
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

						//�p�x�����i�I�C���[�j
						eulerForCompute = ClampVector(eulerForCompute, joint->minRot, joint->maxRot);

						// 0 < val < �� �ɕϊ�
						eulerForCompute.x = ToRadian(eulerForCompute.x);

						//��]��X�����̂ݔ��f�AY,Z�͎c��
						joint->rotate = ConvertToQuaternionFromRollPitchYaw(eulerForCompute.x, -eulerForOrigin.y, -eulerForOrigin.z);
					}
				};
				LocalFunction::RotateUpdate(footIk.pHip,  angle.x, 0);
				LocalFunction::RotateUpdate(footIk.pKnee, angle.y, 1);
				LocalFunction::RotateUpdate(footIk.pAnkle,angle.z, 0);
				UpdateChildTranslate(footIk.pHip);
			}

			//Root���疖�[�Ɍ������ẴW���C���g�̖ڕW���W�̓o�^
			void __fastcall HumanFARBIKManager::BackwardCuluculate(
				std::vector<Vector3>& anserPosition,
				const Vector3& targetPosition,
				ModelObject::Node* pEffector,
				ModelObject::Node* pCurrent,
				const std::shared_ptr<Transform> root)
			{
				//�����ł�pCurrent���q�W���C���g�ApEffector�����ڃW���C���g�Aroot�̓��f����`�悷����W�AhitCoordinate����̍ŏI���W(������͎g���\��Ȃ�)
				const Vector3 effectorPosision{ GetLocation(pEffector->worldTransform * root->GetWorldTransform()) };
				const f32     effectorToChildLength{ Vector3Length(GetLocation(pCurrent->worldTransform * root->GetWorldTransform()) - effectorPosision) };

				//�q���̍ŏI�ڕW���W��o�^
				anserPosition.emplace_back((Vector3Normalize(targetPosition - anserPosition.back()) * effectorToChildLength) + anserPosition.back());
			}

			//TODO : ������(���GUI����)
			void __fastcall HumanFARBIKManager::IKSolver(
				const Math::Vector3& anserPosition,
				ModelObject::Node* pEffector,
				ModelObject::Node* pCurrent,
				const std::shared_ptr<Transform> root)
			{
				//TODO : �s����X�V�����,Root�̍s��̉e�����Ȃ�������
				//�����ł�
				//pCurrent���q�W���C���g�ApEffector�����ڃW���C���g�Aroot�̓��f����`�悷����W�AhitCoordinate����̍ŏI���W(������͎g���\��Ȃ�)
				//Backward�ŏo�����ڕW���W�܂ł̃x�N�g���Ɍ�������CCDIK�̌v�Z������
				//�����ł�Backwork�œo�^����targetPosition����납�珇�Ɏg�p���Ă����i�����ł́@iterate : u32�@�Ɍ����ĂĎg���Ă����j

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

				//��̃��[�J���x�N�g���̉�]�����v�Z
				Vector3 rotationAxis{ /*Vector3Normalize*/(Vector3Cross(basisLocalChildVec ,basisLocalTargetVec)) };

				ImGui::DragFloat3("rotationAxis", rotationAxis);

				//��]�ʂ��o��
				f32 dot{ acos(Vector3Dot(basisLocalTargetVec,basisLocalChildVec)) };

				if (rotationAxis.y < 0.0f)
				{
					dot *= -1.0f;
				}

				ImGui::DragFloat("dot", &dot);

				//��]�l�����Ȃ��ꍇ�v�Z���Ȃ�
				if (std::abs(dot) > Epsilon && dot < PI)
				{
					//��]���Ɖ�]�p�x����N�H�[�^�j�I���𐶐�
					const Vector3 rightVec{ pEffector->rotate.LocalRight() };
					const Quaternion rotationQt{ QuaternionRotationAxis(rightVec, -dot) };

					//�N�H�[�^�j�I���󔃂��ړ]�s��𐶐�
					const Matrix rotationMatrix{ MatrixRotationQuaternion(pEffector->rotate * rotationQt) };
					Vector3 euler{ ConvertToRollPitchYawFromRotationMatrix(rotationMatrix) };

					//TODO : FABRIK�͊p�x�����͉~���ł����ł��Ȃ�(�C���ӏ�)
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

					//���ڃW���C���g�̎p�����X�V
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

					//��̃��[�J���x�N�g���̉�]�����v�Z
					Vector3 rotationAxis{ /*Vector3Normalize*/(Vector3Cross(basisLocalChildVec ,basisLocalTargetVec)) };

					//��]�ʂ��o��
					f32 dot{ acos(Vector3Dot(basisLocalTargetVec,basisLocalChildVec)) };

					//�O�ς��t�����������ꍇacos�̒l�̕��������]����
					if (rotationAxis.y < 0.0f)
					{
						dot *= -1.0f;
					}

					//��]�l�����Ȃ��ꍇ�v�Z���Ȃ�
					if (std::fabsf(dot) > Epsilon && dot < PI)
					{
						//TODO : �]��v�Z�̎d�g�݂�FABRIK1�ł��ł���悤�Ɏ�������
						//Knee -> Ankle�̉������̂���̏C����Hip���S��(CCDIK�̎��̗]��v�Z)
						//Knee->Ankle�@�� Knee->AnsT�̊O�ςƓ���
						const Vector3 kneeToAnkleWorld{ (GetLocation(foot->pAnkle->worldTransform) - GetLocation(foot->pKnee->worldTransform)) };
						const Vector3 kneeToAnsWorld{ (anserPosition[2] - GetLocation(footIk->rootTransform->GetWorldTransform())) - GetLocation(foot->pKnee->worldTransform) };

						//��]���Ɖ�]�ʂ��Z�o
						const Vector3 extraRotationAxis{ Vector3Cross(kneeToAnkleWorld,kneeToAnsWorld) };
						f32           extraDot{ acos(Vector3Dot(Vector3Normalize(kneeToAnsWorld),Vector3Normalize(kneeToAnkleWorld))) };

						//�O�ς��t�����������ꍇacos�̒l�̕����𔽓]����
						if (extraRotationAxis.y < 0.0f)
						{
							extraDot *= -1.0f;
						}

						//��]���Ɖ�]�p�x����N�H�[�^�j�I���𐶐�
						const Vector3    rightVec{ foot->pHip->rotate.LocalRight() };
						const Quaternion rotationQt{ QuaternionRotationAxis(rightVec, -dot) };

						//�N�H�[�^�j�I���󔃂��ړ]�s��𐶐�
						const Matrix rotationMatrix{ MatrixRotationQuaternion(foot->pHip->rotate * rotationQt /** extraRotationQt*/) };
						Vector3 euler{ ConvertToRollPitchYawFromRotationMatrix(rotationMatrix) };

						//TODO : FABRIK�͊p�x�����͉~���ł����ł��Ȃ�(�C���ӏ�)
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

						//���ڃW���C���g�̎p�����X�V
						foot->pHip->rotate = ConvertToQuaternionFromRollPitchYaw(euler.x, euler.y, euler.z);
						UpdateChildTranslate(foot->pHip);
					}
				}
				//ImGui::Separator();
				//DEBUGGUI_END;
			}


			//�Ώۂ̐e�̃��[�J�����W���v�Z����
			void __fastcall HumanFARBIKManager::CulculateParentLocal(const Math::Vector3& basis2EffectDir, const Math::Vector3& basis2TargetDir, f32 rotationAngle, ModelObject::Node* pCurrent, const std::shared_ptr<Transform> root)
			{

			}

			//�Ώۂ̊p�x���v�Z����
			void __fastcall HumanFARBIKManager::CulculateAngle(ModelObject::Node* ankle, ModelObject::Node* hip, const Math::Vector3& targetPos, Math::Vector3& basis2EffectDir, Math::Vector3& basis2TargetDir, f32& rotateAngle, const std::shared_ptr<Transform> root)
			{

			}

			//�Ώۂ�Trasnform���X�V����
			void _fastcall HumanFARBIKManager::UpdateTransform(ModelObject::Node* node)
			{
				const Matrix S{ MatrixScaling(node->scale.x,node->scale.y,node->scale.z)                 },
					         R{ MatrixRotationQuaternion(node->rotate)                                   },
					         T{ MatrixTranslation(node->translate.x,node->translate.y,node->translate.z) };

				node->localTransform = S * R * T;
				node->worldTransform = node->localTransform * node->parent->worldTransform;
			}
			void _fastcall HumanFARBIKManager::UpdateChildTranslate(ModelObject::Node* node)
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
				//ModelComponent��TransformComponent�������ꍇ�������^�[��
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

				//FootIK�̓o�^
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