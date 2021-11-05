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

			//���񏉊���
			void FARBIKManager::Start()
			{

			}

			//������
			void FARBIKManager::Initialize()
			{

			}

			//�X�V
			void FARBIKManager::Update()
			{
				//�o�^����Ă���FABRIK���v�Z����
				for (auto& IK : registedIK)
				{
					FABRIK(IK);

					//TODO : ModelObject->UpdateBoneTransform()���Ă�
					for (auto& joint : *IK->setupJoint)
					{
						//joint->;
					}

				}
			}

			//Update�ł�����Ă�
			void _fastcall FARBIKManager::FABRIK(std::shared_ptr<SetUp::IKSetUp> setupIK)
			{
				//���s�񐔕����[�v����
				for (u32 i = 0; i < FABRIK::LoopNum; ++i)
				{
					FARBIKParentSolver(setupIK);
				}
			}

			//TODO : �����̃G���h�G�t�F�N�^�ɑΉ��ł���悤�ɐ݌v��ς���K�v������
			//TODO : ���[�̃G�t�F�N�^�̖ڕW���W�̎���ė��������l���邱��
			//FARBIK��p���ċr�̍��W��ڕW�n�_�ɕς��Ă���
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

				//�Z�o����anserPosition����Forward�����ɃW���C���g�̊p�x��ς��Ă���
				for (u32 i = 0; i < setupIK->setupJoint->size() - 1; ++i)
				{
					FABRIKSolver(anserPosition.at(i), &setupIK->setupJoint->at(i), setupIK->setupJoint->at(i).child.at(0), setupIK->rootTransform);
				}
			}

			//�q����e�Ɍ������Ă̊e�W���C���g�̃��[���h���W���Z�o
			void __fastcall FARBIKManager::ForwardCuluculate(std::vector<Math::Vector3>& targetPosition, ITransform* pEffector,
				ITransform* pCurrent, const std::shared_ptr<Transform> root)
			{
				//pCurrent���q�W���C���g�ApEffector�����ڃW���C���g�Aroot�̓��f����`�悷����W�AhitCoordinate���@��̍ŏI���W
				const Vector3 parentPosition        { GetLocation(pEffector->worldTransform * root->GetWorldTransform())                                };
				const f32     effectorToParentLength{ Vector3Length(parentPosition - GetLocation(pCurrent->worldTransform * root->GetWorldTransform())) };

				//�e�̖ڕW���W��o�^
				targetPosition.emplace_back((Vector3Normalize(parentPosition - targetPosition.back()) * effectorToParentLength) + targetPosition.back());
			}

			//�e����q�Ɍ������Ă̊e�W���C���g�̃��[���h���W���Z�o
			void __fastcall FARBIKManager::BackwardCuluculate(std::vector<Math::Vector3>& anserPosition, const Math::Vector3& targetPosition,
				ITransform* pEffector, ITransform* pCurrent, const std::shared_ptr<Transform> root)
			{
				//�����ł�pCurrent���q�W���C���g�ApEffector�����ڃW���C���g�Aroot�̓��f����`�悷����W�AhitCoordinate����̍ŏI���W(������͎g���\��Ȃ�)
				const Vector3 effectorPosision     { GetLocation(pEffector->worldTransform * root->GetWorldTransform())                                  };
				const f32     effectorToChildLength{ Vector3Length(GetLocation(pCurrent->worldTransform * root->GetWorldTransform()) - effectorPosision) };

				//�q���̍ŏI�ڕW���W��o�^
				anserPosition.emplace_back((Vector3Normalize(targetPosition - anserPosition.back()) * effectorToChildLength) + anserPosition.back());
			}

			//BackwardCuluculate�ŎZ�o�������W�܂ł̃x�N�g���ɓY���悤�ɃW���C���g����]������
			void __fastcall FARBIKManager::FABRIKSolver
			(
				const Math::Vector3& anserPosition, ITransform*                      pEffector,
				ITransform*          pCurrent,      const std::shared_ptr<Transform> root
			)
			{
				//TODO : �s����X�V�����,Root�̍s��̉e�����Ȃ�������
				// //�����ł�
				// //pCurrent���q�W���C���g�ApEffector�����ڃW���C���g�Aroot�̓��f����`�悷����W�AhitCoordinate����̍ŏI���W(������͎g���\��Ȃ�)
				// //Backward�ŏo�����ڕW���W�܂ł̃x�N�g���Ɍ�������CCDIK�̌v�Z������
				//�����ł�Backwork�œo�^����targetPosition����납�珇�Ɏg�p���Ă����i�����ł́@iterate : u32�@�Ɍ����ĂĎg���Ă����j

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

				//��̃��[�J���x�N�g���̉�]�����v�Z
				Vector3 rotationAxis{ /*Vector3Normalize*/(Vector3Cross(basisLocalChildVec ,basisLocalTargetVec)) };

				//��]�ʂ��o��
				f32 dot{ acos(Vector3Dot(basisLocalTargetVec,basisLocalChildVec)) };

				if (rotationAxis.y < 0.0f)
				{
					dot *= -1.0f;
				}

				//��]�l�����Ȃ��ꍇ�v�Z���Ȃ�
				if (std::abs(dot) > Epsilon && dot < PI)
				{
					//��]���Ɖ�]�p�x����N�H�[�^�j�I���𐶐�
					const Vector3    rightVec  { pEffector->rotate.LocalRight()         };
					const Quaternion rotationQt{ QuaternionRotationAxis(rightVec, -dot) };

					//�N�H�[�^�j�I���󔃂��ړ]�s��𐶐�
					const Matrix rotationMatrix{ MatrixRotationQuaternion(pEffector->rotate * rotationQt) };
					Vector3      euler         { ConvertToRollPitchYawFromRotationMatrix(rotationMatrix)  };

					//���ڃW���C���g�̎p�����X�V
					pEffector->rotate = ConvertToQuaternionFromRollPitchYaw(euler.x, euler.y, euler.z);
				}
				UpdateChildTranslate(pEffector);
			}

			//�Ώۂ̐e�̃��[�J�����W���v�Z����
			void __fastcall FARBIKManager::CulculateParentLocal(const Math::Vector3& basis2EffectDir, const Math::Vector3& basis2TargetDir,
				f32 rotationAngle, ITransform* pCurrent, const std::shared_ptr<Transform> root)
			{

			}

			//�Ώۂ̊p�x���v�Z����
			void __fastcall FARBIKManager::CulculateAngle(ITransform* ankle, ITransform* hip,
				const Math::Vector3& targetPos, Math::Vector3& basis2EffectDir,
				Math::Vector3& basis2TargetDir, f32& rotateAngle, const std::shared_ptr<Transform> root)
			{

			}

			//�Ώۂ�Trasnform���X�V����
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

				//FootIK�̓o�^
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