#pragma once
#include <memory>
#include "FrameWork/FrameBuffer/FrameBuffer.h"
#include "FrameWork/Object/Object.h"
#include "Graphics/Camera.h"


namespace Bread
{
	namespace FrameWork
	{
		class IBL
		{
		private:
			static const int cameraSize = 6;

		private:
			std::unique_ptr<FrameBuffer>       skyIBL;
			std::unique_ptr<Graphics::IShader> shader;
			std::unique_ptr<Graphics::Camera>  captureCamera[cameraSize];

		public:
			IBL() {}
			~IBL() { Finalize(); }

		public:
			static std::unique_ptr<IBL> Create();

			void Initialize();

			void Finalize();

			void Clear(float r = 0, float g = 0, float b = 0, float a = 1);

			void Activate();

			void Deactivate();

			FrameBuffer* GetFrameBuffer() { return skyIBL.get(); }
		};

		class SkyMap
		{
		private:
			struct ShaderConstants
			{
				Math::Vector4 lightDirection;
				Math::Color   color;
			};

		private:
			std::shared_ptr<ModelObject>            model;
			std::unique_ptr<Graphics::ITexture>     texture;
			std::unique_ptr<Graphics::IShader>      shader;
			std::unique_ptr<Graphics::IBuffer>      constantBuffer;
			std::vector<Graphics::VertexBufferKind> vbKinds;

		public:
			SkyMap() {}
			~SkyMap() {}

		public:
			static std::unique_ptr<SkyMap> Create();

			void Initialize(const s8* cubemapFilename);

			void Finalize();

			void Draw(const Math::Matrix& world, const Graphics::Camera& camera, const Math::Vector4& lightDirection, const Math::Color& color);
		};
	}
}