#include "pch.h"
#include "FND/Logger.h"
#include "FND/STD.h"
#include "FND/Util.h"
#include "OS/Path.h"
#include "Utility.h"
#include "ModelDataLoader.h"


namespace Bread
{
	namespace Loader
	{
		//****************************************************************************
		// ボーン影響力構造体
		//****************************************************************************
		struct BoneInfluence
		{
			s32		useCount = 0;
			s32		indices[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
			f32		weights[8] = { 1.0f, 0, 0, 0, 0, 0, 0, 0 };

			void Add(s32 index, f32 weight)
			{
				if (useCount < 8)
				{
					indices[useCount] = index;
					weights[useCount] = weight;
					useCount++;
				}
			}
		};

		//****************************************************************************
		// モデルデータローダー
		//****************************************************************************
		// モデルデータ読み込み
		bool ModelDataLoader::Load(FbxScene* fbxScene, Graphics::ModelData& data, const char* fbxFilename, const char* dirname)
		{
			FbxNode* fbxRootNode = fbxScene->GetRootNode();

			ProcessNodes(fbxRootNode, data, -1);

			ProcessMeshes(fbxRootNode, data);

			ProcessMaterials(fbxScene, data, dirname);

			return true;
		}

		// ノード処理
		void ModelDataLoader::ProcessNodes(FbxNode* fbxNode, Graphics::ModelData& data, s32 parentIndex)
		{
			FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();
			FbxNodeAttribute::EType fbxNodeAttributeType = FbxNodeAttribute::EType::eUnknown;

			if (fbxNodeAttribute != nullptr)
			{
				fbxNodeAttributeType = fbxNodeAttribute->GetAttributeType();
			}

			switch (fbxNodeAttributeType)
			{
			case FbxNodeAttribute::eUnknown:
			case FbxNodeAttribute::eNull:
			case FbxNodeAttribute::eMarker:
			case FbxNodeAttribute::eMesh:
			case FbxNodeAttribute::eSkeleton:
			{
				// ノード構築
				data.nodes.emplace_back(Graphics::ModelData::Node());

				ProcessNode(fbxNode, data.nodes.back(), parentIndex);
			}
			break;
			}

			// 再帰的に子ノードを処理する
			parentIndex = static_cast<s32>(data.nodes.size() - 1);
			for (int i = 0; i < fbxNode->GetChildCount(); ++i)
			{
				ProcessNodes(fbxNode->GetChild(i), data, parentIndex);
			}
		}

		// 解析
		void ModelDataLoader::ProcessNode(FbxNode* fbxNode, Graphics::ModelData::Node& node, s32 parentIndex)
		{
			node.name = fbxNode->GetName();
			node.parentIndex = parentIndex;

#if 0
			// スケール
			node.scale = Utility::FbxDoble3ToVector3(fbxNode->LclScaling.Get());

			// 回転
			// なぜか正しいローカル回転値を取得できないので封印
			FbxDouble3 fbxRotate = fbxNode->LclRotation.Get();
			Math::Quaternion rotateX, rotateY, rotateZ;
			Math::QuaternionRotationX(&rotateX, Math::Rad(static_cast<f32>(fbxRotate[0])));
			Math::QuaternionRotationY(&rotateY, Math::Rad(static_cast<f32>(fbxRotate[1])));
			Math::QuaternionRotationZ(&rotateZ, Math::Rad(static_cast<f32>(fbxRotate[2])));

			FbxEuler::EOrder fbxEulerOrder = fbxNode->RotationOrder.Get();
			switch (fbxEulerOrder)
			{
			case FbxEuler::EOrder::eOrderXYZ:
				node.rotate = rotateZ * rotateY * rotateX;
				break;
			case FbxEuler::EOrder::eOrderXZY:
				node.rotate = rotateY * rotateZ * rotateX;
				break;
			case FbxEuler::EOrder::eOrderYZX:
				node.rotate = rotateX * rotateZ * rotateY;
				break;
			case FbxEuler::EOrder::eOrderYXZ:
				node.rotate = rotateZ * rotateX * rotateY;
				break;
			case FbxEuler::EOrder::eOrderZXY:
				node.rotate = rotateY * rotateX * rotateZ;
				break;
			case FbxEuler::EOrder::eOrderZYX:
				node.rotate = rotateX * rotateY * rotateZ;
				break;
			case FbxEuler::EOrder::eOrderSphericXYZ:
				node.rotate = rotateZ * rotateY * rotateX;
				break;
			default:
				break;
			}

			// 移動
			node.translate = Utility::FbxDoble3ToVector3(fbxNode->LclTranslation.Get());

#elif 1
			FbxAMatrix& fbxLocalTransform = fbxNode->EvaluateLocalTransform();
			node.scale = Utility::FbxDoble4ToVector3(fbxLocalTransform.GetS());
			node.rotate = Utility::FbxDoble4ToQuaternion(fbxLocalTransform.GetQ());
			node.translate = Utility::FbxDoble4ToVector3(fbxLocalTransform.GetT());
#endif
		}

