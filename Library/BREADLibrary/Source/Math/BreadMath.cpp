#include "pch.h"
#include "Math/BreadMath.h"
#include "Math/Arithmetic.h"


namespace Bread
{
	namespace Math
	{
#pragma region Functions for f32
		f32 _fastcall f32Lerp(const f32& f1, const f32& f2, f32 s)
		{
			return (1.0f - s) * (f1)+s * (f2);
		}
#pragma endregion

#pragma region Functions for Vector2
		DirectX::XMFLOAT2 _fastcall ConvertToFloat2FromVector2(const Vector2& v)
		{
			return DirectX::XMFLOAT2{ v.x , v.y};
		}

		DirectX::XMVECTOR _fastcall ConvertToVectorFromVector2(const Vector2& v)
		{
			DirectX::XMFLOAT2 f2{ v.x,v.y };
			return DirectX::XMLoadFloat2(&f2);
		}

		Vector2 _vectorcall ConvertToVector2FromVector(const DirectX::XMVECTOR v)
		{
			DirectX::XMFLOAT2 f2;
			DirectX::XMStoreFloat2(&f2, v);

			return Vector2{ f2.x,f2.y };
		}

		f32 _fastcall Vector2Length(const Vector2& v)
		{
			return SqrtF32((v.x) * (v.x) + (v.y) * (v.y));
		}

		Vector2 _fastcall Vector2Normalize(const Vector2& v)
		{
			DirectX::XMVECTOR vT{ DirectX::XMVector2Normalize(ConvertToVectorFromVector2(v)) };
			return ConvertToVector2FromVector(vT);
		}

		Vector2 _fastcall Vector2Lerp(const Vector2& v1, const Vector2& v2, f32 s)
		{
			return Vector2
			{
				 (1.0f - s) * (v1.x) + s * (v2.x),
				 (1.0f - s) * (v1.y) + s * (v2.y)
			};

		}
#pragma endregion

#pragma region Functions for Vector3
		DirectX::XMFLOAT3 _fastcall ConvertToFloat3FromVector3(const Vector3& v)
		{
			return DirectX::XMFLOAT3{ v.x,v.y,v.z };
		}

		DirectX::XMVECTOR _fastcall ConvertToVectorFromVector3(const Vector3& v)
		{
			DirectX::XMFLOAT3 f3{ v.x,v.y,v.z };
			DirectX::XMVECTOR v3{ DirectX::XMLoadFloat3(&f3) };

			return DirectX::XMLoadFloat3(&f3);
		}

		Vector3 _fastcall ConvertToVector3FromFloat3(const DirectX::XMFLOAT3& f)
		{
			return Vector3{ f.x,f.y,f.z };
		}

		Vector3 _vectorcall ConvertToVector3FromVector(const DirectX::XMVECTOR v)
		{
			DirectX::XMFLOAT3 f3{};
			DirectX::XMStoreFloat3(&f3, v);

			return  Vector3{ f3.x,f3.y ,f3.z };

		}

		Vector3 _fastcall Vector3Normalize(const Vector3& v)
		{
			DirectX::XMVECTOR vT = DirectX::XMVector3Normalize(ConvertToVectorFromVector3(v));
			return ConvertToVector3FromVector(vT);
		}

		// 2つの3Dベクトルを減算する。
		Vector3 _fastcall Vector3Subtract(const Vector3& v1, const Vector3& v2)
		{
			return Vector3
			{
			  v1.x - v2.x,
			  v1.y - v2.y,
			  v1.z - v2.z
			};
		}

		// 2つの3Dベクトルの内積を計算する
		f32 _fastcall Vector3Dot(const Vector3& v1, const Vector3& v2)
		{
			return (v1.x) * (v2.x) + (v1.y) * (v2.y) + (v1.z) * (v2.z);
		}

		// 2つの3Dベクトルの外積を計算する。
		Vector3 _fastcall Vector3Cross(const Vector3& v1, const Vector3& v2)
		{
			return Vector3
			{
				(v1.y) * (v2.z) - (v1.z) * (v2.y),
				(v1.z) * (v2.x) - (v1.x) * (v2.z),
				(v1.x) * (v2.y) - (v1.y) * (v2.x)
			};
		}

		Vector3 _fastcall Vector3Lerp(const Vector3& v1, const Vector3& v2, f32 s)
		{
			return Vector3
			{
				(1.0f - s) * (v1.x) + s * (v2.x),
				(1.0f - s) * (v1.y) + s * (v2.y),
				(1.0f - s) * (v1.z) + s * (v2.z)
			};
		}

		Vector3 _fastcall Vector3SphereLinear(const Vector3& v1, const Vector3& v2, f32& s)
		{
			Vector3
				start{ Vector3Normalize(v1) },
				end{   Vector3Normalize(v2) };

			f32 angle{ acosf(Vector3Dot(start, end)) };

			f32 sinThita{ sinf(angle) };

			f32 startPoint{ sinf(angle * (1.0f - s)) };
			f32 endPoint{   sinf(angle * s) };

			Vector3 outV{ (startPoint * start + endPoint * end) / sinThita };

			return Vector3Normalize(outV);
		}

