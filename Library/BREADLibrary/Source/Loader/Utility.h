#pragma once

#include <fbxsdk.h>
#include "Math/Color.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"


namespace Bread
{
	namespace Loader
	{
		//****************************************************************************
		// FBXユーティリティクラス
		//****************************************************************************
		class Utility
		{
		public:
			// FbxDouble2 → Vector2
			static Math::Vector2 FbxDoble2ToVector2(const FbxDouble2& fbxValue);

			// FbxDouble3 → Vector3
			static Math::Vector3 FbxDoble3ToVector3(const FbxDouble3& fbxValue);

			// FbxDouble4 → Vector3
			static Math::Vector3 FbxDoble4ToVector3(const FbxDouble4& fbxValue);

			// FbxDouble4 → Vector4
			static Math::Vector4 FbxDoble4ToVector4(const FbxDouble4& fbxValue);

			// FbxDouble4 → Quaternion
			static Math::Quaternion FbxDoble4ToQuaternion(const FbxDouble4& fbxValue);

			// FbxQuaternion → Quaternion
			static Math::Quaternion FbxQuaternionToQuaternion(const FbxQuaternion& fbxValue);

			// FbxColor → Color
			static Math::Color FbxColorToColor(const FbxColor& fbxValue);

			// FbxAMatrix → Matrix
			static Math::Matrix FbxAMatrixToMatrix(const FbxAMatrix& fbxValue);
		};

	} // namespace Loader
} // namespace Bread