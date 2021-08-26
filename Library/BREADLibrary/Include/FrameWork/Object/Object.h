#pragma once

#include <vector>
#include "Math/BreadMath.h"
#include "Math//Arithmetic.h"
#include "Graphics/GraphicsDevice.h"
#include "Graphics/Model.h"
#include "Graphics/Animation.h"
#include "Graphics/Texture.h"
#include "OS/Path.h"
#include "FND/STD.h"
#include "../Source/Loader/Loader.h"
#include "FrameWork/Component/Component.h"

namespace Bread
{
	namespace FrameWork
	{
		struct CollisionData
		{
			Math::Vector3 pos;
			f32                  radius    = 0.0f;
			s32                  boneIndex = 0;
			f32                  mass      = 0.0f;
		};

		enum MovableShaft
		{
			X = 0,
			Y,
			Z
		};

		class Animator;
		class ModelObject : public Component
		{
		public:
			struct Node
			{
				std::string        name;
				Node*              parent;
				std::vector<Node*> child;
				Math::Vector3      scale;
				Math::Quaternion   rotate;
				Math::Vector3      translate;
				Math::Matrix       localTransform;
				Math::Matrix       worldTransform;

				bool               Movable[3] = { false };
				Math::Vector3      minRot;
				Math::Vector3      maxRot;
			};

			struct MeshNode
			{
				std::vector<Math::Matrix> boneTransform;
				u32 boneTransformCount = 0;

				MeshNode() {}
			};

			struct Material
			{
				std::string                                      name;
				std::vector<std::unique_ptr<Graphics::ITexture>> textures;
				std::vector<Math::Color>                         colors;
			};

			struct Face
			{
				struct VertexIndex
				{
					std::vector<Math::Vector3> vertex;
				};
				std::vector<VertexIndex>       face;
			};

		private:
			std::shared_ptr<Graphics::IModelResource> modelResource;
			std::unique_ptr<Animator>                 animator;
			std::vector<Node>                         nodes;
			std::vector<MeshNode>                     meshNodes;
			std::vector<Face>                         faces;
			std::vector<Material>                     materials;
			std::unique_ptr<OS::IResourceManager>     resourceManamger;
			std::unique_ptr<OS::IFileStream>          file;
			Graphics::IGraphicsDevice*                graphicsDevice = nullptr;

			std::string fileName;

			// �{�[�����񋓗p
			std::vector<const char*> boneNames;

		public:
			explicit ModelObject(Graphics::IGraphicsDevice* graphicsDevice)
			{
				SetGraphicsDevice(graphicsDevice);
			}
			~ModelObject() override{}

		public:
			//����
			static std::unique_ptr<ModelObject> Create(Graphics::IGraphicsDevice* graphicsDevice);

			// ������
			void Initialize() override;

			//imgui
			void GUI() override;

		public:

			// ���f���̓ǂݍ���
			void Load(const char* filename);

			// �A�j���[�V�����̓ǂݍ���
			s32 LoadAnimation(const char* filename, s32 index);

			/// <summary>
			/// �A�j���[�V�������C���[�̒ǉ�
			/// </summary>
			/// <param name="beginNodeName"> �n�_�̃{�[���� </param>
			/// <param name="endNodeName">   �I�_�̃{�[���� </param>
			/// <returns>                    �A�j���[�V�������C���[�ԍ� </returns>
			s32 AddAnimationLayer(const s8* beginNodeName, const s8* endNodeName);

			/// <summary>
			/// �A�j���[�V�������C���[�̒ǉ�
			/// </summary>
			/// <param name="beginNodeIndex"> �n�_�̃{�[���ԍ� </param>
			/// <param name="endNodeIndex">   �I�_�̃{�[���ԍ� </param>
			/// <returns>                     �A�j���[�V�������C���[�ԍ� </returns>
			s32 AddAnimationLayer(s32 beginNodeIndex = -1, s32 endNodeIndex = -1);

			/// <summary>
			/// �A�j���[�V�������C���[�ɃA�j���[�V�����X�e�[�g��ǉ�
			/// </summary>
			/// <param name="addAnimationIndex">    �A�j���[�V�����ԍ� </param>
			/// <param name="animationLayerIndex">  �A�j���[�V�������C���[�ԍ� </param>
			/// <returns>                           �A�j���[�V�����X�e�[�g�ԍ� </returns>
			s32 AddAnimationStateToLayer(s32 addAnimationIndex, s32 animationLayerIndex);

			/// <summary>
			/// �A�j���[�V�������C���[�Ƀu�����h�c���[��ǉ�
			/// </summary>
			/// <param name="animationLayerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <returns>                          �u�����h�c���[�ԍ� </returns>
			s32 AddBlendTreeToLayer(s32 animationLayerIndex);

			/// <summary>
			/// �u�����h�c���[�Ƀu�����h�A�j���[�V�����X�e�[�g��ǉ�
			/// </summary>
			/// <param name="addAnimationIndex">   �A�j���[�V�����ԍ� </param>
			/// <param name="plot">                 2������̈ʒu(X�͈� : -1.0f ~ 1.0f, Y�͈� : -1.0f ~ 1.0f, Z�͈� : 0.0f) </param>
			/// <param name="animationLayerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="blendTreeIndex">      �u�����h�c���[�ԍ� </param>
			/// <returns>                          �u�����h�c���[�̃X�e�[�g�ԍ� </returns>
			s32 AddBlendAnimationStateToBlendTree(s32 addAnimationIndex, Bread::Math::Vector3 plot, s32 animationLayerIndex, s32 blendTreeIndex);

