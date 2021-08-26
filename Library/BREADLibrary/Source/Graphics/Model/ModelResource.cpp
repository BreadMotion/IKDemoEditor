#include "pch.h"
#include <fstream>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/polymorphic.hpp>
#include "FND/Logger.h"
#include "OS/Path.h"
#include "ModelResource.h"


CEREAL_CLASS_VERSION(Bread::Graphics::ModelData,                1)
CEREAL_CLASS_VERSION(Bread::Graphics::ModelData::Node,      1)
CEREAL_CLASS_VERSION(Bread::Graphics::ModelData::Material,  1)
CEREAL_CLASS_VERSION(Bread::Graphics::ModelData::Subset,    1)
CEREAL_CLASS_VERSION(Bread::Graphics::ModelData::Mesh,      1)


namespace Bread
{
	namespace Graphics
	{
		template<class Archive>
		void ModelData::Node::serialize(Archive& archive, u32 version)
		{
			archive(
				CEREAL_NVP(name),
				CEREAL_NVP(parentIndex),
				CEREAL_NVP(translate),
				CEREAL_NVP(rotate),
				CEREAL_NVP(scale)
			);
		}

		template<class Archive>
		void ModelData::Material::serialize(Archive& archive, u32 version)
		{
			archive(
				CEREAL_NVP(name),
				CEREAL_NVP(textureFilename),
				CEREAL_NVP(color)
			);
		}

		template<class Archive>
		void ModelData::Subset::serialize(Archive& archive, u32 version)
		{
			archive(
				CEREAL_NVP(startIndex),
				CEREAL_NVP(indexCount),
				CEREAL_NVP(materialIndex)
			);
		}

		template<class Archive>
		void ModelData::Mesh::serialize(Archive& archive, u32 version)
		{
			archive(
				CEREAL_NVP(skinningMode),
				CEREAL_NVP(nodeIndex),
				CEREAL_NVP(positions),
				CEREAL_NVP(normals),
				CEREAL_NVP(tangents),
				CEREAL_NVP(colors),
				CEREAL_NVP(texcoords0),
				CEREAL_NVP(texcoords1),
				CEREAL_NVP(texcoords2),
				CEREAL_NVP(texcoords3),
				CEREAL_NVP(blendWeights0),
				CEREAL_NVP(blendWeights1),
				CEREAL_NVP(blendIndices0),
				CEREAL_NVP(blendIndices1),
				CEREAL_NVP(indicesU16),
				CEREAL_NVP(indicesU32),
				CEREAL_NVP(nodeIndices),
				CEREAL_NVP(offsetTransforms),
				CEREAL_NVP(subsets)
			);
		}

		template<class Archive>
		void ModelData::serialize(Archive& archive, u32 version)
		{
			archive(
				CEREAL_NVP(nodes),
				CEREAL_NVP(materials),
				CEREAL_NVP(meshes)
			);
		}

		// シリアライズ
		void ModelData::Serialize(const ModelData& model, const char* filename)
		{
			// バイナリ
			{
				std::ofstream stream(filename, std::ios::binary);
				if (stream.is_open())
				{
					cereal::BinaryOutputArchive archive(stream);

					archive(CEREAL_NVP(model));
				}
			}
			// JSON
			{
				std::ofstream stream(OS::Path::ChangeFileExtension(filename, "json"));
				if (stream.is_open())
				{
					cereal::JSONOutputArchive archive(stream);

					archive(CEREAL_NVP(model));
				}
			}
		}

		// デシリアライズ
		bool ModelData::Deserialize(ModelData& model, const char* filename)
		{
			std::ifstream stream(filename, std::ios::binary);
			if (stream.is_open())
			{
				cereal::BinaryInputArchive archive(stream);

				archive(CEREAL_NVP(model));
			}
			else
			{
				// バイナリファイルがなかった場合はJSONファイルを開く
				stream.open(OS::Path::ChangeFileExtension(filename, "json"));
				if (stream.is_open())
				{
					cereal::JSONInputArchive archive(stream);

					archive(CEREAL_NVP(model));
				}
				else
				{
					BREAD_LOG_GRP_ERROR("not found : %s\n", filename);
					return false;
				}
			}

			return true;
		}

		//****************************************************************************
		// モデルリソースクラス
		//****************************************************************************
		// 初期化
		bool ModelResource::Initialize(Graphics::IDevice* device, const char* filename)
		{
			// デシリアライズ
			if (!ModelData::Deserialize(data, filename))
			{
				return false;
			}

			// メッシュ作成
			for (auto meshData : data.meshes)
			{
				MeshDesc desc{};
				desc.primitiveTopology = PrimitiveTopology::TriangleList;
				desc.vertexCount           = static_cast<u32>(meshData.positions.size());
				if (meshData.indicesU16.size() > 0)
				{
					desc.indexCount = static_cast<u32>(meshData.indicesU16.size());
					desc.indicesU16  = meshData.indicesU16.data();
				}
				else if (meshData.indicesU32.size() > 0)
				{
					desc.indexCount = static_cast<u32>(meshData.indicesU32.size());
					desc.indicesU32  = meshData.indicesU32.data();
				}

				if (meshData.positions.size() > 0)
				{
					desc.positions = meshData.positions.data();
				}
				if (meshData.normals.size() > 0)
				{
					desc.normals = meshData.normals.data();
				}
				if (meshData.tangents.size() > 0)
				{
					desc.tangents = meshData.tangents.data();
				}
				if (meshData.colors.size() > 0)
				{
					desc.colors = meshData.colors.data();
				}
				if (meshData.texcoords0.size() > 0)
				{
					desc.texcoords0 = meshData.texcoords0.data();
				}
				if (meshData.texcoords1.size() > 0)
				{
					desc.texcoords1 = meshData.texcoords1.data();
				}
				if (meshData.texcoords2.size() > 0)
				{
					desc.texcoords2 = meshData.texcoords2.data();
				}
				if (meshData.texcoords3.size() > 0)
				{
					desc.texcoords3 = meshData.texcoords3.data();
				}
				if (meshData.blendWeights0.size() > 0)
				{
					desc.blendWeights0 = meshData.blendWeights0.data();
				}
				if (meshData.blendWeights1.size() > 0)
				{
					desc.blendWeights1 = meshData.blendWeights1.data();
				}
				if (meshData.blendIndices0.size() > 0)
				{
					desc.blendIndices0 = meshData.blendIndices0.data();
				}
				if (meshData.blendIndices1.size() > 0)
				{
					desc.blendIndices1 = meshData.blendIndices1.data();
				}

				std::unique_ptr<IMesh> mesh = IMesh::Create();
				if (!mesh->Initialize(device, desc))
				{
					return false;
				}
				meshes.emplace_back(std::move(mesh));
			}

			return true;
		}

		//============================================================================
		// スタティックメッシュリソースファクトリクラス
		//----------------------------------------------------------------------------
		// 生成
		std::unique_ptr<IModelResourceFactory> IModelResourceFactory::Create(Graphics::IDevice* device)
		{
			return std::make_unique<ModelResourceFactory>(device);
		}

		// リソース作成
		std::unique_ptr<OS::Resource> ModelResourceFactory::CreateResource(u32 type)
		{
			return std::make_unique<ModelResource>();
		}

		// リソース読み込み
		bool ModelResourceFactory::LoadResource(OS::Resource* resource, OS::IFileStream* stream, const char* filename)
		{
			ModelResource* modelResource = static_cast<ModelResource*>(resource);
			return modelResource->Initialize(device, filename);
		}

	}	// namespace grp
}	// namespace ys