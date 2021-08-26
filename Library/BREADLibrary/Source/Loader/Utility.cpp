#include "pch.h"
#include "Utility.h"


namespace Bread
{
	namespace Loader
	{
		//****************************************************************************
		// FBXユーティリティクラス
		//****************************************************************************
		// FbxDouble2 → Vector2
		Math::Vector2 Utility::FbxDoble2ToVector2(const FbxDouble2& fbxValue)
		{
			return Math::Vector2(
				static_cast<f32>(fbxValue[0]),
				static_cast<f32>(fbxValue[1])
			);
		}

		// FbxDouble3 → Vector3
		Math::Vector3 Utility::FbxDoble3ToVector3(const FbxDouble3& fbxValue)
		{
			return Math::Vector3(
				static_cast<f32>(fbxValue[0]),
				static_cast<f32>(fbxValue[1]),
				static_cast<f32>(fbxValue[2])
			);
		}

		// FbxDouble4 → Vector3
		Math::Vector3 Utility::FbxDoble4ToVector3(const FbxDouble4& fbxValue)
		{
			return Math::Vector3(
				static_cast<f32>(fbxValue[0]),
				static_cast<f32>(fbxValue[1]),
				static_cast<f32>(fbxValue[2])
			);
		}

		// FbxDouble4 → Vector4
		Math::Vector4 Utility::FbxDoble4ToVector4(const FbxDouble4& fbxValue)
		{
			return Math::Vector4(
				static_cast<f32>(fbxValue[0]),
				static_cast<f32>(fbxValue[1]),
				static_cast<f32>(fbxValue[2]),
				static_cast<f32>(fbxValue[3])
			);
		}

		// FbxDouble4 → Quaternion
		Math::Quaternion Utility::FbxDoble4ToQuaternion(const FbxDouble4& fbxValue)
		{
			return Math::Quaternion(
				static_cast<f32>(fbxValue[0]),
				static_cast<f32>(fbxValue[1]),
				static_cast<f32>(fbxValue[2]),
				static_cast<f32>(fbxValue[3])
			);
		}

		// FbxQuaternion → Quaternion
		Math::Quaternion Utility::FbxQuaternionToQuaternion(const FbxQuaternion& fbxValue)
		{
			return Math::Quaternion(
				static_cast<f32>(fbxValue[0]),
				static_cast<f32>(fbxValue[1]),
				static_cast<f32>(fbxValue[2]),
				static_cast<f32>(fbxValue[3])
			);
		}

		// FbxColor → Color
		Math::Color Utility::FbxColorToColor(const FbxColor& fbxValue)
		{
			return Math::Color(
				static_cast<f32>(fbxValue[0]),
				static_cast<f32>(fbxValue[1]),
				static_cast<f32>(fbxValue[2]),
				static_cast<f32>(fbxValue[3])
			);
		}

		// FbxAMatrix → Matrix
		Math::Matrix Utility::FbxAMatrixToMatrix(const FbxAMatrix& fbxValue)
		{
			return Math::Matrix(
				static_cast<f32>(fbxValue[0][0]),
				static_cast<f32>(fbxValue[0][1]),
				static_cast<f32>(fbxValue[0][2]),
				static_cast<f32>(fbxValue[0][3]),
				static_cast<f32>(fbxValue[1][0]),
				static_cast<f32>(fbxValue[1][1]),
				static_cast<f32>(fbxValue[1][2]),
				static_cast<f32>(fbxValue[1][3]),
				static_cast<f32>(fbxValue[2][0]),
				static_cast<f32>(fbxValue[2][1]),
				static_cast<f32>(fbxValue[2][2]),
				static_cast<f32>(fbxValue[2][3]),
				static_cast<f32>(fbxValue[3][0]),
				static_cast<f32>(fbxValue[3][1]),
				static_cast<f32>(fbxValue[3][2]),
				static_cast<f32>(fbxValue[3][3])
			);
		}


	} // namespace Loader
} // namespace Bread