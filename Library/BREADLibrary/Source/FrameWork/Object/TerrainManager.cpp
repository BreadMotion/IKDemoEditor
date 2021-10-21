#include <algorithm>
#include "../../../ExternalLibrary/ImGui/Include/imgui.h"

#include "FND/Instance.h"
#include "FrameWork/Object/TerrainManager.h"
#include "FrameWork/SpatialDivisionManager/SpatialDivisionManager.h"
#include "FrameWork/Component/Transform.h"

using Bread::FND::Instance; //SpatialDivisionManager
using namespace Bread::Math;

namespace Bread
{
	namespace FrameWork
	{
		//引数のモデルからどこの空間にポリゴンがあるのか調べて登録する
		void TerrainManager::RegisterPolygon(std::shared_ptr<Actor> model)
		{
			//ポリゴン情報の取得
			auto faces{ model->GetComponent<ModelObject>()->GetFaces() };

			//アクターの登録、    初期化
			//重複している場合は、secondだけ初期化
			TerrainModel spatialData;
			terrains[model] = spatialData;

			//登録したモデルを持つアクターの数ループする
			for (auto& actor : terrains)
			{
				//アクターのワールドTransform
				Matrix parentWorldTrnasform{ actor.first->GetComponent<Transform>()->GetWorldTransform() };

				//アクターの持つメッシュの数ループする
				for (auto& faceCurrent : *actor.first->GetComponent<ModelObject>()->GetFaces())
				{
					//メッシュのポリゴンの数ループする
					for (auto& face : faceCurrent.face)
					{
						//最終的にポリゴンの頂点の平均値を持つ
						Math::Vector3 comprehensive{ Math::Vector3::Zero };

						//ワールドに変換後のVertex情報を登録する
						ModelObject::Face::VertexIndex worldVertexArray;

						//ポリゴンの頂点の数ループする
						for (auto& vertex : face.vertex)
						{
							//頂点情報はローカルなのでステージのTransformからの影響を考慮したデータを保存しなければいけない
#if 0
							const Matrix  mVertex     { Math::MatrixTranslation(vertex.x, vertex.y, vertex.z)                      };
							const Matrix  mScale      { Math::MatrixScaling(Vector3::OneAll.x,Vector3::OneAll.y,Vector3::OneAll.z) };
							const Matrix  mRotate     { Math::MatrixRotationQuaternion(Quaternion::Zero)                           };

							const Vector3 worldVertex{ Math::GetLocation(Math::MatrixMultiply(mScale * mRotate * mVertex , parentWorldTrnasform)) };
#elif 0
							const Vector3 worldVertex{ Math::MultiplyMatrixVector(parentWorldTrnasform,vertex)                                    };
#else
							const Vector3 worldVertex{ Math::Vector3TransformCoord(vertex,parentWorldTrnasform)                                   };
#endif
							//ポリゴンのワールドの頂点座標を足していく
							worldVertexArray.vertex.emplace_back(worldVertex);
							comprehensive += worldVertex;
						}
						//平均値 == ポリゴンの重心
						//平均値 /= 頂点数
						comprehensive /= face.vertex.size();

						//ポリゴンの重心がどの空間座標にいるのかを計算
						//後に登録する
						Vector3S32  spatialIndex{ Instance<SpatialDivisionManager>::instance.SpatialCurrent(comprehensive) };
						std::string spatialID   { toStringFromSpatialIndex(spatialIndex)                                   };
						actor.second.registFace[spatialID].emplace_back(worldVertexArray);
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
			//返り値用の変数を用意
			std::vector<ModelObject::Face::VertexIndex> spatialFace;

			//返り値用の配列にポリゴンを登録するラムダ式
			// @param : 空間座標
			auto _fastcall NeighborhoodSpatialFaces([&]
			    (
				const Math::Vector3S32& index
				)
				{
					//Vector3S32 tempIndex{ index };
					//引数の空間座標をregisteFacesのkeyの型に変換
					std::string spatialID{ toStringFromSpatialIndex(index) };

					//登録したモデルを持つアクターの数ループする
					for (auto& it : terrains)
					{
						//アクターのワールドTransform
						Matrix parentWorldTrnasform{ it.first->GetComponent<Transform>()->GetWorldTransform() };

						//引数の空間座標が登録されているか探す
						auto iterator = it.second.registFace.find(spatialID);
						if (iterator == it.second.registFace.end())
						{
							//無い場合登録を飛ばす
							continue;
						}

						//登録されている空間の中に存在するポリゴンを全て登録する
						for (auto& vertexIndex : it.second.registFace[spatialID])
						{
							ModelObject::Face::VertexIndex vertex;
							std::copy(vertexIndex.vertex.begin(), vertexIndex.vertex.end(), std::back_inserter(vertex.vertex));
							spatialFace.emplace_back(vertex);
						}
					}
				});

			//indexを中心の空間座標として
			//3*3*3の空間のポリゴンの頂点情報を渡す
			for (s32 x = -SpatialDetail::Renge; x <= SpatialDetail::Renge; x++)
			{
				for (s32 y = -1; y <= 1; y++)
				{
					for (s32 z = -1; z <= 1; z++)
					{
						NeighborhoodSpatialFaces(Vector3S32{ index.x + x,index.y + y,index.z + z });
					}
				}
			}

			//コピー渡しなので外部でoriginalに影響は出ない
			return spatialFace;
		}

		void TerrainManager::ReRegisterDirtyActorPolygon()
		{
			//登録したモデルを持つアクターの数ループする
			for (auto& stageActor : terrains)
			{
				//アクターの持つTransformが前フレームに変更が行われていた場合、
				//頂点座標に変更があるため登録し直す
				if (stageActor.first->GetComponent<Transform>()->GetModedPast())
				{
					//初期化後、登録作業を行う
					stageActor.second.registFace.clear();
					RegisterPolygon(stageActor.first);
				}
			}
		}

		void TerrainManager::GUI()
		{
			using namespace ImGui;

#if 1
			ImGui::Begin("TerrainManager");
			{
				for (auto& act : terrains)
				{
					ImGui::Text("spatial Num %d", act.second.registFace.size());
					u32 iterate{ 0 };
					for (auto& spatial : act.second.registFace)
					{
						FaceInfomationNode(spatial);
						if (100  <= iterate++)
						{
							break;
						}
					}
					ImGui::Separator();
				}
			}ImGui::End();
#endif
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