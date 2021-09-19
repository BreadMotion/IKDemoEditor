#include <memory>
#include "FND/Instance.h"

#include "Player.h"
#include "imgui.h"
#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"
#include "Math/BreadMath.h"

using Bread::FND::Instance;

namespace Bread
{
	namespace FrameWork
	{
		void PlayerActor::Initialize()
		{
			using namespace Math;
			std::shared_ptr<Graphics::IGraphicsDevice> wpGraphicsDevice = graphicsDevice.lock();
			if (!wpGraphicsDevice)
			{
				return;
			}
			//コンポーネント追加
			{
				playerModel = AddComponent<ModelObject>(wpGraphicsDevice);
				transform   = AddComponent<Transform>();
				velmap      = AddComponent<VelocityMap>();
				collision   = AddComponent<CollisionCom>(wpGraphicsDevice);
				if (std::shared_ptr<ModelObject> terrain = stageModel.lock())
				{
					rayCast = AddComponent<RayCastCom>(wpGraphicsDevice, terrain.get());
				}

				Graphics::DeviceDX11* dxDevice = dynamic_cast<Graphics::DeviceDX11*>(wpGraphicsDevice->GetDevice());
				ID3D11Device*         device   = dxDevice->GetD3DDevice();
				primitive = AddComponent<GeometricPrimitive>(device, GeometricPrimitive::GeometricPrimitiveType::CYLINDER);
			}

			//veloctiymap の　初期化
			std::shared_ptr<VelocityMap> wpVelMap = velmap.lock();
			if (!wpVelMap)return;
			{
				const float PlayerMass = 100.0f;
				wpVelMap->SetID("velocityMap");
				wpVelMap->Initialize();
				wpVelMap->SetMass(PlayerMass);
			}
			ChangeAnimationState(Player::AnimationState::Idle, 1.0f);

			//ModelObject Componentのモデルデータをロード
			std::shared_ptr<ModelObject>  wpPlayerModel = playerModel.lock();
			if (!wpPlayerModel)return;
			{
				{
					wpPlayerModel->SetID("playerModel");
					wpPlayerModel->Initialize();
					wpPlayerModel->Load("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Idle\\Breathing_Idle.fbx");
					//wpPlayerModel->Load("..\\Data\\Assets\\Model\\Sonic_charactor\\shadow\\sonic-6.fbx");
					wpPlayerModel->SetHipID("Hips");
				}

				// アニメーション読み込み
				s32 beginIndex, endIndex;
				{
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vamsonic-modelpire_A_Lusth\\FowardWalk\\Walking_With_Shopping_Bag.fbx", -1); // 1
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\Running.fbx", -1);
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\SlowRunning.fbx", -1);
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Roll\\Sprinting_Forward_Roll.fbx", -1);

					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Idle\\Ready_Idle.fbx", -1); // 5
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Forward\\Walk_Forward.fbx", -1);
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Back\\Walk_Backward.fbx", -1);
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Right\\Walk_Right.fbx", -1);
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Left\\Walk_Left.fbx", -1);

					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Forward\\Forward_Step.fbx", -1); // 10
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Back\\Back_Step.fbx", -1);
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Right\\Right_Step.fbx", -1);
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Left\\Left_Step.fbx", -1);

					beginIndex = wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\RightPunch\\Righ_Punch_Begin.fbx", -1); // 14
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\RightPunch\\Righ_Punch_End.fbx", -1);

					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\LeftPunch\\Left_Punch_Begin.fbx", -1); // 16
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\LeftPunch\\Left_Punch_End.fbx", -1);

					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\RightHook\\Hook_Punch_Right_Begin.fbx", -1); // 18
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\RightHook\\Hook_Punch_Right_End.fbx", -1);

					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\LeftHook\\Hook_Punch_Left_Begin.fbx", -1); // 20
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\LeftHook\\Hook_Punch_Left_End.fbx", -1);

					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\RightKick\\Roundhouse_Kick_Right_Begin.fbx", -1); // 22
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\RightKick\\Roundhouse_Kick_Right_End.fbx", -1);

					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\LeftKick\\Roundhouse_Kick_Left_Begin.fbx", -1); // 24
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\LeftKick\\Roundhouse_Kick_Left_End.fbx", -1);

					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Punch\\Cross_Punch_Right.fbx", -1); // 26
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Punch\\Cross_Punch_Left.fbx", -1); // 27

					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Hook\\Hook_Punch_Right.fbx", -1); // 28
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Hook\\Hook_Punch_Left.fbx", -1); // 29

					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Turn_Kick_Begin.fbx", -1); // 30
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Turn_Kick_01.fbx", -1);
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Turn_Kick_End.fbx", -1);

					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Mma_Kick.fbx", -1); // 33

					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\InvTurnKick\\Inv_Right_Turn_Kick.fbx", -1); // 34
					endIndex = wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\InvTurnKick\\Inv_Left_Turn_Kick.fbx", -1); // 35

					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Damage\\Head_Hit_Small.fbx", -1); // 36
					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Damage\\Head_Hit.fbx", -1); // 37

					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Guard\\Ready_Idle.fbx", -1); // 38

					wpPlayerModel->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Death\\Dying.fbx", -1); // 39
				}

				// アニメーターデータ構築
				{
					s32 layerNum = 0;
					{
						layerNum = wpPlayerModel->AddAnimationLayer();
						layerIndexList.insert(std::make_pair(Player::LayerType::Base, layerNum));

						layerNum = wpPlayerModel->AddAnimationStateToLayer(56, 65);
						layerIndexList.insert(std::make_pair(Player::LayerType::LowerBody, layerNum));
					}

					s32 stateNum = 0;
					{
						auto AddState = [&](Player::StateType type, u32 animationIndex, u32 layerIndex)
						{
							stateNum = wpPlayerModel->AddAnimationStateToLayer(animationIndex, layerIndex);
							stateIndexList.insert(std::make_pair(type, stateNum));
						};

						layerNum = layerIndexList.at(Player::LayerType::Base);
						{
							for (s32 i = beginIndex; i <= endIndex; ++i)
							{
								wpPlayerModel->AddAnimationStateToLayer(i, layerNum);
							}
							for (s32 i = 0; i <= 5; ++i)
							{
								wpPlayerModel->AddAnimationStateToLayer(i, layerNum);
							}

							AddState(Player::StateType::Idle,         0,            layerNum);
							AddState(Player::StateType::BattleIdle,   5,            layerNum);
							AddState(Player::StateType::DamageSmall,  endIndex + 1, layerNum);
							AddState(Player::StateType::DamageBig,    endIndex + 2, layerNum);
							AddState(Player::StateType::ForwardDedge, 10,           layerNum);
							AddState(Player::StateType::BackDedge,    11,           layerNum);
							AddState(Player::StateType::RightDedge,   12,           layerNum);
							AddState(Player::StateType::LeftDedge,    13,           layerNum);
							AddState(Player::StateType::Death,        endIndex + 4, layerNum);

							s32 blendTreeIndex = wpPlayerModel->AddBlendTreeToLayer(layerNum);
							wpPlayerModel->AddBlendAnimationStateToBlendTree(1, Vector3(0.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
							wpPlayerModel->AddBlendAnimationStateToBlendTree(3, Vector3(1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
						}

						// 下半身レイヤーにブレンドツリー追加
						layerNum = layerIndexList.at(Player::LayerType::LowerBody);
						{
							s32 blendTreeIndex = wpPlayerModel->AddBlendTreeToLayer(layerNum);
							wpPlayerModel->AddBlendAnimationStateToBlendTree(5, Vector3(0.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
							wpPlayerModel->AddBlendAnimationStateToBlendTree(6, Vector3(0.0f, 1.0f, 0.0f), layerNum, blendTreeIndex);
							wpPlayerModel->AddBlendAnimationStateToBlendTree(7, Vector3(0.0f, -1.0f, 0.0f), layerNum, blendTreeIndex);
							wpPlayerModel->AddBlendAnimationStateToBlendTree(8, Vector3(1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
							wpPlayerModel->AddBlendAnimationStateToBlendTree(9, Vector3(-1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
						}
					}

					// 待機モーション開始
					{
						wpPlayerModel->PlayAnimation(layerIndexList.at(Player::LayerType::Base), stateIndexList.at(Player::StateType::Idle), 1);
						wpPlayerModel->SetLoopAnimation(true);
					}

					//モデルのフェイス情報の設定
					{
						if (wpPlayerModel->GetModelResource()->IsReady())
						{
							wpPlayerModel->BuildFaces();
						}
					}
				}
			}

			//transform初期化
			constexpr float firstElapsed = 100.0f;
			std::shared_ptr<Transform> wpTransform = transform.lock();
			if (!wpTransform)return;
			{
				wpTransform->SetID("playerTransform");
				wpTransform->Initialize();
				wpTransform->SetVelmapCom(wpVelMap);

				wpTransform->SetTranslate({ 0.0, 0.0f, 0.0f });
				wpTransform->SetScale({ 1.0f,1.0f ,1.0f });
				wpTransform->SetRotate(ConvertToQuaternionFromRollPitchYaw(0.0f, ToRadian(90.0f), 0.0f));
				wpTransform->Update(firstElapsed);

				wpTransform->mySequence.mFrameMin = -100;
				wpTransform->mySequence.mFrameMax = 1000;
				wpTransform->mySequence.myItems.push_back(Transform::MySequence::MySequenceItem{ 0, 10, 30, false });
				wpTransform->mySequence.myItems.push_back(Transform::MySequence::MySequenceItem{ 1, 20, 30, true });
				wpTransform->mySequence.myItems.push_back(Transform::MySequence::MySequenceItem{ 3, 12, 60, false });
				wpTransform->mySequence.myItems.push_back(Transform::MySequence::MySequenceItem{ 2, 61, 90, false });
				wpTransform->mySequence.myItems.push_back(Transform::MySequence::MySequenceItem{ 4, 90, 99, false });
			}

			//ccdik構築
			std::shared_ptr<ModelObject>             wpStageModel = stageModel.lock();
			std::shared_ptr<Graphics::Camera>        wpCamera     = cameraAct.lock();
			if (
				//!wpCcdik&&
				!wpStageModel
				) return;
			{
				std::shared_ptr<IKTargetActor> wpleftFootTargetActor;
				std::shared_ptr<IKTargetActor> wprightFootTargetActor;
				leftFootTargetActor  = wpleftFootTargetActor   = AddChildActor<IKTargetActor>(wpGraphicsDevice, wpCamera.get());
				rightFootTargetActor = wprightFootTargetActor  = AddChildActor<IKTargetActor>(wpGraphicsDevice, wpCamera.get());

				//チャイルドアクターの追加
				if (wpleftFootTargetActor)
				{
					wpleftFootTargetActor->SetID("leftFootTarget");
					wpleftFootTargetActor->SetTargetFaceIndex(targetFaceIndex);
					wpleftFootTargetActor->SetTerrainModel(wpStageModel);
					wpleftFootTargetActor->SetTargetModel(wpPlayerModel);
					wpleftFootTargetActor->Initialize();
				}
				if (wprightFootTargetActor)
				{
					wprightFootTargetActor->SetID("rightFootTarget");
					wprightFootTargetActor->SetTargetFaceIndex(targetFaceIndex);
					wprightFootTargetActor->SetTerrainModel(wpStageModel);
					wprightFootTargetActor->SetTargetModel(wpPlayerModel);
					wprightFootTargetActor->Initialize();
				}

				std::shared_ptr<RayCastCom> footRay[2]=
				{
				wpleftFootTargetActor->GetComponent<RayCastCom>(),
				wprightFootTargetActor->GetComponent<RayCastCom>()
				};
				Instance<IKManager>::instance.RegisterFootIk(wpPlayerModel, wpTransform, footRay);
			}

			//collisionComの初期化
			std::shared_ptr<CollisionCom> wpCollision = collision.lock();
			if (!wpCollision)return;
			{
				wpCollision->SetID("collision");
				wpCollision->Initialize();
			}

			//rayCastComの初期化
			std::shared_ptr<RayCastCom> wpRaycast = rayCast.lock();
			if (!wpRaycast)return;
			{
				wpRaycast->SetID("rayCast");
				wpRaycast->Initialize();
				wpRaycast->SetTargetFaceIndex(targetFaceIndex);
			}

			//primitiveの初期化
			std::shared_ptr<GeometricPrimitive> wpPrimitive = primitive.lock();
			if (!wpPrimitive)return;
			{
				wpPrimitive->SetID("primitive");
			}
		}

		//事前更新
		void PlayerActor::PreUpdate(const f32& dt)
		{
			for (auto& childAct : GetAllChildActor())
			{
				childAct->PreUpdate(dt);
			}
		}

		//更新
		void PlayerActor::Update(const f32& dt)
		{
			using namespace Bread::Math;

			//子アクターの更新
			for (auto& childAct : GetAllChildActor())
			{
				childAct->Update(dt);
			}
			std::shared_ptr<ModelObject>             wpPlayerModel          = playerModel.lock();
			std::shared_ptr<Transform>               wpTransform            = transform.lock();
			std::shared_ptr<VelocityMap>             wpVelMap               = velmap.lock();
			std::shared_ptr<CollisionCom>            wpCollision            = collision.lock();
			std::shared_ptr<GeometricPrimitive>      wpPrimitive            = primitive.lock();
			std::shared_ptr<RayCastCom>              wpRaycast              = rayCast.lock();
			std::shared_ptr<IKTargetActor>           wpleftFootTargetActor  = leftFootTargetActor.lock();
			std::shared_ptr<IKTargetActor>           wprightFootTargetActor = rightFootTargetActor.lock();
			std::shared_ptr<ModelObject>             wpStageModel           = stageModel.lock();
			if (
				!wpleftFootTargetActor &&
				!wprightFootTargetActor &&
				!wpStageModel
				) return;
			if (
				!wpPlayerModel&&
				!wpTransform&&
				!wpVelMap&&
				!wpCollision&&
				!wpPrimitive&&
				!wpRaycast
				)return;

			//ギズモから出力されたトランスフォームを入力
			{
				Matrix worldTransform = objMatrix;
				wpTransform->SetTranslate(GetLocation(worldTransform));
				wpTransform->SetRotate(GetRotation(worldTransform));
				wpTransform->SetScale(GetScale(worldTransform));
			}

			//プレイヤーの操作
			{
				Controll(dt);                     //自機の操作を行う

				//レイキャスト vsStage
				if (wpRaycast->GetUseFlag())
				{
					Vector3 upVector = GetRotation(wpTransform->GetWorldTransform()).LocalUp();
					const float HipHeight  = 110.0f;
					const float inverseVec = -1.0f;

					Vector3 t1 = GetLocation(wpTransform->GetWorldTransform());
					Vector3 t2 = GetLocation(wpTransform->GetWorldTransform()) + (upVector * HipHeight);

					Vector3 vec = t1 - t2;

					Vector3 rayVec   = (upVector * inverseVec) * Vector3Length(vec);
					Vector3 rayStart = t2;
					Vector3 rayEnd   = rayStart + rayVec;
					wpRaycast->SetStartPosition(rayStart);
					wpRaycast->SetEndPosition(rayEnd);
					wpRaycast->TargetFaceRegister();
					wpRaycast->IntersectRayVsModel();      //レイキャスト判定

					if (wpRaycast->GetHItFlag())
					{
						Vector3 vel = wpVelMap->GetVelocity();
						wpVelMap->SetVelocity({ vel.x,0.0f ,vel.z });
						wpTransform->SetTranslate(wpRaycast->hitResult.position);
						wpTransform->Update(dt);
					}
				}
			}

			//座標の強制
			Vector3 ansTranslate{ GetLocation(wpTransform->GetWorldTransform()) };
			if (ansTranslate.y < 0.0f)
			{
				wpTransform->SetTranslate({ansTranslate.x, 0.0f, ansTranslate.z});
				wpTransform->Update(dt);
			}


			//当たり判定
			{
				wpCollision->Update(dt);     //コリジョンの更新
			}

			//modelの更新
			{
				ChangeAnimation();     //アニメーションの変更
				wpPlayerModel->UpdateTransform(dt / 60.0f);//モデルの更新
			}
		}

		//事後更新
		void PlayerActor::NextUpdate(const f32& dt)
		{
			using namespace Bread::Math;

			for (auto& childAct : GetAllChildActor())
			{
				childAct->NextUpdate(dt);
			}

			std::shared_ptr<ModelObject>   wpPlayerModel          = playerModel.lock();
			std::shared_ptr<Transform>     wpTransform            = transform.lock();
			std::shared_ptr<IKTargetActor> wpleftFootTargetActor  = leftFootTargetActor.lock();
			std::shared_ptr<IKTargetActor> wprightFootTargetActor = rightFootTargetActor.lock();
			std::shared_ptr<RayCastCom>              wpRaycast    = rayCast.lock();

			//CCDIKの更新
			{
				auto    nodes = wpPlayerModel->GetNodes();

				constexpr u32 root = 0;
				constexpr u32 Hips = 1;

				constexpr u32 upRightLeg  = 61;
				constexpr u32 RightLeg    = 62;
				constexpr u32 RightFoot   = 63;
				constexpr u32 RightToe    = 64;
				constexpr u32 RightToeEnd = 65;

				constexpr u32 upLeftLeg  = 56;
				constexpr u32 LeftLeg    = 57;
				constexpr u32 LeftFoot   = 58;
				constexpr u32 LeftToe    = 59;
				constexpr u32 LeftToeEnd = 60;

				constexpr f32 ankleHeight = 10.0f;

				//leftFootの計算
				std::shared_ptr<RayCastCom> leftIKT = wpleftFootTargetActor->GetComponent<RayCastCom>();
				std::shared_ptr<Transform>  leftT   = wpleftFootTargetActor->GetComponent<Transform>();
				{
					Vector3   upVector    = GetRotation(wpTransform->GetWorldTransform()).LocalUp();
					Vector3   rightVector = GetRotation(wpTransform->GetWorldTransform()).LocalRight();
					constexpr f32 inverseVec  = -1.0f;

					Matrix parentM = wpTransform->GetWorldTransform();
					Matrix hipM  = nodes->at(Hips).worldTransform      * parentM;
					Matrix bone  = nodes->at(upLeftLeg).worldTransform * parentM;
					Matrix bone1 = nodes->at(LeftLeg).worldTransform   * parentM;
					Matrix bone2 = nodes->at(LeftFoot).worldTransform  * parentM;

					Vector3 boneVec        = Vector3Subtract(GetLocation(bone2), GetLocation(bone1));
					f32     length         = Vector3Length(boneVec) + ankleHeight;
					f32     halfPelvimetry = Vector3Length(GetLocation(hipM) - GetLocation(bone));

					wpleftFootTargetActor->SetRayVec((upVector)*length);
					wpleftFootTargetActor->SetRayEnd(GetLocation(parentM)   + (rightVector * (halfPelvimetry)));
					wpleftFootTargetActor->SetRayStart(GetLocation(parentM) + (rightVector * (halfPelvimetry)) + ((upVector)*length));
					wpleftFootTargetActor->SetDistance(length);
				}

				//rightFootの計算
				std::shared_ptr<RayCastCom> rightIKT = wprightFootTargetActor->GetComponent<RayCastCom>();
				std::shared_ptr<Transform>  rightT   = wprightFootTargetActor->GetComponent<Transform>();
				{
					Vector3 upVector     = GetRotation(wpTransform->GetWorldTransform()).LocalUp();
					Vector3 rightVector  = GetRotation(wpTransform->GetWorldTransform()).LocalRight();
					constexpr f32 inverseVec = -1.0f;

					Matrix parentM = wpTransform->GetWorldTransform();
					Matrix hipM    = nodes->at(Hips).worldTransform       * parentM;
					Matrix bone    = nodes->at(upRightLeg).worldTransform * parentM;
					Matrix bone1   = nodes->at(RightLeg).worldTransform   * parentM;
					Matrix bone2   = nodes->at(RightFoot).worldTransform  * parentM;

					Vector3 boneVec = Vector3Subtract(GetLocation(bone2), GetLocation(bone1));
					f32     length = Vector3Length(boneVec) + ankleHeight;
					f32     halfPelvimetry = Vector3Length(GetLocation(hipM) - GetLocation(bone));

					wprightFootTargetActor->SetRayVec((upVector)*length);
					wprightFootTargetActor->SetRayEnd(GetLocation(parentM) + (inverseVec * rightVector * (halfPelvimetry)));
					wprightFootTargetActor->SetRayStart(GetLocation(parentM) + (inverseVec * rightVector * (halfPelvimetry)) + ((upVector)*length));
					wprightFootTargetActor->SetDistance(length);
				}
				if (wpRaycast->GetHItFlag())
				{
					Instance<IKManager>::instance.Update();
				}
				Instance<IKManager>::instance.Gui();
			}
		}


		void PlayerActor::Draw(const f32& dt)
		{
			using namespace Math;

			//子アクターの描画
			for (auto& childAct : GetAllChildActor())
			{
				childAct->Draw(dt);
			}
		}

		//操作
		void PlayerActor::Controll(const f32& dt)
		{
			using namespace Bread;
			using namespace Bread::Math;
			using namespace Bread::FrameWork;

			std::shared_ptr<ModelObject>      wpPlayerModel = playerModel.lock();
			std::shared_ptr<Transform>        wpTransform   = transform.lock();
			std::shared_ptr<VelocityMap>      wpVelMap      = velmap.lock();
			std::shared_ptr<Graphics::Camera> wpCameraAct   = cameraAct.lock();

			if (
				!wpPlayerModel &&
				!wpTransform &&
				!wpVelMap&&
				!wpCameraAct
				)return;

			//Vector3 pos = transform->GetTranslate();
			wpVelMap->SetPosition(wpTransform->GetTranslate());
			Quaternion rotate       = wpTransform->GetRotate();
			const f32  speed        = 20.0f;
			const f32  SlowRunSpeed = 20.0f;

			Vector3* blendRate = wpPlayerModel->GetBlendRateF3();

			f32 sX = 0.0f;
			f32 sY = 0.0f;
			static f32 rotateY = 180.0f * 0.01745f;

			sY = GetKeyState('W') < 0 ? -1.0f : sY;
			sY = GetKeyState('S') < 0 ? 1.0f : sY;
			sX = GetKeyState('A') < 0 ? -1.0f : sX;
			sX = GetKeyState('D') < 0 ? 1.0f : sX;

			// プレイヤーの最終方向を決定する角度を計算
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

			// プレイヤー回転
			Quaternion newRotate = Quaternion::Zero;
			Vector3    vVec      = wpVelMap->GetVelocity();
			f32        vlength   = Vector3Length(wpVelMap->GetVelocity());
			if (vlength > 0.0f)
			{
				if (sX != 0.0f || sY != 0.0f)
				{
					UpdateRotateY(sX, sY, wpCameraAct->GetRotateY());
				}
				UpdatePlayer(newRotate);
			}
			else if (sX != 0.0f || sY != 0.0f)
			{
				if (animationState != Player::AnimationState::Walk)
				{
					ChangeAnimationState(Player::AnimationState::Walk, 1.0f);
				}
				UpdateRotateY(sX, sY, wpCameraAct->GetRotateY());
				UpdatePlayer(newRotate);
			}
			else if (animationState != Player::AnimationState::Idle)
			{
				ChangeAnimationState(Player::AnimationState::Idle, 1.0f);
			}

			// 座標更新
			{
				if (animationState == Player::AnimationState::Walk)
				{
					rotate = QuaternionSlerp(rotate, newRotate, 0.17f * (dt));
					Vector3 front = rotate.LocalFront();
					if (sX != 0.0f || sY != 0.0f)
					{
						wpVelMap->AddForce(front * (speed + (SlowRunSpeed) * blendRate->z));
					}
				}
			}

			constexpr float inverse = -1.0f;
			Vector3 vel         = wpVelMap->GetVelocity();
			Vector2 moveVel     = Vector2(vel.x, vel.z);
			Vector2 inverseMoveVel = moveVel * inverse * 10.0f;
			wpVelMap->AddForce(Vector3(inverseMoveVel.x, 0.0f, inverseMoveVel.y));
			wpVelMap->Update(dt);

			wpTransform->SetTranslate(wpVelMap->GetPosition());
			wpTransform->SetRotate(rotate);
			wpTransform->Update(dt);

			// ブレンドレート計算
			{
				Vector3 newVel     = wpVelMap->GetVelocity();
				Vector2 newMoveVel = Vector2(newVel.x, newVel.z);
				blendRate->z = Vector2Length(newMoveVel) / wpVelMap->GetMaxSpeed();
				blendRate->z = 1.0f <= blendRate->z ? 1.0f : blendRate->z;
			}
		}

		//アニメーションの変更
		void PlayerActor::ChangeAnimation()
		{
			using namespace Bread::FrameWork;
			std::shared_ptr<ModelObject> wpPlayerModel = playerModel.lock();
			if (!isChangeAnimation && !wpPlayerModel) return;

			u32 animationNum        = static_cast<u32>(animationState);
			s32 bassLayerIndex      = layerIndexList.at(Player::LayerType::Base);
			s32 lowerBodyLayerIndex = layerIndexList.at(Player::LayerType::LowerBody);

			switch (animationState)
			{
			case Player::AnimationState::Idle:
			{
				wpPlayerModel->PlayAnimation(bassLayerIndex, stateIndexList.at(Player::StateType::Idle), 1, 0.4f);
				wpPlayerModel->SetLoopAnimation(true);
				break;
			}
			case Player::AnimationState::Walk:
			{
				wpPlayerModel->PlayBlendTreeAnimation(bassLayerIndex, 0, 1, 0.2f);
				wpPlayerModel->SetLoopAnimation(true);
				break;
			}
			}

			isChangeAnimation = false;
		}

		//アニメーションステート変更
		void PlayerActor::ChangeAnimationState(const Bread::Player::AnimationState& state, const f32& moveSpeed)
		{
			isChangeAnimation = true;
			animationState    = state;

			animationSpeed    = moveSpeed;
		}
	}
}