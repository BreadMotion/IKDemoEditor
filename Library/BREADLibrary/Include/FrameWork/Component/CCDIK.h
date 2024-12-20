#pragma once
#include "FrameWork/Component/Component.h"
#include "FrameWork/Object/Object.h"
#include "Math/BreadMath.h"
#include <vector>

enum PART
{
	ARM,
	FOOT,
	TOE,
	HEAD,
	NONE
};

namespace Bread{
	namespace FrameWork {
		class CyclicCoordinateDescent : public Component
		{
		public:
			struct ComputePart
			{
				const Bread::Math::Matrix* targetWorldTransform = nullptr;
				Bread::FrameWork::IJoint* beginJoint = nullptr;
				Bread::FrameWork::IJoint* endJoint   = nullptr;
				Bread::Math::Vector3 targetPos      = Bread::Math::Vector3::Zero;
				Bread::Math::Vector3 targetIKNormal = Bread::Math::Vector3::Zero;
				Bread::Math::Vector3 rayStart       = Bread::Math::Vector3::Zero;
				Bread::Math::Vector3 kDown          = Bread::Math::Vector3::Zero;
				Bread::f32           ankleHeight    = 0.0f;
				Bread::s32           iterate        = 0;
				PART part;
				bool                 computeCCDIK   = false;

				ComputePart(
					const Bread::Math::Matrix* transform,
					Bread::FrameWork::IJoint* begin,
					Bread::FrameWork::IJoint* end,
					Bread::Math::Vector3                 target,
					Bread::Math::Vector3                 targetIKPos,
					Bread::s32                           it,
					Bread::f32                           ankle,
					PART                                 par) // TODO : 名前
				{
					targetWorldTransform = transform;
					beginJoint = begin;
					endJoint = end;
					targetPos = target;
					targetIKNormal = targetIKPos;
					iterate = it;
					ankleHeight = ankle;
					part = par;
				};

				//エフェクタの目標地点の設定
				void setTargetPos(const Bread::Math::Vector3& targetPos) { this->targetPos = targetPos; }

				//目標地点の法線情報を設定
				void setTargetIKNormal(const Bread::Math::Vector3& targetIKNormal) { this->targetIKNormal = targetIKNormal; };

				//レイの始点情報の設定
				void setRayStart(const Bread::Math::Vector3& v) { rayStart = v; }

				//骨盤の下ベクトル情報の設定
				void setKDown(const Bread::Math::Vector3& v) { kDown = v; }
			};

		public:
			explicit CyclicCoordinateDescent() {}
			~CyclicCoordinateDescent() override {}

		public:
			// コンストラクタ
			void Construct()  override;

			// デストラクタ
			void Destruct()   override;

			// 初期化
			void Initialize() override;

			// 終了化
			void Finalize()   override;

			//事前更新
			void __fastcall PreUpdate()override {}

			// 更新
			void __fastcall Update()override;

			//事前更新
			void __fastcall NextUpdate()override {}

			//imgui
			void GUI()        override;

			//部分更新
			void __fastcall PartUpdate(const Bread::u32& index);

		public:
			//自身のTransform行列更新後子ジョイントに向けて再起処理
			void __fastcall updateAllPosition(Bread::FrameWork::IJoint& joint, const Bread::Math::Matrix* targetWorldTransform);
			void __fastcall UpdatePosition(   Bread::FrameWork::IJoint& joint, const Bread::Math::Vector3& euler, const Bread::Math::Matrix* targetWorldTransform);

			//角度制限
			bool __fastcall JointAngleLimit(      Bread::Math::Vector3& euler, Bread::FrameWork::IJoint& joint, Bread::Math::Vector3& parentEuler);
			bool __fastcall ZMinusJointAngleLimit(Bread::Math::Vector3& euler, Bread::FrameWork::IJoint& joint, Bread::Math::Vector3& parentEuler);

			//回転値を求める
			float __fastcall LocalRotateAngle(const Bread::Math::Vector3& jointToTarget, const Bread::Math::Vector3& jointToEffector);

			//回転軸を外積で求める
			Bread::Math::Vector3 __fastcall CreateLocalRotationAxis(const Bread::Math::Vector3& toEffector, const Bread::Math::Vector3& toTarget);

		public:
			//足のIK
			void __fastcall FootCCDIK(
				Bread::FrameWork::IJoint* beginJoint,
				Bread::FrameWork::IJoint* endJoint,
				const Bread::Math::Vector3& targetPos,
				const Bread::Math::Vector3& normalVector,
				const Bread::Math::Vector3& rayStart,
				const Bread::Math::Vector3& kDown,
				const Bread::Math::Matrix* targetWorldTransform,
				const float                ankleHeight,
				int                        iteratNum);

		public:
			//IK処理を行う部分を追加する
			void __fastcall AddOrder(
				const Bread::Math::Matrix* transform,
				Bread::FrameWork::IJoint* begin,
				Bread::FrameWork::IJoint* end,
				Bread::Math::Vector3                 target,
				const Bread::Math::Vector3           normalVector,
				const Bread::s32                     it,
				const Bread::f32                     ankle,
				const PART                           par)
			{
				order.emplace_back(
					std::make_shared<ComputePart>(transform, begin, end, target, normalVector, it, ankle, par));
			}
		public:
			std::vector<std::shared_ptr<ComputePart>>             order;
		};
	}
}