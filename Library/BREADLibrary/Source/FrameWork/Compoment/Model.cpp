#include "pch.h"
#include "FrameWork/Component/Model.h"
#include "OS/Path.h"
#include "FND/STD.h"
#include "../Source/Loader/Loader.h"


namespace Bread
{
	namespace FrameWork
	{
		// コンストラクタ
		void Model::Construct()
		{

		}

		// デストラクタ
		void Model::Destruct()
		{

		}

		// 初期化
		void Model::Initialize()
		{

		}

		// 終了化
		void Model::Finalize()
		{

		}

		// 更新
		void Model::Update(const f32& dt)
		{

		}

		// モデルの読み込み
		void Model::Load(Graphics::IGraphicsDevice* graphicsDevice, const char* filename)
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

			modelResource = resourceManamger->LoadImmediate<Graphics::IModelResource>(modelFilename.c_str());
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

				dst.name      = src.name.c_str();
				dst.parent    = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
				dst.scale     = src.scale;
				dst.rotate    = src.rotate;
				dst.translate = src.translate;
			}

			const std::vector<Graphics::ModelData::Material>& resourceMaterials = modelResource->GetModelData().materials;
			materials.resize(resourceMaterials.size());

			for (sizeT i = 0; i < materials.size(); ++i)
			{
				Material& material = materials.at(i);
				material.name      = resourceMaterials.at(i).name;

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
				boneNames.at(i) = nodes.at(i).name;
			}
		}
	}
}