			// �s����X�V
			void UpdateTransform(f32 elapsedTime);

			// ���[�J���ϊ��s����X�V
			void UpdateLocalTransform();

			// ���[���h�ϊ��s����X�V
			void UpdateWorldTransform();

			// �{�[���ϊ��s����X�V
			void UpdateBoneTransform();

			/// <summary>
			/// �w�背�C���[�̃X�e�[�g�̃A�j���[�V�����Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip">       �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime">   ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void PlayAnimation(u32 layerIndex, u32 stateIndex, u32 clip, f32 fadeTime = 0.0f);

			/// <summary>
			/// �w�背�C���[�̃u�����h�c���[�̃A�j���[�V�����Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip">       �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime">   ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void PlayBlendTreeAnimation(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime = 0.0f);

			/// <summary>
			/// �w�背�C���[�̃X�e�[�g�̃A�j���[�V���������Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip">       �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime">   ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void SimultaneousPlayAnimation(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime = 0.0f);

			/// <summary>
			/// �w�背�C���[�̃u�����h�c���[�̃A�j���[�V���������Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip">       �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime">   ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void SimultaneousPlayBlendTreeAniamation(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime = 0.0f);

			// �ꎞ��~/�ĊJ
			void PauseAnimation(bool pause);

			// ��~
			void StopAnimation();

			// ���[�v�Đ��ݒ�
			void SetLoopAnimation(bool loop);

			// ���[�v�����Đ��ݒ�
			void SetBlendLoopAnimation(bool loop);

			// �Đ���
			bool IsPlaying();

			// ���[�v�Đ���
			bool IsLoopAnimation();

			// �A�j���[�V�����̍X�V
			void UpdateAnimation(f32 elapsedTime);

			// �Đ����Ԓ�(�b)�擾
			f32 GetLength();

			// �Đ����x�{���ݒ�
			void SetSpeed(f32 speed);

			// �Đ����x�{���擾
			f32 GetSpeed();

			// �u�����h����
			bool IsBlend();

			// �Đ��ʒu�ݒ�
			void SetCurrentTime(f32 seconds);

			// �Đ��ʒu�擾
			f32 GetAnimCurrentTime();

			// �c��Đ����Ԏ擾
			f32 GetLastTime();

			// �Đ��J�n�ʒu�ݒ�
			void SetBeginTime(f32 seconds);

			// �Đ��J�n�ʒu�擾
			f32 GetBeginTime();

			// �Đ��I���ʒu�ݒ�
			void SetEndTime(f32 seconds);

			// �Đ��I���ʒu�擾
			f32 GetEndTime();

			// ���[�v�Đ��J�n�ʒu�ݒ�
			void SetLoopBeginTime(f32 seconds);

			// ���[�v�Đ��J�n�ʒu�擾
			f32 GetLoopBeginTime();

			// ���[�v�Đ��I���ʒu�ݒ�
			void SetLoopEndTime(f32 seconds);

			// ���[�v�Đ��I���ʒu�擾
			f32 GetLoopEndTime();

			//�u�����h���[�g�̐ݒ�
			void SetBlendRate(f32 rate);
			void SetBlendRate(Bread::Math::Vector3 rate);

			//�u�����h���[�g�̎擾
			f32*                  GetBlendRateF1();
			Math::Vector3* GetBlendRateF3();

			//sketalMesh��Hip��ݒ肷��
			void SetHipID(const Bread::s8* nodeName);

			//graphicsDevice�̃|�C���^�[��ݒ肷��
			void SetGraphicsDevice(Graphics::IGraphicsDevice* graphicsDevice);

			// ���f�����\�[�X�̎擾
			Graphics::IModelResource* GetModelResource() { return modelResource.get(); }

			// �m�[�h�̎擾
			std::vector<Node>* GetNodes() { return &nodes; }

			//���O����W���C���g�̎擾
			Node* GetNodeFromName(const std::string& name)
			{
				for (auto& node: nodes)
				{
					if (node.name == name)
					{
						return &node;
					}
				}
				return nullptr;
			}

			// �{�[�����擾
			std::vector<const char*> GetBoneNames() { return boneNames; }

			// �{�[���g�����X�t�H�[���̎擾
			Math::Matrix* GetBoneTransforms(u32 meshIndex) { return meshNodes.at(meshIndex).boneTransform.data(); }
			Math::Matrix GetBoneTransforms(u32 meshIndex, u32 boneIndex) { return meshNodes.at(meshIndex).boneTransform.at(boneIndex); }

			//�|���S�������擾����
			std::vector<Face>* GetFaces() { return &faces; }

			//�|���S�����̐ݒ�
			void BuildFaces();

			// �{�[���C���f�b�N�X�̎擾
			u32 GetBoneIndex(const char* name)
			{
				for (u32 i = 0; i < nodes.size(); ++i)
				{
					if (FND::StrCmp(nodes[i].name.c_str(), name) == 0)
					{
						return i;
					}
				}

				return -1;
			}

			// ���b�V���m�[�h�̃T�C�Y�擾
			sizeT GetMeshNodes() { return meshNodes.size(); }

			//���b�V���m�[�h�̎擾
			std::vector<MeshNode>& GetMeshNode() { return meshNodes; }

			// �{�[���g�����X�t�H�[���̃T�C�Y�擾
			u32 GetBoneTransformCount(u32 meshIndex) { return meshNodes.at(meshIndex).boneTransformCount; }