		// メッシュ処理
		void ModelDataLoader::ProcessMeshes(FbxNode* fbxNode, Graphics::ModelData& data)
		{
			FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();
			FbxNodeAttribute::EType fbxNodeAttributeType = FbxNodeAttribute::EType::eUnknown;

			if (fbxNodeAttribute != nullptr)
			{
				fbxNodeAttributeType = fbxNodeAttribute->GetAttributeType();
			}

			switch (fbxNodeAttributeType)
			{
			case FbxNodeAttribute::eMesh:
			{
				// メッシュ構築
				data.meshes.emplace_back(Graphics::ModelData::Mesh());

				ProcessMesh(static_cast<FbxMesh*>(fbxNodeAttribute), data.meshes.back(), data);

				// マテリアルを追加
				int fbxMaterialCount = fbxNode->GetMaterialCount();
				for (int i = 0; i < fbxMaterialCount; ++i)
				{
					FbxSurfaceMaterial* fbxSurfaceMaterial = fbxNode->GetMaterial(i);

					// 重複しないように追加する
					bool append = true;
					for (FbxSurfaceMaterial* p : fbxSurfaceMaterials)
					{
						if (p == fbxSurfaceMaterial)
						{
							append = false;
							break;
						}
					}
					if (append)
					{
						fbxSurfaceMaterials.emplace_back(fbxSurfaceMaterial);
					}
				}
			}
			break;
			}

			// 再帰的に子ノードを処理する
			for (int i = 0; i < fbxNode->GetChildCount(); ++i)
			{
				ProcessMeshes(fbxNode->GetChild(i), data);
			}
		}

		// メッシュ処理
		void ModelDataLoader::ProcessMesh(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh, Graphics::ModelData& data)
		{
			mesh.skinningMode = Graphics::ModelData::SkinningMode::None;
			mesh.nodeIndex = FindNodeIndex(data, fbxMesh->GetNode()->GetName());

			// 頂点座標
			ProcessVertexPositions(fbxMesh, mesh);

			// 頂点法線
			ProcessVertexNormals(fbxMesh, mesh);

			// 頂点接線
			ProcessVertexTangents(fbxMesh, mesh);

			// 頂点テクスチャ座標
			ProcessVertexTexcoords(fbxMesh, mesh);

			// 頂点カラー
			ProcessVertexColors(fbxMesh, mesh);

			// 頂点スキン
			ProcessVertexSkins(fbxMesh, mesh, data);

			// サブセット化
			ProcessSubsets(fbxMesh, mesh);

			// 頂点接線
			ComputeTangents(mesh);
		}

		// 頂点座標処理
		void ModelDataLoader::ProcessVertexPositions(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh)
		{
			int fbxControlPointsCount  = fbxMesh->GetControlPointsCount();
			int fbxPolygonVertexCount = fbxMesh->GetPolygonVertexCount();
			const int* fbxPolygonVertices = fbxMesh->GetPolygonVertices();

			//fbxVertexIndexint fbxPolygonCount = fbxMesh->GetPolygonCount();

			mesh.positions.resize(fbxPolygonVertexCount);

			const FbxVector4* fbxControlPoints = fbxMesh->GetControlPoints();
			for (int i = 0; i < fbxPolygonVertexCount; ++i)
			{
				mesh.positions[i] = Utility::FbxDoble4ToVector3(fbxControlPoints[fbxPolygonVertices[i]]);
			}
		}

