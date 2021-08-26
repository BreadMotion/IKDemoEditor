#pragma once

#include "Math/BreadMath.h"
#include "Graphics/Buffer.h"
#include "Graphics/Shader.h"
#include "Graphics/RenderState.h"
#include "Graphics/GraphicsDevice.h"
#include "Graphics/Texture.h"
#include "Graphics/Camera.h"
#include "FrameWork/FrameBuffer/FrameBuffer.h"

#include <d3d11.h>
#include <wrl.h>


namespace Bread
{
	namespace FrameWork
	{
		class Quad
		{
		private:
			struct vertex
			{
				Math::Vector3 position;
				Math::Vector2 texcoord;
				Math::Vector4 color;
			};

			struct FilterCB
			{
				f32 bright; //明度
				f32 contrast; //濃淡
				f32 saturate; //彩度
				f32 option;
				Math::Color screenColor; //色調
			};

			struct DissolveCB
			{
				f32 dissolveThreshold;        //透過閾値
				f32 dissolveEmissiveWidth;    //発光閾値(ディゾルブ・エミッシブ)
				f32 dummy[2];
			};

		private:
			Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
			std::unique_ptr<Graphics::IBuffer>          vertexBuffer;

			std::unique_ptr<Graphics::IShader> embeddedVertexShader;
			std::unique_ptr<Graphics::IShader> embeddedPixelShader[2];
			std::unique_ptr<Graphics::IShader> embeddedFilterPixelShader;
			std::unique_ptr<Graphics::IShader> embeddedAlphaCutOffPixelShader;
			std::unique_ptr<Graphics::IShader> embeddedDissolvePixelShader;

			std::unique_ptr<Graphics::IRasterizer>      embeddedRasterizerState;
			std::unique_ptr<Graphics::IDepthStencil> embeddedDepthStencilState;
			std::unique_ptr<Graphics::ISampler>        embeddedSamplerState;

			std::unique_ptr<Graphics::IBuffer> filterCB;
			std::unique_ptr<Graphics::IBuffer> dissolveCB;

			std::unique_ptr<Graphics::ITexture> dissolveTexture;
			std::unique_ptr<Graphics::ITexture> dissolveTexture02;
			std::unique_ptr<Graphics::ITexture> emissiveTexture;

			f32 bright    = 0.0f; //明度
			f32 contrast = 0.0f; //濃淡
			f32 saturate = 0.0f; //彩度
			Math::Color screenColor; //色調

			f32 dissolveThreshold        = 0.0f;     //透過閾値
			f32 dissolveEmissiveWidth = 0.0f;    //発光閾値(ディゾルブ・エミッシブ)

		public:
			Quad() {}
			 ~Quad() {}

		public:
			static std::unique_ptr<Quad> Create();

			bool Initialize
			(
				Graphics::IGraphicsDevice* graphicsDevice,
				Graphics::SamplerState samplerState = Graphics::SamplerState::AnisotropicBorder,
				Math::Color samplerBoarderColor = Math::Color::Black
			);

			void Finalize();

			void Draw
			(
				Graphics::IGraphicsDevice* graphicsDevice,
				Graphics::ITexture* shaderResourceView,
				float dx, float dy, float dw, float dh,
				float sx, float sy, float sw, float sh,
				float angle/*degree*/,
				float r, float g, float b, float a,
				bool useEmbeddedVertexShader = true,
				bool useEmbeddedPixelShader = true,
				bool useEmbeddedRasterizerState = true,
				bool useEmbeddedDepthStencilState = true,
				bool useEmbeddedSamplerState = true,
				bool useEmbeddedFileter = false,
				bool useEmbeddedDissolve = false,
				bool useEmbeddedDissolveEmissive = false
			) const;

			void Draw
			(
				Graphics::IGraphicsDevice* graphicsDevice,
				Graphics::ITexture* shaderResourceView,
				float dx, float dy, float dw, float dh,
				float angle = 0/*degree*/,
				float r = 1, float g = 1, float b = 1, float a = 1,
				bool useEmbeddedVertexShader = true,
				bool useEmbeddedPixelShader = true,
				bool useEmbeddedRasterizerState = true,
				bool useEmbeddedDepthStencilState = true,
				bool useEmbeddedSamplerState = true,
				bool useEmbeddedFileter = false,
				bool useEmbeddedDissolve = false,
				bool useEmbeddedDissolveEmissive = false
			) const;