			// �}�e���A���̃e�N�X�`���擾
			Graphics::ITexture* GetTexture(u32 index, u32 texIndex)
			{
				return materials.at(index).textures.at(texIndex).get();
			}

			// �}�e���A���̃T�C�Y�擾
			sizeT GetMaterialSize() { return materials.size(); }

			// �}�e���A���̃e�N�X�`���T�C�Y�擾
			sizeT GetTextureSize(u32 index) { return materials.at(index).textures.size(); }
		};

		class Animator
		{
		private:
			struct Animation
			{
				std::string                                   filename;
				std::shared_ptr<Graphics::IAnimationResource> resource;
				std::unique_ptr<Graphics::IAnimationPlayer>   player;
				std::vector<s16>                              bindNodeIDs;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			struct AnimationState
			{
				Animation* animation = nullptr;
			};

			struct BlendAnimationState
			{
				Animation* animation = nullptr;
				Bread::Math::Vector3 plot;
			};

			struct BlendTree
			{
				std::vector<BlendAnimationState> states;
				Bread::Math::Vector3             late;
			};

			struct AnimationLayer
			{
				std::vector<AnimationState> states;
				std::vector<BlendTree>      blendTrees;

				AnimationState* currentState;
				BlendTree*      currentBlendTree;

				s32 beginNodeIndex = -1;
				s32 endNodeIndex   = -1;
			};

		private:
			std::vector<Animation>           animations;
			std::vector<AnimationLayer>      animationLayers;
			std::vector<ModelObject::Node>*  nodes;
			std::unique_ptr<OS::IFileStream> file;

			AnimationLayer* currentAnimationLayer            = nullptr;
			AnimationLayer* blendCurrentAnimationLayer[2]    = { nullptr, nullptr };
			s32             blendCurrentAnimationLayerSize   = 2;

			//f32 blendRate = 0.0f;
			Bread::Math::Vector3 blendRate = { 0.0f, 0.0f, 0.0f };

			Bread::u32 hipNodeID = 0;

		public:
			Animation* GetAnimation(int index)
			{
				return &animations[index];
			}

			void Initialize(ModelObject* model)
			{
				file = OS::IFileStream::Create();
				file->Initialize(nullptr);

				nodes = model->GetNodes();
			}

			// �A�j���[�V�������\�[�X�ǂݍ���
			s32 LoadResource(OS::IResourceManager* resourceManamger, const char* filename, s32 index)
			{
				if (index < 0)
				{
					index = static_cast<s32>(animations.size());
					animations.emplace_back(Animation());
				}

				Animation& animation = animations.at(index);

				animation.filename = OS::Path::ChangeFileExtension(filename, "ani");

				if (OS::Path::CheckFileExtension(filename, "fbx") && !file->Exists(animation.filename.c_str()))
				{
					std::unique_ptr<Loader::ILoader> loader = Loader::ILoader::Create();
					if (!loader->Initialize(filename))
					{
						return -1;
					}
					Graphics::AnimationData data;
					if (!loader->Load(data))
					{
						return -1;
					}
					Graphics::AnimationData::Serialize(data, animation.filename.c_str());
				}
				LoadResource(resourceManamger, animation);

				return index;
			}

			void LoadResource(OS::IResourceManager* resourceManamger, Animation& animation)
			{
				animation.resource = resourceManamger->LoadImmediate<Graphics::IAnimationResource>(animation.filename.c_str());

				if (animation.resource)
				{
					animation.player = Graphics::IAnimationPlayer::Create();
					animation.player->Initialize(animation.resource);
					BindAnimationNodes(animation);
				}
			}

			/// <summary>
			/// �A�j���[�V�������C���[�̒ǉ�
			/// </summary>
			/// <param name="beginNodeName"> �n�_�̃{�[���� </param>
			/// <param name="endNodeName">   �I�_�̃{�[���� </param>
			/// <returns>                    �A�j���[�V�������C���[�ԍ� </returns>
			s32 AddLayer(const s8* beginNodeName, const s8* endNodeName)
			{
				s32 index = static_cast<s32>(animationLayers.size());
				animationLayers.emplace_back(AnimationLayer());

				if (beginNodeName == nullptr)
				{
					return index;
				}
				if (endNodeName == nullptr)
				{
					return index;
				}

				s32 animationNodeCount = static_cast<s32>(nodes->size());
				for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
				{
					if (FND::StrCmp(nodes->at(animationNodeID).name.c_str(), beginNodeName) == 0)
					{
						animationLayers.at(index).beginNodeIndex = animationNodeID;
					}
					if (FND::StrCmp(nodes->at(animationNodeID).name.c_str(), endNodeName) == 0)
					{
						animationLayers.at(index).endNodeIndex = animationNodeID;
					}
				}

				return index;
			}

			/// <summary>
			/// �A�j���[�V�������C���[�̒ǉ�
			/// </summary>
			/// <param name="beginNodeIndex"> �n�_�̃{�[���ԍ� </param>
			/// <param name="endNodeIndex">   �I�_�̃{�[���ԍ� </param>
			/// <returns>                     �A�j���[�V�������C���[�ԍ� </returns>
			s32 AddLayer(s32 beginNodeIndex = -1, s32 endNodeIndex = -1)
			{
				s32 index = static_cast<s32>(animationLayers.size());
				animationLayers.emplace_back(AnimationLayer());
				animationLayers.at(index).beginNodeIndex = beginNodeIndex;
				animationLayers.at(index).endNodeIndex = endNodeIndex;

				return index;
			}

