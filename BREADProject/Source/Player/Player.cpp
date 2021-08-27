#include <memory>
#include "Player.h"
#include "imgui.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"
#include "Math/BreadMath.h"

////////////////////////////////////////////////////

namespace Bread
{
	namespace FrameWork
	{
		std::shared_ptr<Actor> PlayerActor::Create(Graphics::IGraphicsDevice* graphicDevice,Graphics::Camera* cam, ModelObject* stage)
		{
			return std::make_shared<PlayerActor>(graphicDevice,cam, stage);
		}

		void PlayerActor::Initialize()
		{
			using namespace Math;
			//�R���|�[�l���g�ǉ�
			{
				playerModel = AddComponent<ModelObject>(graphicsDevice);
				ccdik       = AddComponent<CyclicCoordinateDescent>();
				transform   = AddComponent<Transform>();
				velmap      = AddComponent<VelocityMap>();
				collision   = AddComponent<CollisionCom>(graphicsDevice);
				rayCast     = AddComponent<RayCastCom>(graphicsDevice, stageModel);

				Graphics::DeviceDX11* dxDevice = static_cast<Graphics::DeviceDX11*>(graphicsDevice->GetDevice());
				ID3D11Device*         device   = dxDevice->GetD3DDevice();
				primitive = AddComponent<GeometricPrimitive>(device, GeometricPrimitive::GeometricPrimitiveType::CYLINDER);
			}

			//veloctiymap �́@������
			{
				const float PlayerMass = 100.0f;
				velmap->SetID("velocityMap");
				velmap->Initialize();
				velmap->SetMass(PlayerMass);
			}

			animationState = Player::AnimationState::Idle;

			//ModelObject Component�̃��f���f�[�^�����[�h
			{
				playerModel->SetID("playerModel");
				playerModel->Initialize();
				playerModel->Load("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Idle\\Breathing_Idle.fbx");
				//playerModel->Load("..\\Data\\Assets\\Model\\Sonic_charactor\\shadow\\sonic-6.fbx");
				playerModel->SetHipID("Hips");
			}

			// �A�j���[�V�����ǂݍ���
			s32 beginIndex, endIndex;
			{
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vamsonic-modelpire_A_Lusth\\FowardWalk\\Walking_With_Shopping_Bag.fbx", -1); // 1
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\Running.fbx", -1);
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\SlowRunning.fbx", -1);
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Roll\\Sprinting_Forward_Roll.fbx", -1);

				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Idle\\Ready_Idle.fbx", -1); // 5
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Forward\\Walk_Forward.fbx", -1);
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Back\\Walk_Backward.fbx", -1);
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Right\\Walk_Right.fbx", -1);
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Left\\Walk_Left.fbx", -1);

				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Forward\\Forward_Step.fbx", -1); // 10
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Back\\Back_Step.fbx", -1);
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Right\\Right_Step.fbx", -1);
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Left\\Left_Step.fbx", -1);

				beginIndex = playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\RightPunch\\Righ_Punch_Begin.fbx", -1); // 14
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\RightPunch\\Righ_Punch_End.fbx", -1);

				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\LeftPunch\\Left_Punch_Begin.fbx", -1); // 16
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\LeftPunch\\Left_Punch_End.fbx", -1);

				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\RightHook\\Hook_Punch_Right_Begin.fbx", -1); // 18
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\RightHook\\Hook_Punch_Right_End.fbx", -1);

				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\LeftHook\\Hook_Punch_Left_Begin.fbx", -1); // 20
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\LeftHook\\Hook_Punch_Left_End.fbx", -1);

				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\RightKick\\Roundhouse_Kick_Right_Begin.fbx", -1); // 22
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\RightKick\\Roundhouse_Kick_Right_End.fbx", -1);

				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\LeftKick\\Roundhouse_Kick_Left_Begin.fbx", -1); // 24
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\LeftKick\\Roundhouse_Kick_Left_End.fbx", -1);

				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Punch\\Cross_Punch_Right.fbx", -1); // 26
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Punch\\Cross_Punch_Left.fbx", -1); // 27

				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Hook\\Hook_Punch_Right.fbx", -1); // 28
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Hook\\Hook_Punch_Left.fbx", -1); // 29

				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Turn_Kick_Begin.fbx", -1); // 30
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Turn_Kick_01.fbx", -1);
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Turn_Kick_End.fbx", -1);

				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Mma_Kick.fbx", -1); // 33

				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\InvTurnKick\\Inv_Right_Turn_Kick.fbx", -1); // 34
				endIndex = playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\InvTurnKick\\Inv_Left_Turn_Kick.fbx", -1); // 35

				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Damage\\Head_Hit_Small.fbx", -1); // 36
				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Damage\\Head_Hit.fbx", -1); // 37

				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Guard\\Ready_Idle.fbx", -1); // 38

				playerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Death\\Dying.fbx", -1); // 39
			}

			// �A�j���[�^�[�f�[�^�\�z
			{
				s32 layerNum = 0;
				{
					layerNum = playerModel->AddAnimationLayer();
					layerIndexList.insert(std::make_pair(Player::LayerType::Base, layerNum));

					layerNum = playerModel->AddAnimationStateToLayer(56, 65);
					layerIndexList.insert(std::make_pair(Player::LayerType::LowerBody, layerNum));
				}

				s32 stateNum = 0;
				{
					auto AddState = [&](Player::StateType type, u32 animationIndex, u32 layerIndex)
					{
						stateNum = playerModel->AddAnimationStateToLayer(animationIndex, layerIndex);
						stateIndexList.insert(std::make_pair(type, stateNum));
					};

					layerNum = layerIndexList.at(Player::LayerType::Base);
					{
						for (s32 i = beginIndex; i <= endIndex; ++i)
						{
							playerModel->AddAnimationStateToLayer(i, layerNum);
						}
						for (s32 i = 0; i <= 5; ++i)
						{
							playerModel->AddAnimationStateToLayer(i, layerNum);
						}

						AddState(Player::StateType::Idle,          0,            layerNum);
						AddState(Player::StateType::BattleIdle,    5,            layerNum);
						AddState(Player::StateType::DamageSmall,   endIndex + 1, layerNum);
						AddState(Player::StateType::DamageBig,     endIndex + 2, layerNum);
						AddState(Player::StateType::ForwardDedge,  10,           layerNum);
						AddState(Player::StateType::BackDedge,     11,           layerNum);
						AddState(Player::StateType::RightDedge,    12,           layerNum);
						AddState(Player::StateType::LeftDedge,     13,           layerNum);
						AddState(Player::StateType::Death,         endIndex + 4, layerNum);

						s32 blendTreeIndex = playerModel->AddBlendTreeToLayer(layerNum);
						playerModel->AddBlendAnimationStateToBlendTree(1, Vector3(0.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
						playerModel->AddBlendAnimationStateToBlendTree(3, Vector3(1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
					}

					// �����g���C���[�Ƀu�����h�c���[�ǉ�
					layerNum = layerIndexList.at(Player::LayerType::LowerBody);
					{
						s32 blendTreeIndex = playerModel->AddBlendTreeToLayer(layerNum);
						playerModel->AddBlendAnimationStateToBlendTree(5, Vector3(0.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
						playerModel->AddBlendAnimationStateToBlendTree(6, Vector3(0.0f, 1.0f, 0.0f), layerNum, blendTreeIndex);
						playerModel->AddBlendAnimationStateToBlendTree(7, Vector3(0.0f, -1.0f, 0.0f), layerNum, blendTreeIndex);
						playerModel->AddBlendAnimationStateToBlendTree(8, Vector3(1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
						playerModel->AddBlendAnimationStateToBlendTree(9, Vector3(-1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
					}
				}

				// �ҋ@���[�V�����J�n
				{
					playerModel->PlayAnimation(layerIndexList.at(Player::LayerType::Base), stateIndexList.at(Player::StateType::Idle), 1);
					//playerModel->UpdateTransform(dt / 60.0f);
					playerModel->SetLoopAnimation(true);
				}

				//���f���̃t�F�C�X���̐ݒ�
				{
					if (playerModel->GetModelResource()->IsReady())
					{
						playerModel->BuildFaces();
					}
				}
			}

			//transform������
			{
				transform->SetID("playerTransform");
				transform->Initialize();
				transform->SetVelmapCom(velmap);

				transform->SetScale({ 1.0f,1.0f ,1.0f });
				transform->SetRotate(ConvertToQuaternionFromRollPitchYaw(0.0f, ToRadian(90.0f), 0.0f));

				transform->mySequence.mFrameMin = -100;
				transform->mySequence.mFrameMax = 1000;
				transform->mySequence.myItems.push_back(Transform::MySequence::MySequenceItem{ 0, 10, 30, false });
				transform->mySequence.myItems.push_back(Transform::MySequence::MySequenceItem{ 1, 20, 30, true });
				transform->mySequence.myItems.push_back(Transform::MySequence::MySequenceItem{ 3, 12, 60, false });
				transform->mySequence.myItems.push_back(Transform::MySequence::MySequenceItem{ 2, 61, 90, false });
				transform->mySequence.myItems.push_back(Transform::MySequence::MySequenceItem{ 4, 90, 99, false });
			}

			//ccdik�\�z
			{
				//�`���C���h�A�N�^�[�̒ǉ�
				{
					leftFootTargetActor = AddChildActor<IKTargetActor>(graphicsDevice, cameraAct);
					leftFootTargetActor->SetID("leftFootTarget");
					leftFootTargetActor->SetTargetFaceIndex(targetFaceIndex);
					leftFootTargetActor->SetTerrainModel(stageModel);
					leftFootTargetActor->SetTargetModel(playerModel);
					leftFootTargetActor->Initialize();

					rightFootTargetActor = AddChildActor<IKTargetActor>(graphicsDevice, cameraAct);
					rightFootTargetActor->SetID("rightFootTarget");
					rightFootTargetActor->SetTargetFaceIndex(targetFaceIndex);
					rightFootTargetActor->SetTerrainModel(stageModel);
					rightFootTargetActor->SetTargetModel(playerModel);
					rightFootTargetActor->Initialize();
				}

				ccdik->SetID("ccdik");
				ccdik->Initialize();
				s32        iterateNum = 50;
				Vector3 targetPos    = Vector3::Zero;
				f32        ankleHeight = 25.0f;

				std::vector<ModelObject::Node>* nodes = playerModel->GetNodes();

				const u32 root = 0;
				const u32 Hips = 1;

				const u32 upRightLeg  = 61;
				const u32 RightLeg      = 62;
				const u32 RightFoot     = 63;
				const u32 RightToe       = 64;
				const u32 RightToeEnd = 65;

				const u32 upLeftLeg  = 56;
				const u32 LeftLeg      = 57;
				const u32 LeftFoot     = 58;
				const u32 LeftToe       = 59;
				const u32 LeftToeEnd = 60;

				//�p�x�����ݒ�
				{
					auto  R([&](f32 val)
						{
							return ToRadian(val);
						});

					{//left foot

					   //nodes->at(LeftToeEnd).Movable[MovableShaft::X] = false;
						//nodes->at(LeftToeEnd).Movable[MovableShaft::Y] = false;
						//nodes->at(LeftToeEnd).Movable[MovableShaft::Z] = false;
						//nodes->at(LeftToeEnd).minRot = { R(0.0f) ,R(0.0f) ,R(0.0f) };
						//nodes->at(LeftToeEnd).maxRot = { R(0.0f) ,R(0.0f) ,R(0.0f) };

						//nodes->at(LeftToe).Movable[MovableShaft::X] = false;
						//nodes->at(LeftToe).Movable[MovableShaft::Y] = false;
						//nodes->at(LeftToe).Movable[MovableShaft::Z] = false;
						//nodes->at(LeftToe).minRot = { R(0.0f) ,R(0.0f) ,R(0.0f) };
						//nodes->at(LeftToe).maxRot = { R(60.0f) ,R(0.0f) ,R(0.0f) };

						nodes->at(LeftFoot).Movable[MovableShaft::X] = true;
						nodes->at(LeftFoot).Movable[MovableShaft::Y] = false;
						nodes->at(LeftFoot).Movable[MovableShaft::Z] = false;
						nodes->at(LeftFoot).minRot = { R(-60.0f) ,R(0.0f) ,R(0.0f) };
						nodes->at(LeftFoot).maxRot = { R(60.0f) ,R(0.0f) ,R(0.0f) };

						nodes->at(LeftLeg).Movable[MovableShaft::X] = true;
						nodes->at(LeftLeg).Movable[MovableShaft::Y] = false;
						nodes->at(LeftLeg).Movable[MovableShaft::Z] = false;
						nodes->at(LeftLeg).minRot = { R(0.0f) ,R(0.0f) ,R(0.0f) };
						nodes->at(LeftLeg).maxRot = { R(89.0f) ,R(0.0f) ,R(0.0f) };

						nodes->at(upLeftLeg).Movable[MovableShaft::X] = true;
						nodes->at(upLeftLeg).Movable[MovableShaft::Y] = false;
						nodes->at(upLeftLeg).Movable[MovableShaft::Z] = true;
						nodes->at(upLeftLeg).minRot = { R(-89.9f) ,R(0.0f) ,R(0.0f) };
						nodes->at(upLeftLeg).maxRot = { R(89.9f) ,R(0.0f) ,R(89.9f) };
					}

					//right Foot
					{
						//nodes->at(RightToeEnd).Movable[MovableShaft::X] = false;
						//nodes->at(RightToeEnd).Movable[MovableShaft::Y] = false;
						//nodes->at(RightToeEnd).Movable[MovableShaft::Z] = false;
						//nodes->at(RightToeEnd).minRot = { R(0.0f) ,R(0.0f) ,R(0.0f) };
						//nodes->at(RightToeEnd).maxRot = { R(0.0f) ,R(0.0f) ,R(0.0f) };

						//nodes->at(RightToe).Movable[MovableShaft::X] = false;
						//nodes->at(RightToe).Movable[MovableShaft::Y] = false;
						//nodes->at(RightToe).Movable[MovableShaft::Z] = false;
						//nodes->at(RightToe).minRot = { R(0.0f) ,R(0.0f) ,R(0.0f) };
						//nodes->at(RightToe).maxRot = { R(60.0f) ,R(0.0f) ,R(0.0f) };

						nodes->at(RightFoot).Movable[MovableShaft::X] = true;
						nodes->at(RightFoot).Movable[MovableShaft::Y] = false;
						nodes->at(RightFoot).Movable[MovableShaft::Z] = false;
						nodes->at(RightFoot).minRot = { R(-60.0f) ,R(0.0f) ,R(0.0f) };
						nodes->at(RightFoot).maxRot = { R(60.0f) ,R(0.0f) ,R(0.0f) };

						nodes->at(RightLeg).Movable[MovableShaft::X] = true;
						nodes->at(RightLeg).Movable[MovableShaft::Y] = false;
						nodes->at(RightLeg).Movable[MovableShaft::Z] = false;
						nodes->at(RightLeg).minRot = { R(0.0f) ,R(0.0f) ,R(0.0f) };
						nodes->at(RightLeg).maxRot = { R(89.0f) ,R(0.0f) ,R(0.0f) };

						nodes->at(upRightLeg).Movable[MovableShaft::X] = true;
						nodes->at(upRightLeg).Movable[MovableShaft::Y] = false;
						nodes->at(upRightLeg).Movable[MovableShaft::Z] = true;
						nodes->at(upRightLeg).minRot = { R(-89.9f) ,R(0.0f) ,R(-89.0f) };
						nodes->at(upRightLeg).maxRot = { R(89.9f) ,R(0.0f) ,R(0.0f) };
					}
				}

				ccdik->AddOrder(
					&transform->GetWorldTransform(),
					&nodes->at(LeftFoot),
					&nodes->at(upLeftLeg),
					targetPos, Vector3{ 0.0f,1.0f,0.0f },
					iterateNum, ankleHeight, PART::FOOT);

				ccdik->AddOrder(
					&transform->GetWorldTransform(),
					&nodes->at(RightFoot),
					&nodes->at(upRightLeg),
					targetPos, Vector3{ 0.0f,1.0f,0.0f },
					iterateNum, ankleHeight, PART::FOOT);
			}

			//collisionCom�̏�����
			{
				collision->SetID("collision");
				collision->Initialize();
			}

			//rayCastCom�̏�����
			{
				rayCast->SetID("rayCast");
				rayCast->Initialize();
				rayCast->SetTargetFaceIndex(targetFaceIndex);
			}

			//primitive�̏�����
			{
				primitive->SetID("primitive");
			}
		}

		//���O�X�V
		void PlayerActor::PreUpdate(const f32& dt)
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->PreUpdate(dt);
			}
		}

		//�X�V
		void PlayerActor::Update(const f32& dt)
		{
			using namespace Bread::Math;

			//�q�A�N�^�[�̍X�V
			for (auto& childAct : GetAllChildActor())
			{
				childAct->Update(dt);
			}

			//�M�Y������o�͂��ꂽ�g�����X�t�H�[�������
			{
				Matrix worldTransform = objMatrix;
				transform->SetTranslate(GetLocation(worldTransform));
				transform->SetRotate(GetRotation(worldTransform));
				transform->SetScale(GetScale(worldTransform));
			}

			//�v���C���[�̑���
			{
				Controll(dt);                     //���@�̑�����s��

				//���C�L���X�g vsStage
					if(rayCast->GetUseFlag())
				{
					Vector3 upVector = GetRotation(transform->GetWorldTransform()).LocalUp();
					const float HipHeight  = 110.0f;
					const float inverseVec = -1.0f;

					Vector3 t1 = GetLocation(transform->GetWorldTransform());
					Vector3 t2 = GetLocation(transform->GetWorldTransform()) + (upVector * HipHeight);

					Vector3 vec = t1 - t2;

					Vector3 rayVec   = (upVector * inverseVec) * Vector3Length(vec);
					Vector3 rayStart = t2;
					Vector3 rayEnd   = rayStart + rayVec;
					rayCast->SetStartPosition(rayStart);
					rayCast->SetEndPosition(rayEnd);
					rayCast->TargetFaceRegister();
					rayCast->IntersectRayVsModel();      //���C�L���X�g����

					if (rayCast->GetHItFlag())
					{
						Vector3 vel = velmap->GetVelocity();
						velmap->SetVelocity({ vel.x,0.0f ,vel.z });
						transform->SetTranslate(rayCast->hitResult.position);
						transform->Update(dt);
					}
				}
			}

			//�����蔻��
			{
				collision->Update(dt);     //�R���W�����̍X�V
			}

			//model�̍X�V
			{
				ChangeAnimation();     //�A�j���[�V�����̕ύX
				playerModel->UpdateTransform(dt / 60.0f);//���f���̍X�V
			}

			//CCDIK�̍X�V
			{
				auto    nodes = playerModel->GetNodes();

				const u32 root        = 0;
				const u32 Hips        = 1;

				const u32 upRightLeg  = 61;
				const u32 RightLeg    = 62;
				const u32 RightFoot   = 63;
				const u32 RightToe    = 64;
				const u32 RightToeEnd = 65;

				const u32 upLeftLeg  = 56;
				const u32 LeftLeg    = 57;
				const u32 LeftFoot   = 58;
				const u32 LeftToe    = 59;
				const u32 LeftToeEnd = 60;

				//leftFoot�̌v�Z
				RayCastCom* leftIKT = leftFootTargetActor->GetComponent<RayCastCom>();
				Transform*  leftT   = leftFootTargetActor->GetComponent<Transform>();
				{
					Vector3   upVector   = GetRotation(transform->GetWorldTransform()).LocalUp();
					Vector3   rightVector = GetRotation(transform->GetWorldTransform()).LocalRight();
					const f32 inverseVec = -1.0f;

					Matrix parentM = transform->GetWorldTransform();
					Matrix hipM    = nodes->at(Hips).worldTransform      * parentM;
					Matrix bone    = nodes->at(upLeftLeg).worldTransform * parentM;
					Matrix bone1   = nodes->at(LeftLeg).worldTransform   * parentM;
					Matrix bone2   = nodes->at(LeftFoot).worldTransform  * parentM;

					Vector3 boneVec         = Vector3Subtract(GetLocation(bone2), GetLocation(bone1));
					f32     length          = Vector3Length(boneVec) + ccdik->order.at(0)->ankleHeight;
					f32     halfPelvimetry  = Vector3Length(GetLocation(hipM) - GetLocation(bone));

					leftFootTargetActor->SetRayVec((upVector)*length);
					leftFootTargetActor->SetRayEnd(GetLocation(parentM) + (rightVector * (halfPelvimetry)));
					leftFootTargetActor->SetRayStart(GetLocation(parentM) + (rightVector * (halfPelvimetry)) + ((upVector)*length));
					leftFootTargetActor->SetDistance(length);

					ccdik->order.at(0)->setTargetPos(GetLocation(leftT->GetWorldTransform()));//CCDIK��Target�̍X�V
					ccdik->order.at(0)->setTargetIKNormal(Vector3{ 0.0f,1.0f,0.0f });
					ccdik->order.at(0)->setRayStart(GetLocation(parentM) + (rightVector * (halfPelvimetry)) + ((upVector)*length));
					Vector3 kDown = Vector3{ 0.0f, -1.0f, 0.0f };;
					ccdik->order.at(0)->setKDown(kDown);
				}

				//rightFoot�̌v�Z
				RayCastCom* rightIKT = rightFootTargetActor->GetComponent<RayCastCom>();
				Transform*    rightT     = rightFootTargetActor->GetComponent<Transform>();
				{
					Vector3 upVector     = GetRotation(transform->GetWorldTransform()).LocalUp();
					Vector3 rightVector  = GetRotation(transform->GetWorldTransform()).LocalRight();
					const f32 inverseVec = -1.0f;

					Matrix parentM = transform->GetWorldTransform();
					Matrix hipM   = nodes->at(Hips).worldTransform * parentM;
					Matrix bone   = nodes->at(upRightLeg).worldTransform * parentM;
					Matrix bone1  = nodes->at(RightLeg).worldTransform * parentM;
					Matrix bone2  = nodes->at(RightFoot).worldTransform * parentM;

					Vector3 boneVec         = Vector3Subtract(GetLocation(bone2), GetLocation(bone1));
					f32        length             = Vector3Length(boneVec) + ccdik->order.at(1)->ankleHeight;
					f32        halfPelvimetry = Vector3Length(GetLocation(hipM) - GetLocation(bone));

					rightFootTargetActor->SetRayVec((upVector)*length);
					rightFootTargetActor->SetRayEnd(GetLocation(parentM) + (inverseVec * rightVector * (halfPelvimetry)));
					rightFootTargetActor->SetRayStart(GetLocation(parentM) + (inverseVec * rightVector * (halfPelvimetry)) + ((upVector)*length));
					rightFootTargetActor->SetDistance(length);

					ccdik->order.at(1)->setTargetPos(GetLocation(rightT->GetWorldTransform()));//CCDIK��Target�̍X�V
					ccdik->order.at(1)->setTargetIKNormal(Vector3{ 0.0f,1.0f,0.0f });
					ccdik->order.at(1)->setRayStart(GetLocation(parentM) + (inverseVec * rightVector * (halfPelvimetry)) + ((upVector)*length));
					Vector3 kDown = Vector3{ 0.0f, -1.0f, 0.0f };;
					ccdik->order.at(1)->setKDown(kDown);
				}

				//���C�L���X�g���Ȃ��Ƃ�
				if (!rayCast->GetUseFlag())
				{
					ccdik->Update(dt);
					playerModel->UpdateBoneTransform(); //�{�[���̍X�V
				}
				else//�v���C���[���n�ʂ̏�ɏ���Ă�Ƃ�
				{
					const Matrix targetWorldTransform = transform->GetWorldTransform();
					CyclicCoordinateDescent::ComputePart* route0 = ccdik->order.at(0).get();
					CyclicCoordinateDescent::ComputePart* route1 = ccdik->order.at(1).get();
					if (leftIKT->GetHItFlag() && (animationState == Player::AnimationState::Idle))
					{
						route0->setTargetIKNormal(GetRotation(targetWorldTransform).LocalUp());
						ccdik->PartUpdate(0);
					}
					if (rightIKT->GetHItFlag() && (animationState == Player::AnimationState::Idle))
					{
						route1->setTargetIKNormal(GetRotation(targetWorldTransform).LocalUp());
						ccdik->PartUpdate(1);
					}
					//if (leftIKT->GetHItFlag() && (animationState == Player::AnimationState::Idle))
						//ccdik->ToeAimIK(
						//	&nodes->at(root),
						//	&nodes->at(LeftFoot),
						//	&nodes->at(LeftToe),
						//	&nodes->at(LeftToeEnd),
						//	&targetWorldTransform,
						//	leftIKT->hitResult.normal,
						//	ccdik->order.at(0)->ankleHeight,
						//	GetLocation(leftT->GetWorldTransform()));
					//if (rightIKT->GetHItFlag() && (animationState == Player::AnimationState::Idle))
						//ccdik->ToeAimIK(
						//	&nodes->at(root),
						//	&nodes->at(RightFoot),
						//	&nodes->at(RightToe),
						//	&nodes->at(RightToeEnd),
						//	&targetWorldTransform,
						//	rightIKT->hitResult.normal,
						//	ccdik->order.at(1)->ankleHeight,
						//	GetLocation(rightT->GetWorldTransform()));

					playerModel->UpdateBoneTransform(); //�{�[���̍X�V
				}
			}
		}

		//����X�V
		void PlayerActor::NextUpdate(const f32& dt)
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->NextUpdate(dt);
			}
		}


		void PlayerActor::Draw(const f32& dt)
		{
			using namespace Math;

			//�q�A�N�^�[�̕`��
			for (auto& childAct : GetAllChildActor())
			{
				childAct->Draw(dt);
			}

#if 0
			Vector3 scale       = transform->GetScale();
			Vector3 translate = transform->GetTranslate();
			Vector3 euler       = ConvertToRollPitchYawFromQuaternion(transform->GetRotate());

			// ���[���h�s����쐬
			Matrix W;
			{
				Matrix S, R, T;
				S = MatrixScaling(scale.x, scale.y, scale.z);
				R = MatrixRotationRollPitchYaw(euler.x, euler.y, euler.z);
				T = MatrixTranslation(translate.x, translate.y, translate.z);

				W = S * R * T;
			}

			Graphics::DeviceDX11* device = static_cast<Graphics::DeviceDX11*>(graphicsDevice->GetDevice());

			//�v���~�e�B�u�̕`��
			primitive->Render
			(
				device->GetD3DContext(),
				ConvertToFloat4x4FromVector4x4(W * cameraAct->GetView() * cameraAct->GetProjection()),
				ConvertToFloat4x4FromVector4x4(W),
				DirectX::XMFLOAT4(1, 1, 1, 1),
				DirectX::XMFLOAT4(0.0f, 0.6f, 0.0f, 1.0f),
				false
			);

#endif

		}

		//����
		void PlayerActor::Controll(const f32& dt)
		{
			using namespace Bread;
			using namespace Bread::Math;
			using namespace Bread::FrameWork;

			//Vector3 pos = transform->GetTranslate();
			velmap->SetPosition(transform->GetTranslate());
			Quaternion rotate       = transform->GetRotate();
			const f32  speed        = 20.0f;
			const f32  SlowRunSpeed = 20.0f;

			Vector3* blendRate = playerModel->GetBlendRateF3();

			f32 sX = 0.0f;
			f32 sY = 0.0f;
			static f32 rotateY = 180.0f * 0.01745f;

			sY = GetKeyState('W') < 0 ? -1.0f : sY;
			sY = GetKeyState('S') < 0 ? 1.0f : sY;
			sX = GetKeyState('A') < 0 ? -1.0f : sX;
			sX = GetKeyState('D') < 0 ? 1.0f : sX;

			// �v���C���[�̍ŏI���������肷��p�x���v�Z
			auto UpdateRotateY = [&](f32 sX, f32 sY, f32 cameraRotateY)
			{
				float len = sqrtf(sX * sX + sY * sY);

				if (len <= 0)
				{
					sX = 0;
					sY = 0;
				}

				float mag = 1 / len;

				sX *= mag;
				sY *= mag;

				rotateY = cameraRotateY + atan2f(sX, sY);
			};
			auto UpdatePlayer = [&](Quaternion& newRotate)
			{
				newRotate = QuaternionRotationAxis(Vector3(0.0f, 1.0f, 0.0f), rotateY);
			};

			// �v���C���[��]
			Quaternion newRotate = Quaternion::Zero;
			Vector3    vVec      = velmap->GetVelocity();
			f32        vlength   = Vector3Length(velmap->GetVelocity());
			if (vlength > 0.0f)
			{
				if (animationState != Player::AnimationState::Walk)
				{
					ChangeAnimationState(Player::AnimationState::Walk, 1.0f);
				}
				if (sX != 0.0f || sY != 0.0f)
				{
					UpdateRotateY(sX, sY, cameraAct->GetRotateY());
				}
				UpdatePlayer(newRotate);
			}
			else if (sX != 0.0f || sY != 0.0f)
			{
				if (animationState != Player::AnimationState::Walk)
				{
					ChangeAnimationState(Player::AnimationState::Walk, 1.0f);
				}
				UpdateRotateY(sX, sY, cameraAct->GetRotateY());
				UpdatePlayer(newRotate);
			}
			else if (animationState != Player::AnimationState::Idle)
			{
				ChangeAnimationState(Player::AnimationState::Idle, 1.0f);
			}

			// ���W�X�V
			{
				if (animationState == Player::AnimationState::Walk)
				{
					rotate = QuaternionSlerp(rotate, newRotate, 0.17f * (dt));
					Vector3 front = rotate.LocalFront();
					if (sX != 0.0f || sY != 0.0f)
					{
						velmap->AddForce(front * (speed + (SlowRunSpeed) * blendRate->z));
					}
				}
				else if (animationState == Player::AnimationState::Idle)
				{

				}
			}

			const float inverse = -1.0f;
			Vector3 vel         = velmap->GetVelocity();
			Vector2 moveVel     = Vector2(vel.x, vel.z);
			Vector2 inverseMoveVel = moveVel * inverse * 10.0f;
			velmap->AddForce(Vector3(inverseMoveVel.x, 0.0f, inverseMoveVel.y));
			velmap->Update(dt);

			transform->SetTranslate(velmap->GetPosition());
			transform->SetRotate(rotate);
			transform->Update(dt);

			// �u�����h���[�g�v�Z
			{
				Vector3 newVel          = velmap->GetVelocity();
				Vector2 newMoveVel = Vector2(newVel.x, newVel.z);
				blendRate->z = Vector2Length(newMoveVel) / velmap->GetMaxSpeed();
				blendRate->z = 1.0f <= blendRate->z ? 1.0f : blendRate->z;
			}
		}

		//�A�j���[�V�����̕ύX
		void PlayerActor::ChangeAnimation()
		{
			using namespace Bread::FrameWork;

			if (!isChangeAnimation) return;

			u32 animationNum = static_cast<u32>(animationState);
			s32 bassLayerIndex = layerIndexList.at(Player::LayerType::Base);
			s32 lowerBodyLayerIndex = layerIndexList.at(Player::LayerType::LowerBody);

			switch (animationState)
			{
			case Player::AnimationState::Idle:
				playerModel->PlayAnimation(bassLayerIndex, stateIndexList.at(Player::StateType::Idle), 1, 0.4f);
				playerModel->SetLoopAnimation(true);
				break;

			case Player::AnimationState::Walk:

				playerModel->PlayBlendTreeAnimation(bassLayerIndex, 0, 1, 0.2f);
				playerModel->SetLoopAnimation(true);
				break;

				//	case Player::AnimationState::Attack:
						//ChangeAttackAnimation(baseLayerIndex);
						//break;

					//case Player::AnimationState::Damage:
					//	if (damagePower == 0)
					//	{
					//		model->PlayAnimation(baseLayerIndex, stateIndexList.at(PLayer::StateType::DamageSmall), 1, 0.2f);
					//		model->SetEndTime(43.0f / 60.0f);
					//	}
					//	else if (damagePower == 1)
					//	{
					//		model->PlayAnimation(baseLayerIndex, stateIndexList.at(PLayer::StateType::DamageBig), 1, 0.2f);
					//	}
					//	model->SetLoopAnimation(false);
					//	break;

					//case Player::AnimationState::Dedge:
					//	model->PlayAnimation(bassLayerIndex, dedgeLayerIndex, 0, 0.2f);
					//	model->SetLoopAnimation(false);
					//	model->SetSpeed(2.25f);
					//	break;

					//case Player::AnimationState::Death:
					//	model->PlayAnimation(baseLayerIndex, stateIndexList.at(PLayer::StateType::Death), 1, 0.2f);
					//	model->SetLoopAnimation(false);
					//	break;

			default: break;
			}

			isChangeAnimation = false;
		}

		//�A�j���[�V�����X�e�[�g�ύX
		void PlayerActor::ChangeAnimationState(const Bread::Player::AnimationState& state, const f32& moveSpeed)
		{
			isChangeAnimation = true;
			animationState    = state;

			animationSpeed    = moveSpeed;
		}
	}
}