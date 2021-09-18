#pragma once
#include <memory>
#include "Math/BreadMath.h"
#include "FrameWork/Object/Object.h"
#include "FrameWork/Component/Transform.h"
#include "../../../BREADProject/Source/RayCast/RayCast.h"

#define FOOT_NUM 2
#define LOOP_MAX 50

namespace Bread {
	namespace FrameWork {

		class IKManager
		{
		private:
			const int _FootNum = 2;

		public:
			explicit IKManager() {};
			~IKManager() {};

		public:

			struct LookAtSetup
			{
				ModelObject::Node* _pHead  = nullptr;
				ModelObject::Node* _pNeck  = nullptr;
				ModelObject::Node* _pSpine = nullptr;
				f32 dot = 0;
				bool* flg = nullptr;
				Math::Vector3 axis;

				Math::Vector3* _targetPos;
			};

			struct LegSetup
			{
				ModelObject::Node* _pHip   = nullptr;
				ModelObject::Node* _pKnee  = nullptr;
				ModelObject::Node* _pAnkle = nullptr;
			};

			struct FootIkSetUp
			{
				const Math::Vector3 cRayHeightOffset{ 0.f, .1f, 0.f };
				const Math::Vector3 kFootRayHeightOffset{ 0.f, 50.f, 0.f };
				const Math::Vector3 kDown{0, -1, 0};

				LegSetup	_legSetup[2];
				std::shared_ptr<RayCastCom> rayCast[2];
				std::shared_ptr<ModelObject> pmodel;

				Math::Vector3 _anklesIniWs[2];
				Math::Vector3 _anklesTgtWs[2];
				Math::Vector3 _pelvisOffset{ 0, 0, 0 };
				const Math::Matrix* _rootTrans = nullptr;

				bool _pelvisCorrection = true;
				bool _heightFlg  = true;
				bool _twoBoneFlg = true;
				bool _aimIk      = true;
				f32 _rootYaw     = 2.f;
				f32 _footHeight  = 15.0f;
				f32 _weight      = 1.f;
				f32 _soften      = 1.f;
			};

			struct ArmSetup
			{
				ModelObject::Node* _chest    = nullptr;
				ModelObject::Node* _shoulder = nullptr;
				ModelObject::Node* _elbow    = nullptr;
				ModelObject::Node* _list     = nullptr;
			};

			struct HoldHandSetup
			{
				ModelObject::Node* _leaderHand;

				ArmSetup _armSetup;
			};

			void Start();

			void Update();

			void __fastcall HoldHand(std::shared_ptr<HoldHandSetup> holdHand);

			void __fastcall HumanLookAt(std::shared_ptr<LookAtSetup> lookAt);

			void __fastcall LookAtNode(std::shared_ptr<LookAtSetup> lookAt, const Math::Vector3& targetPos, const float& rate);

			void __fastcall LookAtParentRotation(std::shared_ptr<LookAtSetup> lookAt, float rate);

			void __fastcall FootIk(std::shared_ptr<FootIkSetUp> footIk);

			bool __fastcall UpdateAnklesTarget(std::shared_ptr<IKManager::FootIkSetUp> footIk);

			bool __fastcall UpdatePelvisOffset(std::shared_ptr<IKManager::FootIkSetUp> footIk);

			Math::Matrix __fastcall GetRootTransform(std::shared_ptr<FootIkSetUp> footIk);

			void __fastcall CCDIKSolver(ModelObject::Node* pEffector, const Math::Vector3& faceNormal, const Math::Vector3& hitCoordinate, const Math::Matrix* root = nullptr);

			void __fastcall HandCCDIKSolver(ModelObject::Node* folHand, ModelObject::Node* leadHand);

			void __fastcall CCDIKParentSolver(ModelObject::Node* pEffector, ModelObject::Node* pCurrent, const Math::Vector3& hitCoordinate, const Math::Matrix* root = nullptr);

			void __fastcall CulculateParentLocal(const Math::Vector3& basis2EffectDir, const Math::Vector3& basis2TargetDir, f32 rotationAngle, ModelObject::Node* pCurrent,const Math::Matrix* root);

			void __fastcall CulculateAngle(ModelObject::Node* ankle, ModelObject::Node* hip, const Math::Vector3& targetPos,  Math::Vector3& basis2EffectDir,  Math::Vector3& basis2TargetDir, f32& rotateAngle, const Math::Matrix* root);

			void __fastcall UpdateTransform(ModelObject::Node* _node);

			void __fastcall UpdateChildTranslate(ModelObject::Node* _pParent);

			void __fastcall RegisterHoldHand(std::shared_ptr<ModelObject> follower, std::shared_ptr<ModelObject> leader);

			void __fastcall UnRegisterHoldHand(std::shared_ptr<IKManager::HoldHandSetup> follower);

			void __fastcall RegisterLookAt(std::shared_ptr<ModelObject> model, Math::Vector3* targetPos, bool* flg);

			void __fastcall UnRegisterLookAt(std::shared_ptr<LookAtSetup> lookAt);

			void __fastcall RegisterFootIk(std::shared_ptr<ModelObject> model, std::shared_ptr<Transform> rootT,const std::shared_ptr<RayCastCom> rayCast[2]);

			void  __fastcall UnRegisterFootIk(std::shared_ptr<FootIkSetUp> lookAt);

			void Gui();
		private:
			std::vector<std::shared_ptr<LookAtSetup>>	_registedLookAt;
			std::vector<std::shared_ptr<FootIkSetUp>>	_registedFootIk;
			std::vector<std::shared_ptr<HoldHandSetup>>	_registedHoldHand;
		};

	}
}