			/// <summary>
			/// �A�j���[�V�������C���[�ɃA�j���[�V�����X�e�[�g��ǉ�
			/// </summary>
			/// <param name="addAnimationIndex">   �A�j���[�V�����ԍ� </param>
			/// <param name="animationLayerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <returns>                          �A�j���[�V�����X�e�[�g�ԍ� </returns>
			s32 AddAnimationStateToLayer(s32 addAnimationIndex, s32 animationLayerIndex)
			{
				s32 animationLayerCount = static_cast<s32>(animationLayers.size());
				if (animationLayerIndex < 0 || animationLayerCount <= animationLayerIndex)
				{
					return -1;
				}

				s32 stateCount = static_cast<s32>(animationLayers.at(animationLayerIndex).states.size());
				animationLayers.at(animationLayerIndex).states.emplace_back(AnimationState());
				animationLayers.at(animationLayerIndex).states.at(stateCount).animation = &animations.at(addAnimationIndex);

				return stateCount;
			}

			/// <summary>
			/// �A�j���[�V�������C���[�Ƀu�����h�c���[��ǉ�
			/// </summary>
			/// <param name="animationLayerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <returns>                          �u�����h�c���[�ԍ� </returns>
			s32 AddBlendTreeToLayer(s32 animationLayerIndex)
			{
				s32 animationLayerCount = static_cast<s32>(animationLayers.size());
				if (animationLayerIndex < 0 || animationLayerCount <= animationLayerIndex)
				{
					return -1;
				}

				s32 blendTreeCount = static_cast<s32>(animationLayers.at(animationLayerIndex).blendTrees.size());
				animationLayers.at(animationLayerIndex).blendTrees.emplace_back(BlendTree());
				animationLayers.at(animationLayerIndex).blendTrees.at(blendTreeCount).late = Bread::Math::Vector3(0.0f, 0.0f, 0.0f);

				return blendTreeCount;
			}

			/// <summary>
			/// �u�����h�c���[�Ƀu�����h�A�j���[�V�����X�e�[�g��ǉ�
			/// </summary>
			/// <param name="addAnimationIndex">   �A�j���[�V�����ԍ� </param>
			/// <param name="plot">                 2������̈ʒu(X�͈� : -1.0f ~ 1.0f, Y�͈� : -1.0f ~ 1.0f, Z�͈� : 0.0f) </param>
			/// <param name="animationLayerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="blendTreeIndex">      �u�����h�c���[�ԍ� </param>
			/// <returns>                          �u�����h�c���[�̃X�e�[�g�ԍ� </returns>
			s32 AddBlendAnimationStateToBlendTree(s32 addAnimationIndex, Bread::Math::Vector3 plot, s32 animationLayerIndex, s32 blendTreeIndex)
			{
				s32 animationLayerCount = static_cast<s32>(animationLayers.size());
				if (animationLayerIndex < 0 || animationLayerCount <= animationLayerIndex)
				{
					return -1;
				}

				s32 blendTreeCount = static_cast<s32>(animationLayers.at(animationLayerIndex).blendTrees.size());
				if (blendTreeIndex < 0 || blendTreeCount <= blendTreeIndex)
				{
					return -1;
				}

				s32 stateCount = static_cast<s32>(animationLayers.at(animationLayerIndex).blendTrees.at(blendTreeIndex).states.size());
				animationLayers.at(animationLayerIndex).blendTrees.at(blendTreeIndex).states.emplace_back(BlendAnimationState());
				animationLayers.at(animationLayerIndex).blendTrees.at(blendTreeIndex).states.at(stateCount).animation = &animations.at(addAnimationIndex);
				animationLayers.at(animationLayerIndex).blendTrees.at(blendTreeIndex).states.at(stateCount).plot = plot;

				return stateCount;
			}

			/// <summary>
			/// �w�背�C���[�̃X�e�[�g�̃A�j���[�V�����Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip">       �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime">   ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void Play(u32 layerIndex, u32 stateIndex, u32 clip, f32 fadeTime = 0.0f)
			{
				if (layerIndex < 0 || layerIndex >= animationLayers.size())
				{
					return;
				}

				AnimationLayer& animatioLayer = animationLayers.at(layerIndex);
				if (stateIndex < 0 || stateIndex >= animatioLayer.states.size())
				{
					return;
				}

				AnimationState& animatioState = animatioLayer.states.at(stateIndex);
				if (!animatioState.animation->resource)
				{
					return;
				}

				const Graphics::AnimationData& data = animatioState.animation->resource->GetAnimationData();
				if (clip < 0 || clip >= data.clips.size())
				{
					return;
				}

				currentAnimationLayer = &animatioLayer;
				blendCurrentAnimationLayer[0] = nullptr;
				blendCurrentAnimationLayer[1] = nullptr;

				animatioLayer.currentState = &animatioState;
				animatioLayer.currentState->animation->player->Play(clip);
				animatioLayer.currentState->animation->player->SetBlendTime(fadeTime);
				animatioLayer.currentBlendTree = nullptr;
			}

