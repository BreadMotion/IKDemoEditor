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

//TODO : ������StageActor�����݂���ꍇDirtyFlag���N�����Ă��Ȃ��̂ɓo�^��Ƃɓ���\������
//TODO : ����ɓ������Ă����Ƃ��������̎d����T�낤

namespace Bread
{
	namespace FrameWork
	{
		//�����̃��f������ǂ��̋�ԂɃ|���S��������̂����ׂēo�^����,
		void TerrainManager::FirstRegisterPolygon(std::shared_ptr<Actor> actor, std::shared_ptr<ModelObject> model)
		{
			//�|���S�����̎擾
			auto faces{ model->GetFaces() };

			//terrainFace[0]���g��
			swapFlag = false;

			//�A�N�^�[�̓o�^�A    ������
            //�d�����Ă���ꍇ�́Asecond����������
			TerrainModel spatialData;
			terrains[swapFlag] [actor] = spatialData;
			terrains[!swapFlag][actor] = spatialData;

			//�|���S���o�^���s�������_��
			auto _fastcall faceRegisterFunction
			{
				[&](std::pair<std::shared_ptr<Actor>, TerrainModel> actor, const ModelObject::Face& faceCurrent, const Matrix& parentWorldTransform, u32  minIndex, u32 maxIndex)
				{
					//���b�V���̃|���S���̐����[�v����
					// for (auto& face : faceCurrent.face)
					for (u32 it = minIndex; it < maxIndex; ++it)
					{
						//�ŏI�I�Ƀ|���S���̒��_�̕��ϒl������
						Math::Vector3 comprehensive{ Math::Vector3::Zero };

						//���[���h�ɕϊ����Vertex����o�^����
						 ModelObject::Face::VertexIndex worldVertexArray;

						 //�|���S���̒��_�̐����[�v����
						  for (auto& vertex : faceCurrent.face[it].vertex)
						  {
							  const Vector3 worldVertex{ Math::Vector3TransformCoord(vertex, parentWorldTransform) };
							  //�|���S���̃��[���h�̒��_���W�𑫂��Ă���
							  worldVertexArray.vertex.emplace_back(worldVertex);
							  comprehensive += worldVertex;
						  }
						  //���ϒl == �|���S���̏d�S
						  //���ϒl /= ���_��
						   comprehensive /= faceCurrent.face[it].vertex.size();

						   //�|���S���̏d�S���ǂ̋�ԍ��W�ɂ���̂����v�Z
						   //��ɓo�^����
							Vector3S32  spatialIndex{ Instance<SpatialDivisionManager>::instance.SpatialCurrent(comprehensive) };
							std::string spatialID   { toStringFromSpatialIndex(spatialIndex) };

							//�ʃX���b�h�ɉe���^���Ȃ��悤�ɏ������ݎ��͎~�߂�
							std::lock_guard  mutex(terrainManagerMutex);
							actor.second.registFace[spatialID].emplace_back(worldVertexArray);
						}
				   }
			};

			std::vector<std::thread> faceRegisterThread;  //�|���S����o�^����X���b�h�̔z��
			constexpr u32            threadMaxNum{ 10 }; //thread�ő吔

			//�o�^�������f�������A�N�^�[�̐����[�v����
			for (auto& actor : terrains[swapFlag])
			{
				//�A�N�^�[�̃��[���hTransform
				Matrix parentWorldTrnasform{ actor.first->GetComponent<Transform>()->GetWorldTransform() };

#if 0           //threadMaxNum�̐��S���|���S���𕪔z���ēo�^��Ƃ��s���AfaceRegisterFunction�ōs��

				//�A�N�^�[�̎����b�V���̐����[�v����
				for (auto& faceCurrent : *actor.first->GetComponent<ModelObject>()->GetFaces())
				{
					for (auto& face : faceCurrent.face)
					{
						const u32 ResponsibleAllocationNum{ faceCurrent.face.size() / threadMaxNum };//�|���S���̔z����

						for (u32 threadIndex = 0; threadIndex < threadMaxNum; ++threadIndex)
						{
							//�S������C���f�b�N�X�̍ŏ��ƍő���Z�o
							u32 minIndex{ (threadIndex       * ResponsibleAllocationNum) };
							u32 maxIndex{ ((threadIndex + 1) * ResponsibleAllocationNum) };

							//�X���b�h�W�J
							faceRegisterThread.emplace_back(std::thread(faceRegisterFunction, actor, faceCurrent, parentWorldTrnasform, minIndex, maxIndex));
						}
					}
#else           //�S�|���S�����P�X���b�h�œo�^
				for (auto& faceCurrent : *actor.first->GetComponent<ModelObject>()->GetFaces())
				{
					//���b�V���̃|���S���̐����[�v����
					for (auto& face : faceCurrent.face)
					{
						//�ŏI�I�Ƀ|���S���̒��_�̕��ϒl������
						Math::Vector3 comprehensive{ Math::Vector3::Zero };

						//���[���h�ɕϊ����Vertex����o�^����
						ModelObject::Face::VertexIndex worldVertexArray;

						//�|���S���̒��_�̐����[�v����
						for (auto& vertex : face.vertex)
						{
							//���_�����[���h���W�ɕϊ�����
							const Vector3 worldVertex{ Math::Vector3TransformCoord(vertex,parentWorldTrnasform) };

							//�|���S���̃��[���h�̒��_���W�𑫂��Ă���
							worldVertexArray.vertex.emplace_back(worldVertex);
							comprehensive += worldVertex;
						}
						//���ϒl == �|���S���̏d�S
						//���ϒl /= ���_��
						comprehensive /= face.vertex.size();

						//�|���S���̏d�S���ǂ̋�ԍ��W�ɂ���̂����v�Z
						//��ɓo�^����
						Vector3S32  spatialIndex{ Instance<SpatialDivisionManager>::instance.SpatialCurrent(comprehensive) };
						std::string spatialID   { toStringFromSpatialIndex(spatialIndex) };

						//�|���S���̓o�^���s���̂Ń��b�N���s��
						actor.second.registFace[spatialID].emplace_back(worldVertexArray);
					}
#endif
				}
			}

			//�������ꂽ�X���b�h���I������̂�ҋ@
			for (auto& thread : faceRegisterThread)
			{
				thread.join();
			}
			memcpy(&terrains[!swapFlag], &terrains[swapFlag], sizeof(terrains[swapFlag]));
		}

		//�����̃��f������ǂ��̋�ԂɃ|���S��������̂����ׂēo�^����
		void TerrainManager::SecondRegisterPolygon(std::shared_ptr<Actor> model)
		{
			//�|���S�����̎擾
			auto faces{ model->GetComponent<ModelObject>()->GetFaces() };

			//�A�N�^�[�̓o�^�A    ������
			//�d�����Ă���ꍇ�́Asecond����������
			TerrainModel spatialData;
			terrains[!GetSwapFlag()][model] = spatialData;

			//�|���S���o�^���s�������_��
			auto _fastcall faceRegisterFunction
			{
				[&](std::pair<std::shared_ptr<Actor>, TerrainModel> actor, const ModelObject::Face& faceCurrent, const Matrix& parentWorldTransform, u32  minIndex, u32 maxIndex)
				{
					//���b�V���̃|���S���̐����[�v����
					// for (auto& face : faceCurrent.face)
					for (u32 it = minIndex; it < maxIndex; ++it)
					{
						//�ŏI�I�Ƀ|���S���̒��_�̕��ϒl������
						Math::Vector3 comprehensive{ Math::Vector3::Zero };

						//���[���h�ɕϊ����Vertex����o�^����
						 ModelObject::Face::VertexIndex worldVertexArray;

						 //�|���S���̒��_�̐����[�v����
						  for (auto& vertex : faceCurrent.face[it].vertex)
						  {
							  const Vector3 worldVertex{ Math::Vector3TransformCoord(vertex, parentWorldTransform) };
							  //�|���S���̃��[���h�̒��_���W�𑫂��Ă���
							  worldVertexArray.vertex.emplace_back(worldVertex);
							  comprehensive += worldVertex;
						  }
						  //���ϒl == �|���S���̏d�S
						  //���ϒl /= ���_��
						   comprehensive /= faceCurrent.face[it].vertex.size();

						   //�|���S���̏d�S���ǂ̋�ԍ��W�ɂ���̂����v�Z
						   //��ɓo�^����
							Vector3S32  spatialIndex{ Instance<SpatialDivisionManager>::instance.SpatialCurrent(comprehensive) };
							std::string spatialID   { toStringFromSpatialIndex(spatialIndex) };

							//�ʃX���b�h�ɉe���^���Ȃ��悤�ɏ������ݎ��͎~�߂�
							std::lock_guard  mutex(terrainManagerMutex);
							actor.second.registFace[spatialID].emplace_back(worldVertexArray);
						}
				   }
			};

			std::vector<std::thread> faceRegisterThread;  //�|���S����o�^����X���b�h�̔z��
			constexpr u32            threadMaxNum{ 10 }; //thread�ő吔

			//�A�N�^�[�̃��[���hTransform
			const Matrix parentWorldTrnasform{ model->GetComponent<Transform>()->GetWorldTransform() };

			//threadMaxNum�̐��S���|���S���𕪔z���ēo�^��Ƃ��s���AfaceRegisterFunction�ōs��
#if 0

				//�A�N�^�[�̎����b�V���̐����[�v����
			for (auto& faceCurrent : *actor.first->GetComponent<ModelObject>()->GetFaces())
			{
				for (auto& face : faceCurrent.face)
				{
					const u32 ResponsibleAllocationNum{ faceCurrent.face.size() / threadMaxNum };//�|���S���̔z����

					for (u32 threadIndex = 0; threadIndex < threadMaxNum; ++threadIndex)
					{
						//�S������C���f�b�N�X�̍ŏ��ƍő���Z�o
						u32 minIndex{ (threadIndex * ResponsibleAllocationNum) };
						u32 maxIndex{ ((threadIndex + 1) * ResponsibleAllocationNum) };

						//�X���b�h�W�J
						faceRegisterThread.emplace_back(std::thread(faceRegisterFunction, actor, faceCurrent, parentWorldTrnasform, minIndex, maxIndex));
					}
				}
#else
			//�S�|���S�����P�X���b�h�œo�^
			for (auto& faceCurrent : *model->GetComponent<ModelObject>()->GetFaces())
			{
				//���b�V���̃|���S���̐����[�v����
				for (auto& face : faceCurrent.face)
				{
					//�ŏI�I�Ƀ|���S���̒��_�̕��ϒl������
					Math::Vector3 comprehensive{ Math::Vector3::Zero };

					//���[���h�ɕϊ����Vertex����o�^����
					ModelObject::Face::VertexIndex worldVertexArray;

					//�|���S���̒��_�̐����[�v����
					for (auto& vertex : face.vertex)
					{
						//���_�����[���h���W�ɕϊ�����
						const Vector3 worldVertex{ Math::Vector3TransformCoord(vertex,parentWorldTrnasform) };

						//�|���S���̃��[���h�̒��_���W�𑫂��Ă���
						worldVertexArray.vertex.emplace_back(worldVertex);
						comprehensive += worldVertex;
					}
					//���ϒl == �|���S���̏d�S
					//���ϒl /= ���_��
					comprehensive /= face.vertex.size();

					//�|���S���̏d�S���ǂ̋�ԍ��W�ɂ���̂����v�Z��ɓo�^����
					Vector3S32  spatialIndex{ Instance<SpatialDivisionManager>::instance.SpatialCurrent(comprehensive) };
					std::string spatialID   { toStringFromSpatialIndex(spatialIndex)                                   };

					//�ʃX���b�h�ɉe���^���Ȃ��悤�ɏ������ݎ��͎~�߂�
					std::lock_guard  mutex(terrainManagerMutex);

					//�|���S���̓o�^���s���̂Ń��b�N���s��
					terrains[!GetSwapFlag()][model].registFace[spatialID].emplace_back(worldVertexArray);
				}
			}
#endif
			//�������ꂽ�X���b�h���I������̂�ҋ@
			for (auto& thread : faceRegisterThread)
			{
				thread.join();
			}
		}

		//��ԍ��W�̃C���f�b�N�X�ԍ���n����
		//3*3*3�̋�Ԃ̃|���S���̒��_����n��
		[[nodiscard]]
		const std::vector<ModelObject::Face::VertexIndex>
			TerrainManager::GetSpatialFaces(const Math::Vector3S32& index)
		{
			//�Ԃ�l�p�̕ϐ���p��
			std::vector<ModelObject::Face::VertexIndex> spatialFace;

			//�Ԃ�l�p�̔z��Ƀ|���S����o�^���郉���_��
			// @param : ��ԍ��W
			auto _fastcall NeighborhoodSpatialFaces([&]
			    (
				const Math::Vector3S32& index
				)
				{
					//Vector3S32 tempIndex{ index };
					//�����̋�ԍ��W��registeFaces��key�̌^�ɕϊ�
					std::string spatialID{ toStringFromSpatialIndex(index) };

					//�o�^�������f�������A�N�^�[�̐����[�v����
					for (auto& it : terrains[GetSwapFlag()])
					{
						//�A�N�^�[�̃��[���hTransform
						Matrix parentWorldTrnasform{ it.first->GetComponent<Transform>()->GetWorldTransform() };

						//�����̋�ԍ��W���o�^����Ă��邩�T��
						auto iterator = it.second.registFace.find(spatialID);
						if (iterator == it.second.registFace.end())
						{
							//�����ꍇ�o�^���΂�
							continue;
						}

						//�o�^���ꂽ�|���S�����󂯎�肽���̂ŕύX��h��
						std::lock_guard lock(terrainManagerMutex);

						//�o�^����Ă����Ԃ̒��ɑ��݂���|���S����S�ēo�^����
						for (auto& vertexIndex : it.second.registFace[spatialID])
						{
							ModelObject::Face::VertexIndex vertex;

							std::copy(vertexIndex.vertex.begin(), vertexIndex.vertex.end(), std::back_inserter(vertex.vertex));
							//memcpy(&vertex.vertex, &vertexIndex.vertex, sizeof(vertexIndex.vertex));
							spatialFace.emplace_back(vertex);
						}
					}
				});

			//index�𒆐S�̋�ԍ��W�Ƃ���
			//3*3*3�̋�Ԃ̃|���S���̒��_����n��
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

			//�R�s�[�n���Ȃ̂ŊO����original�ɉe���͏o�Ȃ�
			return spatialFace;
		}

		void TerrainManager::ReRegisterDirtyActorPolygon()
		{
#if 1
			while (MapInstance<bool>::instance["SceneSystemExist"])
			{
				//TerrainManager�X���b�h���ғ���
				sync = true;
				//Transform�̕ύX�����������ǂ���
				bool changed = false;

				//�o�^�������f�������A�N�^�[�̐����[�v����
				for (auto& stageActor : terrains[GetSwapFlag()])
				{
					//�A�N�^�[�̎���Transform���O�t���[���ɕύX���s���Ă����ꍇ�A
					//���_���W�ɕύX�����邽�ߓo�^������
					if (stageActor.first->GetComponent<Transform>()->GetModedPast())
					{
						//��������A�o�^��Ƃ��s��
						SecondRegisterPolygon(stageActor.first);

						//�ύX���������̂Ńt���O�𗧂Ă�
						changed = true;
					}
				}

				//TerrainManager�X���b�h�̏����I��
				sync = false;

				//���C���X���b�h�̑ҋ@�I��
				while(MapInstance<bool>::instance["SyncMainThread"])
				{
				}

				//�g�p�z��̓���ւ�
				if (changed)
				{
					SwapFlag();
				}
			}

			int a = 0;
#else
			//�o�^�������f�������A�N�^�[�̐����[�v����
			for (auto& stageActor : terrains[swapFlag])
			{
				//�A�N�^�[�̎���Transform���O�t���[���ɕύX���s���Ă����ꍇ�A
				//���_���W�ɕύX�����邽�ߓo�^������
				if (stageActor.first->GetComponent<Transform>()->GetModedPast())
				{
					//��������A�o�^��Ƃ��s��
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

			//TreeNodeEx�̐ݒ���s���ϐ�
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