			void Draw
			(
				Graphics::IGraphicsDevice* graphicsDevice,
				Graphics::ITexture* shaderResourceView,
				Bread::Math::Vector2 pos, Bread::Math::Vector2 size,
				Bread::Math::Vector2 texPos, Bread::Math::Vector2 texSize,
				float angle = 0/*degree*/,
				float r = 1, float g = 1, float b = 1, float a = 1,
				bool useEmbeddedVertexShader = true,
				bool useEmbeddedPixelShader = true,
				bool useEmbeddedRasterizerState = true,
				bool useEmbeddedDepthStencilState = true,
				bool useEmbeddedSamplerState = true,
				bool useEmbeddedFileter = false,
				bool useEmbeddedDissolve = false,
				bool useEmbeddedDissolveEmissive = false
			) const;

			void SetBright(f32 bright) { this->bright = bright; }
			void SetContrast(f32 contrast) { this->contrast = contrast; }
			void SetSaturate(f32 saturate) { this->saturate = saturate; }
			void SetScreenColor(Math::Color screenColor) { this->screenColor = screenColor; }

			void SetDissolveThreshold(f32 threshold) { dissolveThreshold = threshold; }
			void SetDissolveEmissiveWidth(f32 emissiveWidth) { dissolveEmissiveWidth = emissiveWidth; }

			void LoadDissolveTexture(Graphics::IGraphicsDevice* graphicsDevice, const s8* textureFileName) { dissolveTexture->Initialize(graphicsDevice->GetDevice(), textureFileName, Bread::Graphics::MaterialType::Diffuse, Bread::Math::Color::White); }
			void LoadDissolveTexture02(Graphics::IGraphicsDevice* graphicsDevice, const s8* textureFileName) { dissolveTexture02->Initialize(graphicsDevice->GetDevice(), textureFileName, Bread::Graphics::MaterialType::Diffuse, Bread::Math::Color::White); }
			void LoadEmissiveTexture(Graphics::IGraphicsDevice* graphicsDevice, const s8* textureFileName) { emissiveTexture->Initialize(graphicsDevice->GetDevice(), textureFileName, Bread::Graphics::MaterialType::Diffuse, Bread::Math::Color::White); }
		};

		class FullScreenQuad
		{
		private:
			std::unique_ptr<Graphics::IShader> embeddedVertexShader;
			std::unique_ptr<Graphics::IShader> embeddedPixelShader[2];

			std::unique_ptr<Graphics::IRasterizer>     embeddedRasterizerState;
			std::unique_ptr<Graphics::IDepthStencil> embeddedDepthStencilState;

		public:
			FullScreenQuad() {}
			virtual ~FullScreenQuad() { Finalize(); }

		public:
			static std::unique_ptr<FullScreenQuad> Create();

			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			void Finalize();

			void Draw
			(
				Graphics::IGraphicsDevice* graphicsDevice,
				bool useEmbeddedRasterizerState = true,
				bool useEmbeddedDepthStencilState = true,
				bool useEmbeddedPixelShader = false,
				bool enableMSAA = false
			);
		};

		class Bloom : public FullScreenQuad
		{
		private:
			enum { LINEAR_BORDER, POINT, LINEAR, ANISOTROPIC };

			struct ShaderConstants
			{
				float glowExtractionThreshold = 1.02f; // 0.85f
				float blurConvolutionIntensity = 0.22f; // 0.06f
				float lensFlareThreshold = 1.000f;
				float lensFlareGhostDispersal = 0.300f; //dispersion factor
				int numberOfGhosts = 6;
				float lensFlareIntensity = 0.35f;
				int options[2] = { 1 };
			};

		private:
			// if you change value of 'number_of_downsampled', you must change 'number_of_downsampled' in bloom.hlsli to this same value.
			const static u32 numberOfDownsampled = 6;

		public:
			ShaderConstants shaderContants;

		private:
			std::unique_ptr<Graphics::ISampler> samplerState[4];

