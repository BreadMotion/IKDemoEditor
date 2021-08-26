#pragma once

#include <memory>
#include <string>
#include <vector>
#include "Math/Color.h"
#include "Math/Quaternion.h"
#include "Math/Vector.h"
#include "OS/Resource.h"
#include "Graphics/Mesh.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// モデルデータ
		//****************************************************************************
		struct ModelData
		{
			enum class SkinningMode
			{
				None,
				Rigid,
				Smooth
			};

			struct Node
			{
				std::string              name;
				s32                        parentIndex;
				Math::Vector3       translate;
				Math::Quaternion rotate;
				Math::Vector3       scale;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			struct Material
			{
				std::string                         name;
				std::vector<std::string>    textureFilename;
				std::vector<Math::Color> color;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			struct Subset
			{
				u32 startIndex      = 0;
				u32 indexCount    = 0;
				u32 materialIndex = 0;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			struct Mesh
			{
				SkinningMode skinningMode;
				s32 nodeIndex;

				std::vector<Math::Vector3>       positions;
				std::vector<Math::Vector3>       normals;
				std::vector<Math::Vector3>       tangents;
				std::vector<Math::Color>           colors;
				std::vector<Math::Vector2>       texcoords0;
				std::vector<Math::Vector2>       texcoords1;
				std::vector<Math::Vector2>       texcoords2;
				std::vector<Math::Vector2>       texcoords3;
				std::vector<Math::Vector4>       blendWeights0;
				std::vector<Math::Vector4>       blendWeights1;
				std::vector<Math::Vector4_U8> blendIndices0;
				std::vector<Math::Vector4_U8> blendIndices1;
				std::vector<u16>                       indicesU16;
				std::vector<u32>                       indicesU32;
				std::vector<s32>                       nodeIndices;
				std::vector<Math::Matrix>        offsetTransforms;
				std::vector<Subset>                  subsets;

				template<class Archive>
				void serialize(Archive& archive, u32 version);
			};

			std::vector<Node>     nodes;
			std::vector<Material> materials;
			std::vector<Mesh>      meshes;

			template<class Archive>
			void serialize(Archive& archive, u32 version);

			// シリアライズ
			static void Serialize(const ModelData& data, const char* filename);

			// デシリアライズ
			static bool Deserialize(ModelData& data, const char* filename);
		};

		//****************************************************************************
		// モデルリソース操作インターフェース
		//****************************************************************************
		class IModelResource : public OS::Resource
		{
		public:
			// モデルデータ取得
			virtual const ModelData& GetModelData() = 0;

			// メッシュ取得
			virtual IMesh* GetMesh(sizeT index) = 0;

			// メッシュサイズ取得
			virtual sizeT GetMeshSize() = 0;
		};

		//****************************************************************************
		// モデルリソース操作オブジェクト生成インターフェース
		//****************************************************************************
		class IModelResourceFactory : public OS::IResourceFactory
		{
		public:
			// 生成
			static std::unique_ptr<IModelResourceFactory> Create(Graphics::IDevice* device);
		};
	} // namespace Graphics
} // namespace Bread