		// レイヤーエレメントの頂点処理
		template<typename TFbxLayerElement, typename TFbxDouble, typename TVector>
		void ProcessLayerElement(
			FbxMesh* fbxMesh,
			TFbxLayerElement* fbxLayerElement,
			std::vector<TVector>& datas,
			void (function)(TVector& dstValue, const TFbxDouble& srcValue)
		)
		{
			// マッピングモードとリファレンスモードの内容によって頂点データの取り出し方が変わる。
			FbxLayerElement::EMappingMode	fbxMappingMode = fbxLayerElement->GetMappingMode();
			FbxLayerElement::EReferenceMode	fbxReferenceMode = fbxLayerElement->GetReferenceMode();

			auto&& fbxDirectArray = fbxLayerElement->GetDirectArray();

			if (fbxMappingMode == FbxLayerElement::eByControlPoint)
			{
				const int* fbxPolygonVertices = fbxMesh->GetPolygonVertices();
				int fbxPolygonVertexCount = fbxMesh->GetPolygonVertexCount();
				datas.resize(fbxPolygonVertexCount);

				if (fbxReferenceMode == FbxLayerElement::eDirect)
				{
					for (int i = 0; i < fbxPolygonVertexCount; ++i)
					{
						function(datas[i], fbxDirectArray.GetAt(fbxPolygonVertices[i]));
					}
				}
				else if (fbxReferenceMode == FbxLayerElement::eIndexToDirect)
				{
					FbxLayerElementArrayTemplate<int>& fbxIndexArray = fbxLayerElement->GetIndexArray();

					for (int i = 0; i < fbxPolygonVertexCount; ++i)
					{
						int fbxIndex = fbxIndexArray.GetAt(fbxPolygonVertices[i]);
						function(datas[i], fbxDirectArray.GetAt(fbxIndex));
					}
				}
			}
			else if (fbxMappingMode == FbxLayerElement::eByPolygonVertex)
			{
				if (fbxReferenceMode == FbxLayerElement::eDirect)
				{
					int fbxDirectArrayCount = fbxDirectArray.GetCount();
					datas.resize(fbxDirectArrayCount);

					for (int i = 0; i < fbxDirectArrayCount; ++i)
					{
						function(datas[i], fbxDirectArray.GetAt(i));
					}
				}
				else if (fbxReferenceMode == FbxLayerElement::eIndexToDirect)
				{
					FbxLayerElementArrayTemplate<int>& fbxIndexArray = fbxLayerElement->GetIndexArray();

					int fbxIndexArrayCount = fbxIndexArray.GetCount();
					datas.resize(fbxIndexArrayCount);

					for (int i = 0; i < fbxIndexArrayCount; ++i)
					{
						int fbxIndex = fbxIndexArray.GetAt(i);
						function(datas[i], fbxDirectArray.GetAt(fbxIndex));
					}
				}
			}
		}

		// 頂点法線処理
		void ModelDataLoader::ProcessVertexNormals(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh)
		{
			int fbxNormalLayerCount = fbxMesh->GetElementNormalCount();

			for (int fbxLayerIndex = 0; fbxLayerIndex < fbxNormalLayerCount; ++fbxLayerIndex)
			{
				FbxLayerElementNormal* fbxLayerNormal = fbxMesh->GetElementNormal(fbxLayerIndex);

				ProcessLayerElement<FbxLayerElementNormal, FbxDouble4, Math::Vector3>(fbxMesh, fbxLayerNormal, mesh.normals,
					[](Math::Vector3& dst, const FbxDouble4& src)
				{
					dst = Utility::FbxDoble4ToVector3(src);
				});
				break;	// 法線レイヤーは１つしか使わない
			}
		}

		// 頂点接線処理
		void ModelDataLoader::ProcessVertexTangents(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh)
		{
			int fbxTangentLayerCount = fbxMesh->GetElementTangentCount();

			for (int fbxLayerIndex = 0; fbxLayerIndex < fbxTangentLayerCount; ++fbxLayerIndex)
			{
				FbxLayerElementTangent* fbxLayerTangent = fbxMesh->GetElementTangent(fbxLayerIndex);

				ProcessLayerElement<FbxLayerElementTangent, FbxDouble4, Math::Vector3>(fbxMesh, fbxLayerTangent, mesh.tangents,
					[](Math::Vector3& dst, const FbxDouble4& src)
				{
					dst = Utility::FbxDoble4ToVector3(src);
				});

				break;	// 接線レイヤーは１つしか使わない
			}
		}

