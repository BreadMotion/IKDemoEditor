#pragma once
#include <math.h>
#include <DirectXMath.h>

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "AABB.h"
#include "Color.h"
#include "Arithmetic.h"

//simdの各型の詳細
//https://www.xlsoft.com/jp/products/intel/compilers/manual/cpp_all_os/GUID-2A1CEEE6-9E7D-4FE0-A3A8-E66371F1CFE9.htm

namespace Bread
{
	namespace Math
	{
#pragma region Functions for f32
		extern f32 f32Lerp(const f32 f1, const f32 f2, f32 s);
#pragma endregion

#pragma region Functions for Vector2
		extern DirectX::XMFLOAT2 ConvertToFloat2FromVector2(const Vector2 v);
		extern DirectX::XMVECTOR ConvertToVectorFromVector2(const Vector2 v);
		extern Vector2 ConvertToVector2FromVector(const DirectX::XMVECTOR& v);

		// 2Dベクトルの長さを返す。
		extern f32 Vector2Length(const Vector2 v);

		//正規化
		extern Vector2 Vector2Normalize(const Vector2 v);

		//線形補間
		extern Vector2 Vector2Lerp(const Vector2 v1, const Vector2 v2, f32 s);
#pragma endregion

#pragma region Functions for Vector3
		extern DirectX::XMFLOAT3 ConvertToFloat3FromVector3(const Vector3 v);
		extern DirectX::XMVECTOR ConvertToVectorFromVector3(const Vector3 v);

		extern Vector3 ConvertToVector3FromFloat3(const DirectX::XMFLOAT3& f);
		extern Vector3 ConvertToVector3FromVector(const DirectX::XMVECTOR& v);

		//正規化
		extern Vector3 Vector3Normalize(const Vector3 v);

		// 2つの3Dベクトルを減算する。
		extern Vector3 Vector3Subtract(const Vector3 v1, const Vector3 v2);

		// 2つの3Dベクトルの内積を計算する
		extern f32 Vector3Dot(const Vector3 v1, const Vector3 v2);

		// 2つの3Dベクトルの外積を計算する。
		extern Vector3 Vector3Cross(const Vector3 v1, const Vector3 v2);

		extern Vector3 Vector3Lerp(const Vector3 v1, const Vector3 v2, f32 s);

		extern Vector3 Vector3SphereLinear(const Vector3 v1, const Vector3 v2, f32 s);

		// 指定された行列により 3D ベクトルをトランスフォームし、その結果を w = 1 に射影する。
		extern Vector3 Vector3TransformCoord(const Vector3 v, const Matrix m);

		// 3Dベクトルの長さを返す。
		extern f32 Vector3Length(const Vector3 v);
		extern f32 Vector3LengthSq(const Vector3 v);

		Vector3 ConvertToRollPitchYawFromRotationMatrix(const Matrix m);
		Vector3 ConvertToYawPitchRollFromRotationMatrix(const Matrix m);

		Vector3 ConvertToRollPitchYawFromQuaternion(const Quaternion q);
		Vector3 ConvertToYawPitchRollFromQuaternion(const Quaternion q);

#pragma endregion

#pragma region Functions for Vector4
		extern DirectX::XMFLOAT4 ConvertToFloat4FromVector4(const Vector4 v);
		extern DirectX::XMVECTOR ConvertToVectorFromVector4(const Vector4 v);
		extern Vector4 ConvertToVector4FromVector(const DirectX::XMVECTOR& v);

		// 4Dベクトルの長さを返す
		f32 Vector4Length(const Vector4 v);

		// 4Dベクトルの正規化したベクトルを返す。
		Vector4 Vector4Normalize(const Vector4 v);

		// 2つの4Dベクトルの内積を計算する。
		f32 Vector4Dot(const Vector4 v1, const Vector4 v2);

		Vector4 Vector4Rotate(const Vector4 v, const Quaternion q);
#pragma endregion

#pragma region Functions for Matrix
		extern DirectX::XMFLOAT4X4 ConvertToFloat4x4FromVector4x4(const Matrix m);
		extern DirectX::XMMATRIX ConvertToMatrixFromVector4x4(const Matrix m);

		extern Matrix ConvertToVector4x4FromFloat4x4(const DirectX::XMFLOAT4X4& m);
		extern Matrix ConvertToVector4x4FromMatrix(const DirectX::XMMATRIX& m);

