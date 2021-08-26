#pragma once

#include <memory>
#include <string>
#include "Graphics/Material.h"
#include "Graphics/Model.h"
#include "FrameWork/Renderer/Renderer.h"


namespace Bread
{
	namespace FrameWork
	{
		//****************************************************************************
		// ƒ‚ƒfƒ‹ƒŒƒ“ƒ_ƒ‰
		//****************************************************************************
		class ModelRenderer : public Renderer
		{
		public:
			ModelRenderer() {}
			~ModelRenderer() {}

		public:
			void Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera) override;
			void Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model, IShader* shader) override;
			void End(Graphics::IGraphicsDevice* graphicsDevice) override;
		};
	} // namespace FrameWork
} // namespace Bread