		// 頂点テクスチャ座標処理
		void ModelDataLoader::ProcessVertexTexcoords(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh)
		{
			std::vector<Math::Vector2>* texcoords[] =
			{
				&mesh.texcoords0,
				&mesh.texcoords1,
				&mesh.texcoords2,
				&mesh.texcoords3,
			};

			int fbxUvLayerCount = fbxMesh->GetElementUVCount();

			fbxUvLayerCount = FND::Min(fbxUvLayerCount, 4);
			for (int fbxLayerIndex = 0; fbxLayerIndex < fbxUvLayerCount; ++fbxLayerIndex)
			{
				FbxLayerElementUV* fbxLayerUv = fbxMesh->GetElementUV(fbxLayerIndex);

				//mesh.uvsetNames[fbxLayerIndex] = fbxLayerUv->GetName();

				ProcessLayerElement<FbxLayerElementUV, FbxDouble2, Math::Vector2>(fbxMesh, fbxLayerUv, *texcoords[fbxLayerIndex],
					[](Math::Vector2& dst, const FbxDouble2& src)
				{
					dst = Utility::FbxDoble2ToVector2(src);
				});
			}
		}

		// 頂点カラー処理
		void ModelDataLoader::ProcessVertexColors(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh)
		{
			int fbxColorLayerCount = fbxMesh->GetElementVertexColorCount();

			for (int fbxLayerIndex = 0; fbxLayerIndex < fbxColorLayerCount; ++fbxLayerIndex)
			{
				FbxLayerElementVertexColor* fbxLayerColor = fbxMesh->GetElementVertexColor(fbxLayerIndex);
				FbxLayerElementArrayTemplate<FbxColor>& fbxDirectArray = fbxLayerColor->GetDirectArray();
				ProcessLayerElement<FbxLayerElementVertexColor, FbxColor, Math::Color>(fbxMesh, fbxLayerColor, mesh.colors,
					[](Math::Color& dst, const FbxColor& src)
				{
					dst = Utility::FbxColorToColor(src);
				});

				break;	// カラーレイヤーは１つしか使わない
			}
		}

		// 頂点スキン処理
		void ModelDataLoader::ProcessVertexSkins(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh, Graphics::ModelData& data)
		{
			int fbxControlPointsCount = fbxMesh->GetControlPointsCount();

			int fbxDeformerCount = fbxMesh->GetDeformerCount(FbxDeformer::eSkin);
			if (fbxDeformerCount == 0)
			{
				return;
			}
			mesh.skinningMode = Graphics::ModelData::SkinningMode::Smooth;

			std::vector<BoneInfluence> boneInfluences;
			boneInfluences.resize(fbxControlPointsCount);

			FbxNode* fbxNode = fbxMesh->GetNode();
			FbxAMatrix fbxGeometryTransform(
				fbxNode->GetGeometricTranslation(FbxNode::eSourcePivot),
				fbxNode->GetGeometricRotation(FbxNode::eSourcePivot),
				fbxNode->GetGeometricScaling(FbxNode::eSourcePivot)
			);

			for (int fbxDeformerIndex = 0; fbxDeformerIndex < fbxDeformerCount; ++fbxDeformerIndex)
			{
				FbxSkin* fbxSkin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(fbxDeformerIndex));

				// 影響するボーンの数
				int fbxClusterCount = fbxSkin->GetClusterCount();

				for (int fbxClusterIndex = 0; fbxClusterIndex < fbxClusterCount; ++fbxClusterIndex)
				{
					FbxCluster* fbxCluster = fbxSkin->GetCluster(fbxClusterIndex);

					// 頂点影響力データを抽出する
					{
						// コントロールポイント毎のボーン影響力データを取り出す
						int fbxControl_pointIndicesCount = fbxCluster->GetControlPointIndicesCount();
						const int* fbxControl_pointIndices = fbxCluster->GetControlPointIndices();
						const double* fbxControl_point_weights = fbxCluster->GetControlPointWeights();

						for (int fbxControl_pointIndex = 0; fbxControl_pointIndex < fbxControl_pointIndicesCount; ++fbxControl_pointIndex)
						{
							BoneInfluence& boneInfluence = boneInfluences.at(fbxControl_pointIndices[fbxControl_pointIndex]);

							boneInfluence.Add(fbxClusterIndex, static_cast<f32>(fbxControl_point_weights[fbxControl_pointIndex]));
						}
					}

					// ボーン変換行列用のオフセット行列の計算をする
					{
						// メッシュ空間からワールド空間への変換行列
						FbxAMatrix fbxMeshSpaceTransform;
						fbxCluster->GetTransformMatrix(fbxMeshSpaceTransform);

						// ボーン空間からワールド空間への変換行列
						FbxAMatrix fbx_boneSpaceTransform;
						fbxCluster->GetTransformLinkMatrix(fbx_boneSpaceTransform);

						// オフセット行列を計算する
						FbxAMatrix fbxOffsetTransform = fbx_boneSpaceTransform.Inverse() * fbxMeshSpaceTransform * fbxGeometryTransform;

						Math::Matrix offsetTransform = Utility::FbxAMatrixToMatrix(fbxOffsetTransform);
						mesh.offsetTransforms.emplace_back(offsetTransform);

						// 影響するボーンインデックスを名前で検索する
						const char* fbxLink_name = fbxCluster->GetLink()->GetName();
						s32 nodeIndex = FindNodeIndex(data, fbxLink_name);
						mesh.nodeIndices.emplace_back(nodeIndex);
					}
				}
			}