		Vector3 _fastcall Vector3TransformCoord(const Vector3& v, const Matrix& m)
		{
			f32 t{ m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3] };

			if (NotZero(t))
			{
				t = 1.0f / t;

				return Vector3
				{
					(m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0]) * t,
					(m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1]) * t,
					(m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2]) * t
				};
			}
			else
			{
				return Vector3{ 0.0f,0.0f ,0.0f };
			}
		}

		// 3Dベクトルの長さを返す。
		f32 _fastcall Vector3Length(const Vector3& v)
		{
			return SqrtF32((v.x) * (v.x) + (v.y) * (v.y) + (v.z) * (v.z));
		}

		f32 _fastcall  Vector3LengthSq(const Vector3& v)
		{
			return (v.x) * (v.x) + (v.y) * (v.y) + (v.z) * (v.z);
		}
	    //xyz
		Vector3 _fastcall ConvertToRollPitchYawFromRotationMatrix(const Matrix& m)
		{
			f32 sy{ m.operator()(0, 2) };
			auto unlocked{ std::abs(sy) < 0.99999f };

			return Vector3
			(
				unlocked ? ATan2F32(-m.operator()(1, 2), m.operator()(2, 2)) : std::atan2(m.operator()(2, 1), m.operator()(1, 1)),
				std::asin(sy),
				unlocked ? ATan2F32(-m.operator()(0, 1), m.operator()(0, 0)) : 0
			);
		}

		//zyx
		Vector3 _fastcall ConvertToYawPitchRollFromRotationMatrix(const Matrix& m )
		{
			f32 sy{ -m.operator()(2, 0) };
			auto unlocked{ std::abs(sy) < 0.99999f };

			return Vector3
			(
				unlocked ? ATan2F32(m.operator()(2, 1), m.operator()(2, 2)) : 0,
				ASinF32(sy),
				unlocked ? ATan2F32(m.operator()(1, 0), m.operator()(0, 0)) : ATan2F32(-m.operator()(0, 1), m.operator()(1, 1))
			);
		}

		//xyz
		Vector3 _fastcall ConvertToRollPitchYawFromQuaternion(const Quaternion& q)
		{
			f32 sy{ 2 * q.x * q.z + 2 * q.y * q.w };
			auto unlocked{ std::abs(sy) < 0.99999f };

			return Vector3
			(
				unlocked ? ATan2F32(-(2 * q.y * q.z - 2 * q.x * q.w), 2 * q.w * q.w + 2 * q.z * q.z - 1)
				: ATan2F32(2 * q.y * q.z + 2 * q.x * q.w, 2 * q.w * q.w + 2 * q.y * q.y - 1),
				ASinF32(sy),
				unlocked ? ATan2F32(-(2 * q.x * q.y - 2 * q.z * q.w), 2 * q.w * q.w + 2 * q.x * q.x - 1) : 0
			);
		}

		//zyx
		Vector3 _fastcall ConvertToYawPitchRollFromQuaternion(const Quaternion& q)
		{
			f32 sy{ -(2 * q.x * q.z - 2 * q.y * q.w) };
			auto unlocked{ std::abs(sy) < 0.99999f };

			return Vector3
			(
				unlocked ?
				ATan2F32(2 * q.y * q.z + 2 * q.x * q.w, 2 * q.w * q.w + 2 * q.z * q.z - 1) : 0,
				ASinF32(sy),
				unlocked ?
				ATan2F32(2 * q.x * q.y + 2 * q.z * q.w, 2 * q.w * q.w + 2 * q.x * q.x - 1)
				: ATan2F32(-(2 * q.x * q.y - 2 * q.z * q.w), 2 * q.w * q.w + 2 * q.y * q.y - 1)
			);
		}

		bool _fastcall ToEulerAngleZXY(float& _rXOut, float& _rYOut, float& _rZOut, const Matrix& _rRot)
		{

			//	ジンバルロック判定
			if (_rRot.m[2][1] > 1.0f - FLT_EPSILON || _rRot.m[2][1] < -1.0f + FLT_EPSILON)
			{
				_rXOut = _rRot.m[2][1] < 0 ? DirectX::XM_PI / 2 : -DirectX::XM_PI / 2;
				_rYOut = atan2f(-_rRot.m[0][2], _rRot.m[0][0]);
				_rZOut = 0.0f;
				return	false;
			}

			_rXOut = -asinf(_rRot.m[2][1]);
			_rZOut = asinf(_rRot.m[0][1] / cosf(_rXOut));

			//	ジンバルロック判定
			if (isnan(_rZOut))
			{
				_rXOut = _rRot.m[2][1] < 0 ? DirectX::XM_PI / 2 : -DirectX::XM_PI / 2;
				_rYOut = atan2f(-_rRot.m[0][2], _rRot.m[0][0]);
				_rZOut = 0.0f;

				return	false;
			}

			if (_rRot.m[1][1] < 0.0f)
			{
				_rZOut = DirectX::XM_PI - _rZOut;
			}

			_rYOut = atan2f(_rRot.m[2][0], _rRot.m[2][2]);
			return	true;
		}

		bool _fastcall ToEulerAngleZXY(Vector3& _rOut, const Matrix& _rRot)
		{

			//	ジンバルロック判定
			if (_rRot.m[2][1] > 1.0f - FLT_EPSILON || _rRot.m[2][1] < -1.0f + FLT_EPSILON)
			{
				_rOut.x = _rRot.m[2][1] < 0 ? DirectX::XM_PI / 2 : -DirectX::XM_PI / 2;
				_rOut.y = atan2f(-_rRot.m[0][2], _rRot.m[0][0]);
				_rOut.z = 0.0f;
				return	false;
			}

			_rOut.x = -asinf(_rRot.m[2][1]);
			_rOut.z = asinf(_rRot.m[0][1] / cosf(_rOut.x));

			//	ジンバルロック判定
			if (isnan(_rOut.z))
			{
				_rOut.x = _rRot.m[2][1] < 0 ? DirectX::XM_PI / 2 : -DirectX::XM_PI / 2;
				_rOut.y = atan2f(-_rRot.m[0][2], _rRot.m[0][0]);
				_rOut.z = 0.0f;

				return	false;
			}

			if (_rRot.m[1][1] < 0.0f)
			{
				_rOut.z = DirectX::XM_PI - _rOut.z;
			}

			_rOut.y = atan2f(_rRot.m[2][0], _rRot.m[2][2]);
			return	true;
		}

		Vector3 _fastcall Clamp(const Vector3& v, const Vector3& a,const Vector3& b)
		{
			DirectX::XMFLOAT3 fV{ ConvertToFloat3FromVector3(v) };
			DirectX::XMFLOAT3 fA{ ConvertToFloat3FromVector3(a) };
			DirectX::XMFLOAT3 fB{ ConvertToFloat3FromVector3(b) };

			DirectX::XMVECTOR V{ DirectX::XMLoadFloat3(&fV) };
			DirectX::XMVECTOR A{ DirectX::XMLoadFloat3(&fA) };
			DirectX::XMVECTOR B{ DirectX::XMLoadFloat3(&fB) };

			return ConvertToVector3FromVector(DirectX::XMVectorClamp(V, A, B));
		}

		Vector3 _fastcall ClampVector(Vector3& clamper, Vector3& min, Vector3& max)
		{
			Vector3 result;
			if (min.x > max.x)
			{
				min.x = max.x;
			}
			else if (min.y > max.y)
			{
				min.y = max.y;

			}
			else if (min.z > max.z)
			{
				min.z = max.z;

			}

			if (clamper.x >= 180.f)
			{
				clamper.x -= 360.f;
			}
			else if (clamper.x <= -180.f)
			{
				clamper.x += 360.f;
			}

			if (clamper.y >= 180.f)
			{
				clamper.y -= 360.f;
			}
			else if (clamper.y <= -180.f)
			{
				clamper.y += 360.f;
			}

			if (clamper.z >= 180.f)
			{
				clamper.z -= 360.f;
			}
			else if (clamper.z <= -180.f)
			{
				clamper.z += 360.f;
			}

			result = Clamp(clamper, min, max);


			return result;
		}
