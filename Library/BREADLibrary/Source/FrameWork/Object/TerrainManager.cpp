#include <algorithm>
#include <thread>
#include "../../../ExternalLibrary/ImGui/Include/imgui.h"

#include "FND/Instance.h"
#include "FrameWork/Object/TerrainManager.h"
#include "FrameWork/SpatialDivisionManager/SpatialDivisionManager.h"
#include "FrameWork/Component/Transform.h"

using Bread::FND::Instance;    //SpatialDivisionManager
using Bread::FND::MapInstance; //"TerrainManager_PolygonRegisterFunction1" ,"TerrainManager_PolygonRegisterFunction2", SyncMainThread ,"SceneSystemExist"
using namespace Bread::Math;

//TODO : 複数のStageActorが存在する場合DirtyFlagが起動していないのに登録作業に入る可能性あり
//TODO : 並列に当たってもっといい処理の仕方を探ろう

namespace Bread
{
	namespace FrameWork
	{
		//引数のモデルからどこの空間にポリゴンがあるのか調べて登録する,
		void TerrainManager::FirstRegisterPolygon(std::shared_ptr<Actor> actor, std::shared_ptr<ModelObject> model)
		{
			//ポリゴン情報の取得
			auto faces{ model->GetFaces() };

			//terrainFace[0]を使う
			swapFlag = false;

			//アクターの登録、    初期化
            //重複している場合は、secondだけ初期化
			TerrainModel spatialData;
			terrains[swapFlag] [actor] = spatialData;
			terrains[!swapFlag][actor] = spatialData;

			//ポリゴン登録を行うラムダ式
			auto _fastcall faceRegisterFunction
			{
				[&](std::pair<std::shared_ptr<Actor>, TerrainModel> actor, const ModelObject::Face& faceCurrent, const Matrix& parentWorldTransform, u32  minIndex, u32 maxIndex)
				{
					//メッシュのポリゴンの数ループする
					// for (auto& face : faceCurrent.face)
					for (u32 it = minIndex; it < maxIndex; ++it)
					{
						//最終的にポリゴンの頂点の平均値を持つ
						Math::Vector3 comprehensive{ Math::Vector3::Zero };

						//ワールドに変換後のVertex情報を登録する
						 ModelObject::Face::VertexIndex worldVertexArray;

						 //ポリゴンの頂点の数ループする
						  for (auto& vertex : faceCurrent.face[it].vertex)
						  {
							  const Vector3 worldVertex{ Math::Vector3TransformCoord(vertex, parentWorldTransform) };
							  //ポリゴンのワールドの頂点座標を足していく
							  worldVertexArray.vertex.emplace_back(worldVertex);
							  comprehensive += worldVertex;
						  }
						  //平均値 == ポリゴンの重心
						  //平均値 /= 頂点数
						   comprehensive /= faceCurrent.face[it].vertex.size();

						   //ポリゴンの重心がどの空間座標にいるのかを計算
						   //後に登録する
							Vector3S32  spatialIndex{ Instance<SpatialDivisionManager>::instance.SpatialCurrent(comprehensive) };
							std::string spatialID   { toStringFromSpatialIndex(spatialIndex) };

							//別スレッドに影響与えないように書き込み時は止める
							std::lock_guard  mutex(terrainManagerMutex);
							actor.second.registFace[spatialID].emplace_back(worldVertexArray);
						}
				   }
			};

			std::vector<std::thread> faceRegisterThread;  //ポリゴンを登録するスレッドの配列
			constexpr u32            threadMaxNum{ 10 }; //thread最大数

			//登録したモデルを持つアクターの数ループする
			for (auto& actor : terrains[swapFlag])
			{
				//アクターのワールドTransform
				Matrix parentWorldTrnasform{ actor.first->GetComponent<Transform>()->GetWorldTransform() };

#if 0           //threadMaxNumの数担当ポリゴンを分配して登録作業を行う、faceRegisterFunctionで行う

				//アクターの持つメッシュの数ループする
				for (auto& faceCurrent : *actor.first->GetComponent<ModelObject>()->GetFaces())
				{
					for (auto& face : faceCurrent.face)
					{
						const u32 ResponsibleAllocationNum{ faceCurrent.face.size() / threadMaxNum };//ポリゴンの配分数

						for (u32 threadIndex = 0; threadIndex < threadMaxNum; ++threadIndex)
						{
							//担当するインデックスの最小と最大を算出
							u32 minIndex{ (threadIndex       * ResponsibleAllocationNum) };
							u32 maxIndex{ ((threadIndex + 1) * ResponsibleAllocationNum) };

							//スレッド展開
							faceRegisterThread.emplace_back(std::thread(faceRegisterFunction, actor, faceCurrent, parentWorldTrnasform, minIndex, maxIndex));
						}
					}
#else           //全ポリゴンを１スレッドで登録
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
							//頂点をワールド座標に変換する
							const Vector3 worldVertex{ Math::Vector3TransformCoord(vertex,parentWorldTrnasform) };

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
						std::string spatialID   { toStringFromSpatialIndex(spatialIndex) };

						//ポリゴンの登録を行いのでロックを行う
						actor.second.registFace[spatialID].emplace_back(worldVertexArray);
					}
#endif
				}
			}

