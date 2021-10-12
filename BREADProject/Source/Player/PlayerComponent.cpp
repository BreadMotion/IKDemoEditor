#include "playerComponent.h"
#include "Types.h"

#include "FND/Instance.h"

#include "../Source/Graphics/Device/Win/DirectX11/DeviceDX11.h"
#include "../Source/Graphics/Context/Win/DirectX11/ContextDX11.h"
#include "../Source/Graphics/GraphicsDevice/Win/DirectX11/GraphicsDeviceDX11.h"
#include "Graphics/Camera.h"
#include "Graphics/RenderManager.h"

#include "FrameWork/Input/InputDevice.h"

#include "FrameWork/Actor/ActorManager.h"
#include "FrameWork/Object/TerrainManager.h"

using Bread::FND::Instance;
using Bread::FND::SharedInstance;

using namespace Bread::Math;

namespace Bread
{
	namespace FrameWork
	{
		//コンストラクタ
		PlayerComponent::PlayerComponent()
		{
		}

		//デストラクタ
		PlayerComponent::~PlayerComponent()
		{

		}

		//初期化
		void PlayerComponent::Initialize()
		{
			std::shared_ptr<Actor> owner   { GetOwner() };
			Graphics::DeviceDX11*  dxDevice{ dynamic_cast<Graphics::DeviceDX11*>(SharedInstance<Graphics::GraphicsDeviceDX11>::instance->GetDevice()) };
			ID3D11Device*          device  { dxDevice->GetD3DDevice() };

			transform = owner->GetComponent<Transform>();
			model     = owner->AddComponent<ModelObject>();
			velMap    = owner->AddComponent<VelocityMap>();
			collision = owner->AddComponent<CollisionCom>();
			rayCast   = owner->AddComponent<RayCastCom>(Instance<ActorManager>::instance.GetActorFromID("stage")->GetComponent<ModelObject>());
			geometric = owner->AddComponent<GeometricPrimitive>(device, GeometricPrimitive::GeometricPrimitiveType::CYLINDER);

			ComponentConstruction();

			{
				owner->AddChildActor<Actor>()->SetID("leftIKTarget");
				leftIKTarget = owner->GetChildActorFromID<Actor>("leftIKTarget");
				leftIKTargetComponent = leftIKTarget->AddComponent<IKTargetComponent>();
				leftIKTargetTransform = leftIKTarget->GetComponent<Transform>();
				leftIKTargetRayCast   = leftIKTarget->GetComponent<RayCastCom>();
			}

			{
				owner->AddChildActor<Actor>()->SetID("rightIKTarget");
				rightIKTarget = owner->GetChildActorFromID<Actor>("rightIKTarget");
				rightIKTargetComponent = rightIKTarget->AddComponent<IKTargetComponent>();
				rightIKTargetTransform = rightIKTarget->GetComponent<Transform>();
				rightIKTargetRayCast   = rightIKTarget->GetComponent<RayCastCom>();
			}
		}

		//事前更新
		void PlayerComponent::PreUpdate()
		{

		}

		//更新
		void PlayerComponent::Update()
		{
			Controll();

			rayCast->SetTargetFaceIndex(Instance<TerrainManager>
				::instance.GetSpatialFaces(GetOwner()->GetComponent<SpatialIndexComponent>()->GetSpatialIndex()));

			//レイキャスト vsStage
			if (rayCast->GetUseFlag())
			{
				Vector3 upVector{ GetRotation(transform->GetWorldTransform()).LocalUp() };
				const float HipHeight{ 110.0f };
				const float inverseVec{ -1.0f };

				Vector3 t1{ GetLocation(transform->GetWorldTransform()) };
				Vector3 t2{ GetLocation(transform->GetWorldTransform()) + (upVector * HipHeight) };

				Vector3 vec{ t1 - t2 };

				Vector3 rayVec{ (upVector * inverseVec) * Vector3Length(vec) };
				Vector3 rayStart{ t2 };
				Vector3 rayEnd{ rayStart + rayVec };
				rayCast->SetStartPosition(rayStart);
				rayCast->SetEndPosition(rayEnd);
				rayCast->IntersectRayVsModel();      //レイキャスト判定

				if (rayCast->GetHItFlag())
				{
					Vector3 vel{ velMap->GetVelocity() };
					velMap->SetVelocity({ vel.x,0.0f ,vel.z });
					transform->SetTranslate(rayCast->hitResult.position);
					transform->Update();
				}
			}

			//座標の強制
			Vector3 ansTranslate{ GetLocation(transform->GetWorldTransform()) };
			if (ansTranslate.y < 0.0f)
			{
				transform->SetTranslate({ ansTranslate.x, 0.0f, ansTranslate.z });
				transform->Update();
			}

			//当たり判定
			{
				collision->Update();     //コリジョンの更新
			}

			//modelの更新
			{
				//ChangeAnimation();     //アニメーションの変更
				model->UpdateTransform(MapInstance<f32>::instance["elapsedTime"] / 60.0f);//モデルの更新
			}
		}