			/// <summary>
			/// �w�背�C���[�̃u�����h�c���[�̃A�j���[�V�����Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip">       �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime">   ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void PlayBlendTree(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime = 0.0f)
			{
				if (layerIndex < 0 || layerIndex >= animationLayers.size())
				{
					return;
				}

				AnimationLayer& animatioLayer = animationLayers.at(layerIndex);
				if (blendTreeIndex < 0 || blendTreeIndex >= animatioLayer.blendTrees.size())
				{
					return;
				}

				BlendTree& blendTree = animatioLayer.blendTrees.at(blendTreeIndex);
				for (auto& state : blendTree.states)
				{
					if (!state.animation->resource)
					{
						return;
					}
				}

				for (auto& state : blendTree.states)
				{
					const Graphics::AnimationData& data = state.animation->resource->GetAnimationData();
					if (clip < 0 || clip >= data.clips.size())
					{
						return;
					}
				}

				currentAnimationLayer = &animatioLayer;
				blendCurrentAnimationLayer[0] = nullptr;
				blendCurrentAnimationLayer[1] = nullptr;

				animatioLayer.currentState = nullptr;
				animatioLayer.currentBlendTree = &blendTree;
				for (auto& state : blendTree.states)
				{
					state.animation->player->Play(clip);
					state.animation->player->SetBlendTime(fadeTime);
				}
			}

			/// <summary>
			/// �w�背�C���[�̃X�e�[�g�̃A�j���[�V���������Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip">       �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime">   ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void BlendPlay(u32 layerIndex, u32 stateIndex, u32 clip, f32 fadeTime = 0.0f)
			{
				if (layerIndex < 0 || layerIndex >= animationLayers.size())
				{
					return;
				}

				AnimationLayer& animatioLayer = animationLayers.at(layerIndex);
				if (stateIndex < 0 || stateIndex >= animatioLayer.states.size())
				{
					return;
				}

				AnimationState& animatioState = animatioLayer.states.at(stateIndex);
				if (!animatioState.animation->resource)
				{
					return;
				}

				const Graphics::AnimationData& data = animatioState.animation->resource->GetAnimationData();
				if (clip < 0 || clip >= data.clips.size())
				{
					return;
				}

				if (blendCurrentAnimationLayer[0] == nullptr) blendCurrentAnimationLayer[0] = &animatioLayer;
				else if (blendCurrentAnimationLayer[1] == nullptr) blendCurrentAnimationLayer[1] = &animatioLayer;

				animatioLayer.currentState = &animatioState;
				animatioLayer.currentState->animation->player->Play(clip);
				animatioLayer.currentState->animation->player->SetBlendTime(fadeTime);
				animatioLayer.currentBlendTree = nullptr;
			}

			/// <summary>
			/// �w�背�C���[�̃u�����h�c���[�̃A�j���[�V���������Đ�
			/// </summary>
			/// <param name="layerIndex"> �A�j���[�V�������C���[�ԍ� </param>
			/// <param name="stateIndex"> �A�j���[�V�����X�e�[�g�ԍ� </param>
			/// <param name="clip">       �A�j���[�V�����N���b�v�ԍ� </param>
			/// <param name="fadeTime">   ���̃A�j���[�V�����Ƀu�����h���Ă����X�s�[�h(�u�����h���[�g) </param>
			void BlendPlayBlendTree(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime = 0.0f)
			{
				if (layerIndex < 0 || layerIndex >= animationLayers.size())
				{
					return;
				}

				AnimationLayer& animatioLayer = animationLayers.at(layerIndex);
				if (blendTreeIndex < 0 || blendTreeIndex >= animatioLayer.blendTrees.size())
				{
					return;
				}

				BlendTree& blendTree = animatioLayer.blendTrees.at(blendTreeIndex);
				for (auto& state : blendTree.states)
				{
					if (!state.animation->resource)
					{
						return;
					}
				}

				for (auto& state : blendTree.states)
				{
					const Graphics::AnimationData& data = state.animation->resource->GetAnimationData();
					if (clip < 0 || clip >= data.clips.size())
					{
						return;
					}
				}

				if (blendCurrentAnimationLayer[0]      == nullptr) blendCurrentAnimationLayer[0] = &animatioLayer;
				else if (blendCurrentAnimationLayer[1] == nullptr) blendCurrentAnimationLayer[1] = &animatioLayer;

				animatioLayer.currentState = nullptr;
				animatioLayer.currentBlendTree = &blendTree;
				for (auto& state : blendTree.states)
				{
					state.animation->player->Play(clip);
					state.animation->player->SetBlendTime(fadeTime);
				}
			}

			// �ꎞ��~/�ĊJ
			void Pause(bool pause)
			{
				if (!currentAnimationLayer) return;
				if (currentAnimationLayer->currentState)
				{
					currentAnimationLayer->currentState->animation->player->Pause(pause);
				}
				else if (currentAnimationLayer->currentBlendTree)
				{
					for (auto& state : currentAnimationLayer->currentBlendTree->states)
					{
						state.animation->player->Pause(pause);
					}
				}
			}

			// ��~
			void Stop()
			{
				if (!currentAnimationLayer) return;
				if (currentAnimationLayer->currentState)
				{
					currentAnimationLayer->currentState->animation->player->Stop();
				}
				else if (currentAnimationLayer->currentBlendTree)
				{
					for (auto& state : currentAnimationLayer->currentBlendTree->states)
					{
						state.animation->player->Stop();
					}
				}
			}

			// ���[�v�Đ��ݒ�
			void SetLoop(bool loop)
			{
				if (!currentAnimationLayer) return;
				if (currentAnimationLayer->currentState)
				{
					currentAnimationLayer->currentState->animation->player->SetLoop(loop);
				}
				else if (currentAnimationLayer->currentBlendTree)
				{
					for (auto& state : currentAnimationLayer->currentBlendTree->states)
					{
						state.animation->player->SetLoop(loop);
					}
				}
			}

