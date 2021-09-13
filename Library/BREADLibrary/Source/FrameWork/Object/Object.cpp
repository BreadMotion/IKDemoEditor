#include "pch.h"
#include "FrameWork/Object/Object.h"
#include "../../../ExternalLibrary/ImGui/Include/imgui.h"
#include "../../../ExternalLibrary/ImGuizmo/ImCurveEdit.h"
#include "../../../ExternalLibrary/ImGuizmo/ImGradient.h"
#include "../../../ExternalLibrary/ImGuizmo/ImGuizmo.h"
#include "../../../ExternalLibrary/ImGuizmo/ImSequencer.h"

namespace Bread
{
	namespace FrameWork
	{
		// ����
		std::shared_ptr<ModelObject> ModelObject::Create(Graphics::IGraphicsDevice* graphicsDevice)
		{
			return std::make_shared<ModelObject>(graphicsDevice);
		}

		// ������
		void ModelObject::Initialize()
		{
			if (std::shared_ptr<Actor> owner = GetOwner())
			{
				SetID(owner->GetID());
			}
			resourceManager = FND::SharedInstance<OS::ResourceManager>::instance;

			file = OS::IFileStream::Create();
			file->Initialize(nullptr);

		}

		void ModelObject::GUI()
		{
			using namespace ImGui;
			ImGuiTreeNodeFlags treeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Bullet;
			if (ImGui::CollapsingHeader(u8"���f���I�u�W�F�N�g", ImGuiTreeNodeFlags_NavLeftJumpsBackHere | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet))
			{
				if(ImGui::TreeNodeEx(u8"�W���C���g", treeFlag))
				{
					char  name[128] = {};
					FND::StrCpy(name, sizeof(name), GetID().c_str());
					ImGui::Text(u8"���O"); ImGui::SameLine();
					ImGui::InputText(("##" + GetID()).c_str(), name, IM_ARRAYSIZE(name));
					SetID(name);

					ImGui::Separator();

					static Node* selectNode = &nodes[0];
					{
						static char buf1[128] = "";
						ImGui::InputText("FIlter", buf1, 128, ImGuiInputTextFlags_EnterReturnsTrue);

						ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
						ImGui::BeginChild(u8"�W���C���g���X�g", ImVec2(ImGui::GetWindowContentRegionWidth() * 1.0f, 260), true);

						for (auto& node : nodes)
						{
							std::string t1 = buf1;
							if (!strstr(node.name.c_str(), t1.c_str()))continue;

							if (ImGui::Selectable(node.name.c_str(), selectNode->name == node.name))
							{
								selectNode = &node;
							}
						}
						ImGui::EndChild();
						ImGui::PopStyleVar();
					}

					std::string jName = selectNode->name;
					ImGui::DragFloat3("translate", &selectNode->translate.x);
					RegisterWatchVal("translate - " + jName, &selectNode->translate);
					ImGui::DragFloat4("rotate", &selectNode->rotate.x);
					RegisterWatchVal("rotate - " + jName, &selectNode->rotate);
					ImGui::DragFloat3("scale", &selectNode->scale.x);
					RegisterWatchVal("scale - " + jName, &selectNode->scale);
					ImGui::Separator();

					ImGui::DragFloat3("worldT", Math::GetLocation(selectNode->worldTransform));
					ImGui::DragFloat4("worldR", Math::GetRotation(selectNode->worldTransform));
					ImGui::DragFloat3("worldS", Math::GetScale(selectNode->worldTransform));

					ImGui::TreePop();
				}

				ImGui::Separator();

				std::string guiName = "jointNames : ";
				guiName += fileName;
				guiName = "modelData : " + fileName;
				if (ImGui::TreeNodeEx(u8"���f���f�[�^", treeFlag))
				{
					ImGui::Text("meshSize %d",  modelResource->GetMeshSize());
					ImGui::Text("nodes %d",     modelResource->GetModelData().nodes.size());
					ImGui::Text("materials %d", modelResource->GetModelData().materials.size());
					ImGui::Text("meshs %d",     modelResource->GetModelData().meshes.size());
					ImGui::Text("face index : %d", faces[0].face.size());
					ImGui::Text("AnimCurrentTime : %f", animator->GetAnimation(0)->player->GetAnimCurrentTime());
					ImGui::Separator();

					ImGui::TreePop();
				}

				guiName = "boneTransform : " + fileName;
				if (ImGui::TreeNodeEx(u8"�{�[���g�����X�t�H�[��", treeFlag))
				{
					int iterate = 0;
					const int maxDigit = 3;
					for (auto& node : meshNodes)
					{
						if (ImGui::TreeNode(std::to_string(iterate).c_str()))
						{
							int index = -1;
							static char buf1[3] = "";
							ImGui::InputText("FIlter", buf1, maxDigit + 1, ImGuiInputTextFlags_EnterReturnsTrue); ImGui::SameLine();
							ImGui::Text(" / 3");
							for (auto& mat : node.boneTransform)
							{
								char u8Str[3] = {};
								FND::StrCpy(u8Str, maxDigit, std::to_string(++index).c_str());
								for (int i = 0; i < (maxDigit - (FND::StrLen(u8Str) + maxDigit) % maxDigit); i++)
									FND::StrCat(u8Str, sizeof(u8Str), "\0");

								std::string t1 = buf1, t2 = u8Str;
								for (int it = 0; it < t1.size(); it++)
								{
									if (it > t1.find('\0'))
										t1[it] = NULL;
								}

								if (t1.compare(t2) != 0)continue;

								ImGui::DragFloat3("pos", Math::GetLocation(mat));
								//RegisterWatchVal(std::to_string(index) + "bonePos" + ID, &Math::GetLocation(mat));
								ImGui::DragFloat4("rotate", Math::GetRotation(mat));
								//RegisterWatchVal(std::to_string(index) + "boneRotate" + ID, &Math::GetRotation(mat));
								ImGui::DragFloat3("scale", Math::GetScale(mat));
								//RegisterWatchVal(std::to_string(index) + "boneScale" + ID,  &Math::GetScale(mat));
								ImGui::Separator();
							}
							ImGui::TreePop();
						}
						iterate++;
					}

					ImGui::TreePop();
				}

			}
}