		//事後更新
		void PlayerComponent::NextUpdate()
		{
			//CCDIKの更新
			{
				auto nodes{ model->GetNodes() };
#pragma region JointIndex
				constexpr u32 root{ 0 };
				constexpr u32 Hips{ 1 };

				constexpr u32 upRightLeg { 61 };
				constexpr u32 RightLeg   { 62 };
				constexpr u32 RightFoot  { 63 };
				constexpr u32 RightToe   { 64 };
				constexpr u32 RightToeEnd{ 65 };

				constexpr u32 upLeftLeg { 56 };
				constexpr u32 LeftLeg   { 57 };
				constexpr u32 LeftFoot  { 58 };
				constexpr u32 LeftToe   { 59 };
				constexpr u32 LeftToeEnd{ 60 };

				static f32 ankleHeight{ 10.0f };
#pragma endregion
				//leftFootの計算
				{
					Vector3   upVector   { GetRotation(leftIKTargetTransform->GetWorldTransform()).LocalUp()};
					Vector3   rightVector{ GetRotation(leftIKTargetTransform->GetWorldTransform()).LocalRight() };
					constexpr f32 inverseVec{ -1.0f };

					Matrix parentM{ leftIKTargetTransform->GetWorldTransform() };
					Matrix hipM { nodes->at(Hips).worldTransform * parentM };
					Matrix bone { nodes->at(upLeftLeg).worldTransform * parentM };
					Matrix bone1{ nodes->at(LeftLeg).worldTransform * parentM };
					Matrix bone2{ nodes->at(LeftFoot).worldTransform * parentM };

					Vector3 boneVec{ Vector3Subtract(GetLocation(bone2), GetLocation(bone1)) };
					f32     length { Vector3Length(boneVec) + ankleHeight };
					f32     halfPelvimetry{ Vector3Length(GetLocation(hipM) - GetLocation(bone)) };

					leftIKTargetComponent->SetRayVec((upVector)*length);
					leftIKTargetComponent->SetRayEnd(GetLocation(parentM) + (rightVector * (halfPelvimetry)));
					leftIKTargetComponent->SetRayStart(GetLocation(parentM) + (rightVector * (halfPelvimetry)) + ((upVector)*length));
					leftIKTargetComponent->SetDistance(length);
				}

				//rightFootの計算
				{
					Vector3 upVector        { GetRotation(rightIKTargetTransform->GetWorldTransform()).LocalUp() };
					Vector3 rightVector     { GetRotation(rightIKTargetTransform->GetWorldTransform()).LocalRight() };
					constexpr f32 inverseVec{ -1.0f };

					Matrix parentM{ rightIKTargetTransform->GetWorldTransform() };
					Matrix hipM   { nodes->at(Hips).worldTransform       * parentM };
					Matrix bone   { nodes->at(upRightLeg).worldTransform * parentM };
					Matrix bone1  { nodes->at(RightLeg).worldTransform   * parentM };
					Matrix bone2  { nodes->at(RightFoot).worldTransform  * parentM };

					Vector3 boneVec{ Vector3Subtract(GetLocation(bone2), GetLocation(bone1)) };
					f32     length { Vector3Length(boneVec) + ankleHeight };
					f32     halfPelvimetry{ Vector3Length(GetLocation(hipM) - GetLocation(bone)) };

					rightIKTargetComponent->SetRayVec((upVector)*length);
					rightIKTargetComponent->SetRayEnd(GetLocation(parentM) + (inverseVec * rightVector * (halfPelvimetry)));
					rightIKTargetComponent->SetRayStart(GetLocation(parentM) + (inverseVec * rightVector * (halfPelvimetry)) + ((upVector)*length));
					rightIKTargetComponent->SetDistance(length);
				}

				if (rayCast->GetHItFlag())
				{
					Instance<IKManager>::instance.Update();
				}
				Instance<IKManager>::instance.Gui();
			}
		}

