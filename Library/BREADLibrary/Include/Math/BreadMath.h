#pragma once
#include <ivec.h>
#include <fvec.h>
#include <dvec.h>
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
		/// <summary>
		/// 線形補間量を返す
		/// </summary>
		/// <param name="f1">始点</param>
		/// <param name="f2">終点</param>
		/// <param name="s">補間量 (0 - 1 => 0 - 100)</param>
		/// <returns>線形補間量</returns>
		extern f32     _fastcall f32Lerp(const f32& f1, const f32& f2, f32 s);

		/// <summary>
        ///ヘロンの定理で面積を算出
        /// </summary>
        /// <param name="A">辺の長さ</param>
        /// <param name="B">辺の長さ</param>
        /// <param name="C">辺の長さ</param>
        /// <returns>三角形の面積</returns>
		extern f32     _fastcall f32Area_HeronFormula(const f32& A, const f32& B, const f32& C);

		/// <summary>
        ///ヘロンの定理で各角度を算出
        /// </summary>
        /// <param name="A">、辺の長さ</param>
        /// <param name="B">、辺の長さ</param>
        /// <param name="C">、辺の長さ</param>
        /// <returns>Vector3(Aの対角の角度、Bの対角の角度、Cの対角の角度)</returns>
		extern Vector3 _fastcall f32Angle_HeronFormula(const f32& A, const f32& B, const f32& C);
#pragma endregion

#pragma region Functions for Vector2
		extern DirectX::XMFLOAT2 _fastcall ConvertToFloat2FromVector2(const Vector2& v);
		extern DirectX::XMVECTOR _fastcall ConvertToVectorFromVector2(const Vector2& v);
		extern Vector2 _vectorcall ConvertToVector2FromVector(const DirectX::XMVECTOR v);

		// 2Dベクトルの長さを返す。
		extern f32 _fastcall Vector2Length(const Vector2& v);

		//正規化
		extern Vector2 _fastcall Vector2Normalize(const Vector2& v);

		//線形補間
		extern Vector2 _fastcall Vector2Lerp(const Vector2& v1, const Vector2& v2, f32 s);
#pragma endregion

#pragma region Functions for Vector3
		extern DirectX::XMFLOAT3 _fastcall ConvertToFloat3FromVector3(const Vector3& v);
		extern DirectX::XMVECTOR _fastcall ConvertToVectorFromVector3(const Vector3& v);

		extern Vector3 _fastcall ConvertToVector3FromFloat3(const DirectX::XMFLOAT3& f);
		extern Vector3 _vectorcall ConvertToVector3FromVector(const DirectX::XMVECTOR v);

		//正規化
		extern Vector3 _fastcall Vector3Normalize(const Vector3& v);

		// 2つの3Dベクトルを減算する。
		extern Vector3 _fastcall Vector3Subtract(const Vector3& v1, const Vector3& v2);

		// 2つの3Dベクトルの内積を計算するVector3Lerp
		extern f32 _fastcall Vector3Dot(const Vector3& v1, const Vector3& v2);

		// 2つの3Dベクトルの外積を計算する。
		extern Vector3 _fastcall Vector3Cross(const Vector3& v1, const Vector3& v2);

		extern Vector3 _fastcall Vector3Lerp(const Vector3& v1, const Vector3& v2, f32 s);

		extern Vector3 _fastcall Vector3SphereLinear(const Vector3& v1, const Vector3& v2, f32& s);

		// 指定された行列により 3D ベクトルをトランスフォームし、その結果を w = 1 に射影する。
		extern Vector3 _fastcall Vector3TransformCoord(const Vector3& v, const Matrix& m);

		// 3Dベクトルの長さを返す。
		extern f32 _fastcall Vector3Length(const Vector3& v);
		extern f32 _fastcall Vector3LengthSq(const Vector3& v);

		extern Vector3 _fastcall ConvertToRollPitchYawFromRotationMatrix(const Matrix& m);
		extern Vector3 _fastcall ConvertToYawPitchRollFromRotationMatrix(const Matrix& m);

		extern Vector3 _fastcall ConvertToRollPitchYawFromQuaternion(const Quaternion& q);
		extern Vector3 _fastcall ConvertToYawPitchRollFromQuaternion(const Quaternion& q);

		extern bool _fastcall ToEulerAngleZXY(float& _rXOut, float& _rYOut, float& _rZOut, const Matrix& _rRot);
		extern bool _fastcall ToEulerAngleZXY(Vector3& _rOut, const Matrix& _rRot);

		extern Vector3 _fastcall ClampVector(Vector3& clamper, Vector3& min, Vector3& max);

