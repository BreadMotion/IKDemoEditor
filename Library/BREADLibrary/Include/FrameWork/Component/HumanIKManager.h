#pragma once
#include <memory>
#include "Math/BreadMath.h"
#include "FrameWork/Object/Object.h"
#include "FrameWork/Object/BasicObjectElement/INode.h"
#include "FrameWork/Component/Transform.h"
#include "../../../BREADProject/Source/RayCast/RayCast.h"

namespace Bread {
	namespace FrameWork {
		namespace InverseKinematics
		{
			class HumanCCDIKManager
			{
			private:
				const int FootNum = 2;

			public:
				explicit HumanCCDIKManager() {};
				~HumanCCDIKManager() {};

			public:

				struct LookAtSetup
				{
					IJoint* _pHead  = nullptr;
					IJoint* _pNeck  = nullptr;
					IJoint* _pSpine = nullptr;
					f32 dot = 0;
					bool* flg = nullptr;
					Math::Vector3 axis;

					Math::Vector3* _targetPos;
				};

				struct LegSetup
				{
					IJoint* _pHip   = nullptr;
					IJoint* _pKnee  = nullptr;
					IJoint* _pAnkle = nullptr;
				};

				struct FootIkSetUp
				{
					const Math::Vector3 cRayHeightOffset    { 0.f, .1f , 0.f };
					const Math::Vector3 kFootRayHeightOffset{ 0.f, 50.f, 0.f };
					const Math::Vector3 kDown               { 0  , -1  , 0   };

					LegSetup	                _legSetup[2];
					std::shared_ptr<RayCastCom> rayCast  [2];
					std::shared_ptr<ModelObject> pmodel;

					Math::Vector3 _anklesIniWs[2];
					Math::Vector3 _anklesTgtWs[2];
					Math::Vector3 _pelvisOffset{ 0, 0, 0 };
					std::shared_ptr<Transform> _rootTrans = nullptr;

					bool _pelvisCorrection = true;
					bool _heightFlg        = true;
					bool _twoBoneFlg       = true;
					bool _aimIk            = true;
					f32 _rootYaw    = 2.f;
					f32 _footHeight = 10.0f;
					f32 _weight     = 1.f;
					f32 _soften     = 1.f;
				};

				struct ArmSetup
				{
					IJoint* _chest    = nullptr;
					IJoint* _shoulder = nullptr;
					IJoint* _elbow    = nullptr;
					IJoint* _list     = nullptr;
				};

				struct HoldHandSetup
				{
					IJoint* _leaderHand;

					ArmSetup _armSetup;
				};

				void Start();

				void Update();

				void __fastcall HoldHand(std::shared_ptr<HoldHandSetup> holdHand);

				void __fastcall HumanLookAt(std::shared_ptr<LookAtSetup> lookAt);

				void __fastcall LookAtNode(std::shared_ptr<LookAtSetup> lookAt, const Math::Vector3& targetPos, const float& rate);

				void __fastcall LookAtParentRotation(std::shared_ptr<LookAtSetup> lookAt, float rate);

				void __fastcall FootIk(std::shared_ptr<FootIkSetUp> footIk);

				bool __fastcall UpdateAnklesTarget(std::shared_ptr<HumanCCDIKManager::FootIkSetUp> footIk);

				bool __fastcall UpdatePelvisOffset(std::shared_ptr<HumanCCDIKManager::FootIkSetUp> footIk);

				Math::Matrix __fastcall GetRootTransform(std::shared_ptr<FootIkSetUp> footIk);

				void __fastcall CCDIKSolver(IJoint* pEffector, const Math::Vector3& faceNormal, const Math::Vector3& hitCoordinate, const std::shared_ptr<Transform> root = nullptr);

				void __fastcall HandCCDIKSolver(IJoint* folHand, IJoint* leadHand);

				void __fastcall CCDIKParentSolver(IJoint* pEffector, IJoint* pCurrent, const Math::Vector3& hitCoordinate, const std::shared_ptr<Transform> root = nullptr);

				void __fastcall CulculateParentLocal(const Math::Vector3& basis2EffectDir, const Math::Vector3& basis2TargetDir, f32 rotationAngle, IJoint* pCurrent, const std::shared_ptr<Transform> root);

				void __fastcall CulculateAngle(IJoint* ankle, IJoint* hip, const Math::Vector3& targetPos, Math::Vector3& basis2EffectDir, Math::Vector3& basis2TargetDir, f32& rotateAngle, const std::shared_ptr<Transform> root);

				void __fastcall UpdateTransform(ITransform* _node);

				void __fastcall UpdateChildTranslate(ITransform* _pParent);

				void __fastcall RegisterHoldHand(std::shared_ptr<ModelObject> follower, std::shared_ptr<ModelObject> leader);

				void __fastcall UnRegisterHoldHand(std::shared_ptr<HumanCCDIKManager::HoldHandSetup> follower);

				void __fastcall RegisterLookAt(std::shared_ptr<ModelObject> model, Math::Vector3* targetPos, bool* flg);

				void __fastcall UnRegisterLookAt(std::shared_ptr<LookAtSetup> lookAt);

				void __fastcall RegisterFootIk(std::shared_ptr<ModelObject> model, std::shared_ptr<Transform> rootT, const std::shared_ptr<RayCastCom> rayCast[2]);

				void  __fastcall UnRegisterFootIk(std::shared_ptr<FootIkSetUp> lookAt);

				void Gui();
			private:
				std::vector<std::shared_ptr<LookAtSetup>>	_registedLookAt;
				std::vector<std::shared_ptr<FootIkSetUp>>	_registedFootIk;
				std::vector<std::shared_ptr<HoldHandSetup>>	_registedHoldHand;
			};
		}

	}
}