		//GUi
		void PlayerComponent::GUI()
		{

		}

		void PlayerComponent::ComponentConstruction()
		{
			VelocityMapConstruction();
			ModelObjectConstruction();
			TransformConstruction();
			CollisionConstruction();
			RayCastConstruction();
			GeometricConstruction();
		}

		void PlayerComponent::ModelObjectConstruction()
		{
			{
				model->SetID("playerModel");
				model->Initialize();
				model->GetShaderMethod().SetShaderNema(Graphics::ShaderNameVal::basicSkinShader);
				model->Load("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Idle\\Breathing_Idle.fbx");
				//playerModel->Load("..\\Data\\Assets\\Model\\Sonic_charactor\\shadow\\sonic-6.fbx");
				model->SetHipID("Hips");
			}

			// アニメーション読み込み
			s32 beginIndex, endIndex;
#pragma region LoadResource
			{
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vamsonic-modelpire_A_Lusth\\FowardWalk\\Walking_With_Shopping_Bag.fbx", -1); // 1
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\Running.fbx", -1);
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\FowardRun\\SlowRunning.fbx", -1);
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Roll\\Sprinting_Forward_Roll.fbx", -1);

				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Idle\\Ready_Idle.fbx", -1); // 5
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Forward\\Walk_Forward.fbx", -1);
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Back\\Walk_Backward.fbx", -1);
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Right\\Walk_Right.fbx", -1);
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Walk\\Left\\Walk_Left.fbx", -1);

				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Forward\\Forward_Step.fbx", -1); // 10
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Back\\Back_Step.fbx", -1);
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Right\\Right_Step.fbx", -1);
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Dodge\\Left\\Left_Step.fbx", -1);

				beginIndex = model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\RightPunch\\Righ_Punch_Begin.fbx", -1); // 14
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\RightPunch\\Righ_Punch_End.fbx", -1);

				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\LeftPunch\\Left_Punch_Begin.fbx", -1); // 16
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Punch\\LeftPunch\\Left_Punch_End.fbx", -1);

				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\RightHook\\Hook_Punch_Right_Begin.fbx", -1); // 18
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\RightHook\\Hook_Punch_Right_End.fbx", -1);

				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\LeftHook\\Hook_Punch_Left_Begin.fbx", -1); // 20
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Hook\\LeftHook\\Hook_Punch_Left_End.fbx", -1);

				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\RightKick\\Roundhouse_Kick_Right_Begin.fbx", -1); // 22
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\RightKick\\Roundhouse_Kick_Right_End.fbx", -1);

				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\LeftKick\\Roundhouse_Kick_Left_Begin.fbx", -1); // 24
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Weak\\Kick\\LeftKick\\Roundhouse_Kick_Left_End.fbx", -1);

				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Punch\\Cross_Punch_Right.fbx", -1); // 26
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Punch\\Cross_Punch_Left.fbx", -1); // 27

				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Hook\\Hook_Punch_Right.fbx", -1); // 28
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\Hook\\Hook_Punch_Left.fbx", -1); // 29

				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Turn_Kick_Begin.fbx", -1); // 30
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Turn_Kick_01.fbx", -1);
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Turn_Kick_End.fbx", -1);

				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\TurnKick\\Mma_Kick.fbx", -1); // 33

				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\InvTurnKick\\Inv_Right_Turn_Kick.fbx", -1); // 34
				endIndex = model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Attack\\Strong\\InvTurnKick\\Inv_Left_Turn_Kick.fbx", -1); // 35

				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Damage\\Head_Hit_Small.fbx", -1); // 36
				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Damage\\Head_Hit.fbx", -1); // 37

				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Guard\\Ready_Idle.fbx", -1); // 38

				model->LoadAnimation("..\\Data\\Assets\\Model\\Player\\Vampire_A_Lusth\\Death\\Dying.fbx", -1); // 39
			}
#pragma endregion

			// アニメーターデータ構築
			{
				s32 layerNum = 0;
				{
					layerNum = model->AddAnimationLayer();
					layerIndexList.insert(std::make_pair(Player::LayerType::Base, layerNum));

					layerNum = model->AddAnimationStateToLayer(56, 65);
					layerIndexList.insert(std::make_pair(Player::LayerType::LowerBody, layerNum));
				}

				s32 stateNum = 0;
				{
					auto AddState = [&](Player::StateType type, u32 animationIndex, u32 layerIndex)
					{
						stateNum = model->AddAnimationStateToLayer(animationIndex, layerIndex);
						stateIndexList.insert(std::make_pair(type, stateNum));
					};

					layerNum = layerIndexList.at(Player::LayerType::Base);
					{
						for (s32 i = beginIndex; i <= endIndex; ++i)
						{
							model->AddAnimationStateToLayer(i, layerNum);
						}
						for (s32 i = 0; i <= 5; ++i)
						{
							model->AddAnimationStateToLayer(i, layerNum);
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

						s32 blendTreeIndex{ model->AddBlendTreeToLayer(layerNum) };
						model->AddBlendAnimationStateToBlendTree(1, Vector3(0.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
						model->AddBlendAnimationStateToBlendTree(3, Vector3(1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
					}

					// 下半身レイヤーにブレンドツリー追加
					layerNum = layerIndexList.at(Player::LayerType::LowerBody);
					{
						s32 blendTreeIndex{ model->AddBlendTreeToLayer(layerNum) };
						model->AddBlendAnimationStateToBlendTree(5, Vector3(0.0f, 0.0f, 0.0f),  layerNum, blendTreeIndex);
						model->AddBlendAnimationStateToBlendTree(6, Vector3(0.0f, 1.0f, 0.0f),  layerNum, blendTreeIndex);
						model->AddBlendAnimationStateToBlendTree(7, Vector3(0.0f, -1.0f, 0.0f), layerNum, blendTreeIndex);
						model->AddBlendAnimationStateToBlendTree(8, Vector3(1.0f, 0.0f, 0.0f),  layerNum, blendTreeIndex);
						model->AddBlendAnimationStateToBlendTree(9, Vector3(-1.0f, 0.0f, 0.0f), layerNum, blendTreeIndex);
					}
				}

				// 待機モーション開始
				{
					model->PlayAnimation(layerIndexList.at(Player::LayerType::Base), stateIndexList.at(Player::StateType::Idle), 1);
					model->SetLoopAnimation(true);
				}

				//モデルのフェイス情報の設定
				while (1)
				{
					if (Graphics::IModelResource* resource = model->GetModelResource())
					{
						if (resource->IsReady())
						{
							model->BuildFaces();
							break;
						}
					}
				}
			}
		}

		void PlayerComponent::TransformConstruction()
		{
			transform->SetID("playerTransform");
			transform->SetVelmapCom(velMap);

			transform->SetTranslate({ 655.0f, 300.0f, 310.0f });
			transform->SetScale({ 1.0f,1.0f ,1.0f });
			transform->SetRotate(ConvertToQuaternionFromRollPitchYaw(0.0f, 0.0f, 0.0f));
			transform->Update();

			transform->mySequence.mFrameMin = -100;
			transform->mySequence.mFrameMax = 1000;
			transform->mySequence.myItems.push_back(Transform::MySequence::MySequenceItem{ 0, 10, 30, false });
			transform->mySequence.myItems.push_back(Transform::MySequence::MySequenceItem{ 1, 20, 30, true });
			transform->mySequence.myItems.push_back(Transform::MySequence::MySequenceItem{ 3, 12, 60, false });
			transform->mySequence.myItems.push_back(Transform::MySequence::MySequenceItem{ 2, 61, 90, false });
			transform->mySequence.myItems.push_back(Transform::MySequence::MySequenceItem{ 4, 90, 99, false });
		}

		void PlayerComponent::VelocityMapConstruction()
		{
			constexpr float PlayerMass = 100.0f;
			velMap->SetID("velocityMap");

			velMap->SetMass(PlayerMass);
			velMap->SetGrabityflag(true);
		}

		void PlayerComponent::CollisionConstruction()
		{
			collision->SetID("collision");
		}

		void PlayerComponent::RayCastConstruction()
		{
			rayCast->SetID("rayCast");
			rayCast->SetTargetFaceIndex(Instance<TerrainManager>::instance
				.GetSpatialFaces(GetOwner()->GetComponent<SpatialIndexComponent>()->GetSpatialIndex()));
		}

		void PlayerComponent::GeometricConstruction()
		{
			geometric->SetID("primitive");
		}

		//操作
		void PlayerComponent::Controll()
		{
			using namespace Bread;
			using namespace Bread::Math;
			using namespace Bread::FrameWork;

			//Vector3 pos = transform->GetTranslate();
			velMap->SetPosition(transform->GetTranslate());
			Quaternion rotate{ transform->GetRotate() };
			const f32 speed{ 20.0f };
			const f32 SlowRunSpeed{ 20.0f };

			Vector3* blendRate{ model->GetBlendRateF3() };

			f32 sX { 0.0f};
			f32 sY { 0.0f};
			static f32 rotateY{ 180.0f * 0.01745f };

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
			Quaternion newRotate{ Quaternion::Zero };
			Vector3    vVec{ velMap->GetVelocity() };
			f32        vlength{ Vector3Length(velMap->GetVelocity()) };
			if (vlength > 0.0f)
			{
				if (sX != 0.0f || sY != 0.0f)
				{
					UpdateRotateY(sX, sY, Instance<ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>()->GetRotateY());
				}
				UpdatePlayer(newRotate);
			}
			else if (sX != 0.0f || sY != 0.0f)
			{
				if (animationState != Player::AnimationState::Walk)
				{
					ChangeAnimationState(Player::AnimationState::Walk, 1.0f);
				}
				UpdateRotateY(sX, sY, Instance<ActorManager>::instance.GetActorFromID("camera")->GetComponent<Graphics::Camera>()->GetRotateY());
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
					rotate = QuaternionSlerp(rotate, newRotate, 0.17f * (MapInstance<f32>::instance["elapsedTime"]));
					Vector3 front = rotate.LocalFront();
					if (sX != 0.0f || sY != 0.0f)
					{
						velMap->AddForce(front * (speed + (SlowRunSpeed)*blendRate->z));
					}
				}
			}

			constexpr float inverse{ -1.0f };
			Vector3 vel{ velMap->GetVelocity() };
			Vector2 moveVel{ Vector2(vel.x, vel.z) };
			Vector2 inverseMoveVel{ moveVel * inverse * 10.0f };
			velMap->AddForce(Vector3(inverseMoveVel.x, 0.0f, inverseMoveVel.y));
			velMap->Update();

			transform->SetTranslate(velMap->GetPosition());
			transform->SetRotate(rotate);
			transform->Update();

			// ブレンドレート計算
			{
				Vector3 newVel{ velMap->GetVelocity() };
				Vector2 newMoveVel{ Vector2(newVel.x, newVel.z) };
				blendRate->z = Vector2Length(newMoveVel) / velMap->GetMaxSpeed();
				blendRate->z = 1.0f <= blendRate->z ? 1.0f : blendRate->z;
			}
		}

		//アニメーションの変更
		void PlayerComponent::ChangeAnimation()
		{
			using namespace Bread::FrameWork;
			if (!isChangeAnimation && !model) return;

			u32 animationNum = static_cast<u32>(animationState);
			s32 bassLayerIndex = layerIndexList.at(Player::LayerType::Base);
			s32 lowerBodyLayerIndex = layerIndexList.at(Player::LayerType::LowerBody);

			switch (animationState)
			{
			case Player::AnimationState::Idle:
			{
				model->PlayAnimation(bassLayerIndex, stateIndexList.at(Player::StateType::Idle), 1, 0.4f);
				model->SetLoopAnimation(true);
				break;
			}
			case Player::AnimationState::Walk:
			{
				model->PlayBlendTreeAnimation(bassLayerIndex, 0, 1, 0.2f);
				model->SetLoopAnimation(true);
				break;
			}
			}

			isChangeAnimation = false;
		}

		//アニメーションステート変更
		void PlayerComponent::ChangeAnimationState(const Bread::Player::AnimationState& state, const f32& moveSpeed)
		{
			isChangeAnimation = true;
			animationState = state;

			animationSpeed = moveSpeed;
		}
	}
}