#pragma endregion

#pragma region Functions for Vector4
		extern DirectX::XMFLOAT4 _fastcall ConvertToFloat4FromVector4(const Vector4& v);
		extern DirectX::XMVECTOR _fastcall ConvertToVectorFromVector4(const Vector4& v);
		extern Vector4 _vectorcall ConvertToVector4FromVector(const DirectX::XMVECTOR v);

		// 4Dベクトルの長さを返す
		extern f32 _fastcall Vector4Length(const Vector4& v);

		// 4Dベクトルの正規化したベクトルを返す。
		extern Vector4 _fastcall Vector4Normalize(const Vector4& v);

		// 2つの4Dベクトルの内積を計算する。
		extern f32 _fastcall Vector4Dot(const Vector4& v1, const Vector4& v2);

		extern Vector4 _fastcall Vector4Rotate(const Vector4& v, const Quaternion& q);

		extern Vector4 _fastcall GetColX(const Matrix& m);

		extern Vector4 _fastcall GetColY(const Matrix& m);

		extern Vector4 _fastcall GetColZ(const Matrix& m);

		extern Vector4 _fastcall GetColW(const Matrix& m);
#pragma endregion

#pragma region Functions for Matrix
		extern DirectX::XMFLOAT4X4 _fastcall ConvertToFloat4x4FromVector4x4(const Matrix& m);
		extern DirectX::XMMATRIX _fastcall ConvertToMatrixFromVector4x4(const Matrix& m);

		extern Matrix _fastcall ConvertToVector4x4FromFloat4x4(const DirectX::XMFLOAT4X4& m);
		extern Matrix _vectorcall ConvertToVector4x4FromMatrix(const DirectX::XMMATRIX m);

		extern Matrix  MatrixIdentity();
		extern Vector3 _fastcall MultiplyMatrixVector(const Matrix& a, const Vector3& v);
		extern Vector3 _fastcall MultiplyRowMatrixVector(const Vector3& v, const Matrix& a);
		extern Matrix  _fastcall MatrixInverse(const Matrix& m);
		extern Matrix  _fastcall MatrixMultiply(const Matrix& m1, const Matrix& m2);
		extern Matrix  _fastcall MatrixTranspose(const Matrix& m);
		extern Matrix  _fastcall MatrixMultiplyTranspose(const Matrix& m1, const Matrix& m2);


		extern Vector3 _fastcall GetVector3ColX(const Matrix& m);

		extern Vector3 _fastcall GetVector3ColY(const Matrix& m);

		extern Vector3 _fastcall GetVector3ColZ(const Matrix& m);

		//行列からLocatio成分を取得する
		extern Vector3 _fastcall GetLocation(const Matrix& m);

		//行列からScale成分を取得する
		extern Vector3 _fastcall GetScale(const Matrix& m);

		//行列からRotate成分を取得する
		extern Quaternion _fastcall GetRotation(const Matrix& m);

		// 平行移動オフセットを指定して行列を作成する。
		extern Matrix _fastcall MatrixTranslation(const f32& x, const f32& y, const f32& z);

		// x 軸、y 軸、z 軸に沿ってスケーリングする行列を作成する。
		extern Matrix _fastcall MatrixScaling(const f32& sx, const f32& sy, const f32& sz);

		// x 軸を回転軸にして回転する行列を作成する。
		extern Matrix _fastcall MatrixRotationX(const f32& angle);

		// y 軸を回転軸にして回転する行列を作成する。
		extern Matrix _fastcall MatrixRotationY(const f32& angle);

		// z 軸を回転軸にして回転する行列を作成する。
		extern Matrix _fastcall MatrixRotationZ(const f32& angle);

		//回転行列からfront方向の単位ベクトルを取得する
		extern Vector3 _fastcall FrontVectorFromRotationMatrix(const Matrix& m);

		//回転行列からRight方向の単位ベクトルを取得する
		extern Vector3 _fastcall RightVectorFromRotationMatrix(const Matrix& m);

		//回転行列からUp方向の単位ベクトルを取得する
		extern Vector3 _fastcall UpVectorFromRotationMatrix(const Matrix& m);

		// ロール、ピッチ、およびヨーを指定して行列を作成する。
		extern Matrix _fastcall MatrixRotationRollPitchYaw(const f32& roll, const f32& pitch, const f32& yaw);

		// ヨー、ピッチ、およびロールを指定して行列を作成する。
		extern Matrix _fastcall  MatrixRotationYawPitchRoll(const f32& yaw, const f32& pitch, const f32& roll);

		// クォータニオンから回転行列を作成する。
		Matrix _fastcall MatrixRotationQuaternion(const Quaternion& q);

		// 視野に基づいて、右手座標系パースペクティブ射影行列を作成する。
		extern Matrix _fastcall MatrixPerspectiveFov(const f32& fovY, const f32& aspect, const f32& zn, const f32& zf);

		// カスタマイズした右手座標系正射影行列を作成する。
		extern Matrix _fastcall  MatrixOrthoOffCenter(const f32& l, const f32& r, const f32& b, const f32& t, const f32& zn, const f32& zf);

		// 右手座標系ビュー行列を作成する。
		extern Matrix _fastcall MatrixLookAt(const Vector3& eye, const Vector3& at, const Vector3& up);

		// 右手座標系正射影行列を作成する。
		extern Matrix _fastcall MatrixOrtho(const f32& w, const f32& h, const f32& zn, const f32& zf);
