#pragma once
#include <map>
#include <vector>
#include <memory>
#include <string>

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
			struct TerrainModel
			{
				std::map<std::string, std::vector<ModelObject::Face::VertexIndex>> registFace;
			};
			std::map<std::shared_ptr<Actor>, TerrainModel> terrains;
		public:
			TerrainManager() = default;
			~TerrainManager() {};

		public://public Initialize Stage Function

			//引数のモデルコンポーネントからどこの空間にポリゴンがあるのか調べて登録する
			void RegisterPolygon(std::shared_ptr<Actor> model);

		public://public Update Stage Function

			//空間座標のインデックス番号を渡して
			//3*3*3の空間のポリゴンのワールド空間での頂点情報を渡す
			[[nodiscard]]
			const std::vector<ModelObject::Face::VertexIndex> GetSpatialFaces(const Math::Vector3S32& index);

		public://public GUI Function

			//TerrainManagerの内部データを表示するための関数
			void GUI();

		private://private TerrainManager Function

			//空間番号をstringで保存できる形で残す
			std::string toStringFromSpatialIndex(const Math::Vector3S32& index);

		private://private GUI Function

			//FaceをImGUi::Node関数で表示するための関数
			void FaceInfomationNode(std::pair<const std::string, std::vector<ModelObject::Face::VertexIndex>>& spatial);
		};
	}//namespace FrameWork
}//namespace Bread