			std::unique_ptr<Graphics::IBlend> blendState;

			std::unique_ptr<Graphics::IShader> glowExtractionPS;
			std::unique_ptr<Graphics::IShader> gaussianBlurHorizontalPS;
			std::unique_ptr<Graphics::IShader> gaussianBlurVerticalPS;
			std::unique_ptr<Graphics::IShader> gaussianBlurConvolutionPS;
			std::unique_ptr<Graphics::IShader> gaussianBlurDownsamplingPS;
			std::unique_ptr<Graphics::IShader> bloomBlendPS;

			std::unique_ptr<Graphics::IShader> lensFlarePS;
			std::unique_ptr<Graphics::ITexture> gradientMap;
			std::unique_ptr<Graphics::ITexture> noiseMap;

			std::unique_ptr<Graphics::IBuffer> constantBuffer;

			std::unique_ptr<FrameBuffer> glowExtraction;
			std::unique_ptr<FrameBuffer> gaussianBlur[numberOfDownsampled][2];
			std::unique_ptr<FrameBuffer> lensFlare;

			std::unique_ptr<FullScreenQuad> fullScreenQuad;

		public:
			Bloom() : FullScreenQuad() {}
			~Bloom() {}

		public:
			static std::unique_ptr<Bloom> Create();

			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice, u32 width, u32 height);

			void Finalize();

			void Generate(Graphics::IGraphicsDevice* graphicsDevice, Graphics::ITexture* hdrTexture, bool enableLensFlare);

			void Draw(Graphics::IGraphicsDevice* graphicsDevice);

			void Blend(Graphics::IGraphicsDevice* graphicsDevice, Graphics::ITexture* originTexture, Graphics::ITexture* bloomTexture);
		};

		class PostProcessingEffects : public FullScreenQuad
		{
		private:
			struct ShadowMapConstants
			{
				Math::Vector3 color = Math::Vector3(0.65f, 0.65f, 0.65f);
				f32 bias = 0.0008f;
			};

			struct ShaderConstants
			{
				Math::Matrix lightViewProjection     = Math::Matrix::Zero;
				Math::Matrix inverseViewProjection = Math::Matrix::Zero;
				ShadowMapConstants shadowMap;
			};

		private:
			std::unique_ptr<Graphics::IShader> postProcessingEffectsPS;
			std::unique_ptr<Graphics::IBuffer> shaderConstantsBuffer;
			std::unique_ptr<Graphics::ISampler> comparisonSamplerState;

		public:
			ShaderConstants shaderContexts;

		public:
			PostProcessingEffects() : FullScreenQuad() {}
			~PostProcessingEffects() {}

		public:
			static std::unique_ptr<PostProcessingEffects> Create();

			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice);

			void Finalize();

