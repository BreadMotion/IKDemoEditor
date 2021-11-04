#pragma once
#include <memory>
#include <vector>
#include <map>
#include <string>

#include "FND/Base.h"
#include "FrameWork/Shader/Shader.h"
#include "FrameWork\Shader\BasicShader.h"
#include "FrameWork\Shader\BasicSkinShader.h"
#include "FrameWork\Shader\StandardShader.h"
#include "FrameWork\Shader\PBRShader.h"

#include "FrameWork\Quad/Quad.h"
#include "FrameWork\IBL/IBL.h"
#include "FrameWork\ComputeShader/TestComputeShader.h"
#include "FrameWork\COmputeShader/BitonicSort.h"
#include "FrameWork\ComputeShader/GPUParticle.h"

namespace Bread
{
	namespace Graphics
	{
		//使用されるShaderのキーはこの変数から使用する
		namespace ShaderNameVal
		{
			static const char* basicShader    { "basicShader"     };
			static const char* basicSkinShader{ "basicSkinShader" };
			static const char* standardShader { "standardShader"  };
			static const char* pbrShader      { "pbrShader"       };
			static const char* pbrSkinShader  { "pbrSkinShader"   };
		}

		/// <summary>
		///使用されてるシングルトン
		/// <para> Instance       - ActorManager      </para>
		/// <para> SharedInstance - GraphicsDeviceDx11</para>
		/// <para> UniqueInstance - DisplayWin        </para>
		/// </summary>
		class RenderManager : public FND::Base
		{
			using ShaderName = std::string;

		private:
			struct ShaderConstants
			{
				Math::Matrix   lightViewProjection{};
				Math::Vector3  color{ Bread::Math::Vector3() };
				f32            bias{ 0.0008f };
			};

		private:
			//シーン内オブジェクト
			std::map<ShaderName, std::shared_ptr<FrameWork::IShader>> shaders;
			std::shared_ptr<Graphics::Camera>                         screenSpaceCamera;

			//フレームバッファー
			std::unique_ptr< FrameWork::FrameBuffer> frameBuffer[6];

			//ポストプロセス
			std::unique_ptr<Bread::FrameWork::PostProcessingEffects> postProcessingEffects;

			//シャドウマップ
			std::unique_ptr < Graphics::Camera>       lightSpaceCamera;
			std::unique_ptr < FrameWork::FrameBuffer> shadowMap;
			std::unique_ptr < Graphics::IShader>      voidPS;
			std::unique_ptr < Graphics::IBuffer>      shaderConstantsBuffer;
			std::unique_ptr < Graphics::ISampler>     comparisonSampler;
			ShaderConstants shaderContexts;

			//モーションブラー
			bool isMotionBlur{ true };
			std::unique_ptr<FrameWork::MotionBlur>  motionBlur;

			//ブルーム
			std::unique_ptr<FrameWork::Quad>        quad;
			std::unique_ptr<FrameWork::MSAAResolve> msaaResolve;
			std::unique_ptr<FrameWork::Bloom>       bloom;

			//スカイマップ
			std::unique_ptr<FrameWork::SkyMap>      skyMap;

			//トーンマップ
			std::unique_ptr<FrameWork::ToneMap>     toneMap;

			//IBL
			std::unique_ptr<FrameWork::FrameBuffer> skyFrameBuffer;
			std::unique_ptr<FrameWork::IBL>         ibl;

			// フェード
			bool onFade{ false };
			std::unique_ptr<Graphics::ITexture> fadeTexture;
			f32 roundFadeAlpha = 0.0f;

			//スクリーンフィルター
			f32 bright   { 0.1f}; //明度
			f32 contrast { 0.9f}; //濃淡
			f32 saturate { 1.1f}; //彩度
			Math::Color screenColor{ Math::Color::White };

		private:
			Math::Vector2 texSize;

			Math::Vector3 tempCameraFouce{ Math::Vector3::Zero };
			f32 sphereLinearSpeed{ 0.0f };
			f32 distanceToFouceFromCamera{ 0.0f };

			std::unique_ptr<FrameWork::IComputeShader> testComputeShader;
			std::unique_ptr<FrameWork::BitonicSort>    bitonicSort;
			std::unique_ptr<FrameWork::GPUParticle>    gpuParticle;

			f32 dis   { 10000.0f };
			f32 width { 10000.0f };
			f32 height{ 10000.0f };
			f32 nearZ { 1.0f };
			f32 farZ  { 10000.0f };

			bool isHitCollision{ true };
			bool enableMSAA    { true };
			bool bloomBlend    { true };

		public:
			RenderManager() = default;
			~RenderManager() {}

		public:
			//初期化
			void Initialize();

			//描画
			void Render();

		public:
			//モデル描画用のShader(FlyWeight)
			RenderManager& RegisterModelRenderShader
			(const std::string& str,
				std::shared_ptr<FrameWork::IShader> shader);

		private:
			//---------------   Actor Render Function   ---------------//
			void RenderActor(const std::string& shaderName);
			void RenderChildActor(const std::string& shaderName, std::shared_ptr<FrameWork::Actor>childAct);
			////////////////////////////////////////////////////////
			//---------------MotionBlur Render Function---------------//
			void RenderSkyMap();
			////////////////////////////////////////////////////////

			//---------------MotionBlur Render Function---------------//
			void RenderCollisionPrimitive(Graphics::IContext* context);
			////////////////////////////////////////////////////////


			//---------------Normal Render Function---------------//
			void RenderNomal();
			void RenderObjectNormal(const std::string& shaderName);
			////////////////////////////////////////////////////////

			//---------------shadow Render Function---------------//
			void GenerateShadows();
			void RenderObjectShadow(const std::string& shaderName);
			////////////////////////////////////////////////////////

			//---------------MotionBlur Render Function---------------//
			void GenerateMotionBlur();
			void RenderObjectMotionBlur(const std::string& shaderName);
			////////////////////////////////////////////////////////

			void UpdateLightDirection();
		};
	}//namespace Graphics
}//namespace Bread