#pragma endregion

#pragma region Functions for Vector4
		DirectX::XMFLOAT4 _fastcall ConvertToFloat4FromVector4(const Vector4& v)
		{
			DirectX::XMFLOAT4 f4;

			f4.x = v.x;
			f4.y = v.y;
			f4.z = v.z;
			f4.w = v.w;

			return DirectX::XMFLOAT4{ v.x,v.y,v.z,v.w };
		}
		Vector4 _fastcall ConvertToFloat4FromVector4(const DirectX::XMFLOAT4& v)
		{
			return Vector4{ v.x,v.y,v.z,v.w };
		}

		DirectX::XMVECTOR _fastcall ConvertToVectorFromVector4(const Vector4& v)
		{
			DirectX::XMFLOAT4 f4{ v.x,v.y,v.z,v.w };

			return DirectX::XMLoadFloat4(&f4);
		}

		Vector4 _vectorcall ConvertToVector4FromVector(const DirectX::XMVECTOR v)
		{
			DirectX::XMFLOAT4 f4;
			DirectX::XMStoreFloat4(&f4, v);

			return Vector4{ f4.x,f4.y,f4.z,f4.w };
		}

		// 4Dベクトルの長さを返す
		f32 _fastcall Vector4Length(const Vector4& v)
		{
			return SqrtF32((v.x) * (v.x) + (v.y) * (v.y) + (v.z) * (v.z) + (v.w) * (v.w));
		}

		// 4Dベクトルの正規化したベクトルを返す
		Vector4 _fastcall Vector4Normalize(const Vector4& v)
		{
			f32 norm{ Vector4Length(v) };
			if (!norm)
			{
				return Vector4{ 0.0f,0.0f ,0.0f ,0.0f };
			}
			else
			{
				norm = 1.0f / norm;
				return Vector4
				{
				v.x * norm,
				v.y * norm,
				v.z * norm,
				v.w * norm
				};
			}
		}

		// 2つの4Dベクトルの内積を計算する
		f32 _fastcall Vector4Dot(const Vector4& v1, const Vector4& v2)
		{
			return (v1.x) * (v2.x) + (v1.y) * (v2.y) + (v1.z) * (v2.z) + (v1.w) * (v2.w);
		}

		Vector4 _fastcall Vector4Rotate(const Vector4& v, const Quaternion& q)
		{
			DirectX::XMVECTOR r{ DirectX::XMVector3Rotate(ConvertToVectorFromVector4(v), ConvertToVectorFromQuaternion(q)) };
			return ConvertToVector4FromVector(r);
		}