			// ���[�v�����Đ��ݒ�
			void SetBlendLoop(bool loop)
			{
				for (s32 i = 0; i < blendCurrentAnimationLayerSize; ++i)
				{
					if (!blendCurrentAnimationLayer[i]) continue;
					if (blendCurrentAnimationLayer[i]->currentState)
					{
						blendCurrentAnimationLayer[i]->currentState->animation->player->SetLoop(loop);
					}
					else if (blendCurrentAnimationLayer[i]->currentBlendTree)
					{
						for (auto& state : blendCurrentAnimationLayer[i]->currentBlendTree->states)
						{
							state.animation->player->SetLoop(loop);
						}
					}
				}
			}

			// �Đ���
			bool IsPlaying()
			{
				return currentAnimationLayer->currentState->animation->player->IsPlaying();
			}

			// ���[�v�Đ���
			bool IsLoop()
			{
				return currentAnimationLayer->currentState->animation->player->IsLoop();
			}

			// �Đ����Ԓ�(�b)�擾
			f32 GetLength()
			{
				return currentAnimationLayer->currentState->animation->player->GetLength();
			}

			// �Đ����x�{���ݒ�
			void SetSpeed(f32 speed)
			{
				currentAnimationLayer->currentState->animation->player->SetSpeed(speed);
			}

			// �Đ����x�{���擾
			f32 GetSpeed()
			{
				return currentAnimationLayer->currentState->animation->player->GetSpeed();
			}

			// �u�����h����
			bool IsBlend()
			{
				return currentAnimationLayer->currentState->animation->player->IsBlend();
			}
			// �Đ��ʒu�ݒ�
			void SetCurrentTime(f32 seconds)
			{
				currentAnimationLayer->currentState->animation->player->SetCurrentTime(seconds);
			}

			// �Đ��ʒu�擾
			f32 GetAnimCurrentTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetAnimCurrentTime();
			}

			// �c��Đ����Ԏ擾
			f32 GetLastTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetLastTime();
			}

			// �Đ��J�n�ʒu�ݒ�
			void SetBeginTime(f32 seconds)
			{
				currentAnimationLayer->currentState->animation->player->SetBeginTime(seconds);
			}

			// �Đ��J�n�ʒu�擾
			f32 GetBeginTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetBeginTime();
			}

			// �Đ��I���ʒu�ݒ�
			void SetEndTime(f32 seconds)
			{
				currentAnimationLayer->currentState->animation->player->SetEndTime(seconds);
			}