		// ���f���̓ǂݍ���
		void ModelObject::Load(const char* filename)
		{
			const char* fullPass     = OS::Path::GetFullPath(filename);
			std::string animFullPass = std::string(fullPass);

			std::string modelFilename;
			modelFilename = OS::Path::ChangeFileExtension(fullPass, "mdl");

			if (OS::Path::CheckFileExtension(fullPass, "fbx") && !file->Exists(modelFilename.c_str()))
			{
				std::unique_ptr<Loader::ILoader> loader = Loader::ILoader::Create();
				if (!loader->Initialize(fullPass))
				{
					return;
				}
				Graphics::ModelData data;
				if (!loader->Load(data, OS::Path::GetDirectoryName(filename)))
				{
					return;
				}
				Graphics::ModelData::Serialize(data, modelFilename.c_str());
			}

			std::shared_ptr<OS::IResourceManager> resourceManagerwp = resourceManager.lock();
			if (resourceManagerwp)
			{
				modelResource = resourceManagerwp->LoadImmediate<Graphics::IModelResource>(modelFilename.c_str());
			}
			if (modelResource == nullptr)
			{
				return;
			}

			const std::vector<Graphics::ModelData::Node>& resourceNodes = modelResource->GetModelData().nodes;
			nodes.resize(resourceNodes.size());

			for (sizeT nodeIndex = 0; nodeIndex < nodes.size(); nodeIndex++)
			{
				auto&& src = resourceNodes.at(nodeIndex);
				auto&& dst = nodes.at(nodeIndex);

				dst.name   = src.name.c_str();
				dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
				dst.scale  = src.scale;
				dst.rotate = src.rotate;
				dst.translate = src.translate;
			}

			const std::vector<Graphics::ModelData::Material>& resourceMaterials = modelResource->GetModelData().materials;
			materials.resize(resourceMaterials.size());

			for (sizeT i = 0; i < materials.size(); ++i)
			{
				ModelObject::Material& material = materials.at(i);
				material.name = resourceMaterials.at(i).name;

				material.textures.resize(resourceMaterials.at(i).textureFilename.size());
				material.colors.resize(resourceMaterials.at(i).color.size());

				for (sizeT j = 0; j < material.textures.size(); ++j)
				{
					material.colors.at(j)   = resourceMaterials.at(i).color.at(j);
					material.textures.at(j) = Graphics::ITexture::Create();
					material.textures.at(j)->Initialize(graphicsDevice->GetDevice(), resourceMaterials.at(i).textureFilename.at(j).c_str(), static_cast<Graphics::MaterialType>(j), material.colors.at(j));
				}
			}

			boneNames.resize(nodes.size());
			for (sizeT i = 0; i < boneNames.size(); ++i)
			{
				boneNames.at(i) = nodes.at(i).name.c_str();
			}

			animator = std::make_unique<Animator>();
			animator->Initialize(this);

			LoadAnimation(animFullPass.c_str(), -1);
		}

