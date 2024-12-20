#pragma once

#include <memory>
#include <vector>
#include "Component.h"
#include "Graphics/Model.h"
#include "Graphics/Texture.h"
#include "Graphics/GraphicsDevice.h"
#include "Math/BreadMath.h"


namespace Bread
{
	namespace FrameWork
	{
		//class Animator;
		class Model : public Component
		{
		private:
			struct Node
			{
				const char*      name;
				Node*            parent;
				Math::Vector3    scale;
				Math::Quaternion rotate;
				Math::Vector3    translate;
				Math::Matrix     localTransform;
				Math::Matrix     worldTransform;
			};

			struct MeshNode
			{
				std::vector<Math::Matrix> boneTransform;
				u32 boneTransformCount = 0;

				MeshNode() {}
			};

			struct Material
			{
				std::string name;
				std::vector<std::unique_ptr<Graphics::ITexture>> textures;
				std::vector<Math::Color>                         colors;
			};

		private:
			std::shared_ptr<Graphics::IModelResource> modelResource;
			//std::unique_ptr<Animator> animator;
			std::vector<Node>                     nodes;
			std::vector<MeshNode>                 meshNodes;
			std::vector<Material>                 materials;
			std::unique_ptr<OS::IResourceManager> resourceManamger;
			std::unique_ptr<OS::IFileStream>      file;

			// ボーン名列挙用
			std::vector<const char*> boneNames;

		public:
			Model() {}
			~Model() {}

		public:
			// コンストラクタ
			void Construct() override;

			// デストラクタ
			void Destruct() override;

			// 初期化
			void Initialize() override;

			// 終了化
			void Finalize() override;

			//事前更新
			void __fastcall PreUpdate()override {}

			// 更新
			void __fastcall Update()override;

			//事前更新
			void __fastcall NextUpdate()override {}

		public:
			// モデルの読み込み
			void __fastcall Load(Graphics::IGraphicsDevice* graphicsDevice, const char* filename);
		};
	}
}