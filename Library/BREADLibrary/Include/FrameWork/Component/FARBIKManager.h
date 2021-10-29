#pragma once
#include <memory>
#include "Math/BreadMath.h"
#include "FrameWork/Object/Object.h"
#include "FrameWork/Component/Transform.h"
#include "../../../BREADProject/Source/RayCast/RayCast.h"

namespace Bread
{
	namespace FrameWork
	{
		namespace FootIK
		{
			//足の判定に使う変数
			const Math::Vector3 rayHeightOffset      { 0.0f, 0.1f , 0.0f };
			const Math::Vector3 footRayHeightOffset  { 0.0f, 50.0f, 0.0f };
			const Math::Vector3 downVec              { 0.0f, -1.0f, 0.0f };
			const Math::Vector3 upVec                { 0.0f,1.0f,0.0f    };

			//足ジョイントの設定
			struct LegSetUp
			                             {
				ModelObject::Node* pHip  { nullptr };
				ModelObject::Node* pKnee { nullptr };
				ModelObject::Node* pAnkle{ nullptr };
			};

			//FootIKのセットアップ
			struct FootIKSetUp
			{
				//足２本参照変数
				LegSetUp legSetup[2];

				//必要コンポーネントにアクセスする用変数
				std::shared_ptr<RayCastCom>  rayCast[2];
				std::shared_ptr<ModelObject> model;
				std::shared_ptr<Transform>   rootTransform;

				//ジョイントの必要データのキャッシュ変数
				Math::Vector3 anklesIniWs   [2];
				Math::Vector3 anklesTargetWs[2];
				Math::Vector3 pelvisOffset;

				//IKシステムの設定変数 (GUIにて動的変更ができるように定数にしない)
				bool pelbisCorrection{ true };
				bool heightFlag      { true };
				bool twoBoneFlag     { true };
				bool  aimIK          { true };
				f32 rootYaw   { 2.0f  };
				f32 footHeight{ 20.0f };
				f32 weight    { 1.0f  };
				f32 soften    { 1.0f  };
			};

		}

		class [[]] FARBIKManager final
		{
		private://humanIKをするにあたって確定された定義
			std::vector<std::shared_ptr<FootIK::FootIKSetUp>> registedFootIK;

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
			void _fastcall FootIK(std::shared_ptr<FootIK::FootIKSetUp> footIk);

			//踝の目標座標の更新
			void __fastcall UpdateAnklesTarget(std::shared_ptr<FootIK::FootIKSetUp> footIk);

			//骨盤の目標座標の更新
			void __fastcall UpdatePelvisOffset(std::shared_ptr<FootIK::FootIKSetUp> footIk);

		private://FootIK Suport Function
			const Math::Matrix& _fastcall GetRootTransform(std::shared_ptr<FootIK::FootIKSetUp> footIk);

			//FootIK用 - 子から親に向かっての角ジョイントのワールド座標を算出
			//※AnkleTargetWorldが股関節から膝に向けてのベクトルの上にいた場合の例外処理 (膝が逆方向に曲がる計算になってしまうため)
			void __fastcall ExceptionCuluculate_HingeJoint(std::vector<Math::Vector3>& targetPosition, std::vector<Math::Vector3>&      anserPosition,
				                                           FootIK::LegSetUp&           footIk,         const std::shared_ptr<Transform> root);


		private: //Shared function
			//CCDIKを用いて足の座標を目標地点に変えていく
			void __fastcall CCDIKSolver       (ModelObject::Node* pEffector, const Math::Vector3& faceNormal, const Math::Vector3& hitCoordinate, const std::shared_ptr<Transform> root = nullptr);

			//FARBIKを用いて脚の座標を目標地点に変えていく
			void __fastcall FARBIKParentSolver(std::shared_ptr<FootIK::FootIKSetUp> footIk, const u32 iterate);

			//子から親に向かっての各ジョイントのワールド座標を算出
			void __fastcall ForwardCuluculate (std::vector<Math::Vector3>& targetPosition, ModelObject::Node*               pEffector,
				                               ModelObject::Node*          pCurrent,       const std::shared_ptr<Transform> root = nullptr);

			//親から子に向かっての各ジョイントのワールド座標を算出
			void __fastcall BackwardCuluculate(std::vector<Math::Vector3>& anserPosition, const Math::Vector3& targetPosition,
				                               ModelObject::Node*          pEffector,     ModelObject::Node*    pCurrent, const std::shared_ptr<Transform> root = nullptr);

			//BackwardCuluculateで算出した座標までのベクトルに添うようにジョイントを回転させる
			void __fastcall IKSolver(const Math::Vector3& anserPosition, ModelObject::Node*   pEffector,
				                     ModelObject::Node*   pCurrent, const std::shared_ptr<Transform> root = nullptr);

			//BackwardCuluculateで算出した座標までのベクトルに添うようにジョイントを回転させる
			void __fastcall IKSolver2(std::vector<Math::Vector3> anserPosition, std::shared_ptr<FootIK::FootIKSetUp> footIk, const u32 iterate);


			//対象の親のローカル座標を計算する
			void __fastcall CulculateParentLocal(const Math::Vector3& basis2EffectDir, const Math::Vector3& basis2TargetDir,
				                                 f32                  rotationAngle,   ModelObject::Node*   pCurrent,          const std::shared_ptr<Transform> root);

			//対象の角度を計算する
			void __fastcall CulculateAngle(ModelObject::Node*   ankle,           ModelObject::Node* hip,
				                           const Math::Vector3& targetPos,       Math::Vector3&     basis2EffectDir,
				                           Math::Vector3&       basis2TargetDir, f32&               rotateAngle,       const std::shared_ptr<Transform> root);

			//対象のTrasnformを更新する
			void _fastcall UpdateTransform     (ModelObject::Node* node);
			void _fastcall UpdateChildTranslate(ModelObject::Node* node);

		public://Register, UnRegister Function
			//FootIK Register
			void _fastcall RegisterFootIK  (std::shared_ptr<ModelObject> model, std::shared_ptr<Transform> rootT, const std::shared_ptr<RayCastCom> rayCast[2]);
			void _fastcall UnRegisterFootIK(std::shared_ptr<FootIK::FootIKSetUp> footIk);

		public://FARBIK GUI
			void GUI();
		};
	}
}