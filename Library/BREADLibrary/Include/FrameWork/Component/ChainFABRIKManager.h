#pragma once
#include <memory>
#include "Math/BreadMath.h"
#include "FrameWork/Object/Object.h"
#include "FrameWork/Object/BasicObjectElement/INode.h"
#include "FrameWork/Component/Transform.h"

namespace Bread
{
	namespace FrameWork
	{
		namespace SetUp
		{
			//セットアップ
			struct IKSetUp
			{
				//ターゲットジョイントの参照変数
				std::vector<IJoint>* setupJoint{ nullptr };

				//必要コンポーネントにアクセスする用変数
				std::shared_ptr<Transform>   rootTransform;

				Math::Vector3* worldTargetPos{ nullptr };
			};
		}

		namespace InverseKinematics
		{
			class [[]] FARBIKManager final
			{
			private://humanIKをするにあたって確定された定義
				std::vector<std::shared_ptr<SetUp::IKSetUp>> registedIK;

			public:
				explicit FARBIKManager() {};
				~FARBIKManager() {};

			public:
				//初回初期化
				void Start();

				//初期化
				void Initialize();

				//更新
				void Update();

			private://FootIK Functions
				//Updateでこれを呼ぶ
				void _fastcall FABRIK(std::shared_ptr<SetUp::IKSetUp> setupIK);

			private: //Shared function
				//FARBIKを用いて脚の座標を目標地点に変えていく
				void __fastcall FARBIKParentSolver(std::shared_ptr<SetUp::IKSetUp> setupIK);

				//子から親に向かっての各ジョイントのワールド座標を算出
				void __fastcall ForwardCuluculate(std::vector<Math::Vector3>& targetPosition, ITransform*                      pEffector,
					                              ITransform*                 pCurrent,       const std::shared_ptr<Transform> root = nullptr);

				//親から子に向かっての各ジョイントのワールド座標を算出
				void __fastcall BackwardCuluculate(std::vector<Math::Vector3>& anserPosition, const Math::Vector3& targetPosition,
					                               ITransform*                 pEffector,     ITransform*          pCurrent,        const std::shared_ptr<Transform> root = nullptr);

				//BackwardCuluculateで算出した座標までのベクトルに添うようにジョイントを回転させる
				void __fastcall FABRIKSolver(const Math::Vector3& anserPosition, ITransform*                      pEffector,
					                         ITransform*          pCurrent,      const std::shared_ptr<Transform> root = nullptr);

				//対象の親のローカル座標を計算する
				void __fastcall CulculateParentLocal(const Math::Vector3& basis2EffectDir, const Math::Vector3& basis2TargetDir,
					                                 f32                  rotationAngle,  ITransform*           pCurrent, const std::shared_ptr<Transform> root);

				//対象の角度を計算する
				void __fastcall CulculateAngle(ITransform*          ankle,           ITransform*    hip,
					                           const Math::Vector3& targetPos,       Math::Vector3& basis2EffectDir,
					                           Math::Vector3&       basis2TargetDir, f32&           rotateAngle, const std::shared_ptr<Transform> root);

				//対象のTrasnformを更新する
				void _fastcall UpdateTransform     (ITransform* node);
				void _fastcall UpdateChildTranslate(ITransform* node);

			public://Register, UnRegister Function
				//FootIK Register
				void _fastcall RegisterFABRIK  (std::vector<IJoint>*           model, std::shared_ptr<Transform> rootT, Math::Vector3* targetPos);
				void _fastcall UnRegisterFABRIK(std::shared_ptr<SetUp::IKSetUp> setupIK);

			public://FARBIK GUI
				void GUI();
			};
		}
	}
}