			//生成されたスレッドが終了するのを待機
			for (auto& thread : faceRegisterThread)
			{
				thread.join();
			}
			memcpy(&terrains[!swapFlag], &terrains[swapFlag], sizeof(terrains[swapFlag]));
		}

		//引数のモデルからどこの空間にポリゴンがあるのか調べて登録する
		void TerrainManager::SecondRegisterPolygon(std::shared_ptr<Actor> model)
		{
			//ポリゴン情報の取得
			auto faces{ model->GetComponent<ModelObject>()->GetFaces() };

			//アクターの登録、    初期化
			//重複している場合は、secondだけ初期化
			TerrainModel spatialData;
			terrains[!GetSwapFlag()][model] = spatialData;

			//ポリゴン登録を行うラムダ式
			auto _fastcall faceRegisterFunction
			{
				[&](std::pair<std::shared_ptr<Actor>, TerrainModel> actor, const ModelObject::Face& faceCurrent, const Matrix& parentWorldTransform, u32  minIndex, u32 maxIndex)
				{
					//メッシュのポリゴンの数ループする
					// for (auto& face : faceCurrent.face)
					for (u32 it = minIndex; it < maxIndex; ++it)
					{
						//最終的にポリゴンの頂点の平均値を持つ
						Math::Vector3 comprehensive{ Math::Vector3::Zero };

						//ワールドに変換後のVertex情報を登録する
						 ModelObject::Face::VertexIndex worldVertexArray;

						 //ポリゴンの頂点の数ループする
						  for (auto& vertex : faceCurrent.face[it].vertex)
						  {
							  const Vector3 worldVertex{ Math::Vector3TransformCoord(vertex, parentWorldTransform) };
							  //ポリゴンのワールドの頂点座標を足していく
							  worldVertexArray.vertex.emplace_back(worldVertex);
							  comprehensive += worldVertex;
						  }
						  //平均値 == ポリゴンの重心
						  //平均値 /= 頂点数
						   comprehensive /= faceCurrent.face[it].vertex.size();

						   //ポリゴンの重心がどの空間座標にいるのかを計算
						   //後に登録する
							Vector3S32  spatialIndex{ Instance<SpatialDivisionManager>::instance.SpatialCurrent(comprehensive) };
							std::string spatialID   { toStringFromSpatialIndex(spatialIndex) };

							//別スレッドに影響与えないように書き込み時は止める
							std::lock_guard  mutex(terrainManagerMutex);
							actor.second.registFace[spatialID].emplace_back(worldVertexArray);
						}
				   }
			};

			std::vector<std::thread> faceRegisterThread;  //ポリゴンを登録するスレッドの配列
			constexpr u32            threadMaxNum{ 10 }; //thread最大数

			//アクターのワールドTransform
			const Matrix parentWorldTrnasform{ model->GetComponent<Transform>()->GetWorldTransform() };

			//threadMaxNumの数担当ポリゴンを分配して登録作業を行う、faceRegisterFunctionで行う
#if 0

				//アクターの持つメッシュの数ループする
			for (auto& faceCurrent : *actor.first->GetComponent<ModelObject>()->GetFaces())
			{
				for (auto& face : faceCurrent.face)
				{
					const u32 ResponsibleAllocationNum{ faceCurrent.face.size() / threadMaxNum };//ポリゴンの配分数

					for (u32 threadIndex = 0; threadIndex < threadMaxNum; ++threadIndex)
					{
						//担当するインデックスの最小と最大を算出
						u32 minIndex{ (threadIndex * ResponsibleAllocationNum) };
						u32 maxIndex{ ((threadIndex + 1) * ResponsibleAllocationNum) };

						//スレッド展開
						faceRegisterThread.emplace_back(std::thread(faceRegisterFunction, actor, faceCurrent, parentWorldTrnasform, minIndex, maxIndex));
					}
				}
#else
			//全ポリゴンを１スレッドで登録
			for (auto& faceCurrent : *model->GetComponent<ModelObject>()->GetFaces())
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
						//頂点をワールド座標に変換する
						const Vector3 worldVertex{ Math::Vector3TransformCoord(vertex,parentWorldTrnasform) };

						//ポリゴンのワールドの頂点座標を足していく
						worldVertexArray.vertex.emplace_back(worldVertex);
						comprehensive += worldVertex;
					}
					//平均値 == ポリゴンの重心
					//平均値 /= 頂点数
					comprehensive /= face.vertex.size();

					//ポリゴンの重心がどの空間座標にいるのかを計算後に登録する
					Vector3S32  spatialIndex{ Instance<SpatialDivisionManager>::instance.SpatialCurrent(comprehensive) };
					std::string spatialID   { toStringFromSpatialIndex(spatialIndex)                                   };

					//別スレッドに影響与えないように書き込み時は止める
					std::lock_guard  mutex(terrainManagerMutex);

					//ポリゴンの登録を行いのでロックを行う
					terrains[!GetSwapFlag()][model].registFace[spatialID].emplace_back(worldVertexArray);
				}
			}
