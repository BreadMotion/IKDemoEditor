#include "FND/Instance.h"
#include "FrameWork/Object/TerrainManager.h"
#include "FrameWork/SpatialDivisionManager/SpatialDivisionManager.h"

using Bread::FND::Instance;
using Bread::Math::Vector3S32;

namespace Bread
{
	namespace FrameWork
	{
		//引数のモデルからどこの空間にポリゴンがあるのか調べて登録する
		void TerrainManager::RegisterPolygon(std::shared_ptr<ModelObject> model)
		{
			auto faces = model->GetFaces();

			for (auto& it : *faces)
			{
				TerrainModel data;
				terrains.insert(std::make_pair(&it, data));
			}

			for (auto& it : terrains)
			{
				for (auto& face : it.first->face)
				{
					Math::Vector3 comprehensive{ Math::Vector3::Zero };
					u32  vertexNum{ 0 };
					for (auto& vertex : face.vertex)
					{
						comprehensive += vertex;
						vertexNum++;
					}
					comprehensive /= vertexNum;

					it.second.registFace
						[Instance<SpatialDivisionManager>::instance.
						SpatialCurrent(comprehensive)]->emplace_back(face);
				}
			}
		}

		//空間座標のインデックス番号を渡して
		//3*3*3の空間のポリゴンの頂点情報を渡す
		[[nodiscard]]
		const std::vector<ModelObject::Face::VertexIndex>
			TerrainManager::GetSpatialFaces(const Math::Vector3S32& index)
		{
			std::vector<ModelObject::Face::VertexIndex> spatialFace;

			auto NeighborhoodSpatialFaces([&]
			    (
				const Math::Vector3S32& index
				)
				{
					for (auto& it : terrains)
					{
						for (auto& vertexIndex : *it.second.registFace[index])
						{
							spatialFace.emplace_back(vertexIndex);
						}
					}
				});

			//indexを中心の空間座標として
			//3*3*3の空間のポリゴンの頂点情報を渡す
			for (s32 x = -1; x <= 1; x++)
			{
				for (s32 y = -1; y <= 1; y++)
				{
					for (s32 z = -1; z <= 1; x++)
					{
						NeighborhoodSpatialFaces(Vector3S32{ index.x + x,index.y + y,index.z + z });
					}
				}
			}

			return spatialFace;
		}
	}//namespace FrameWork
}//namespace Bread