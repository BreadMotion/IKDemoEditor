#include "FND/Instance.h"
#include "FrameWork/Object/TerrainManager.h"
#include "FrameWork/SpatialDivisionManager/SpatialDivisionManager.h"
#include "FrameWork/Component/Transform.h"

using Bread::FND::Instance;
using namespace Bread::Math;

namespace Bread
{
	namespace FrameWork
	{
		//引数のモデルからどこの空間にポリゴンがあるのか調べて登録する
		void TerrainManager::RegisterPolygon(std::shared_ptr<Actor> model)
		{
			auto faces = model->GetComponent<ModelObject>()->GetFaces();

			for (auto& it : *faces)
			{
				TerrainModel data;
				terrains[model] = data;
			}

			for (auto& it : terrains)
			{
				for (auto& faceCurrent : *it.first->GetComponent<ModelObject>()->GetFaces())
				{
					for (auto& face : faceCurrent.face)
					{
						Math::Vector3 comprehensive{ Math::Vector3::Zero };
						u32  vertexNum{ 0 };
						for (auto& vertex : face.vertex)
						{
							comprehensive += vertex;
							vertexNum++;
						}
						comprehensive /= vertexNum;

						it.second
							.registFace[Instance<SpatialDivisionManager>::instance
							.SpatialCurrent(comprehensive)]
							.emplace_back(face);
					}
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
						Matrix parentWorldTrnasform{ it.first->GetComponent<Transform>()->GetWorldTransform() };
						for (auto& vertexIndex : it.second.registFace[index])
						{
							ModelObject::Face::VertexIndex vertex;
							vertex.vertex.clear();
							for (auto& vertexPos : vertexIndex.vertex)
							{
								Matrix scale, rotate, translate;
								scale     = MatrixScaling(Vector3::OneAll.x, Vector3::OneAll.y, Vector3::OneAll.z);
								rotate    = MatrixRotationQuaternion(Quaternion::Zero);
								translate = MatrixTranslation(vertexPos.x, vertexPos.y, vertexPos.z);

								Matrix localTransform{ vertexPos };
								vertexPos = GetLocation(localTransform * parentWorldTrnasform);
								vertex.vertex.emplace_back(vertexPos);
							}

							spatialFace.emplace_back(vertex);
						}
					}
				});

			//indexを中心の空間座標として
			//3*3*3の空間のポリゴンの頂点情報を渡す
			for (s32 x = -1; x <= 1; x++)
			{
				for (s32 y = -1; y <= 1; y++)
				{
					for (s32 z = -1; z <= 1; z++)
					{
						NeighborhoodSpatialFaces(Vector3S32{ index.x + x,index.y + y,index.z + z });
					}
				}
			}

			return spatialFace;
		}
	}//namespace FrameWork
}//namespace Bread