#pragma endregion

#pragma region Functions for Vector4x4
		DirectX::XMFLOAT4X4 _fastcall ConvertToFloat4x4FromVector4x4(const Matrix& m)
		{
			return DirectX::XMFLOAT4X4
			{
			  m._11,  m._12, m._13,m._14,
			  m._21,  m._22, m._23,m._24,
			  m._31,  m._32, m._33,m._34,
			  m._41,  m._42, m._43,m._44
			};
		}

		DirectX::XMMATRIX _fastcall ConvertToMatrixFromVector4x4(const Matrix& m)
		{
			DirectX::XMFLOAT4X4 mT
			{
			  m._11,  m._12, m._13,m._14,
			  m._21,  m._22, m._23,m._24,
			  m._31,  m._32, m._33,m._34,
			  m._41,  m._42, m._43,m._44
			};

			return { DirectX::XMLoadFloat4x4(&mT) };
		}

		Matrix _fastcall ConvertToVector4x4FromFloat4x4(const DirectX::XMFLOAT4X4& m)
		{
			return Matrix
			{
			  m._11,  m._12, m._13,m._14,
			  m._21,  m._22, m._23,m._24,
			  m._31,  m._32, m._33,m._34,
			  m._41,  m._42, m._43,m._44
			};
		}

		Matrix _vectorcall ConvertToVector4x4FromMatrix(const DirectX::XMMATRIX m)
		{
			DirectX::XMFLOAT4X4 mT;
			DirectX::XMStoreFloat4x4(&mT, m);

			return Matrix
			{
			  mT._11, mT._12, mT._13, mT._14,
			  mT._21, mT._22, mT._23, mT._24,
			  mT._31, mT._32, mT._33, mT._34,
			  mT._41, mT._42, mT._43, mT._44
			};
		}

		Matrix MatrixIdentity()
		{
			return Matrix
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			};
		}


		Vector3 _fastcall MultiplyMatrixVector(const Matrix& a, const Vector3& v)
		{
			Vector4 W;
			const Vector4 V{ v.x,v.y,v.z,0.0f };
			f32 x;

			for (u32 i = 0; i < 4; ++i)
			{
				x = 0.0;

				for (u32 k = 0; k < 4; ++k)
				{
					x += a.m[i][k] * V.v[k];
				}
				W.v[i] = x;
			}

			return Vector3{ W.x,W.y ,W.z };
		}

		Vector3 _fastcall MultiplyRowMatrixVector(const Vector3& v, const Matrix& a)
		{
			Vector4 w;
			f32     x;

			for (u32 j = 0; j < 4; j++)
			{
				x = 0.0;

				for (u32 k = 0; k < 4; k++)
				{
					x += v.v[k] * a.m[k][j];
				}
				w.v[j] = x;
			}

			return Vector3{ w.x,w.y,w.z };

		}

		Matrix _fastcall MatrixInverse(const Matrix& m)
		{
			DirectX::XMMATRIX mT, rM;

			mT = ConvertToMatrixFromVector4x4(m);
			rM = DirectX::XMMatrixInverse(nullptr, mT);

			return ConvertToVector4x4FromMatrix(rM);
		}

		Matrix _fastcall MatrixMultiply(const Matrix& m1, const Matrix& m2)
		{
			DirectX::XMMATRIX m1T{ ConvertToMatrixFromVector4x4(m1) };
			DirectX::XMMATRIX m2T{ ConvertToMatrixFromVector4x4(m2) };
			DirectX::XMMATRIX rM{ DirectX::XMMatrixMultiply(m1T, m2T) };

			return ConvertToVector4x4FromMatrix(rM);
		}

		Matrix _fastcall MatrixTranspose(const Matrix& m)
		{
			DirectX::XMMATRIX mT{ ConvertToMatrixFromVector4x4(m) };
			DirectX::XMMATRIX rM{ DirectX::XMMatrixTranspose(mT) };

			return ConvertToVector4x4FromMatrix(rM);
		}

		Matrix _fastcall MatrixMultiplyTranspose(const Matrix& m1, const Matrix& m2)
		{
			return MatrixTranspose(MatrixMultiply(m1, m2));
		}

		Vector4 _fastcall GetColX(const Matrix& m)
		{
			return Vector4{ m._11,m._12 ,m._13 ,m._14 };
		}

		Vector3 _fastcall GetVector3ColX(const Matrix& m)
		{
			return Vector3{ m._11,m._12 ,m._13 };
		}

		Vector4 _fastcall GetColY(const Matrix& m)
		{
			return Vector4{ m._21,m._22 ,m._23 ,m._24 };
		}

		Vector3 _fastcall GetVector3ColY(const Matrix& m)
		{
			return Vector3{ m._21,m._22 ,m._23 };
		}

		Vector4 _fastcall GetColZ(const Matrix& m)
		{
			return Vector4{ m._31,m._32 ,m._33 ,m._34 };
		}

		Vector3 _fastcall GetVector3ColZ(const Matrix& m)
		{
			return Vector3{ m._31,m._32 ,m._33 };
		}

		Vector4 _fastcall GetColW(const Matrix& m)
		{
			return Vector4{ m._41,m._42 ,m._43 ,m._44 };
		}

		Vector3 _fastcall GetLocation(const Matrix& m)
		{
			using namespace DirectX;
			XMMATRIX newM{ ConvertToMatrixFromVector4x4(m) };
			XMVECTOR S, R, T;
			XMMatrixDecompose(&S, &R, &T, newM);

			return ConvertToVector3FromVector(T);
		}

		Vector3 _fastcall GetScale(const Matrix& m)
		{
			using namespace DirectX;
			XMMATRIX newM{ ConvertToMatrixFromVector4x4(m) };
			XMVECTOR S, R, T;
			XMMatrixDecompose(&S, &R, &T, newM);

			return ConvertToVector3FromVector(S);
		}

		Quaternion _fastcall GetRotation(const Matrix& m)
		{
			using namespace DirectX;
			XMMATRIX newM{ ConvertToMatrixFromVector4x4(m) };
			XMVECTOR S, R, T;
			XMMatrixDecompose(&S, &R, &T, newM);

			return ConvertToQuaternionFromVector(R);
		}

		// 平行移動オフセットを指定して行列を作成する。
		Matrix _fastcall MatrixTranslation(const f32& x, const f32& y, const f32& z)
		{
			return Matrix
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				x,    y,    z,    1.0f
			};
		}

		// x 軸、y 軸、z 軸に沿ってスケーリングする行列を作成する。
		Matrix _fastcall MatrixScaling(const f32& sx, const f32& sy, const f32& sz)
		{
			Matrix rM{ MatrixIdentity() };

			rM.m[0][0] = sx;
			rM.m[1][1] = sy;
			rM.m[2][2] = sz;

			return rM;
		}

		// x 軸を回転軸にして回転する行列を作成する
		Matrix _fastcall MatrixRotationX(const f32& angle)
		{
			Matrix rM{ MatrixIdentity() };

			f32 c{ CosF32(angle) };
			f32 s{ SinF32(angle) };

			rM.m[1][1] = c;
			rM.m[2][2] = c;
			rM.m[1][2] = s;
			rM.m[2][1] = -s;

			return rM;
		}

		// y 軸を回転軸にして回転する行列を作成する
		Matrix _fastcall MatrixRotationY(const f32& angle)
		{
			Matrix rM{ MatrixIdentity() };

			f32 c{ CosF32(angle) };
			f32 s{ SinF32(angle) };

			rM.m[0][0] = c;
			rM.m[2][2] = c;
			rM.m[0][2] = -s;
			rM.m[2][0] = s;

			return rM;
		}

		// z 軸を回転軸にして回転する行列を作成する
		Matrix _fastcall MatrixRotationZ(const f32& angle)
		{
			Matrix rM{ MatrixIdentity() };

			f32 c { CosF32(angle)};
			f32 s { SinF32(angle)};

			rM.m[0][0] = c;
			rM.m[1][1] = c;
			rM.m[0][1] = s;
			rM.m[1][0] = -s;

			return rM;
		}

		//回転行列からfront方向の単位ベクトルを取得する
		Vector3 _fastcall FrontVectorFromRotationMatrix(const Matrix& m)
		{
			return Vector3{ m._31,m._32,m._33 };
		}

		//回転行列からRight方向の単位ベクトルを取得する
		Vector3 _fastcall RightVectorFromRotationMatrix(const Matrix& m)
		{
			return Vector3{ m._11,m._12,m._13 };
		}

		//回転行列からUp方向の単位ベクトルを取得する
		Vector3 _fastcall UpVectorFromRotationMatrix(const Matrix& m)
		{
			return Vector3{ m._21,m._22,m._23 };
		}


		// ロール、ピッチ、およびヨーを指定して行列を作成する。
		Matrix _fastcall MatrixRotationRollPitchYaw(const f32& roll, const f32& pitch, const f32& yaw)
		{
			Matrix m;

			Matrix out3{ MatrixIdentity() };
			m          = MatrixRotationZ(yaw);
			Matrix out2{ MatrixMultiply(out3, m) };
			m          = MatrixRotationX(roll);
			Matrix out1{ MatrixMultiply(out2, m) };
			m          = MatrixRotationY(pitch);

			return MatrixMultiply(out1, m);
		}

		// ヨー、ピッチ、およびロールを指定して行列を作成する
		Matrix _fastcall MatrixRotationYawPitchRoll(const f32& yaw, const f32& pitch, const f32& roll)
		{
			Matrix m;

			Matrix out3{ MatrixIdentity() };
			m     = MatrixRotationZ(roll);
			Matrix out2{ MatrixMultiply(out3, m) };
			m     = MatrixRotationX(pitch);
			Matrix out1{ MatrixMultiply(out2, m) };
			m     = MatrixRotationY(yaw);

			return MatrixMultiply(out1, m);
		}

		// クォータニオンから回転行列を作成する。
		Matrix _fastcall MatrixRotationQuaternion(const Quaternion& q)
		{
			Matrix rM{ MatrixIdentity() };

			/*rM.m[0][0] = 1.0f - 2.0f * (q->y * q->y + q->z * q->z);
			  rM.m[0][1] = 2.0f * (q->x * q->y + q->z * q->w);
			  rM.m[0][2] = 2.0f * (q->x * q->z - q->y * q->w);
			  rM.m[1][0] = 2.0f * (q->x * q->y - q->z * q->w);
			  rM.m[1][1] = 1.0f - 2.0f * (q->x * q->x + q->z * q->z);
			  rM.m[1][2] = 2.0f * (q->y * q->z + q->x * q->w);
			  rM.m[2][0] = 2.0f * (q->x * q->z + q->y * q->w);
			  rM.m[2][1] = 2.0f * (q->y * q->z - q->x * q->w);
			  rM.m[2][2] = 1.0f - 2.0f * (q->x * q->x + q->y * q->y);*/

			rM.m[0][0] = 1.0f - (2.0f * q.y * q.y) - (2.0f * q.z * q.z);
			rM.m[0][1] = (2.0f * q.x * q.y) + (2.0f * q.w * q.z);
			rM.m[0][2] = (2.0f * q.x * q.z) - (2.0f * q.w * q.y);
			rM.m[0][3] = 0.0f;

			rM.m[1][0] = (2.0f * q.x * q.y) - (2.0f * q.w * q.z);
			rM.m[1][1] = 1.0f - (2.0f * q.x * q.x) - (2.0f * q.z * q.z);
			rM.m[1][2] = (2.0f * q.y * q.z) + (2.0f * q.w * q.x);
			rM.m[1][3] = 0.0f;

			rM.m[2][0] = (2.0f * q.x * q.z) + (2.0f * q.w * q.y);
			rM.m[2][1] = (2.0f * q.y * q.z) - (2.0f * q.w * q.x);
			rM.m[2][2] = 1.0f - (2.0f * q.x * q.x) - (2.0f * q.y * q.y);
			rM.m[2][3] = 0.0f;

			rM.m[3][0] = 0.0f;
			rM.m[3][1] = 0.0f;
			rM.m[3][2] = 0.0f;
			rM.m[3][3] = 1.0f;

			return rM;
		}

		// 視野に基づいて、右手座標系パースペクティブ射影行列を作成する。
		Matrix _fastcall MatrixPerspectiveFov(const f32& fovY, const f32& aspect, const f32& zn, const f32& zf)
		{
			Matrix rM{ MatrixIdentity() };

			rM.m[0][0] = 1.0f / (aspect * TanF32(fovY / 2.0f));
			rM.m[1][1] = 1.0f / TanF32(fovY / 2.0f);
			rM.m[2][2] = zf / (zn - zf);
			rM.m[2][3] = -1.0f;
			rM.m[3][2] = (zf * zn) / (zn - zf);
			rM.m[3][3] = 0.0f;

			return rM;

			/*DirectX::XMMATRIX m = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, zn, zf);
			return ConvertToVector4x4FromMatrix(m);*/
		}

		// カスタマイズした右手座標系正射影行列を作成する。
		Matrix _fastcall MatrixOrthoOffCenter(const f32& l, const f32& r, const f32& b, const f32& t, const f32& zn, const f32& zf)
		{
			Matrix rM{ MatrixIdentity() };

			rM.m[0][0] = 2.0f / (r - l);
			rM.m[1][1] = 2.0f / (t - b);
			rM.m[2][2] = 1.0f / (zn - zf);
			rM.m[3][0] = -1.0f - 2.0f * l / (r - l);
			rM.m[3][1] = 1.0f + 2.0f * t / (b - t);
			rM.m[3][2] = zn / (zn - zf);

			return rM;
		}

		// 右手座標系ビュー行列を作成する。
		Matrix _fastcall MatrixLookAt(const Vector3& eye, const Vector3& at, const Vector3& up)
		{
			Matrix rM;
			Vector3 x, y, z, t;

			t = Vector3Subtract(at, eye);
			z = Vector3Normalize(t);
			t = Vector3Cross(up, z);
			x = Vector3Normalize(t);
			y = Vector3Cross(z, x);

			rM.m[0][0] = -x.x;
			rM.m[1][0] = -x.y;
			rM.m[2][0] = -x.z;
			rM.m[3][0] = Vector3Dot(x, eye);
			rM.m[0][1] = y.x;
			rM.m[1][1] = y.y;
			rM.m[2][1] = y.z;
			rM.m[3][1] = -Vector3Dot(y, eye);
			rM.m[0][2] = -z.x;
			rM.m[1][2] = -z.y;
			rM.m[2][2] = -z.z;
			rM.m[3][2] = Vector3Dot(z, eye);
			rM.m[0][3] = 0.0f;
			rM.m[1][3] = 0.0f;
			rM.m[2][3] = 0.0f;
			rM.m[3][3] = 1.0f;

			return rM;

			/*DirectX::XMVECTOR E = DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(eye.x, eye.y, eye.z, 1.0f));
			DirectX::XMVECTOR F = DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(at.x, at.y, at.z, 1.0f));
			DirectX::XMVECTOR U = DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(up.x, up.y, up.z, 1.0f));

			DirectX::XMMATRIX m = DirectX::XMMatrixLookAtLH(E, F, U);
			return ConvertToVector4x4FromMatrix(m);*/
		}

		// 右手座標系正射影行列を作成する。
		Matrix _fastcall MatrixOrtho(const f32& w, const f32& h, const f32& zn, const f32& zf)
		{
			Matrix rM{ MatrixIdentity() };

			rM.m[0][0] = 2.0f / w;
			rM.m[1][1] = 2.0f / h;
			rM.m[2][2] = 1.0f / (zn - zf);
			rM.m[3][2] = zn / (zn - zf);

			return rM;

			/*DirectX::XMMATRIX m = DirectX::XMMatrixOrthographicLH(w, h, zn, zf);
			return ConvertToVector4x4FromMatrix(m);*/
		}