			// コントロールポイント毎に取り出したボーン影響力データを頂点データに設定する
			int fbxPolygonVertexCount = fbxMesh->GetPolygonVertexCount();
			const int* fbxPolygonVertices = fbxMesh->GetPolygonVertices();

			mesh.blendIndices0.resize(fbxPolygonVertexCount);
			mesh.blendIndices1.resize(fbxPolygonVertexCount);
			mesh.blendWeights0.resize(fbxPolygonVertexCount);
			mesh.blendWeights1.resize(fbxPolygonVertexCount);
			for (int fbxPolygonVertexIndex = 0; fbxPolygonVertexIndex < fbxPolygonVertexCount; ++fbxPolygonVertexIndex)
			{
				const BoneInfluence& src = boneInfluences.at(fbxPolygonVertices[fbxPolygonVertexIndex]);
				// ブレンドウェイト
				{
					Math::Vector4& dst0 = mesh.blendWeights0[fbxPolygonVertexIndex];
					Math::Vector4& dst1 = mesh.blendWeights1[fbxPolygonVertexIndex];
					FND::MemSet(dst0, 0, sizeof(Math::Vector4));
					FND::MemSet(dst1, 0, sizeof(Math::Vector4));

#if 0
					s32 useCount = FND::Min(src.useCount, FND::ArraySize(dst.v)) - 1;
					f32 remain = 1.0f;
					if (useCount >= 0)
					{
						for (s32 useIndex = 0; useIndex < useCount; ++useIndex)
						{
							dst.v[useIndex] = src.weights[useIndex];
							remain -= src.weights[useIndex];
						}
						dst.v[useCount] = remain;
					}
#else
					f32 remain = 1.0f;
					for (s32 useIndex = 0; useIndex < 4; ++useIndex)
					{
						dst0.v[useIndex] = src.weights[0 + useIndex];
						dst1.v[useIndex] = src.weights[4 + useIndex];
						remain -= src.weights[0 + useIndex];
						remain -= src.weights[4 + useIndex];
					}
					dst1.v[3] = remain;
#endif
				}
				// ブレンドインデックス
				{
					Math::Vector4_U8& dst0 = mesh.blendIndices0[fbxPolygonVertexIndex];
					Math::Vector4_U8& dst1 = mesh.blendIndices1[fbxPolygonVertexIndex];
					FND::MemSet(&dst0, 0, sizeof(Math::Vector4_U8));
					FND::MemSet(&dst1, 0, sizeof(Math::Vector4_U8));
#if 0
					s32 use_count = FND::Min(src.use_count, FND::ArraySize(dst.v));
					for (s32 useIndex = 0; useIndex < use_count; ++useIndex)
					{
						dst.v[useIndex] = static_cast<u8>(src.indices[useIndex]);
					}
#else
					for (s32 useIndex = 0; useIndex < 4; ++useIndex)
					{
						dst0.v[useIndex] = static_cast<u8>(src.indices[0 + useIndex]);
						dst1.v[useIndex] = static_cast<u8>(src.indices[4 + useIndex]);
					}
#endif
				}
			}
		}

