#pragma once
#include <map>
#include <vector>
#include <memory>

#include "Math/BreadMath.h"
#include "FrameWork/Object/Object.h"
#include "FND/Base.h"

namespace Bread
{
	namespace FrameWork
	{
		//地面判定のオブジェクトのメッシュがどこの空間にあるのかを管理するクラス
		class TerrainManager : public FND::Base
		{
			using SpatialPosition = Math::Vector3S32;
			struct TerrainModel
			{
				std::map<SpatialPosition, std::vector<ModelObject::Face::VertexIndex>*> registFace;
			};
			std::map<ModelObject::Face*, TerrainModel> terrains;
		public:
			TerrainManager() = default;
			~TerrainManager() {};

		private:
			//引数のモデルからどこの空間にポリゴンがあるのか調べて登録する
			void RegisterPolygon(std::shared_ptr<ModelObject> model);

		public:
			//空間座標のインデックス番号を渡して
			//3*3*3の空間のポリゴンの頂点情報を渡す
			[[nodiscard]]
			const std::vector<ModelObject::Face::VertexIndex> GetSpatialFaces(const Math::Vector3S32& index);

		};
	}//namespace FrameWork
}//namespace Bread