		extern Matrix MatrixIdentity();
		extern Matrix MatrixInverse(const Matrix m);
		extern Matrix MatrixMultiply(const Matrix m1, const Matrix m2);
		extern Matrix MatrixTranspose(const Matrix m);
		extern Matrix MatrixMultiplyTranspose(const Matrix m1, const Matrix m2);

		//行列からLocatio成分を取得する
		Vector3 GetLocation(const Matrix& m);

		//行列からScale成分を取得する
		Vector3 GetScale(const Matrix& m);

		//行列からRotate成分を取得する
		Quaternion GetRotation(const Matrix& m);

		// 平行移動オフセットを指定して行列を作成する。
		Matrix MatrixTranslation(f32 x, f32 y, f32 z);

		// x 軸、y 軸、z 軸に沿ってスケーリングする行列を作成する。
		Matrix MatrixScaling(f32 sx, f32 sy, f32 sz);

		// x 軸を回転軸にして回転する行列を作成する。
		Matrix MatrixRotationX(f32 angle);

		// y 軸を回転軸にして回転する行列を作成する。
		Matrix MatrixRotationY(f32 angle);

		// z 軸を回転軸にして回転する行列を作成する。
		Matrix MatrixRotationZ(f32 angle);

		//回転行列からfront方向の単位ベクトルを取得する
		Vector3 FrontVectorFromRotationMatrix(const Matrix& m);

		//回転行列からRight方向の単位ベクトルを取得する
		Vector3 RightVectorFromRotationMatrix(const Matrix& m);

		//回転行列からUp方向の単位ベクトルを取得する
		Vector3 UpVectorFromRotationMatrix(const Matrix& m);

		// ロール、ピッチ、およびヨーを指定して行列を作成する。
		Matrix MatrixRotationRollPitchYaw(f32 roll, f32 pitch, f32 yaw);

		// ヨー、ピッチ、およびロールを指定して行列を作成する。
		Matrix MatrixRotationYawPitchRoll(f32 yaw, f32 pitch, f32 roll);

		// クォータニオンから回転行列を作成する。
		Matrix MatrixRotationQuaternion(const Quaternion* q);

		// 視野に基づいて、右手座標系パースペクティブ射影行列を作成する。
		extern Matrix MatrixPerspectiveFov(f32 fovY, f32 aspect, f32 zn, f32 zf);

		// カスタマイズした右手座標系正射影行列を作成する。
		extern Matrix MatrixOrthoOffCenter(f32 l, f32 r, f32 b, f32 t, f32 zn, f32 zf);

		// 右手座標系ビュー行列を作成する。
		extern Matrix MatrixLookAt(const Vector3 eye, const Vector3 at, const Vector3 up);

		// 右手座標系正射影行列を作成する。
		extern Matrix MatrixOrtho(f32 w, f32 h, f32 zn, f32 zf);
#pragma endregion

#pragma region Functions for Quaternion
		extern DirectX::XMVECTOR ConvertToVectorFromQuaternion(const Quaternion q);
		extern Quaternion ConvertToQuaternionFromVector(const DirectX::XMVECTOR& v);

		extern Quaternion ConvertToQuaternionFromRotationMatrix(const Matrix m);
		extern Matrix ConvertToRotationMatrixFromQuaternion(const Quaternion q);

		Quaternion ConvertToQuaternionFromYawPitchRoll(f32 yaw, f32 pitch, f32 roll);
		Quaternion ConvertToQuaternionFromRollPitchYaw(f32 roll, f32 pitch, f32 yaw);

		Quaternion QuaternionIdentity();

		//extern Quaternion QuaternionMultiply(const Quaternion q1, const Quaternion q2);
		//extern Quaternion QuaternionRotationAxis(Vector4 axis, float angle);

		/*extern Vector4 RotateVector(const Quaternion q, const Vector4& target);
		extern Vector4 RotateFrontVector(const Quaternion q);
		extern Vector4 RotateUpVector(const Quaternion q);
		extern Vector4 RotateRightVector(const Quaternion q);*/

		extern f32              QuaternionDot(const Quaternion q1, const Quaternion q2);
		extern Quaternion QuaternionMultiply(const Quaternion& q1, const Quaternion& q2);
		extern Quaternion QuaternionRotationAxis(Vector3 axis, float angle);

		// Y軸を回転軸としてクォータニオンを回転させる
		Quaternion QuaternionRotationY(f32 angle);

		extern Quaternion QuaternionSlerp(const Quaternion q1, const Quaternion q2, f32 t);
#pragma endregion
	} // namespace Math
} // namespace Bread