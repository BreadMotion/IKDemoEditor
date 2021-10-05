#include "pch.h"
#include "FND/STD.h"
#include "FND/Util.h"
#include "Math/Arithmetic.h"
#include "Math/BreadMath.h"
#include "Graphics/Camera.h"
#include "FrameWork/Input/InputDevice.h"
#include "../ImGui/Include/imgui.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// カメラ操作オブジェクト
		//****************************************************************************
		Camera::Camera()
		{
			SetPerspective(30.0f * 0.01745f, 16.0f / 9.0f, 0.1f, 100000.0f);
			SetLookAt(Math::Vector3::OneAll, Math::Vector3::Zero, Math::Vector3::OneY);

			rotateX  = 0.5f;
			rotateY  = 0.0f; //-3.14f
			distance = 5000.0f;
		}

		// ビュータイプ設定
		void Camera::SetViewType(ViewType viewType)
		{
			if (this->viewType != viewType)
			{
				this->viewType = viewType;

				if (this->viewType == ViewType::Perspective)
				{
					SetPerspective(fovY, aspect, nearZ, farZ);
				}
				else
				{
					Math::Vector3 front, up;
					GetViewVectors(front, up);

					f32 d = lookAtDistance;
					SetLookAt(focus - front * d, focus, up);
					SetOrthographic(
						d * aspect / 2,
						-d * aspect / 2,
						d / 2,
						-d / 2,
						nearZ,
						farZ);
				}
			}
		}

		void Camera::SetViewType(bool perspective)
		{
			if (perspective)
			{
				viewType = ViewType::Perspective;
				SetPerspective(fovY, aspect, nearZ, farZ);
			}
			else
			{
				viewType = ViewType::Orthographic;
				f32 d = lookAtDistance;
				SetOrthographic(
					d * aspect / 2,
					-d * aspect / 2,
					d / 2,
					-d / 2,
					nearZ,
					farZ);
			}
		}


		// 更新
		void Camera::Update()
		{
			FreeCamera();
			DataUpdate();
		}

		void Camera::GUI()
		{
			using namespace ImGui;
			float rotatex = rotateX;
			float rotatey = rotateY;
			if (ImGui::CollapsingHeader(u8"カメラ", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
			{
				DragFloat3("eye",                      &eye.x);
				RegisterWatchVal("eye -" + GetID(),    &eye);
				DragFloat3("focus",                    &focus.x);
				RegisterWatchVal("focus -" + GetID(),  &focus);
				DragFloat3("target",                   &target.x);
				RegisterWatchVal("target -" + GetID(), &target);

				DragFloat("rotateX", &rotatex);
				DragFloat("rotateY", &rotatey);
				Separator();

				static int selected = 0;
				int          rastIndex = 0;
				//ViewType viewType = ViewType::Perspective;
				for (; rastIndex <= (int)ViewType::TypeNum;)
				{
					if (ImGui::Selectable(std::to_string(rastIndex).c_str(), selected == rastIndex))//TODO : 列挙型を文字列に変換できるようにする
					{
						selected  = rastIndex;
						viewType = (ViewType)rastIndex;
						SetViewType(viewType);
					}
					rastIndex++;
				}
			}
		}

		void Camera::DataUpdate()
		{
			// ビュー
			Math::Matrix inverse;
			inverse = Math::MatrixInverse(view);

			right.x = inverse.m[0][0];
			right.y = inverse.m[0][1];
			right.z = inverse.m[0][2];

			up.x = inverse.m[1][0];
			up.y = inverse.m[1][1];
			up.z = inverse.m[1][2];

			front.x = -inverse.m[2][0];
			front.y = -inverse.m[2][1];
			front.z = -inverse.m[2][2];

			eye.x = inverse.m[3][0];
			eye.y = inverse.m[3][1];
			eye.z = inverse.m[3][2];

			focus.x = eye.x + front.x * lookAtDistance;
			focus.y = eye.y + front.y * lookAtDistance;
			focus.z = eye.z + front.z * lookAtDistance;

			// プロジェクション
			Math::Matrix inverseProjection;
			inverseProjection = Math::MatrixInverse(projection);

			Math::Vector3 p0, p1;
			p0 = Math::Vector3TransformCoord(Math::Vector3::Zero, inverseProjection);
			p1 = Math::Vector3TransformCoord(Math::Vector3::OneZ, inverseProjection);

			if (!IsOrtho())
			{
				fovY = 2.0f * Math::ATanF32(1.0f / projection.m[1][1]);
			}
			aspect = projection.m[1][1] / projection.m[0][0];
			nearZ  = FND::FAbs(p0.z);
			farZ     = FND::FAbs(p1.z);

			// 視錐体
			/*Math::Matrix view_projection;
			Math::MatrixMultiply(&view_projection, &m_view, &m_projection);
			m_frustum.InitFromMatrix(view_projection);*/
		}

		// ビュープロジェクション設定
		void Camera::SetViewProjection(const Math::Matrix& view, const Math::Matrix& projection)
		{
			this->view = view;
			this->projection = projection;

			// ビュー
			Math::Matrix inverseView;
			inverseView = Math::MatrixInverse(view);

			right.x = inverseView.m[0][0];
			right.y = inverseView.m[0][1];
			right.z = inverseView.m[0][2];

			up.x = inverseView.m[1][0];
			up.y = inverseView.m[1][1];
			up.z = inverseView.m[1][2];

			front.x = -inverseView.m[2][0];
			front.y = -inverseView.m[2][1];
			front.z = -inverseView.m[2][2];

			eye.x = inverseView.m[3][0];
			eye.y = inverseView.m[3][1];
			eye.z = inverseView.m[3][2];

			focus.x = eye.x + front.x * lookAtDistance;
			focus.y = eye.y + front.y * lookAtDistance;
			focus.z = eye.z + front.z * lookAtDistance;

			// プロジェクション
			Math::Matrix inverseProjection;
			inverseProjection = Math::MatrixInverse(projection);

			Math::Vector3 p0, p1;
			p0 = Math::Vector3TransformCoord(Math::Vector3::Zero, inverseProjection);
			p1 = Math::Vector3TransformCoord(Math::Vector3::OneZ, inverseProjection);

			if (!IsOrtho())
			{
				fovY = 2.0f * Math::ATanF32(1.0f / projection.m[1][1]);
			}
			aspect = projection.m[1][1] / projection.m[0][0];
			nearZ  = FND::FAbs(p0.z);
			farZ   = FND::FAbs(p1.z);

			// 視錐体
			/*Math::Matrix viewProjection;
			viewProjection = Math::MatrixMultiply(view, projection);
			frustum.InitFromMatrix(viewProjection);*/
		}


		// Y軸画角設定
		void Camera::SetFovY(f32 fovY)
		{
			this->fovY = fovY;
			SetViewType(!IsOrtho());
		}

		// アスペクト比設定
		void Camera::SetAspect(f32 aspect)
		{
			this->aspect = aspect;
			SetViewType(!IsOrtho());
		}

		// ニアクリップ値設定
		void Camera::SetNearZ(f32 nearZ)
		{
			this->nearZ = nearZ;
			SetViewType(!IsOrtho());
		}

		// ファークリップ値設定
		void Camera::SetFarZ(f32 farZ)
		{
			this->farZ = farZ;
			SetViewType(!IsOrtho());
		}

		// 透視投影行列設定
		void Camera::SetPerspective(f32 fovY, f32 aspect, f32 nearZ, f32 farZ)
		{
			projection = Math::MatrixPerspectiveFov(fovY, aspect, nearZ, farZ);
		}

		// 並行投影行列設定
		void Camera::SetOrthographic(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ)
		{
			projection = Math::MatrixOrthoOffCenter(left, right, bottom, top, nearZ, farZ);
		}

		// ビュー行列設定
		void Camera::SetLookAt(const Math::Vector3& eye, const Math::Vector3& focus, const Math::Vector3& up)
		{
			view = Math::MatrixLookAt(eye, focus, up);
			lookAtDistance = Math::Vector3Length(focus - eye);
		}

		// ビュー行列を設定
		void Camera::SetView(const Math::Matrix& view)
		{
			this->view = view;
		}

		// プロジェクション行列を設定
		void Camera::SetProjection(const Math::Matrix& projection)
		{
			this->projection = projection;
		}

		// スクリーン用ビュープロジェクション行列設定
		void Camera::SetScreen(f32 width, f32 height)
		{
			aspect = width / height;

			view         = Math::MatrixIdentity();
			view.m[2][2] = -1.0f;
			view.m[3][0] = -width * 0.5f;
			view.m[3][1] = -height * 0.5f;

			projection = Math::MatrixOrtho(width, -height, 0, 1);
		}

		// compute unit per pixel at the given world position.
		f32 Camera::ComputeUnitPerPixel(const Math::Vector3& worldPosition, f32 vh) const
		{
			f32 worldWidth, worldHeight;
			ComputeWorldDimensions(worldPosition, worldWidth, worldHeight);
			return (worldHeight / vh);
		}

		// compute world dimensions at the given world position.
		void Camera::ComputeWorldDimensions(const Math::Vector3& worldPosition, f32& width, f32& height) const
		{
			if (IsOrtho())
			{
				width  = 2.0f / projection._11;
				height = 2.0f / projection._22;
			}
			else
			{
				// Transform worldPos to view space.
				Math::Vector3 v;
				v = Math::Vector3TransformCoord(worldPosition, view);
				f32  dist = FND::FAbs(v.z);
				width  = (2.0f * dist) / projection._11;
				height = (2.0f * dist) / projection._22;
			}
		}

		// ビルボード行列を計算
		void Camera::ComputeBillboard(const Math::Vector3& worldPosition, Math::Matrix& billboard) const
		{
			billboard._11 = view._11;
			billboard._12 = view._21;
			billboard._13 = view._31;
			billboard._14 = 0.0f;
			billboard._21 = view._12;
			billboard._22 = view._22;
			billboard._23 = view._32;
			billboard._24 = 0.0f;
			billboard._31 = view._13;
			billboard._32 = view._23;
			billboard._33 = view._33;
			billboard._34 = 0.0f;
			billboard._41 = worldPosition.x;
			billboard._42 = worldPosition.y;
			billboard._43 = worldPosition.z;
			billboard._44 = 1.0f;
		}

		// ビューベクトルを取得
		void Camera::GetViewVectors(Math::Vector3& front, Math::Vector3& up)
		{
			switch (viewType)
			{
			case ViewType::Front:
				front = Math::Vector3(0, 0, -1);
				up     = Math::Vector3(0, 1, 0);
				break;

			case ViewType::Back:
				front = Math::Vector3(0, 0, 1);
				up    = Math::Vector3(0, 1, 0);
				break;

			case ViewType::Top:
				front = Math::Vector3(0, -1, 0);
				up    = Math::Vector3(0, 0, -1);
				break;

			case ViewType::Bottom:
				front = Math::Vector3(0, 1, 0);
				up    = Math::Vector3(0, 0, -1);
				break;

			case ViewType::Left:
				front = Math::Vector3(1, 0, 0);
				up    = Math::Vector3(0, 1, 0);
				break;

			case ViewType::Right:
				front = Math::Vector3(-1, 0, 0);
				up    = Math::Vector3(0, 1, 0);
				break;

			case ViewType::Perspective:
				front = this->front;
				up    = this->up;
				break;
			}
		}

		// 視錐体を設定
		/*void Camera::SetFructum(const Math::Matrix& view, const Math::Matrix& projection)
		{
			Math::Matrix view_projection;
			Math::MatrixMultiply(&view_projection, &view, &projection);
			m_frustum.InitFromMatrix(view_projection);
		}*/

		// 球の範囲が視界に入るようにズーム
		void Camera::ZoomOnSphere(const Math::Vector3& center, f32 radius)
		{
			f32 d = radius;
			if (d == 0.0f)
			{
				d = 1;
			}

			if (d > farZ)
			{
				d = farZ;
			}
			else if (d < nearZ)
			{
				d = nearZ;
			}

			focusRadius = d;

			Math::Vector3 front, up;
			GetViewVectors(front, up);

			Math::Vector3 eye = center - front * d;
			SetLookAt(eye, center, up);

#if 0
			if (m_view_type == ViewType::Perspective)
			{
				f32 near_z = fnd::FAbs(radius) * 0.1f;
				near_z = fnd::Max(near_z, 0.001f);
				m_near_z = near_z;
			}
#endif
		}

		// フリーカメラ
		void Camera::FreeCamera()
		{
			// TODO : Win関数を別の関数に差し替え
			POINT cursor;
			GetCursorPos(&cursor);

			oldCursor  = newCursor;
			newCursor = Math::Vector2(static_cast<float>(cursor.x), static_cast<float>(cursor.y));

			f32 moveX = (newCursor.x - oldCursor.x) * 0.02f;
			f32 moveY = (newCursor.y - oldCursor.y) * 0.02f;

#if 1
			if (GetKeyState(VK_SPACE) < 0)
			{
				if (GetKeyState(VK_LBUTTON) < 0)
				{
					rotateY -= moveX * 0.5f;
					rotateX += moveY * 0.5f;
				}
				else if (GetKeyState(VK_MBUTTON) < 0)
				{
					float s = distance * 0.35f;
					float x = moveX * s;
					float y = moveY * s;

					target.x -= right.x * x;
					target.y -= right.y * x;
					target.z -= right.z * x;

					target.x += up.x * y;
					target.y += up.y * y;
					target.z += up.z * y;
				}
				else if (GetKeyState(VK_RBUTTON) < 0)
				{
					distance += (-moveY - moveX) * distance * 0.1f;
				}
				else
				{
					distance -= (float)MOUSE.diMouseState.lZ / 1.0f;
				}
			}

			f32 xSin  = sinf(rotateX);
			f32 xCos = cosf(rotateX);
			f32 ySin  = sinf(rotateY);
			f32 yCos = cosf(rotateY);

			Math::Vector3 front  = { -xCos * ySin, -xSin, -xCos * yCos };
			Math::Vector3 _right = { yCos, 0.0f, -ySin };
			Math::Vector3 _up    = Math::Vector3Cross(_right, front);

			Math::Vector3 _target    = target;
			Math::Vector3 _distance = { distance, distance, distance };
			Math::Vector3 _pos        =  _target - (front * _distance);

			SetLookAt(_pos, _target, _up);

#else
			else
			if (GetKeyState(VK_RBUTTON) < 0)
			{
				f32 xSin = sinf(rotateX);
				f32 xCos = cosf(rotateX);
				f32 ySin = sinf(rotateY);
				f32 yCos = cosf(rotateY);

				Math::Vector3 front = { xCos * -ySin, xSin, xCos * -yCos };
				Math::Vector3 _right = { yCos, 0.0f, -ySin };
				Math::Vector3 _up = Math::Vector3Cross(_right, front);

				rotateY -= moveX * 0.25f;
				rotateX += moveY * 0.25f;

				if (GetKeyState('W') < 0)
				{
					eye += -front * speed;
				}
				if (GetKeyState('S') < 0)
				{
					eye += front * speed;
				}
				if (GetKeyState('A') < 0)
				{
					eye += _right * speed;
				}
				if (GetKeyState('D') < 0)
				{
					eye += -_right * speed;
				}
				if (GetKeyState('Q') < 0)
				{
					eye += -_up * speed;
				}
				if (GetKeyState('E') < 0)
				{
					eye += _up * speed;
				}

				xSin = sinf(rotateX);
				xCos = cosf(rotateX);
				ySin = sinf(rotateY);
				yCos = cosf(rotateY);

				front = { xCos * -ySin, xSin, xCos * -yCos };
				_right = { yCos, 0.0f, -ySin };
				_up = Math::Vector3Cross(_right, front);

				Math::Vector3 _pos = eye;
				Math::Vector3 _distance = { distance, distance, distance };
				//Math::Vector3 _pos = _target - (front * _distance);
				Math::Vector3 _target = _pos - (front * _distance);
				//target = _target;

				SetLookAt(_pos, _target, _up);
			}
#endif
		}

		void Camera::SurveyCamera(f32 addRotateX, f32 addRotateY, f32 distance, Math::Vector3 target)
		{
			rotateX += addRotateX;
			rotateY += addRotateY;

			f32 xSin  = sinf(rotateX);
			f32 xCos = cosf(rotateX);
			f32 ySin  = sinf(rotateY);
			f32 yCos = cosf(rotateY);

			Math::Vector3 front  = { -xCos * ySin, -xSin, -xCos * yCos };
			Math::Vector3 _right = { yCos, 0.0f, -ySin };
			Math::Vector3 _up    = Math::Vector3Cross(_right, front);

			Math::Vector3 _target     = target;
			Math::Vector3 _distance = { distance, distance, distance };
			Math::Vector3 _pos        = _target - (front * _distance);

			SetLookAt(_pos, _target, _up);
		}

		void Camera::ControllerCamera(const Math::Vector3& center, const Math::Vector3& adjust, const Bread::f32 lerpTime)
		{
			// TODO : Win関数を別の関数に差し替え
			POINT cursor;
			GetCursorPos(&cursor);
			oldCursor  = newCursor;
			newCursor = Math::Vector2(static_cast<float>(cursor.x), static_cast<float>(cursor.y));

			Bread::f32 sX = 0.0f;
			Bread::f32 sY = 0.0f;

			sX = xInput[0].sRX / 1000.0f;
			sY = xInput[0].sRY / 1000.0f;

			sY = GetKeyState(VK_UP)         < 0 ? -1.0f : sY;
			sY = GetKeyState(VK_DOWN)  < 0 ? 1.0f : sY;
			sX = GetKeyState(VK_LEFT)      < 0 ? -1.0f : sX;
			sX = GetKeyState(VK_RIGHT)   < 0 ? 1.0f : sX;

			if (GetKeyState(VK_RBUTTON) < 0)
			{
				f32 moveX = (newCursor.x - oldCursor.x) * 0.02f;
				f32 moveY = (newCursor.y - oldCursor.y) * 0.02f;
				rotateY -= moveX * 0.5f;
				rotateX += moveY * 0.5f;
			}
			else
			{
				rotateY -= sX * 3.5f * 0.01745f;
				rotateX -= sY * 3.5f * 0.01745f;
				if (0.0f < rotateX)
				{
					rotateX = 0.0f;
				}
				if (rotateX < -1.0f)
				{
					rotateX = -1.0f;
				}
			}

			f32 xSin  = sinf(rotateX);
			f32 xCos = cosf(rotateX);
			f32 ySin  = sinf(rotateY);
			f32 yCos = cosf(rotateY);

			// このフレームのベクトルを使用するのでここで計算しておく
			Math::Vector3 _front = { xCos * -ySin, xSin, xCos * -yCos };
			Math::Vector3 _right = { yCos, 0.0f, -ySin };
			Math::Vector3 _up    = Math::Vector3Cross(_right, _front);
			front = _front;

			// ここで中心を計算しておくことで注視点とカメラ座標を求めることができる
			//eye = (center + adjust);
			eye = Bread::Math::Vector3Lerp(eye, (center + adjust), lerpTime);

			// 注視点がターゲットに移動している時と移動していない時で分岐しておくことで処理が見やすくなる
			if (onTarget)
			{
				// 画面にターゲットが映っていない状態からいきなり映ると違和感が出る
				// そこで線形補間を使用して、徐々に注視点をターゲットに近づけていくことで振り向いたようになる
				focus = Bread::Math::Vector3Lerp(focus, targetPos, 0.1f);

				// ある程度、近づいてきたらフリーカメラに戻したため
				f32 dis = fabs(Bread::Math::Vector3Length(focus - targetPos));
				if (dis <= 1.0f)
				{
					targetCnt = 0;
					onTarget = false;
				}

				// このフレームで回転した値を次のフレームで使用するのでここで計算しておく
				Math::Vector3 dir = center - focus;
				dir = Math::Vector3Normalize(dir);
				rotateX = atan2f(-dir.y, dir.z);
				rotateY = atan2f(dir.x, dir.z);
			}
			else
			{
				focus = Bread::Math::Vector3Lerp(focus, (eye - (front * -7.5f)), 0.5f);
			}

			// カメラ座標を計算する時に、前ベクトルが変わってしまっているので再計算
			front = Bread::Math::Vector3Normalize(focus - eye);

			Math::Vector3 _pos = eye - (front * 7.5f);
			_pos.y = _pos.y <= 1.0f ? 1.0f : _pos.y; // ステージに埋まってしまうので押し出し

			// カメラシェイク値を注視点に加算するとズレが生じるので変数に代入
			Math::Vector3 _target = focus + shake;

			_right = Math::Vector3Cross(front, _up);
			_up = Math::Vector3Cross(_right, front);

			SetLookAt(_pos, _target, _up);
		}

		void Camera::ControllerCamera02(bool onControl, const Math::Vector3& center, const Math::Vector3& adjust, const Bread::f32 len, const Bread::f32 elapsedTime, const Bread::f32 lerpTime, bool adjustRotate, const Bread::Math::Vector3 targetFrontVec)
		{
			// TODO : Win関数を別の関数に差し替え
			POINT cursor;
			GetCursorPos(&cursor);
			oldCursor = newCursor;
			newCursor = Math::Vector2(static_cast<float>(cursor.x), static_cast<float>(cursor.y));

			Bread::f32 sX = 0.0f;
			Bread::f32 sY = 0.0f;

			sX = xInput[0].sRX / 1000.0f;
			sY = xInput[0].sRY / 1000.0f;

			sY = GetKeyState(VK_UP)    < 0 ? -1.0f : sY;
			sY = GetKeyState(VK_DOWN)  < 0 ?  1.0f : sY;
			sX = GetKeyState(VK_LEFT)  < 0 ? -1.0f : sX;
			sX = GetKeyState(VK_RIGHT) < 0 ?  1.0f : sX;

			if (onControl)
			{
				if (GetKeyState(VK_RBUTTON) < 0)
				{
					f32 moveX = (newCursor.x - oldCursor.x) * 0.02f;
					f32 moveY = (newCursor.y - oldCursor.y) * 0.02f;
					rotateY  -= moveX * 0.5f * elapsedTime;
					//rotateX += moveY * 0.5f * elapsedTime;
				}
				else
				{
					rotateY -= sX * 3.5f * 0.01745f * elapsedTime;
					/*rotateX -= sY * 3.5f * 0.01745f * elapsedTime;
					if (0.5f < rotateX)
					{
						rotateX = 0.5f;
					}
					if (rotateX < -0.5f)
					{
						rotateX = -0.5f;
					}*/
				}
			}

			//if (sY == 0.0f)
			{
				//rotateX = Bread::Math::f32Lerp(rotateX, -0.225f, 0.15f * elapsedTime);
				rotateX = -0.225f;
			}

			f32 xSin = sinf(rotateX);
			f32 xCos = cosf(rotateX);
			f32 ySin = sinf(rotateY);
			f32 yCos = cosf(rotateY);

			// このフレームのベクトルを使用するのでここで計算しておく
			Math::Vector3 _front = { xCos * -ySin, xSin, xCos * -yCos };
			Math::Vector3 _right = { yCos, 0.0f, -ySin };
			Math::Vector3 _up    = Math::Vector3Cross(_right, _front);
			front = _front;

			// 斜めに補間
			bool hit = false;
			static bool oldHit = false;
			if (sX == 0.0f && adjustRotate && (targetFrontVec.x != 0.0f || targetFrontVec.z != 0.0f))
			{
				_front.y = 0.0f;

				Bread::f32 dot   = Bread::Math::Vector3Dot(targetFrontVec, _front);
				Bread::f32 angle = acosf(dot) / 0.01745f;

				if (1e-8f < fabs(angle))
				{
					Bread::f32 angleR;
					angleR = acosf(Bread::Math::Vector3Dot(targetFrontVec, _right));
					angleR -= (90.0f * 0.01745f);

					if (0.0f < angleR)
					{
						angle *= -1;
					}

					if (0.0f <= fabs(angle) && fabs(angle) < 35.0f)
					{
						hit = true;

						Bread::f32 originAngle = rotateY;
						Bread::f32 newAngle = originAngle + (angle * 0.01745f);

						if (!oldHit && 25.0f <= fabs(angle) && fabs(angle) < 35.0f)
						{
							adjustAngle = rotateY;
						}
						else
						{
							adjustAngle = Bread::Math::f32Lerp(adjustAngle, newAngle, lerpTime * elapsedTime);
							rotateY     = adjustAngle;
							oldHit      = hit;
						}
					}
				}
			}
			if (!hit)
			{
				adjustAngle = rotateY;
				oldHit      = false;
			}

			// 注視点が変わっても線形補間で違和感をなくす
			focus = Bread::Math::Vector3Lerp(focus, (center + adjust), lerpTime * elapsedTime);

			Math::Vector3 _pos = focus - (front * len);
			_pos.y = _pos.y <= 1.0f ? 1.0f : _pos.y; // ステージに埋まってしまうので押し出し
			eye    = _pos;

			// カメラシェイク値を注視点に加算するとズレが生じるので変数に代入
			Math::Vector3 _target = focus + shake;

			_right = Math::Vector3Cross(front, _up);
			_up    = Math::Vector3Cross(_right, front);

			SetLookAt(_pos, _target, _up);
		}

		void Camera::LockOnCamera(const Math::Vector3& center, const Math::Vector3& target, const Math::Vector3& centerAdjust, const Math::Vector3& targetAdjust, bool isLerp)
		{
#if 0
			Math::Vector3 cameraPos;

			Math::Vector3 dir = pos - center;
			dir.y = 0.0f;

			f32 len = Math::Vector3Length(dir) + 400.0f;
			Math::Vector3 dirN = Math::Vector3Normalize(dir);

			rotateY = atan2f(dirN.x, dirN.z);
			cameraPos = center + dirN * len;

			Math::Vector3 right = Math::Vector3Cross(Math::Vector3::OneY, -dirN);
			cameraPos += right * -150.0f;
			cameraPos.y = pos.y;

			SetLookAt(cameraPos, center, Math::Vector3::OneY);
#elif 0
			Math::Vector3 dir = (center + centerAdjust) - (target + targetAdjust);
			dir.y = center.y - target.y /*0.0f*/;
			dir = Math::Vector3Normalize(dir);

			rotateX = 0.0f;
			rotateY = atan2f(dir.x, dir.z);

			eye = (center + centerAdjust); /*Bread::Math::Vector3Lerp(eye, center, 0.6f)*/
			focus = Bread::Math::Vector3Lerp(focus, (target + targetAdjust), 0.05f);
			front = Bread::Math::Vector3Lerp(front, -dir, 0.05f);

			Math::Vector3 _pos = eye - (front * 6.5f);
			Math::Vector3 _target = focus;

			dir = center - target;
			dir.y = 0.0f;
			Math::Vector3 dirN = Math::Vector3Normalize(dir);
			Math::Vector3 right = Math::Vector3Cross(Math::Vector3::OneY, -dirN);
			_pos += right * -0.75f;

			_pos.y = _pos.y <= 1.0f ? 1.0f : _pos.y;

			SetLookAt(_pos, _target, Math::Vector3::OneY);
#elif 1
			eye = (center + centerAdjust); /*Bread::Math::Vector3Lerp(eye, center, 0.6f)*/
			if (isLerp) focus = Bread::Math::Vector3Lerp(focus, (target + targetAdjust), 0.05f);
			else        focus = target + targetAdjust;

			Math::Vector3 dir = eye - focus;
			dir.y = center.y - target.y /*0.0f*/;
			dir   = Math::Vector3Normalize(dir);

			rotateX = 0.0f;
			rotateY = atan2f(dir.x, dir.z);

			front = Bread::Math::Vector3Lerp(front, -dir, 0.05f);

			Math::Vector3 _pos    = eye - (front * 6.5f);
			Math::Vector3 _target = focus + shake;

			dir   = center - target;
			dir.y = 0.0f;
			Math::Vector3 dirN = Math::Vector3Normalize(dir);

			right = Math::Vector3Cross(Math::Vector3::OneY, -dirN);
			_pos += right * -adjustRight;

			//Bread::Math::Vector3 vec = Vector3SphereLinear(right, front, 0.5f);
			//_pos += vec * -adjustRight;

			_pos.y = _pos.y <= 1.0f ? 1.0f : _pos.y;

			SetLookAt(_pos, _target, Math::Vector3::OneY);
#endif
		}

		void Camera::SetTargetPos(const Math::Vector3& target, const Math::Vector3& targetAdjust)
		{
			onTarget  = true;
			targetPos = target + targetAdjust;
			targetCnt = 0;
		}

		void Camera::SetTarget(const Math::Vector3& target, const Math::Vector3& targetAdjust)
		{
			this->target = target + targetAdjust;
		}

		void Camera::SphereLinearLockOnCamera(const Math::Vector3& center, const Math::Vector3& start, const Math::Vector3& end, const Math::Vector3& centerAdjust, f32 sphereLinearSpeed, f32 distanceToFouceFromCamera)
		{
			eye                    = (center + centerAdjust); /*Bread::Math::Vector3Lerp(eye, center, 0.6f)*/
			Math::Vector3 startVec = start - eye;
			Math::Vector3 endVec   = end - eye;

			startVec.y = 0.0f;
			endVec.y   = 0.0f;

			Math::Vector3 vec = Bread::Math::Vector3SphereLinear(start, end, sphereLinearSpeed);
			focus             = eye + vec * distanceToFouceFromCamera;

			Math::Vector3 dir = eye - focus;
			dir.y            = center.y - target.y /*0.0f*/;
			dir              = Math::Vector3Normalize(dir);

			rotateX = 0.0f;
			rotateY = atan2f(dir.x, dir.z);

			front = Bread::Math::Vector3Lerp(front, -dir, 0.05f);

			Math::Vector3 _pos    = eye - (front * 6.5f);
			Math::Vector3 _target = focus;

			dir   = center - target;
			dir.y = 0.0f;
			Math::Vector3 dirN = Math::Vector3Normalize(dir);
			Math::Vector3 right = Math::Vector3Cross(Math::Vector3::OneY, -dirN);
			_pos += right * -0.75f;

			_pos.y = _pos.y <= 1.0f ? 1.0f : _pos.y;

			SetLookAt(_pos, _target, Math::Vector3::OneY);
		}

		void Camera::InitEventCamera(const Math::Vector3& focus, const Math::Vector3& front, Bread::f32 len)
		{
			this->focus = focus;

			Math::Vector3 _target = focus + shake;
			Math::Vector3 _right  = Math::Vector3Cross(front, Math::Vector3::OneY);
			Math::Vector3 _up     = Math::Vector3Cross(_right, front);
			Math::Vector3 _pos    = this->focus - (front * len);

			eye = _pos;

			SetLookAt(_pos, _target, _up);
		}

		void Camera::EventCamera(const Math::Vector3& focus, const Math::Vector3& front, Bread::f32 len)
		{
			this->focus = Bread::Math::Vector3Lerp(this->focus, focus, 0.5f);

			Math::Vector3 _target = focus + shake;
			Math::Vector3 _right  = Math::Vector3Cross(front, Math::Vector3::OneY);
			Math::Vector3 _up     = Math::Vector3Cross(_right, front);
			Math::Vector3 _pos    = this->focus - (front * len);

			eye = _pos;

			SetLookAt(_pos, _target, _up);
		}
	} // namespace Graphics
} // namespace Bread