			// �Đ��I���ʒu�擾
			f32 GetEndTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetEndTime();
			}

			// ���[�v�Đ��J�n�ʒu�ݒ�
			void SetLoopBeginTime(f32 seconds)
			{
				currentAnimationLayer->currentState->animation->player->SetLoopBeginTime(seconds);
			}

			// ���[�v�Đ��J�n�ʒu�擾
			f32 GetLoopBeginTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetLoopBeginTime();
			}

			// ���[�v�Đ��I���ʒu�ݒ�
			void SetLoopEndTime(f32 seconds)
			{
				currentAnimationLayer->currentState->animation->player->SetLoopEndTime(seconds);
			}

			// ���[�v�Đ��I���ʒu�擾
			f32 GetLoopEndTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetLoopEndTime();
			}

			// �u�����h���[�g�̐ݒ�
			void SetBlendRate(f32 rate)
			{
				blendRate.x = rate;
				blendRate.y = 0.0f;
				blendRate.z = 0.0f;
			}

			void SetBlendRate(Bread::Math::Vector3 rate)
			{
				blendRate = rate;
			}

			void SetHipID(const Bread::s8* nodeName)
			{
				for (u32 i = 0; i < nodes->size(); ++i)
				{
					if (FND::StrCmp(nodes->at(i).name.c_str(), nodeName) == 0)
					{
						hipNodeID = i;
						break;
					}
				}
			}

			//�u�����h���[�g�̎擾
			f32* GetBlendRateF1()
			{
				return &blendRate.x;
			}

			Math::Vector3* GetBlendRateF3()
			{
				return &blendRate;
			}

			// �A�j���[�V�����o���N�C���f�b�N�X�擾
			u32 GetAnimationBankIndex(const char* name) const
			{
				for (size_t i = 0; i < animations.size(); ++i)
				{
					if (0 == FND::StrCmp(name, OS::Path::GetFileNameWithoutExtension(animations.at(i).filename.c_str())))
					{
						return static_cast<u32>(i);
					}
				}
				return -1;
			}

			void NodeBlend(ModelObject::Node& currentNode, Math::Vector3& scale, Math::Quaternion& rotate, Math::Vector3& translate, Math::Vector3& blendScale, Math::Quaternion& blendRotate, Math::Vector3& blendTranslate, f32 blendRate, Graphics::IAnimationPlayer* player, s32 nodeID)
			{
				player->CalculateScale(nodeID, blendScale);
				player->CalculateRotate(nodeID, blendRotate);
				player->CalculateTranslate(nodeID, blendTranslate);

				currentNode.scale     = Math::Vector3Lerp(scale, blendScale, fabsf(blendRate));
				currentNode.rotate    = Math::QuaternionSlerp(rotate, blendRotate, fabsf(blendRate));
				currentNode.translate = Math::Vector3Lerp(translate, blendTranslate, fabsf(blendRate));
			}

			void CheckBlendOfEachAxis(ModelObject::Node& currentNode, Math::Vector3& scale, Math::Quaternion& rotate, Math::Vector3& translate, Math::Vector3& blendScale, Math::Quaternion& blendRotate, Math::Vector3& blendTranslate, f32 blendRate, f32 plot, Graphics::IAnimationPlayer* player, s32 nodeID)
			{
				if (!(fabsf(blendRate) <= Bread::Math::Epsilon))
				{
					if (0.0f < blendRate)
					{
						if (0.0f < plot)
						{
							NodeBlend(currentNode, scale, rotate, translate, blendScale, blendRotate, blendTranslate, blendRate, player, nodeID);
						}
					}
					else if (blendRate < 0.0f)
					{
						if (plot < 0.0f)
						{
							NodeBlend(currentNode, scale, rotate, translate, blendScale, blendRotate, blendTranslate, blendRate, player, nodeID);
						}
					}
				}
			}

			std::vector<ModelObject::Node> UpdateLayer(AnimationLayer* animationLayer, f32 elapsedTime)
			{
				std::vector<ModelObject::Node> nodes = *this->nodes;
				s32 animationNodeCount = static_cast<s32>(this->nodes->size());

				if (animationLayer->currentState)
				{
					std::unique_ptr<Graphics::IAnimationPlayer>& animationPlayer = animationLayer->currentState->animation->player;
					animationPlayer->Update(elapsedTime);

					for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
					{
						s16 bindNodeID = animationLayer->currentState->animation->bindNodeIDs.at(animationNodeID);
						if (bindNodeID < 0) continue;

						ModelObject::Node& node = nodes.at(animationNodeID);
						ModelObject::Node& totalNode = nodes.at(animationNodeID);

						Math::Vector3 scale     = node.scale;
						Math::Quaternion rotate = node.rotate;
						Math::Vector3 translate = node.translate;

						animationPlayer->CalculateScale(animationNodeID, scale);
						animationPlayer->CalculateRotate(animationNodeID, rotate);
						animationPlayer->CalculateTranslate(animationNodeID, translate);

						totalNode.scale     = scale;
						totalNode.rotate    = rotate;
						totalNode.translate = translate;
					}
				}
				else if (animationLayer->currentBlendTree)
				{
					for (auto& state : animationLayer->currentBlendTree->states)
					{
						std::unique_ptr<Graphics::IAnimationPlayer>& animationPlayer = state.animation->player;
						animationPlayer->Update(elapsedTime);

						for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
						{
							s16 bindNodeID = state.animation->bindNodeIDs.at(animationNodeID);
							if (bindNodeID < 0) continue;

							ModelObject::Node& node        = nodes.at(animationNodeID);
							ModelObject::Node& totalNode   = nodes.at(animationNodeID);
							ModelObject::Node currentNodeX = nodes.at(animationNodeID);
							ModelObject::Node currentNodeY = nodes.at(animationNodeID);

							Math::Vector3 scale     = node.scale;
							Math::Quaternion rotate = node.rotate;
							Math::Vector3 translate = node.translate;

							Math::Vector3 blendScale[]     = { node.scale, node.scale };
							Math::Quaternion blendRotate[] = { node.rotate, node.rotate };
							Math::Vector3 blendTranslate[] = { node.translate, node.translate };

							if (blendRate == Bread::Math::Vector3(0.0f, 0.0f, 0.0f))
							{
								totalNode.scale     = scale;
								totalNode.rotate    = rotate;
								totalNode.translate = translate;
							}
							else if (state.plot == Bread::Math::Vector3(0.0f, 0.0f, 0.0f))
							{
								animationPlayer->CalculateScale(animationNodeID, scale);
								animationPlayer->CalculateRotate(animationNodeID, rotate);
								animationPlayer->CalculateTranslate(animationNodeID, translate);

								totalNode.scale     = scale;
								totalNode.rotate    = rotate;
								totalNode.translate = translate;
							}
							else
							{
								CheckBlendOfEachAxis
								(
									currentNodeX,
									scale, rotate, translate,
									blendScale[0], blendRotate[0], blendTranslate[0],
									blendRate.x,
									state.plot.x,
									state.animation->player.get(),
									animationNodeID
								);
								CheckBlendOfEachAxis
								(
									currentNodeY,
									scale, rotate, translate,
									blendScale[1], blendRotate[1], blendTranslate[1],
									blendRate.y,
									state.plot.y,
									state.animation->player.get(),
									animationNodeID
								);

								totalNode.scale     = Math::Vector3Lerp(currentNodeX.scale, currentNodeY.scale, fabsf(blendRate.y));			// ��ԗ���fabsf(blendRate.y)�����邱�Ƃ�
								totalNode.rotate    = Math::QuaternionSlerp(currentNodeX.rotate, currentNodeY.rotate, fabsf(blendRate.y));		// �傫�����y���ɂ���currentNodeY���D�悳��āA
								totalNode.translate = Math::Vector3Lerp(currentNodeX.translate, currentNodeY.translate, fabsf(blendRate.y));	// ���������x���ɂ���currentNodeX���D�悳���B
							}
						}
					}
				}

				return nodes;
			}

			void Update(f32 elapsedTime)
			{
				if (currentAnimationLayer)
				{
					std::vector<ModelObject::Node> nodes = UpdateLayer(currentAnimationLayer, elapsedTime);
					s32 animationNodeCount               = static_cast<s32>(this->nodes->size());
					for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
					{
						if (currentAnimationLayer->currentState)
						{
							s16 bindNodeID = currentAnimationLayer->currentState->animation->bindNodeIDs.at(animationNodeID);
							if (bindNodeID < 0) continue;

							ModelObject::Node& node             = this->nodes->at(animationNodeID);
							ModelObject::Node& currentNode = nodes.at(animationNodeID);

							Math::Vector3      scale        = currentNode.scale;
							Math::Quaternion rotate     = currentNode.rotate;
							Math::Vector3       translate = currentNode.translate;

							node.scale     = scale;
							node.rotate    = rotate;
							node.translate = translate;
						}
						else if (currentAnimationLayer->currentBlendTree)
						{
							bool contionue = false;
							for (auto& state : currentAnimationLayer->currentBlendTree->states)
							{
								s16 bindNodeID = state.animation->bindNodeIDs.at(animationNodeID);
								if (bindNodeID < 0) contionue = true;
							}
							if (contionue) continue;

							ModelObject::Node& node             = this->nodes->at(animationNodeID);
							ModelObject::Node& currentNode = nodes.at(animationNodeID);

							Math::Vector3       scale       = currentNode.scale;
							Math::Quaternion rotate     = currentNode.rotate;
							Math::Vector3       translate = currentNode.translate;

							node.scale       = scale;
							node.rotate     = rotate;
							node.translate = translate;
						}
					}

					for (Bread::s32 layerCount = 0; layerCount < blendCurrentAnimationLayerSize; ++layerCount)
					{
						if (!blendCurrentAnimationLayer[layerCount]) continue;

						std::vector<ModelObject::Node> blendNodes = UpdateLayer(blendCurrentAnimationLayer[layerCount], elapsedTime);
						s32 animationNodeCount = static_cast<s32>(this->nodes->size());
						for (s32 animationNodeID = 0; animationNodeID < animationNodeCount; ++animationNodeID)
						{
							if (blendCurrentAnimationLayer[layerCount]->currentState)
							{
								s16 bindNodeID = blendCurrentAnimationLayer[layerCount]->currentState->animation->bindNodeIDs.at(animationNodeID);
								if (bindNodeID < 0) continue;

								ModelObject::Node& node                      = this->nodes->at(animationNodeID);
								ModelObject::Node& blendCurrentNode = blendNodes.at(animationNodeID);

								Math::Vector3       scale       = node.scale;
								Math::Quaternion rotate     = node.rotate;
								Math::Vector3       translate = node.translate;

								if (animationNodeID == hipNodeID || (blendCurrentAnimationLayer[layerCount]->beginNodeIndex <= animationNodeID && animationNodeID <= blendCurrentAnimationLayer[layerCount]->endNodeIndex))
								{
									scale     = blendCurrentNode.scale;
									rotate    = blendCurrentNode.rotate;
									translate = blendCurrentNode.translate;
								}

								node.scale     = scale;
								node.rotate    = rotate;
								node.translate = translate;
							}
							else if (blendCurrentAnimationLayer[layerCount]->currentBlendTree)
							{
								bool contionue = false;
								for (auto& state : blendCurrentAnimationLayer[layerCount]->currentBlendTree->states)
								{
									s16 bindNodeID = state.animation->bindNodeIDs.at(animationNodeID);
									if (bindNodeID < 0) contionue = true;
								}
								if (contionue) continue;

								ModelObject::Node& node = this->nodes->at(animationNodeID);
								ModelObject::Node& blendCurrentNode = blendNodes.at(animationNodeID);

								Math::Vector3       scale       = node.scale;
								Math::Quaternion rotate     = node.rotate;
								Math::Vector3       translate = node.translate;

								if (animationNodeID == hipNodeID || (blendCurrentAnimationLayer[layerCount]->beginNodeIndex <= animationNodeID && animationNodeID <= blendCurrentAnimationLayer[layerCount]->endNodeIndex))
								{
									scale     = blendCurrentNode.scale;
									rotate    = blendCurrentNode.rotate;
									translate = blendCurrentNode.translate;
								}

								node.scale     = scale;
								node.rotate    = rotate;
								node.translate = translate;
							}
						}
					}
				}
			}

			// �A�j���[�V�����m�[�h�ƃo�C���h
			void BindAnimationNodes(Animation& animation)
			{
				const Graphics::AnimationData& data = animation.resource->GetAnimationData();
				animation.bindNodeIDs.resize(data.nodeNames.size());

				for (size_t i = 0; i < data.nodeNames.size(); ++i)
				{
					animation.bindNodeIDs.at(i) = -1;

					const std::string& nodeName = data.nodeNames[i];
					for (size_t j = 0; j < nodes->size(); ++j)
					{
						ModelObject::Node& node = nodes->at(j);
						if (nodeName == node.name)
						{
							animation.bindNodeIDs.at(i) = static_cast<s16>(static_cast<s32>(j));
						}
					}
				}
			}

			// �A�j���[�V�����N���b�v������
			const Graphics::AnimationData::Clip* FindAnimationClip(const char* name)
			{
				for (Animation& animation : animations)
				{
					const Graphics::AnimationData& data = animation.resource->GetAnimationData();
					for (const Graphics::AnimationData::Clip& clip : data.clips)
					{
						if (clip.name == name)
						{
							return &clip;
						}
					}
				}
				return nullptr;
			}

		public:
			template<class Archive>
			void serialize(Archive& archive, u32 version);
		};
	}
}