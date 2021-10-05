#pragma once

#include "Shader.h"


namespace Bread
{
	namespace FrameWork
	{
		class BasicShader : public IShader
		{
		private:
			struct CbMaterial
			{
				Bread::Math::Color color;
			};

		private:
			std::unique_ptr<Graphics::IShader> shader;
			std::unique_ptr<Graphics::IBuffer> cbMatrial;
			std::vector<Graphics::VertexBufferKind> vbKinds;

		public:
			BasicShader() {}
			~BasicShader() override { Finalize(); }

		public:
			// 生成
			static std::unique_ptr<IShader> Create();

			// 初期化
			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice) override;

			// 終了化
			void Finalize() override;

			// シェーダー開始
			void Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera) override;

			// シェーダー終了
			void End(Graphics::IGraphicsDevice* graphicsDevice) override;

			// 描画
			void Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model) override;

			// 頂点バッファ種類の取得
			Graphics::VertexBufferKind* GetVectexBuferKinds() override { return vbKinds.data(); }

			// 頂点バッファ種類のサイズ
			sizeT GetVectexBuferKindsSize() override { return vbKinds.size(); }
		};
	}
}