		// �A�j���[�V�����̓ǂݍ���
		s32 ModelObject::LoadAnimation(const char* filename, s32 index)
		{
			std::shared_ptr<OS::IResourceManager> resourceManagerwp = resourceManager.lock();
			if (resourceManagerwp)
			{
				return animator->LoadResource(resourceManagerwp.get(), filename, index);
			}
			return 0;
		}

		s32 ModelObject::AddAnimationLayer(const s8* beginNodeName, const s8* endNodeName)
		{
			return animator->AddLayer(beginNodeName, endNodeName);
		}

		s32 ModelObject::AddAnimationLayer(s32 beginNodeIndex, s32 endNodeIndex)
		{
			return animator->AddLayer(beginNodeIndex, endNodeIndex);
		}

		s32 ModelObject::AddAnimationStateToLayer(s32 addAnimationIndex, s32 animationLayerIndex)
		{
			return animator->AddAnimationStateToLayer(addAnimationIndex, animationLayerIndex);
		}

		s32 ModelObject::AddBlendTreeToLayer(s32 animationLayerIndex)
		{
			return animator->AddBlendTreeToLayer(animationLayerIndex);
		}

		s32 ModelObject::AddBlendAnimationStateToBlendTree(s32 addAnimationIndex, Bread::Math::Vector3 plot, s32 animationLayerIndex, s32 blendTreeIndex)
		{
			return animator->AddBlendAnimationStateToBlendTree(addAnimationIndex, plot, animationLayerIndex, blendTreeIndex);
		}

		// �s����X�V
		void ModelObject::UpdateTransform(f32 elapsedTime)
		{
			UpdateAnimation(elapsedTime);
			UpdateLocalTransform();
			UpdateWorldTransform();
			UpdateBoneTransform();
		}

		// ���[�J���ϊ��s����X�V
		void ModelObject::UpdateLocalTransform()
		{
			for (sizeT i = 0; i < nodes.size(); ++i)
			{
				Math::Matrix scale, rotate, translate;
				scale     = Math::MatrixScaling(nodes[i].scale.x, nodes[i].scale.y, nodes[i].scale.z);
				rotate    = Math::MatrixRotationQuaternion(&nodes[i].rotate);
				translate = Math::MatrixTranslation(nodes[i].translate.x, nodes[i].translate.y, nodes[i].translate.z);

				nodes[i].localTransform = scale * rotate * translate;
			}
		}

		// ���[���h�ϊ��s����X�V
		void ModelObject::UpdateWorldTransform()
		{
			for (sizeT i = 0; i < nodes.size(); ++i)
			{
				if (nodes[i].parent != nullptr)
				{
					nodes[i].worldTransform = nodes[i].localTransform * nodes[i].parent->worldTransform;
				}
				else
				{
					nodes[i].worldTransform = nodes[i].localTransform;
				}
			}
		}

		// �{�[���ϊ��s����X�V
		void ModelObject::UpdateBoneTransform()
		{
			const Graphics::ModelData& modelData = GetModelResource()->GetModelData();

			if (meshNodes.size() < modelData.meshes.size())
			{
				meshNodes.resize(modelData.meshes.size());
			}

			for (sizeT i = 0; i < meshNodes.size(); ++i)
			{
				MeshNode& meshNode = meshNodes.at(i);

				if (meshNode.boneTransform.size() < modelData.meshes.at(i).nodeIndices.size())
				{
					meshNode.boneTransform.resize(modelData.meshes.at(i).nodeIndices.size());
				}

				for (sizeT j = 0; j < meshNode.boneTransform.size(); ++j)
				{
					meshNode.boneTransform.at(j)
						= Math::MatrixMultiply(modelData.meshes.at(i).offsetTransforms.at(j),
							nodes.at(modelData.meshes.at(i).nodeIndices.at(j)).worldTransform);
				}
				meshNode.boneTransformCount = static_cast<u32>(meshNode.boneTransform.size());
			}
		}

		// �A�j���[�V�����̍Đ�
		void ModelObject::PlayAnimation(u32 layerIndex, u32 stateIndex, u32 clip, f32 fadeTime)
		{
			animator->Play(layerIndex, stateIndex, clip, fadeTime);
		}

		// �u�����h�c���[�̃A�j���[�V�����Đ�
		void ModelObject::PlayBlendTreeAnimation(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime)
		{
			animator->PlayBlendTree(layerIndex, blendTreeIndex, clip, fadeTime);
		}

		// �A�j���[�V�����̓����Đ�
		void ModelObject::SimultaneousPlayAnimation(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime)
		{
			animator->BlendPlay(layerIndex, blendTreeIndex, clip, fadeTime);
		}

		// �u�����h�c���[�̃A�j���[�V���������Đ�
		void ModelObject::SimultaneousPlayBlendTreeAniamation(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime)
		{
			animator->BlendPlayBlendTree(layerIndex, blendTreeIndex, clip, fadeTime);
		}

