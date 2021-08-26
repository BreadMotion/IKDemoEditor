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

			// ボーン名列挙用
			std::vector<const char*> boneNames;

		public:
			explicit ModelObject(Graphics::IGraphicsDevice* graphicsDevice)
			{
				SetGraphicsDevice(graphicsDevice);
			}
			~ModelObject() override{}

		public:
			//生成
			static std::unique_ptr<ModelObject> Create(Graphics::IGraphicsDevice* graphicsDevice);

			// 初期化
			void Initialize() override;

			//imgui
			void GUI() override;

		public:

			// モデルの読み込み
			void Load(const char* filename);

			// アニメーションの読み込み
			s32 LoadAnimation(const char* filename, s32 index);

			/// <summary>
			/// アニメーションレイヤーの追加
			/// </summary>
			/// <param name="beginNodeName"> 始点のボーン名 </param>
			/// <param name="endNodeName">   終点のボーン名 </param>
			/// <returns>                    アニメーションレイヤー番号 </returns>
			s32 AddAnimationLayer(const s8* beginNodeName, const s8* endNodeName);

			/// <summary>
			/// アニメーションレイヤーの追加
			/// </summary>
			/// <param name="beginNodeIndex"> 始点のボーン番号 </param>
			/// <param name="endNodeIndex">   終点のボーン番号 </param>
			/// <returns>                     アニメーションレイヤー番号 </returns>
			s32 AddAnimationLayer(s32 beginNodeIndex = -1, s32 endNodeIndex = -1);

			/// <summary>
			/// アニメーションレイヤーにアニメーションステートを追加
			/// </summary>
			/// <param name="addAnimationIndex">    アニメーション番号 </param>
			/// <param name="animationLayerIndex">  アニメーションレイヤー番号 </param>
			/// <returns>                           アニメーションステート番号 </returns>
			s32 AddAnimationStateToLayer(s32 addAnimationIndex, s32 animationLayerIndex);

			/// <summary>
			/// アニメーションレイヤーにブレンドツリーを追加
			/// </summary>
			/// <param name="animationLayerIndex"> アニメーションレイヤー番号 </param>
			/// <returns>                          ブレンドツリー番号 </returns>
			s32 AddBlendTreeToLayer(s32 animationLayerIndex);

			/// <summary>
			/// ブレンドツリーにブレンドアニメーションステートを追加
			/// </summary>
			/// <param name="addAnimationIndex">   アニメーション番号 </param>
			/// <param name="plot">                 2次元上の位置(X範囲 : -1.0f ~ 1.0f, Y範囲 : -1.0f ~ 1.0f, Z範囲 : 0.0f) </param>
			/// <param name="animationLayerIndex"> アニメーションレイヤー番号 </param>
			/// <param name="blendTreeIndex">      ブレンドツリー番号 </param>
			/// <returns>                          ブレンドツリーのステート番号 </returns>
			s32 AddBlendAnimationStateToBlendTree(s32 addAnimationIndex, Bread::Math::Vector3 plot, s32 animationLayerIndex, s32 blendTreeIndex);

			// 行列を更新
			void UpdateTransform(f32 elapsedTime);

			// ローカル変換行列を更新
			void UpdateLocalTransform();

			// ワールド変換行列を更新
			void UpdateWorldTransform();

			// ボーン変換行列を更新
			void UpdateBoneTransform();

			/// <summary>
			/// 指定レイヤーのステートのアニメーション再生
			/// </summary>
			/// <param name="layerIndex"> アニメーションレイヤー番号 </param>
			/// <param name="stateIndex"> アニメーションステート番号 </param>
			/// <param name="clip">       アニメーションクリップ番号 </param>
			/// <param name="fadeTime">   このアニメーションにブレンドしていくスピード(ブレンドレート) </param>
			void PlayAnimation(u32 layerIndex, u32 stateIndex, u32 clip, f32 fadeTime = 0.0f);

			/// <summary>
			/// 指定レイヤーのブレンドツリーのアニメーション再生
			/// </summary>
			/// <param name="layerIndex"> アニメーションレイヤー番号 </param>
			/// <param name="stateIndex"> アニメーションステート番号 </param>
			/// <param name="clip">       アニメーションクリップ番号 </param>
			/// <param name="fadeTime">   このアニメーションにブレンドしていくスピード(ブレンドレート) </param>
			void PlayBlendTreeAnimation(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime = 0.0f);

			/// <summary>
			/// 指定レイヤーのステートのアニメーション同時再生
			/// </summary>
			/// <param name="layerIndex"> アニメーションレイヤー番号 </param>
			/// <param name="stateIndex"> アニメーションステート番号 </param>
			/// <param name="clip">       アニメーションクリップ番号 </param>
			/// <param name="fadeTime">   このアニメーションにブレンドしていくスピード(ブレンドレート) </param>
			void SimultaneousPlayAnimation(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime = 0.0f);

			/// <summary>
			/// 指定レイヤーのブレンドツリーのアニメーション同時再生
			/// </summary>
			/// <param name="layerIndex"> アニメーションレイヤー番号 </param>
			/// <param name="stateIndex"> アニメーションステート番号 </param>
			/// <param name="clip">       アニメーションクリップ番号 </param>
			/// <param name="fadeTime">   このアニメーションにブレンドしていくスピード(ブレンドレート) </param>
			void SimultaneousPlayBlendTreeAniamation(u32 layerIndex, u32 blendTreeIndex, u32 clip, f32 fadeTime = 0.0f);

			// 一時停止/再開
			void PauseAnimation(bool pause);

			// 停止
			void StopAnimation();

			// ループ再生設定
			void SetLoopAnimation(bool loop);

			// ループ同時再生設定
			void SetBlendLoopAnimation(bool loop);

			// 再生中
			bool IsPlaying();

			// ループ再生か
			bool IsLoopAnimation();

			// アニメーションの更新
			void UpdateAnimation(f32 elapsedTime);

			// 再生時間長(秒)取得
			f32 GetLength();

			// 再生速度倍率設定
			void SetSpeed(f32 speed);

			// 再生速度倍率取得
			f32 GetSpeed();

			// ブレンド中か
			bool IsBlend();

			// 再生位置設定
			void SetCurrentTime(f32 seconds);

			// 再生位置取得
			f32 GetAnimCurrentTime();

			// 残り再生時間取得
			f32 GetLastTime();

			// 再生開始位置設定
			void SetBeginTime(f32 seconds);

			// 再生開始位置取得
			f32 GetBeginTime();

			// 再生終了位置設定
			void SetEndTime(f32 seconds);

			// 再生終了位置取得
			f32 GetEndTime();

			// ループ再生開始位置設定
			void SetLoopBeginTime(f32 seconds);

			// ループ再生開始位置取得
			f32 GetLoopBeginTime();

			// ループ再生終了位置設定
			void SetLoopEndTime(f32 seconds);

			// ループ再生終了位置取得
			f32 GetLoopEndTime();

			//ブレンドレートの設定
			void SetBlendRate(f32 rate);
			void SetBlendRate(Bread::Math::Vector3 rate);

			//ブレンドレートの取得
			f32*                  GetBlendRateF1();
			Math::Vector3* GetBlendRateF3();

			//sketalMeshのHipを設定する
			void SetHipID(const Bread::s8* nodeName);

			//graphicsDeviceのポインターを設定する
			void SetGraphicsDevice(Graphics::IGraphicsDevice* graphicsDevice);

			// モデルリソースの取得
			Graphics::IModelResource* GetModelResource() { return modelResource.get(); }

			// ノードの取得
			std::vector<Node>* GetNodes() { return &nodes; }

			//名前からジョイントの取得
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

			// ボーン名取得
			std::vector<const char*> GetBoneNames() { return boneNames; }

			// ボーントランスフォームの取得
			Math::Matrix* GetBoneTransforms(u32 meshIndex) { return meshNodes.at(meshIndex).boneTransform.data(); }
			Math::Matrix GetBoneTransforms(u32 meshIndex, u32 boneIndex) { return meshNodes.at(meshIndex).boneTransform.at(boneIndex); }

			//ポリゴン情報を取得する
			std::vector<Face>* GetFaces() { return &faces; }

			//ポリゴン情報の設定
			void BuildFaces();

			// ボーンインデックスの取得
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

			// メッシュノードのサイズ取得
			sizeT GetMeshNodes() { return meshNodes.size(); }

			//メッシュノードの取得
			std::vector<MeshNode>& GetMeshNode() { return meshNodes; }

			// ボーントランスフォームのサイズ取得
			u32 GetBoneTransformCount(u32 meshIndex) { return meshNodes.at(meshIndex).boneTransformCount; }

			// マテリアルのテクスチャ取得
			Graphics::ITexture* GetTexture(u32 index, u32 texIndex)
			{
				return materials.at(index).textures.at(texIndex).get();
			}

			// マテリアルのサイズ取得
			sizeT GetMaterialSize() { return materials.size(); }

			// マテリアルのテクスチャサイズ取得
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

			// アニメーションリソース読み込み
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
			/// アニメーションレイヤーの追加
			/// </summary>
			/// <param name="beginNodeName"> 始点のボーン名 </param>
			/// <param name="endNodeName">   終点のボーン名 </param>
			/// <returns>                    アニメーションレイヤー番号 </returns>
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
			/// アニメーションレイヤーの追加
			/// </summary>
			/// <param name="beginNodeIndex"> 始点のボーン番号 </param>
			/// <param name="endNodeIndex">   終点のボーン番号 </param>
			/// <returns>                     アニメーションレイヤー番号 </returns>
			s32 AddLayer(s32 beginNodeIndex = -1, s32 endNodeIndex = -1)
			{
				s32 index = static_cast<s32>(animationLayers.size());
				animationLayers.emplace_back(AnimationLayer());
				animationLayers.at(index).beginNodeIndex = beginNodeIndex;
				animationLayers.at(index).endNodeIndex = endNodeIndex;

				return index;
			}

			/// <summary>
			/// アニメーションレイヤーにアニメーションステートを追加
			/// </summary>
			/// <param name="addAnimationIndex">   アニメーション番号 </param>
			/// <param name="animationLayerIndex"> アニメーションレイヤー番号 </param>
			/// <returns>                          アニメーションステート番号 </returns>
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
			/// アニメーションレイヤーにブレンドツリーを追加
			/// </summary>
			/// <param name="animationLayerIndex"> アニメーションレイヤー番号 </param>
			/// <returns>                          ブレンドツリー番号 </returns>
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
			/// ブレンドツリーにブレンドアニメーションステートを追加
			/// </summary>
			/// <param name="addAnimationIndex">   アニメーション番号 </param>
			/// <param name="plot">                 2次元上の位置(X範囲 : -1.0f ~ 1.0f, Y範囲 : -1.0f ~ 1.0f, Z範囲 : 0.0f) </param>
			/// <param name="animationLayerIndex"> アニメーションレイヤー番号 </param>
			/// <param name="blendTreeIndex">      ブレンドツリー番号 </param>
			/// <returns>                          ブレンドツリーのステート番号 </returns>
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
			/// 指定レイヤーのステートのアニメーション再生
			/// </summary>
			/// <param name="layerIndex"> アニメーションレイヤー番号 </param>
			/// <param name="stateIndex"> アニメーションステート番号 </param>
			/// <param name="clip">       アニメーションクリップ番号 </param>
			/// <param name="fadeTime">   このアニメーションにブレンドしていくスピード(ブレンドレート) </param>
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
			/// 指定レイヤーのブレンドツリーのアニメーション再生
			/// </summary>
			/// <param name="layerIndex"> アニメーションレイヤー番号 </param>
			/// <param name="stateIndex"> アニメーションステート番号 </param>
			/// <param name="clip">       アニメーションクリップ番号 </param>
			/// <param name="fadeTime">   このアニメーションにブレンドしていくスピード(ブレンドレート) </param>
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
			/// 指定レイヤーのステートのアニメーション同時再生
			/// </summary>
			/// <param name="layerIndex"> アニメーションレイヤー番号 </param>
			/// <param name="stateIndex"> アニメーションステート番号 </param>
			/// <param name="clip">       アニメーションクリップ番号 </param>
			/// <param name="fadeTime">   このアニメーションにブレンドしていくスピード(ブレンドレート) </param>
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
			/// 指定レイヤーのブレンドツリーのアニメーション同時再生
			/// </summary>
			/// <param name="layerIndex"> アニメーションレイヤー番号 </param>
			/// <param name="stateIndex"> アニメーションステート番号 </param>
			/// <param name="clip">       アニメーションクリップ番号 </param>
			/// <param name="fadeTime">   このアニメーションにブレンドしていくスピード(ブレンドレート) </param>
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

			// 一時停止/再開
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

			// 停止
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

			// ループ再生設定
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

			// ループ同時再生設定
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

			// 再生中
			bool IsPlaying()
			{
				return currentAnimationLayer->currentState->animation->player->IsPlaying();
			}

			// ループ再生か
			bool IsLoop()
			{
				return currentAnimationLayer->currentState->animation->player->IsLoop();
			}

			// 再生時間長(秒)取得
			f32 GetLength()
			{
				return currentAnimationLayer->currentState->animation->player->GetLength();
			}

			// 再生速度倍率設定
			void SetSpeed(f32 speed)
			{
				currentAnimationLayer->currentState->animation->player->SetSpeed(speed);
			}

			// 再生速度倍率取得
			f32 GetSpeed()
			{
				return currentAnimationLayer->currentState->animation->player->GetSpeed();
			}

			// ブレンド中か
			bool IsBlend()
			{
				return currentAnimationLayer->currentState->animation->player->IsBlend();
			}
			// 再生位置設定
			void SetCurrentTime(f32 seconds)
			{
				currentAnimationLayer->currentState->animation->player->SetCurrentTime(seconds);
			}

			// 再生位置取得
			f32 GetAnimCurrentTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetAnimCurrentTime();
			}

			// 残り再生時間取得
			f32 GetLastTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetLastTime();
			}

			// 再生開始位置設定
			void SetBeginTime(f32 seconds)
			{
				currentAnimationLayer->currentState->animation->player->SetBeginTime(seconds);
			}

			// 再生開始位置取得
			f32 GetBeginTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetBeginTime();
			}

			// 再生終了位置設定
			void SetEndTime(f32 seconds)
			{
				currentAnimationLayer->currentState->animation->player->SetEndTime(seconds);
			}

			// 再生終了位置取得
			f32 GetEndTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetEndTime();
			}

			// ループ再生開始位置設定
			void SetLoopBeginTime(f32 seconds)
			{
				currentAnimationLayer->currentState->animation->player->SetLoopBeginTime(seconds);
			}

			// ループ再生開始位置取得
			f32 GetLoopBeginTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetLoopBeginTime();
			}

			// ループ再生終了位置設定
			void SetLoopEndTime(f32 seconds)
			{
				currentAnimationLayer->currentState->animation->player->SetLoopEndTime(seconds);
			}

			// ループ再生終了位置取得
			f32 GetLoopEndTime()
			{
				return currentAnimationLayer->currentState->animation->player->GetLoopEndTime();
			}

			// ブレンドレートの設定
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

			//ブレンドレートの取得
			f32* GetBlendRateF1()
			{
				return &blendRate.x;
			}

			Math::Vector3* GetBlendRateF3()
			{
				return &blendRate;
			}

			// アニメーションバンクインデックス取得
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

								totalNode.scale     = Math::Vector3Lerp(currentNodeX.scale, currentNodeY.scale, fabsf(blendRate.y));			// 補間率にfabsf(blendRate.y)を入れることで
								totalNode.rotate    = Math::QuaternionSlerp(currentNodeX.rotate, currentNodeY.rotate, fabsf(blendRate.y));		// 大きければy軸にあるcurrentNodeYが優先されて、
								totalNode.translate = Math::Vector3Lerp(currentNodeX.translate, currentNodeY.translate, fabsf(blendRate.y));	// 小さければx軸にあるcurrentNodeXが優先される。
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

			// アニメーションノードとバインド
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

			// アニメーションクリップを検索
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