		// サブセット処理
		void ModelDataLoader::ProcessSubsets(FbxMesh* fbxMesh, Graphics::ModelData::Mesh& mesh)
		{
			FbxNode* fbxNode = fbxMesh->GetNode();
			int fbxPolygonCount = fbxMesh->GetPolygonCount();
			int fbxMaterialCount = fbxNode->GetMaterialCount();

			int indexCount = fbxPolygonCount * 3;
			if (indexCount > 0xFFFF)
			{
				mesh.indicesU32.resize(indexCount);
			}
			else
			{
				mesh.indicesU16.resize(indexCount);
			}
			mesh.subsets.resize(fbxMaterialCount > 0 ? fbxMaterialCount : 1);

			// サブセットのマテリアル設定
			for (int fbxMaterialIndex = 0; fbxMaterialIndex < fbxMaterialCount; ++fbxMaterialIndex)
			{
				FbxSurfaceMaterial* fbxSurfaceMaterial = fbxNode->GetMaterial(fbxMaterialIndex);

				Graphics::ModelData::Subset& subset = mesh.subsets.at(fbxMaterialIndex);
				subset.materialIndex = FindMaterialIndex(fbxNode->GetScene(), fbxSurfaceMaterial);
			}

			// サブセットの頂点インデックス開始位置設定
			if (fbxMaterialCount > 0)
			{
				// ポリゴン毎に設定されているマテリアルインデックスの配列
				FbxLayerElementArrayTemplate<int>& fbxIndexArray = fbxMesh->GetElementMaterial()->GetIndexArray();

				for (int fbxPolygonIndex = 0; fbxPolygonIndex < fbxPolygonCount; ++fbxPolygonIndex)
				{
					int fbxMaterialIndex = fbxIndexArray.GetAt(fbxPolygonIndex);
					mesh.subsets.at(fbxMaterialIndex).indexCount += 3;
				}

				int offset = 0;
				for (Graphics::ModelData::Subset& subset : mesh.subsets)
				{
					subset.startIndex = offset;
					offset += subset.indexCount;

					subset.indexCount = 0;
				}
			}

			// サブセットの頂点インデックス数とインデックスバッファを設定
			u32 vertexIndex = 0;
			for (int fbxPolygonIndex = 0; fbxPolygonIndex < fbxPolygonCount; ++fbxPolygonIndex)
			{
				// ポリゴンに適用されているマテリアルインデックスを取得する
				int fbxMaterialIndex = 0;
				if (fbxMaterialCount > 0)
				{
					fbxMaterialIndex = fbxMesh->GetElementMaterial()->GetIndexArray().GetAt(fbxPolygonIndex);
				}
				Graphics::ModelData::Subset& subset = mesh.subsets.at(fbxMaterialIndex);
				const int indexOffset = subset.startIndex + subset.indexCount;

				for (int fbx_vertexIndex = 0; fbx_vertexIndex < 3; ++fbx_vertexIndex)
				{
					if (indexCount > 0xFFFF)
					{
						mesh.indicesU32.at(indexOffset + fbx_vertexIndex) = vertexIndex++;
					}
					else
					{
						mesh.indicesU16.at(indexOffset + fbx_vertexIndex) = vertexIndex++;
					}
				}
				subset.indexCount += 3;
			}
		}

		// マテリアル処理
		void ModelDataLoader::ProcessMaterials(FbxScene* fbxScene, Graphics::ModelData& data, const char* dirname)
		{
			// 使用されているマテリアルのみ構築
			size_t materialCount = fbxSurfaceMaterials.size();
			data.materials.resize(materialCount > 0 ? materialCount : 1);

			// マテリアルの設定
			for (size_t i = 0; i < materialCount; ++i)
			{
				ProcessMaterial(fbxSurfaceMaterials[i], data.materials.at(i), dirname);
			}

			// メッシュに設定していたマテリアルインデックスはシーン全体のマテリアルのため、
			// 使用されているマテリアルのテーブル用にインデックスを振りなおす。
			for (Graphics::ModelData::Mesh& mesh : data.meshes)
			{
				for (Graphics::ModelData::Subset& subset : mesh.subsets)
				{
					FbxSurfaceMaterial* fbxSurfaceMaterial = fbxScene->GetMaterial(subset.materialIndex);
					for (size_t i = 0; i < materialCount; ++i)
					{
						if (fbxSurfaceMaterial == fbxSurfaceMaterials[i])
						{
							subset.materialIndex = static_cast<s32>(i);
							break;
						}
					}
				}
			}
		}