		// �ꎞ��~/�ĊJ
		void ModelObject::PauseAnimation(bool pause)
		{
			animator->Pause(pause);
		}

		// ��~
		void ModelObject::StopAnimation()
		{
			animator->Stop();
		}

		// ���[�v�Đ��ݒ�
		void ModelObject::SetLoopAnimation(bool loop)
		{
			animator->SetLoop(loop);
		}

		// ���[�v�����Đ��ݒ�
		void ModelObject::SetBlendLoopAnimation(bool loop)
		{
			animator->SetBlendLoop(loop);
		}

		// �Đ���
		bool ModelObject::IsPlaying()
		{
			return animator->IsPlaying();
		}

		// ���[�v�Đ�����
		bool ModelObject::IsLoopAnimation()
		{
			return animator->IsLoop();
		}

		// �A�j���[�V�����̍X�V
		void ModelObject::UpdateAnimation(f32 elapsedTime)
		{
			animator->Update(elapsedTime);
		}

		// �Đ����Ԓ�(�b)�擾
		f32 ModelObject::GetLength()
		{
			return animator->GetLength();
		}

		// �Đ����x�{���ݒ�
		void ModelObject::SetSpeed(f32 speed)
		{
			animator->SetSpeed(speed);
		}

		// �Đ����x�{���擾
		f32 ModelObject::GetSpeed()
		{
			return animator->GetSpeed();
		}

		// �u�����h����
		bool ModelObject::IsBlend()
		{
			return animator->IsBlend();
		}

		// �Đ��ʒu�ݒ�
		void ModelObject::SetCurrentTime(f32 seconds)
		{
			animator->SetCurrentTime(seconds);
		}

		// �Đ��ʒu�擾
		f32 ModelObject::GetAnimCurrentTime()
		{
			return animator->GetAnimCurrentTime();
		}

		// �c��Đ����Ԏ擾
		f32 ModelObject::GetLastTime()
		{
			return animator->GetLastTime();
		}

		// �Đ��J�n�ʒu�ݒ�
		void ModelObject::SetBeginTime(f32 seconds)
		{
			animator->SetBeginTime(seconds);
		}

		// �Đ��J�n�ʒu�擾
		f32 ModelObject::GetBeginTime()
		{
			return animator->GetBeginTime();
		}

		// �Đ��I���ʒu�ݒ�
		void ModelObject::SetEndTime(f32 seconds)
		{
			animator->SetEndTime(seconds);
		}

		// �Đ��I���ʒu�擾
		f32 ModelObject::GetEndTime()
		{
			return animator->GetEndTime();
		}

		// ���[�v�Đ��J�n�ʒu�ݒ�
		void ModelObject::SetLoopBeginTime(f32 seconds)
		{
			animator->SetLoopBeginTime(seconds);
		}

		// ���[�v�Đ��J�n�ʒu�擾
		f32 ModelObject::GetLoopBeginTime()
		{
			return animator->GetLoopBeginTime();
		}

		// ���[�v�Đ��I���ʒu�ݒ�
		void ModelObject::SetLoopEndTime(f32 seconds)
		{
			animator->SetLoopEndTime(seconds);
		}

		// ���[�v�Đ��I���ʒu�擾
		f32 ModelObject::GetLoopEndTime()
		{
			return animator->GetLoopEndTime();
		}

		// �u�����h���[�g�̐ݒ�
		void ModelObject::SetBlendRate(f32 rate)
		{
			animator->SetBlendRate(rate);
		}
		void ModelObject::SetBlendRate(Bread::Math::Vector3 rate)
		{
			animator->SetBlendRate(rate);
		}

		//�u�����h���[�g�̎擾
		f32* ModelObject::GetBlendRateF1()
		{
			return animator->GetBlendRateF1();
		}

		Math::Vector3* ModelObject::GetBlendRateF3()
		{
			return animator->GetBlendRateF3();
		}

		//sketalMesh��Hip��ݒ肷��
		void ModelObject::SetHipID(const Bread::s8* nodeName)
		{
			animator->SetHipID(nodeName);
		}

		void ModelObject::BuildFaces()
		{
			for (auto& mesh : modelResource->GetModelData().meshes)
			{
				Face faceEnt;
				faceEnt.face.emplace_back();

				for (int i = 0; i < mesh.positions.size(); i++)
				{
					faceEnt.face.back().vertex.emplace_back(mesh.positions[i]);

					//face���̍\�z
					if (i % 3 == 2)
					{
						faceEnt.face.emplace_back();
					}
				}
				faces.emplace_back(faceEnt);
			}
		}
	}
}