#endif
			//生成されたスレッドが終了するのを待機
			for (auto& thread : faceRegisterThread)
			{
				thread.join();
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
					for (auto& it : terrains[GetSwapFlag()])
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

						//登録されたポリゴンを受け取りたいので変更を防ぐ
						std::lock_guard lock(terrainManagerMutex);

						//登録されている空間の中に存在するポリゴンを全て登録する
						for (auto& vertexIndex : it.second.registFace[spatialID])
						{
							ModelObject::Face::VertexIndex vertex;

							std::copy(vertexIndex.vertex.begin(), vertexIndex.vertex.end(), std::back_inserter(vertex.vertex));
							//memcpy(&vertex.vertex, &vertexIndex.vertex, sizeof(vertexIndex.vertex));
							spatialFace.emplace_back(vertex);
						}
					}
				});

			//indexを中心の空間座標として
			//3*3*3の空間のポリゴンの頂点情報を渡す
			for (s32 x = -Renge; x <= Renge; x++)
			{
				for (s32 y = -Renge; y <= Renge; y++)
				{
					for (s32 z = -Renge; z <= Renge; z++)
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
#if 1
			while (MapInstance<bool>::instance["SceneSystemExist"])
			{
				//TerrainManagerスレッドが稼働中
				sync = true;
				//Transformの変更があったかどうか
				bool changed = false;

				//登録したモデルを持つアクターの数ループする
				for (auto& stageActor : terrains[GetSwapFlag()])
				{
					//アクターの持つTransformが前フレームに変更が行われていた場合、
					//頂点座標に変更があるため登録し直す
					if (stageActor.first->GetComponent<Transform>()->GetModedPast())
					{
						//初期化後、登録作業を行う
						SecondRegisterPolygon(stageActor.first);

						//変更があったのでフラグを立てる
						changed = true;
					}
				}

				//TerrainManagerスレッドの処理終了
				sync = false;

				//メインスレッドの待機終了
				while(MapInstance<bool>::instance["SyncMainThread"])
				{
				}

				//使用配列の入れ替え
				if (changed)
				{
					SwapFlag();
				}
			}

			int a = 0;
#else
			//登録したモデルを持つアクターの数ループする
			for (auto& stageActor : terrains[swapFlag])
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
#endif
		}

		void TerrainManager::GUI()
		{
			using namespace ImGui;

#if 0
			ImGui::Begin("TerrainManager");
			{
				ImGui::Text("%d", swapFlag);
				for (auto& act : terrains[swapFlag])
				{
					ImGui::Text("%d : spatial Num %d", swapFlag ,act.second.registFace.size());
				}
				for (auto& act : terrains[!swapFlag])
				{
					ImGui::Text("%d : spatial Num %d", !swapFlag, act.second.registFace.size());
				}
				ImGui::Separator();
				for (auto& act : terrains[swapFlag])
				{
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