		// マテリアル処理
		void ModelDataLoader::ProcessMaterial(FbxSurfaceMaterial* fbxSurfaceMaterial, Graphics::ModelData::Material& material, const char* dirname)
		{
			FbxDouble3 fbxColor;
			FbxDouble fbxFactor;
			FbxFileTexture* fbxTexture = nullptr;

			material.name = fbxSurfaceMaterial->GetName();
			material.textureFilename.resize(6);
			material.color.resize(6);

			std::function<void(Graphics::ModelData::Material&, int, const char*, const char*)> FetchMaterial = [&](Graphics::ModelData::Material& material, int index, const char* propertyName, const char* factorName)
			{
				material.color.at(index) = Math::Color::White;

				const FbxProperty property = fbxSurfaceMaterial->FindProperty(propertyName);

				if (property.IsValid())
				{
					// Texture
					if (GetMaterialProperty(fbxSurfaceMaterial, propertyName, factorName, fbxColor, fbxTexture))
					{
						if (fbxTexture != nullptr)
						{
							material.textureFilename.at(index) = OS::Path::Combine(dirname, fbxTexture->GetRelativeFileName());
						}
						material.color.at(index).r = static_cast<f32>(fbxColor[0]);
						material.color.at(index).g = static_cast<f32>(fbxColor[1]);
						material.color.at(index).b = static_cast<f32>(fbxColor[2]);
					}
					// Transparency
					if (GetMaterialFactorProperty(fbxSurfaceMaterial, FbxSurfaceMaterial::sTransparencyFactor, fbxFactor))
					{
						if (fbxFactor > 0)
						{
							material.color.at(index).a = static_cast<f32>(fbxFactor);
						}
					}
				}
			};
			FetchMaterial(material, 0, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor);
			FetchMaterial(material, 1, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor);
			FetchMaterial(material, 2, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor);
			FetchMaterial(material, 3, FbxSurfaceMaterial::sNormalMap, FbxSurfaceMaterial::sBumpFactor);
			FetchMaterial(material, 4, FbxSurfaceMaterial::sBump, FbxSurfaceMaterial::sBumpFactor);
			FetchMaterial(material, 5, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor);
		}

		// マテリアルプロパティ取得
		bool ModelDataLoader::GetMaterialProperty(
				const FbxSurfaceMaterial* fbxSurfaceMaterial,
				const char* fbxColorPropertyName,
				const char* fbxFactorPropertyName,
				FbxDouble3& fbxColor,
				FbxFileTexture*& fbxTexture)
		{
			FbxProperty fbxProperty = fbxSurfaceMaterial->FindProperty(fbxColorPropertyName);

			if (GetMaterialProperty(fbxProperty, fbxColor, fbxTexture))
			{
				FbxDouble fbx_factor;
				if (GetMaterialFactorProperty(fbxSurfaceMaterial, fbxFactorPropertyName, fbx_factor))
				{
					fbxColor[0] *= fbx_factor;
					fbxColor[1] *= fbx_factor;
					fbxColor[2] *= fbx_factor;
				}
				return true;
			}

			return false;
		}

		bool ModelDataLoader::GetMaterialProperty(
			const FbxSurfaceMaterial* fbxSurfaceMaterial,
			const char* fbxColorPropertyName,
			FbxDouble3& fbxColor,
			FbxFileTexture*& fbxTexture)
		{
			FbxProperty fbxProperty = fbxSurfaceMaterial->FindProperty(fbxColorPropertyName);

			return GetMaterialProperty(fbxProperty, fbxColor, fbxTexture);
		}

		// マテリアルプロパティ取得
		bool ModelDataLoader::GetMaterialProperty(
			FbxProperty fbxProperty,
			FbxDouble3& fbxColor,
			FbxFileTexture*& fbxTexture)
		{
			if (fbxProperty.IsValid())
			{
				fbxColor = fbxProperty.Get<FbxDouble3>();

				s32 fbxTextureCount = fbxProperty.GetSrcObjectCount<FbxFileTexture>();
				if (fbxTextureCount > 0)
				{
					fbxTexture = fbxProperty.GetSrcObject<FbxFileTexture>();
				}
				else
				{
					fbxTexture = nullptr;
				}
				return true;
			}

			fbxColor[0] = 1.0f;
			fbxColor[1] = 1.0f;
			fbxColor[2] = 1.0f;
			fbxTexture = nullptr;

			return false;
		}

		// マテリアルファクタープロパティ取得
		bool ModelDataLoader::GetMaterialFactorProperty(
			const FbxSurfaceMaterial* fbxSurfaceMaterial,
			const char* fbx_factorPropertyName,
			FbxDouble& fbx_factor)
		{
			FbxProperty fbxProperty = fbxSurfaceMaterial->FindProperty(fbx_factorPropertyName);

			return GetMaterialFactorProperty(fbxProperty, fbx_factor);
		}

		// マテリアルファクタープロパティ取得
		bool ModelDataLoader::GetMaterialFactorProperty(
			FbxProperty fbxProperty,
			FbxDouble& fbx_factor)
		{
			if (fbxProperty.IsValid())
			{
				fbx_factor = fbxProperty.Get<FbxDouble>();
				return true;
			}

			fbx_factor = 1.0f;
			return false;
		}