#pragma endregion

#pragma region Functions for Quaternion
		DirectX::XMVECTOR _fastcall ConvertToVectorFromQuaternion(const Quaternion& q)
		{
			DirectX::XMVECTOR v{ DirectX::XMVectorSet(q.x, q.y, q.z, q.w) };;

			return DirectX::XMVECTOR{ DirectX::XMVectorSet(q.x, q.y, q.z, q.w) };
		}

		Quaternion _vectorcall ConvertToQuaternionFromVector(const  DirectX::XMVECTOR v)
		{
			DirectX::XMFLOAT4 f;
			DirectX::XMStoreFloat4(&f, v);

			return Quaternion{ f.x, f.y, f.z, f.w };
		}

		Quaternion _fastcall ConvertToQuaternionFromRotationMatrix(const Matrix& m)
		{
			DirectX::XMMATRIX mT{ ConvertToMatrixFromVector4x4(m) };
			DirectX::XMVECTOR v{ DirectX::XMQuaternionRotationMatrix(mT) };

			return ConvertToQuaternionFromVector(v);
		}

		Matrix _fastcall ConvertToRotationMatrixFromQuaternion(const Quaternion& q)
		{
			DirectX::XMVECTOR v{ ConvertToVectorFromQuaternion(q) };
			DirectX::XMMATRIX r{ DirectX::XMMatrixRotationQuaternion(v) };

			return ConvertToVector4x4FromMatrix(r);
		}

		// ヨー、ピッチ、およびロールを指定してクオータニオンを作成する。
		Quaternion _fastcall ConvertToQuaternionFromYawPitchRoll(const f32& yaw, const  f32& pitch, const f32& roll)
		{
			f32 cx{ CosF32(0.5f * roll) };
			f32 sx{ SinF32(0.5f * roll) };
			f32 cy { CosF32(0.5f * pitch) };
			f32 sy { SinF32(0.5f  * pitch) };
			f32 cz { CosF32(0.5f * yaw) };
			f32 sz { SinF32(0.5f  * yaw) };

			return 	Quaternion
			{
				sx* cy* cz - cx * sy * sz,
				sx* cy* sz + cx * sy * cz,
				-sx * sy * cz + cx * cy * sz,
				sx* sy* sz + cx * cy * cz
			};
		}

		// ロール、ピッチ、およびヨーを指定してクオータニオンを作成する。
		Quaternion _fastcall ConvertToQuaternionFromRollPitchYaw(const f32& roll, const f32& pitch, const f32& yaw)
		{
			f32 cx{ CosF32(0.5f * roll) };
			f32 sx{ SinF32(0.5f * roll) };
			f32 cy{ CosF32(0.5f * pitch) };
			f32 sy{ SinF32(0.5f * pitch) };
			f32 cz{ CosF32(0.5f * yaw) };
			f32 sz{ SinF32(0.5f * yaw) };


			return 	Quaternion
			{
				cx * sy * sz + sx * cy * cz,
				-sx * cy * sz + cx * sy * cz,
				cx * cy * sz + sx * sy * cz,
				-sx * sy * sz + cx * cy * cz
			};
		}
		Quaternion QuaternionIdentity()
		{
			return ConvertToQuaternionFromVector(DirectX::XMQuaternionIdentity());
		}

		f32 _fastcall QuaternionDot(const Quaternion& q1, const Quaternion& q2)
		{
			return (q1.x) * (q2.x) + (q1.y) * (q2.y) + (q1.z) * (q2.z) + (q1.w) * (q2.w);
		}

		Quaternion _fastcall QuaternionMultiply(const Quaternion& q1, const Quaternion& q2)
		{
			DirectX::XMFLOAT4 v1{ DirectX::XMFLOAT4(q1.x, q1.y, q1.z, q1.w) };
			DirectX::XMFLOAT4 v2  {DirectX::XMFLOAT4(q2.x, q2.y, q2.z, q2.w) };
			DirectX::XMVECTOR vq1 {DirectX::XMLoadFloat4(&v1) };
			DirectX::XMVECTOR vq2 {DirectX::XMLoadFloat4(&v2) };
			DirectX::XMVECTOR rvq {DirectX::XMQuaternionMultiply(vq1, vq2) };

			DirectX::XMFLOAT4 fq;
			DirectX::XMStoreFloat4(&fq, rvq);

			return 	Quaternion{ fq.x,fq.y,fq.z,fq.w };
		}

		Quaternion _fastcall QuaternionRotationAxis(const Vector3& axis, const float& angle)
		{
			//if(axis)
			DirectX::XMFLOAT4 v  { DirectX::XMFLOAT4(axis.x, axis.y, axis.z, 1.0f)                     };
			DirectX::XMVECTOR vq { DirectX::XMQuaternionRotationAxis(DirectX::XMLoadFloat4(&v), angle) };

			DirectX::XMFLOAT4 fq;
			DirectX::XMStoreFloat4(&fq, vq);

			return 	Quaternion{ fq.x,fq.y,fq.z,fq.w };
		}

		// Y軸を回転軸としてクォータニオンを回転させる
		Quaternion _fastcall QuaternionRotationY(f32 angle)
		{
			angle *= 0.5f;
			return Quaternion{ 0.0f, SinF32(angle) ,0,CosF32(angle) };
		}

		Quaternion _fastcall QuaternionSlerp(const Quaternion& q1, const Quaternion& q2, f32 t)
		{
			DirectX::XMFLOAT4 v1{ DirectX::XMFLOAT4(q1.x, q1.y, q1.z, q1.w) };
			DirectX::XMFLOAT4 v2 {DirectX::XMFLOAT4(q2.x, q2.y, q2.z, q2.w) };
			DirectX::XMVECTOR q1T{DirectX::XMLoadFloat4(&v1) };
			DirectX::XMVECTOR q2T{DirectX::XMLoadFloat4(&v2) };

			DirectX::XMVECTOR qV{ DirectX::XMQuaternionSlerp(q1T, q2T, t) };
			DirectX::XMFLOAT4 qF;
			DirectX::XMStoreFloat4(&qF, qV);

			/*f32 epsilon = 1.0f;
			f32 dot = QuaternionDot(q1, q2);
			if (dot < 0.0f) epsilon = -1.0f;
			qT.x = (1.0f - t) * q1.x + epsilon * t * q2.x;
			qT.y = (1.0f - t) * q1.y + epsilon * t * q2.y;
			qT.z = (1.0f - t) * q1.z + epsilon * t * q2.z;
			qT.w = (1.0f - t) * q1.w + epsilon * t * q2.w;*/

			/*f32 epsilon = 1.0f;
			f32 dot = QuaternionDot(q1, q2);
			if (dot < 0.0f) epsilon = -1.0f;

			qT.x = (1.0f - t) * q1.x + epsilon * t * q2.x;
			qT.y = (1.0f - t) * q1.y + epsilon * t * q2.y;
			qT.z = (1.0f - t) * q1.z + epsilon * t * q2.z;
			qT.w = (1.0f - t) * q1.w + epsilon * t * q2.w;*/

			return 	Quaternion{ qF.x,qF.y,qF.z,qF.w };
		}
#pragma endregion

#pragma region Functions for Vector
		f32 _vectorcall VectorLength(const vector v)
		{
			Vector lengthSq{ _mm_mul_ps(v,v) };
			Vector vDistance{ _mm_sqrt_ps(_mm_dp_ps(lengthSq.v, lengthSq.v, 0x7f)) };
			return vDistance.StoreVector().x;
		}

		vector _vectorcall VectorNormalize(const vector v)
		{
			vector vLengthSq{ _mm_dp_ps(v, v, 0x7f) };
			vector vResult   {_mm_sqrt_ps(vLengthSq) };
			vector vZeroMask {_mm_setzero_ps() };
			vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
			vLengthSq = _mm_cmpneq_ps(vLengthSq, Vector::simInfinity);

			vResult   = _mm_div_ps(v, vResult);
			vResult   = _mm_and_ps(vResult, vZeroMask);

			vector vTemp1 { _mm_andnot_ps(vLengthSq, Vector::simQNaN) };
			vector vTemp2 { _mm_and_ps(vResult, vLengthSq) };

			return _mm_or_ps(vTemp1, vTemp2);
		}
#pragma endregion
	} // namespace Math
} // namespace Bread