#pragma endregion

#pragma region Functions for Quaternion
		extern  DirectX::XMVECTOR _fastcall ConvertToVectorFromQuaternion(const Quaternion& q);
		extern Quaternion _vectorcall ConvertToQuaternionFromVector(const DirectX::XMVECTOR v);

		extern Quaternion _fastcall ConvertToQuaternionFromRotationMatrix(const Matrix& m);
		extern Matrix _fastcall ConvertToRotationMatrixFromQuaternion(const Quaternion& q);

		extern Quaternion _fastcall ConvertToQuaternionFromYawPitchRoll(const f32& yaw, const f32& pitch, const f32& roll);
		extern Quaternion _fastcall ConvertToQuaternionFromRollPitchYaw(const f32& roll, const f32& pitch, const f32& yaw);

		extern Quaternion QuaternionIdentity();

		//extern Quaternion QuaternionMultiply(const Quaternion q1, const Quaternion q2);
		//extern Quaternion QuaternionRotationAxis(Vector4 axis, float angle);

		/*extern Vector4 RotateVector(const Quaternion q, const Vector4& target);
		extern Vector4 RotateFrontVector(const Quaternion q);
		extern Vector4 RotateUpVector(const Quaternion q);
		extern Vector4 RotateRightVector(const Quaternion q);*/

		extern f32        _fastcall QuaternionDot(const Quaternion& q1, const Quaternion& q2);
		extern Quaternion _fastcall QuaternionMultiply(const Quaternion& q1, const Quaternion& q2);
		extern Quaternion _fastcall QuaternionRotationAxis(const Vector3& axis, const float& angle);

		// Y軸を回転軸としてクォータニオンを回転させる
		extern Quaternion _fastcall QuaternionRotationY(f32 angle);

		extern Quaternion _fastcall QuaternionSlerp(const Quaternion& q1, const Quaternion& q2, f32 t);
#pragma endregion

#pragma region Functions for Vector
		extern f32    _vectorcall VectorLength(const vector v);
		extern vector _vectorcall VectorNormalize(const vector v);
#pragma endregion
	} // namespace Math
} // namespace Bread