			void Draw(Graphics::IGraphicsDevice* graphicsDevice, Graphics::ITexture* colorTexture, Graphics::ITexture* depthTexture, Graphics::ITexture* shadowTexture, const Math::Matrix& lightViewProjection, const Math::Matrix& inverseViewProjection);
		};

		class ToneMap : public FullScreenQuad
		{
		private:
			struct ShaderConstants
			{
				float whitePoint = 1.5f; //The luminance level to use as the upper end of a tone mapping curve.
				float averageGray = 1.137f; //The luminance level to use as the midpoint of a tone mapping curve.

				//Brightness - Contrast Effect
				//The brightness - contrast effect allows you to modify the brightness and contrast of the rendered image.
				//Brightness: The brighness of the image.Ranges from - 1 to 1 (-1 is solid black, 0 no change, 1 solid white).
				//Contrast : The contrast of the image.Ranges from - 1 to 1 (-1 is solid gray, 0 no change, 1 maximum contrast).
				float brightness = 0.0f;
				float contrast = 0.0f;

				//Hue - Saturation Effect
				//The hue - saturation effect allows you to modify the hue and saturation of the rendered image.
				//Hue: The hue of the image.Ranges from - 1 to 1 (-1 is 180 degrees in the negative direction, 0 no change, 1 is 180 degrees in the postitive direction).
				//Saturation : The saturation of the image.Ranges from - 1 to 1 (-1 is solid gray, 0 no change, 1 maximum saturation).
				float hue = 0.0f;
				float saturation = 0.0f;

				//Sepia Effect
				//The Sepia effect makes the image look like an old photograph.
				//Amount: Controls the intensity of the effect. Ranges from 0 to 1.
				float amount = 0.0f;

				//Vignette Effect
				//In photography and optics, vignetting is a reduction of an image's brightness or saturation at the periphery compared to the image center.
				//You can use it to draw attention to the center of the frame. (from Wikipedia)
				//Offset: Controls the offset of the effect.
				//Darkness : Controls the darkness of the effect.
				float offset = 0.0f;
				float darkness = 1.0f;

				float options[3] = {};
			};

		private:
			std::unique_ptr<FrameBuffer> averageLuminance;

			std::unique_ptr<Graphics::IBuffer> shaderConstantsBuffer;

			std::unique_ptr<Graphics::IShader> averageLuminancePS;
			std::unique_ptr<Graphics::IShader> toneMapPS;

			Graphics::Viewport viewport = {};

			float cumulativeTime = 0;

		public:
			ShaderConstants shaderConstant;

		public:
			ToneMap() : FullScreenQuad() {}
			~ToneMap() {}

		public:
			static std::unique_ptr<ToneMap> Create();

			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice, u32 width, u32 height);

			void Finalize();

			void Draw(Graphics::IGraphicsDevice* graphicsDevice, Graphics::ITexture* colorTexture, f32 elapsedTime);
		};

		class MotionBlur : public FullScreenQuad
		{
		private:
			enum { LINEAR_BORDER, POINT, LINEAR, ANISOTROPIC };

			struct VelocityConstants
			{
				f32 exposureTime = 480.0f;
				f32 screenWidth = 1280.0f;
				f32 screenHeight = 720.0f;
				f32 frameRate = 0.01666667f; // 1.0f / 60.0f
			};

			struct BlurConstants
			{
			    u32 loop = 5;
			    f32 softZExtent = 0.0f;
			    f32 div = 0.166666667f;	// 1.0 / (loop + 1)
			    s32 jitter = 1;			// ジッタのON/OFF
			    s32 mix = 1;			// 速度のミックスを行うかどうか
			    Math::Vector3 padding = Math::Vector3::OneAll;
			};

			const f32 kBlockK = 20.0f;

		public:
			VelocityConstants velocityConstants;
			BlurConstants blurConstants;

		private:
			std::unique_ptr<Graphics::ISampler> samplerState[4];

			std::unique_ptr<Graphics::IBlend> blendState;

			std::unique_ptr<Graphics::IShader> velocityPS;
			std::unique_ptr<Graphics::IShader> normalBlurPS;
			std::unique_ptr<Graphics::IShader> tileMaxPS;
			std::unique_ptr<Graphics::IShader> neighborMaxPS;
			std::unique_ptr<Graphics::IShader> reconstructionBlurPS;

			std::unique_ptr<FrameBuffer> velocity;
			std::unique_ptr<FrameBuffer> depth;
			std::unique_ptr<FrameBuffer> titleMax;
			std::unique_ptr<FrameBuffer> neighborMax;

			std::unique_ptr<Graphics::IBuffer> velocityConstantsBuffer;
			std::unique_ptr<Graphics::IBuffer> blurConstantsBuffer;

		public:
			MotionBlur() : FullScreenQuad() {}
			~MotionBlur() {}

		public:
			static std::unique_ptr<MotionBlur> Create();

			bool Initialize(Graphics::IGraphicsDevice* graphicsDevice, u32 width, u32 height);

			void Finalize();

			void ActivateVelocity(Graphics::IGraphicsDevice* graphicsDevice);
			void DeactivateVelocity(Graphics::IGraphicsDevice* graphicsDevice);

			void ActivateVelocityPS(Graphics::IGraphicsDevice* graphicsDevice);
			void DeactivateVelocityPS(Graphics::IGraphicsDevice* graphicsDevice);

			void Draw(Graphics::IGraphicsDevice* graphicsDevice, Graphics::ITexture* originTexture, const Graphics::Camera& camera, bool isNormal);

			FrameBuffer* GetVelocityFrameBuffer() { return velocity.get(); }
		};
	}
}