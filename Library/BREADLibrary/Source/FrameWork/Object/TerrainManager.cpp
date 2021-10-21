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
		//�����̃��f������ǂ��̋�ԂɃ|���S��������̂����ׂēo�^����
		void TerrainManager::RegisterPolygon(std::shared_ptr<Actor> model)
		{
			//�|���S�����̎擾
			auto faces{ model->GetComponent<ModelObject>()->GetFaces() };

			//�A�N�^�[�̓o�^�A    ������
			//�d�����Ă���ꍇ�́Asecond����������
			TerrainModel spatialData;
			terrains[model] = spatialData;

			//�o�^�������f�������A�N�^�[�̐����[�v����
			for (auto& actor : terrains)
			{
				//�A�N�^�[�̃��[���hTransform
				Matrix parentWorldTrnasform{ actor.first->GetComponent<Transform>()->GetWorldTransform() };

				//�A�N�^�[�̎����b�V���̐����[�v����
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
							//���_���̓��[�J���Ȃ̂ŃX�e�[�W��Transform����̉e�����l�������f�[�^��ۑ����Ȃ���΂����Ȃ�
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
						std::string spatialID   { toStringFromSpatialIndex(spatialIndex)                                   };
						actor.second.registFace[spatialID].emplace_back(worldVertexArray);
					}
				}
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
					for (auto& it : terrains)
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

						//�o�^����Ă����Ԃ̒��ɑ��݂���|���S����S�ēo�^����
						for (auto& vertexIndex : it.second.registFace[spatialID])
						{
							ModelObject::Face::VertexIndex vertex;
							std::copy(vertexIndex.vertex.begin(), vertexIndex.vertex.end(), std::back_inserter(vertex.vertex));
							spatialFace.emplace_back(vertex);
						}
					}
				});

			//index�𒆐S�̋�ԍ��W�Ƃ���
			//3*3*3�̋�Ԃ̃|���S���̒��_����n��
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

			//�R�s�[�n���Ȃ̂ŊO����original�ɉe���͏o�Ȃ�
			return spatialFace;
		}

		void TerrainManager::ReRegisterDirtyActorPolygon()
		{
			//�o�^�������f�������A�N�^�[�̐����[�v����
			for (auto& stageActor : terrains)
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