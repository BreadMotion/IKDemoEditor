#pragma once
#include <string>
#include "FND/NonCopyable.h"
#include "FrameWork//Component/Component.h"
//#include "Math/Frustum.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// ビュータイプ
		//****************************************************************************
		enum class ViewType
		{
			Front,				// 並行投影・前
			Back,				// 並行投影・後
			Top,				// 並行投影・上
			Bottom,				// 並行投影・下
			Left,				// 並行投影・左
			Right,				// 並行投影・右
			Perspective,		// 透視投影
			Orthographic,		// 並行投影

			TypeNum
		};

		//****************************************************************************
		// カメラ操作オブジェクト
		//****************************************************************************
		class Camera final : public FrameWork::Component
		{
		private:
			ViewType viewType = ViewType::Perspective;

			Math::Matrix projection;
			Math::Matrix view;
			Math::Matrix inverseView;
			//Math::Frustum	frustum;
			Math::Vector3 eye;
			Math::Vector3 focus;
			Math::Vector3 front;
			Math::Vector3 up;
			Math::Vector3 right;
			f32 lookAtDistance;
			f32 focusRadius;
			f32 aspect;
			f32 fovY;
			f32 nearZ;
			f32 farZ;

			Math::Vector3 target;
			Math::Vector2 oldCursor;
			Math::Vector2 newCursor;
			f32   rotateX;
			f32   rotateY;
			float distance;

			Bread::Math::Vector3 shake = Bread::Math::Vector3(0.0f, 0.0f, 0.0f);

			f32 adjustAngle = 0.0f;

		public:
			f32 speed       = 1.0f;
			f32 adjustRight = 0.75f;

			bool onTarget              = false;
			s32 targetCnt              = 0;
			s32 targetMaxCnt           = 10;
			Math::Vector3 targetPos = { 0.0f, 0.0f, 0.0f };

		public:
			explicit  Camera();
			~Camera() {}

		public:
			//コンストラクタ
			void Construct()override {}

			//デストラクタ
			void Destruct() override{}

			//初期化
			void Initialize()override {}

			//終了処理
			void Finalize() override{}

			//事前更新
			void __fastcall PreUpdate(const f32& dt)override {}

			// 更新
			void __fastcall Update(const f32& dt)override;

			//事後更新
			void __fastcall NextUpdate(const f32& dt)override {}

			//描画
			void __fastcall Draw(const f32& dt)override {}

			//GUI
			void GUI()override;

		public:
			//カメラデータの更新
			void DataUpdate();

			// ビュータイプ設定
			void __fastcall SetViewType(ViewType viewType);
			void __fastcall SetViewType(bool perspective);

			// ビュータイプ取得
			ViewType GetViewType() const { return viewType; }


			// ビュープロジェクション設定
			void __fastcall SetViewProjection(const Math::Matrix& view, const Math::Matrix& projection);

			// 視錐体取得
			//const Math::Frustum& GetFrustum() const { return m_frustum; }

			// プロジェクション行列取得
			const Math::Matrix& GetProjection() const { return projection; }

			// ビュー行列取得
			const Math::Matrix& GetView() const { return view; }

			// 視点位置取得
			const Math::Vector3& GetEye() const { return eye; }

			// 注視点位置取得
			const Math::Vector3& GetFocus() const { return focus; }

			// 前ベクトル取得
			const Math::Vector3& GetFront() const { return front; }

			// 上ベクトル取得
			const Math::Vector3& GetUp() const { return up; }

			// 右ベクトル取得
			const Math::Vector3& GetRight() const { return right; }

			// 視点から注視点の距離取得
			f32 GetDistanceFromLookAt() const { return lookAtDistance; }

			// 視点を中心としたフォーカス半径取得
			f32 GetFocusRadius() const { return focusRadius; }

			// アスペクト比取得
			f32 GetAspect() const { return aspect; }

			// Y軸画角取得
			f32 GetFovY() const { return fovY; }

			// ニアクリップ値取得
			f32 GetNearZ() const { return nearZ; }

			// ファークリップ値取得
			f32 GetFarZ() const { return farZ; }

			//X回転値取得
			f32 GetRotateX() const { return rotateX; }

			//Y回転値取得
			f32 GetRotateY() const { return rotateY; }

			void __fastcall SetCameraShake(Bread::Math::Vector3 cameraShake) { shake = cameraShake; }

			// 視点位置設定
			void __fastcall SetEye(Math::Vector3 eye) { this->eye = eye; }

			// 注視点位置設定
			void __fastcall SetFocus(Math::Vector3 focus) { this->focus = focus; }

			void __fastcall SetRotateX(f32 rotate) { rotateX = rotate; }
			void __fastcall SetRotateY(f32 rotate) { rotateY = rotate; }

			// 平行投影か判定
			bool IsOrtho() const { return projection.m[2][3] == 0.0f; }

			// Y軸画角設定
			void __fastcall SetFovY(f32 fov_y);

			// アスペクト比設定
			void __fastcall SetAspect(f32 aspect);

			// ニアクリップ値設定
			void __fastcall SetNearZ(f32 near_z);

			// ファークリップ値設定
			void __fastcall SetFarZ(f32 far_z);

			// 透視投影行列を設定
			void __fastcall SetPerspective(f32 fovY, f32 aspect, f32 nearZ, f32 farZ);

			// 並行投影行列を設定
			void __fastcall SetOrthographic(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ);

			// ビュー行列を設定
			void __fastcall SetLookAt(const Math::Vector3& eye, const Math::Vector3& focus, const Math::Vector3& up);

			// ビュー行列を設定
			void __fastcall SetView(const Math::Matrix& view);

			// プロジェクション行列を設定
			void __fastcall SetProjection(const Math::Matrix& projection);

			// スクリーン用ビュープロジェクション行列
			void __fastcall SetScreen(f32 width, f32 height);

			// compute world dimensions at the given world position.
			void __fastcall ComputeWorldDimensions(const Math::Vector3& worldPosition, f32& width, f32& height) const;

			// compute unit per pixel at the given world position.
			// posW: world positoin.
			// vh:  view port height in pixels.
			f32 __fastcall ComputeUnitPerPixel(const Math::Vector3& worldPosition, f32 vh) const;

			// ビルボード行列を計算
			void __fastcall ComputeBillboard(const Math::Vector3& worldPosition, Math::Matrix& billboard) const;

			// 球の範囲が視界に入るようにズーム
			void __fastcall ZoomOnSphere(const Math::Vector3& center, f32 radius);

			// フリーカメラ
			void FreeCamera();

			void __fastcall SurveyCamera(f32 addRotateX, f32 addRotateY, f32 distance, Math::Vector3 target);

			void __fastcall ControllerCamera(const Math::Vector3& center, const Math::Vector3& adjust, const Bread::f32 lerpTime = 1.0f);
			void __fastcall ControllerCamera02(bool onControl, const Math::Vector3& center, const Math::Vector3& adjust, const Bread::f32 len, const Bread::f32 elapsedTime, const Bread::f32 lerpTime = 1.0f, bool adjustRotate = false, const Bread::Math::Vector3 targetFrontVec = Bread::Math::Vector3());

			void __fastcall LockOnCamera(const Math::Vector3& center, const Math::Vector3& target, const Math::Vector3& centerAdjust, const Math::Vector3& targetAdjust, bool isLerp = true);

			void __fastcall SetTargetPos(const Math::Vector3& target, const Math::Vector3& targetAdjust);
			void __fastcall SetTarget(const Math::Vector3& target, const Math::Vector3& targetAdjust);

			void __fastcall SphereLinearLockOnCamera(const Math::Vector3& center, const Math::Vector3& start, const Math::Vector3& end, const Math::Vector3& centerAdjust, f32 sphereLinearSpeed, f32 distanceToFouceFromCamera);

			void __fastcall InitEventCamera(const Math::Vector3& focus, const Math::Vector3& front, Bread::f32 len);
			void __fastcall EventCamera(const Math::Vector3& focus, const Math::Vector3& front, Bread::f32 len);

			void InitCursor()
			{
				POINT cursor;
				GetCursorPos(&cursor);

				oldCursor = newCursor;
				newCursor = Math::Vector2(static_cast<float>(cursor.x), static_cast<float>(cursor.y));
			}
		private:
			// ビューベクトルを取得
			void __fastcall GetViewVectors(Math::Vector3& front, Math::Vector3& up);

			// 視錐体を設定
			//void SetFructum(const Math::Matrix& view, const Math::Matrix& projection);
		};
	} // namespace Graphics
} // namespace Bread