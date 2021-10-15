#include "../../../ExternalLibrary/ImGui/Include/imgui.h"

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
				TerrainModel      data;
				terrains[model] = data;
			}

			for (auto& it : terrains)
			{
				Matrix parentWorldTrnasform{ it.first->GetComponent<Transform>()->GetWorldTransform() };
				for (auto& faceCurrent : *it.first->GetComponent<ModelObject>()->GetFaces())
				{
					for (auto& face : faceCurrent.face)
					{
						Math::Vector3 comprehensive{ Math::Vector3::Zero };
						u32  vertexNum{ 0 };
						for (auto& vertex : face.vertex)
						{
							Matrix scale, rotate, translate;
							scale     = MatrixScaling(Vector3::OneAll.x, Vector3::OneAll.y, Vector3::OneAll.z);
							rotate    = MatrixRotationQuaternion(Quaternion::Zero);
							translate = MatrixTranslation(vertex.x, vertex.y, vertex.z);

							Matrix localTransform{ scale * rotate * translate };
							vertex = GetLocation(localTransform * parentWorldTrnasform);

							comprehensive += vertex;
							vertexNum++;
						}
						comprehensive /= vertexNum;

						Vector3S32  spatialIndex{ Instance<SpatialDivisionManager>::instance.SpatialCurrent(comprehensive) };
						std::string spatialID   { toStringFromSpatialIndex(spatialIndex) };
						it.second.registFace[spatialID].emplace_back(face);
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
			int oldSize{ 0 };

			auto NeighborhoodSpatialFaces([&]
			    (
				const Math::Vector3S32& index
				)
				{
					std::string spatialID{ toStringFromSpatialIndex(index) };
					for (auto& it : terrains)
					{
						Matrix parentWorldTrnasform{ it.first->GetComponent<Transform>()->GetWorldTransform() };

						auto iterator = it.second.registFace.find(spatialID);
						if (iterator == it.second.registFace.end())
						{
							continue;
						}

						for (auto& vertexIndex : it.second.registFace[spatialID])
						{
							ModelObject::Face::VertexIndex vertex;
							vertex.vertex.clear();

							for (auto& vertexPos : vertexIndex.vertex)
							{
								oldSize = it.second.registFace.size();

								Matrix scale, rotate, translate;
								scale     = MatrixScaling(Vector3::OneAll.x, Vector3::OneAll.y, Vector3::OneAll.z);
								rotate    = MatrixRotationQuaternion(Quaternion::Zero);
								translate = MatrixTranslation(vertexPos.x, vertexPos.y, vertexPos.z);

								Matrix localTransform{ scale * rotate * translate };
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

		void TerrainManager::GUI()
		{
			using namespace ImGui;

			ImGui::Begin("TerrainManager");
			{
				for (auto& act : terrains)
				{
					ImGui::Text("spatial Num %d", act.second.registFace.size());
					for (auto& spatial : act.second.registFace)
					{
						FaceInfomationNode(spatial);
					}ImGui::Separator();
				}
			}ImGui::End();
		}

		void TerrainManager::FaceInfomationNode(std::pair<const std::string, std::vector<ModelObject::Face::VertexIndex>>& spatial)
		{
			using namespace ImGui;

			//TreeNodeExの設定を行う変数
			ImGuiTreeNodeFlags treeNodeFlags{ ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiSelectableFlags_AllowItemOverlap };

			if (ImGui::TreeNodeEx(("Key :" + spatial.first).c_str(), treeNodeFlags))
			{
				ImGui::Text("Existence Polygon %d", spatial.second.size());
				ImGui::Separator();

				u32 faceIndex{ 0 };
				for (auto& face : spatial.second)
				{
					u32 vertexIndex{ 0 };
					for (auto& vertex : face.vertex)
					{
						ImGui::DragFloat3((spatial.first + "_" + std::to_string(faceIndex) + "_" +  std::to_string(vertexIndex)).c_str(), face.vertex[vertexIndex].v);
						vertexIndex++;
					}
					ImGui::Separator();

					faceIndex++;
				}

				ImGui::TreePop();
			}ImGui::Separator();
		}

		std::string TerrainManager::toStringFromSpatialIndex(const Vector3S32& index)
		{
			return { std::to_string(index.x) + ("-") + std::to_string(index.y) + ("-") + std::to_string(index.z) };
		}

	}//namespace FrameWork
}//namespace Bread