		// 頂点接線を法線とUV座標から計算する
		void ModelDataLoader::ComputeTangents(Graphics::ModelData::Mesh& mesh)
		{
			if (mesh.tangents.size() != 0 ||
				mesh.positions.size() == 0 ||
				mesh.normals.size() == 0 ||
				mesh.texcoords0.size() == 0)
			{
				return;
			}
			if (mesh.indicesU16.size() == 0 &&
				mesh.indicesU32.size() == 0)
			{
				return;
			}

			u32 vertexCount = static_cast<u32>(mesh.normals.size());
			u32 indexCount = mesh.indicesU16.size() > 0
				? static_cast<u32>(mesh.indicesU16.size())
				: static_cast<u32>(mesh.indicesU32.size());

			std::unique_ptr<Math::Vector3[]> tan1 = std::make_unique<Math::Vector3[]>(vertexCount);
			FND::MemSet(tan1.get(), 0, sizeof(Math::Vector3) * vertexCount);

			for (u32 a = 0; a + 2 < indexCount; a += 3)
			{
				u32 i1, i2, i3;
				if (mesh.indicesU16.size() > 0)
				{
					i1 = mesh.indicesU16.at(a + 0);
					i2 = mesh.indicesU16.at(a + 1);
					i3 = mesh.indicesU16.at(a + 2);
				}
				else
				{
					i1 = mesh.indicesU32.at(a + 0);
					i2 = mesh.indicesU32.at(a + 1);
					i3 = mesh.indicesU32.at(a + 2);
				}

				const Math::Vector3& v1 = mesh.positions.at(i1);
				const Math::Vector3& v2 = mesh.positions.at(i2);
				const Math::Vector3& v3 = mesh.positions.at(i3);

				const Math::Vector2& w1 = mesh.texcoords0.at(i1);
				const Math::Vector2& w2 = mesh.texcoords0.at(i2);
				const Math::Vector2& w3 = mesh.texcoords0.at(i3);

				// position differences
				f32 x1 = v2.x - v1.x;
				f32 x2 = v3.x - v1.x;
				f32 y1 = v2.y - v1.y;
				f32 y2 = v3.y - v1.y;
				f32 z1 = v2.z - v1.z;
				f32 z2 = v3.z - v1.z;

				// texcoord differences
				f32 s1 = w2.x - w1.x;
				f32 s2 = w3.x - w1.x;
				f32 t1 = w2.y - w1.y;
				f32 t2 = w3.y - w1.y;

				// ratio
				f32 r = 1.0f / (s1 * t2 - s2 * t1);

				// vector in the s direction ( tangent )
				Math::Vector3 sdir = {
					(t2 * x1 - t1 * x2) * r,
					(t2 * y1 - t1 * y2) * r,
					(t2 * z1 - t1 * z2) * r
				};

				//accumulate tangents for each triangle
				tan1[i1] += sdir;
				tan1[i2] += sdir;
				tan1[i3] += sdir;
			}

			// loop through and ortho normalize the tangents
			mesh.tangents.resize(vertexCount);
			for (u32 a = 0; a < vertexCount; ++a)
			{
				Math::Vector3 n = mesh.normals[a];
				Math::Vector3 t = tan1[a];
				{
					// OrthoNormalize
					n = Math::Vector3Normalize(n);
					Math::Vector3 tmp = n * Math::Vector3Dot(n, t);
					t = Math::Vector3Normalize(t - tmp);
				}
				mesh.tangents[a] = t;
			}
		}

		// マテリアルインデックス検索
		s32 ModelDataLoader::FindMaterialIndex(FbxScene* fbxScene, FbxSurfaceMaterial* fbxSurfaceMaterial)
		{
			int fbxMaterialCount = fbxScene->GetMaterialCount();

			for (int i = 0; i < fbxMaterialCount; ++i)
			{
				if (fbxScene->GetMaterial(i) == fbxSurfaceMaterial)
				{
					return i;
				}
			}
			return -1;
		}

		// ノードインデックスを検索
		s32 ModelDataLoader::FindNodeIndex(const Graphics::ModelData& data, const char* name)
		{
			for (size_t i = 0; i < data.nodes.size(); ++i)
			{
				if (data.nodes[i].name == name)
				{
					return static_cast<s32>(i);
				}
			}
			return -1;
		}

	} // namespace Loader
} // namespace Bread