#pragma once
#include <vector>
#include <string>

#include "Math/BreadMath.h"

namespace Bread
{
	namespace FrameWork
	{
		//全てのジョイントの基盤となる構造体
		struct IJoint
		{
			std::string          name;        //ジョイントの名前
			IJoint*              parent;      //親ジョイントのポインター
			std::vector<IJoint*> child;       //子ジョイントのポインター配列

			Math::Vector3      scale;         //ジョイントのスケール値
			Math::Quaternion   rotate;        //ジョイントの回転値
			Math::Vector3      translate;     //ジョイントの平行移動値

			Math::Matrix       localTransform;//ジョイントのローカル行列
			Math::Matrix       worldTransform;//ジョイントのワールド行列

			Math::Vector3      minRot;        //ジョイントの最小回転量(Euler)
			Math::Vector3      maxRot;        //ジョイントの最大回転量(Euler)
		};

		//全てのポリゴンの基礎となる構造体
		struct IFace
		{
			struct VertexIndex
			{
				std::vector<Math::Vector3> vertex; //ポリゴンの頂点座標
			};
			std::vector<VertexIndex>       face;   //ポリゴン
		};

		struct IMeshNode
		{
			std::vector<Math::Matrix> boneTransform;
			u32 boneTransformCount = 0;
		};

		//全てのシェーダーの名前を格納する構造体
		struct IShaderName
		{
		protected:
			std::string shaderName;   //描画するシェーダの名前
		};

		//RenderManagerで使用するシェーダーを判別するため
		struct MyShaderName : public IShaderName
		{
		public:
			void SetShaderNema(const std::string& name)
			{
				shaderName = name;
			}
			const std::string& GetShaderName()
			{
				return shaderName;
			}
		};
	}
}