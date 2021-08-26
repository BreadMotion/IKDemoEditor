#pragma once

#include "Math/Color.h"
#include "Graphics/GraphicsDevice.h"
#include "FrameWork/Object/Object.h"
#include "Graphics/Camera.h"
#include "FrameWork/Shader/Shader.h"


namespace Bread
{
	namespace FrameWork
	{
		//****************************************************************************
		// ƒŒƒ“ƒ_ƒ‰Šî’ê
		//****************************************************************************
		class Renderer
		{
		public:
			Renderer() {}
			virtual ~Renderer() {}

		public:
			virtual void Begin(Graphics::IGraphicsDevice* graphicsDevice, const Graphics::Camera& camera) = 0;
			virtual void Draw(Graphics::IGraphicsDevice* graphicsDevice, const Math::Matrix& worldTransform, ModelObject* model, IShader* shader) = 0;
			virtual void End(Graphics::IGraphicsDevice* graphicsDevice) = 0;
		};
	} // namespace FrameWork
} // namespace Bread