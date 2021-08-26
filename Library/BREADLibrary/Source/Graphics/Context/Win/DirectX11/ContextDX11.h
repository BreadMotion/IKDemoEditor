#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <assert.h>
#include "Graphics/Context.h"
#include "Graphics/Surface.h"
#include "Graphics/Viewport.h"
#include "Graphics/Buffer.h"
#include "Math/BreadMath.h"
#include "../../../RenderStateUtillity/Win/DirectX11/RenderStateUtillityDX11.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// DirectX11版コンテキスト操作オブジェクト
		//****************************************************************************
		class ContextDX11 final : public IContext
		{
		public:
			struct CbScene
			{
				Math::Matrix view;
				Math::Matrix projection;
				Math::Matrix viewProjection;
				Math::Matrix viewInv;
				Math::Matrix projectionInv;
				Math::Matrix viewProjectionInv;
				Math::Matrix prevView;
				Math::Matrix prevProjection;
				Math::Matrix prevViewProjection;
				Math::Matrix prevViewInv;
				Math::Matrix prevProjectionInv;
				Math::Matrix prevViewProjectionInv;
				Math::Vector4 viewport;
			};

			struct CbMesh
			{
				Math::Matrix world;
				Math::Matrix worldInverse;
				Math::Matrix texture;
			};

			struct CbBone
			{
				Math::Matrix bones[512];
			};

			struct CbShadow
			{
				Math::Matrix transform;
				f32 texelSize;
				f32 padding[3];
			};

		private:
			ID3D11DeviceContext* deviceContext   = nullptr;
			ID3D11DeviceContext* deferredContext = nullptr;

			std::unique_ptr<IBuffer> cbScene = nullptr;
			std::unique_ptr<IBuffer> cbMesh  = nullptr;
			std::unique_ptr<IBuffer> cbBone  = nullptr;

			std::unique_ptr<IBlend>        blendState[static_cast<int>(BlendState::TypeNum)]           = { nullptr };
			std::unique_ptr<IDepthStencil> depthStencilState[static_cast<int>(DepthState::TypeNum)]    = { nullptr };
			std::unique_ptr<IRasterizer>   rasterizerState[static_cast<int>(RasterizerState::TypeNum)] = { nullptr };
			std::unique_ptr<ISampler>      samplerState[static_cast<int>(SamplerState::TypeNum)]       = { nullptr };

			ID3D11Query* query[2] = { nullptr };

			CbScene cbSceneData;

		public:
			ContextDX11() {}
			~ContextDX11() override { Finalize(); }

		public:
			// 初期化
			bool Initialize(IDevice* device)    override;
			bool Initialize_DF(IDevice* device) override;

			// 終了化
			void Finalize() override;

			// 描画
			void Draw(u32 vertexCount, u32 startVertexLocation)       override;
			void DrawDf(u32 vertexCount, u32 startVertexLocation)     override;
			void DrawInstancedIndirect(IBuffer* buffer, u32 offset)   override;
			void DrawInstancedIndirectDf(IBuffer* buffer, u32 offset) override;

			// 描画開始
			void Begin() override;

			// 描画終了
			void End() override;

			// 描画同期待ち
			void WaitSync() override;

			// レンダーターゲットビュークリア
			void ClearRenderTargetView(IRenderTargetSurface* renderTargetSurface, const f32* color)   override;
			void ClearRenderTargetViewDf(IRenderTargetSurface* renderTargetSurface, const f32* color) override;

			// 深度ステンシルビュークリア
			void ClearDepthStencilView(IDepthStencilSurface* depthStencilSurface, f32 depth, u8 stencil)   override;
			void ClearDepthStencilViewDf(IDepthStencilSurface* depthStencilSurface, f32 depth, u8 stencil) override;

			// レンダーターゲット設定
			void SetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface)   override;
			void SetRenderTargetsDf(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) override;

			// レンダーターゲット取得
			void GetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface)   override;
			void GetRenderTargetsDf(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) override;

			// ビューポート設定
			void SetViewports(u32 count, const Viewport viewports[])   override;
			void SetViewportsDf(u32 count, const Viewport viewports[]) override;

			// ビューポート取得
			void GetViewports(u32 count, Viewport* viewports[])   override;
			void GetViewportsDf(u32 count, Viewport* viewports[]) override;

			// シェーダーリソースビュー設定
			void SetShaderResources(ShaderType shadowType, u32 startSlot, u32 numViews, ITexture* texture[])   override;
			void SetShaderResourcesDf(ShaderType shadowType, u32 startSlot, u32 numViews, ITexture* texture[]) override;

			// アンオーダーアクセスビュー設定
			void SetUnorderedAccess(u32 startSlot, u32 numViews, ITexture* texture[], u32* uavInitialCounts)   override;
			void SetUnorderedAccessDf(u32 startSlot, u32 numViews, ITexture* texture[], u32* uavInitialCounts) override;

			// ブレンドステート設定
			void SetBlend(IBlend* blend, const f32* blendFactor, u32 samplerMask)   override;
			void SetBlendDf(IBlend* blend, const f32* blendFactor, u32 samplerMask) override;

			// 深度ステンシルステート設定
			void SetDepthStencil(IDepthStencil* depthStencil, u32 stencilRef)   override;
			void SetDepthStencilDf(IDepthStencil* depthStencil, u32 stencilRef) override;

			// 深度ステンシルステート取得
			void GetDepthStencil(IDepthStencil* depthStencil, u32 stencilRef)   override;
			void GetDepthStencilDf(IDepthStencil* depthStencil, u32 stencilRef) override;

			// ラスタライザーステート設定
			void SetRasterizer(IRasterizer* rasterizer)   override;
			void SetRasterizerDf(IRasterizer* rasterizer) override;

			// ラスタライザーステート取得
			void GetRasterizer(IRasterizer* rasterizer)   override;
			void GetRasterizerDf(IRasterizer* rasterizer) override;

			// サンプラーステート設定
			void SetSamplers(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[])   override;
			void SetSamplersDf(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[]) override;

			// サンプラーステート取得
			void GetSamplers(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[])   override;
			void GetSamplersDf(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[]) override;

			// バッファ更新
			void UpdateSubresource(IBuffer* buffer, u32 dstSubresource, const PhoenixBox* dstBox, const void* drcData, u32 srcRowPitch, u32 srcDepthPitch)   override;
			void UpdateSubresourceDf(IBuffer* buffer, u32 dstSubresource, const PhoenixBox* dstBox, const void* drcData, u32 srcRowPitch, u32 srcDepthPitch) override;

			// バッファ更新開始
			void Map(IBuffer* buffer, u32 subresource, PhoenixMap mapType, u32 mapFlags, PhoenixMappedSubresource* mappedSubresource)   override;
			void MapDf(IBuffer* buffer, u32 subresource, PhoenixMap mapType, u32 mapFlags, PhoenixMappedSubresource* mappedSubresource) override;

			// バッファ更新終了
			void Unmap(IBuffer* buffer, u32 subresource)   override;
			void UnmapDf(IBuffer* buffer, u32 subresource) override;

			// バッファ設定
			void SetConstantBuffers(ShaderType shadowType, u32 startSlot, u32 numViews, IBuffer* buffer[])   override;
			void SetConstantBuffersDf(ShaderType shadowType, u32 startSlot, u32 numViews, IBuffer* buffer[]) override;

			// シーン定数バッファ更新
			void UpdateConstantBufferScene(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform)   override;
			void UpdateConstantBufferSceneDf(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform) override;

			// 前フレームのシーン定数バッファ更新
			void UpdateConstantBufferPrevScene(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform)   override;
			void UpdateConstantBufferPrevSceneDf(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform) override;

			// メッシュ定数バッファ更新
			void UpdateConstantBufferMesh(const Math::Matrix& worldTransform)   override;
			void UpdateConstantBufferMeshDf(const Math::Matrix& worldTransform) override;

			// ボーン定数バッファ更新
			void UpdateConstantBufferBone(const Math::Matrix boneTransforms[], s32 boneTransformCount)   override;
			void UpdateConstantBufferBoneDf(const Math::Matrix boneTransforms[], s32 boneTransformCount) override;

			// 定数バッファ(シーン毎)取得
			IBuffer* GetConstantBufferScene() override { return cbScene.get(); }

			// 定数バッファ(メッシュ毎)取得
			IBuffer* GetConstantBufferMesh() override { return cbMesh.get(); }

			// 定数バッファ(ボーン変換行列)取得
			IBuffer* GetConstantBufferBone() override { return cbBone.get(); }

			// ブレンドステート取得
			IBlend* GetBlendState(BlendState type) override { return blendState[static_cast<int>(type)].get(); }

			// 深度ステンシルステート取得
			IDepthStencil* GetDepthStencilState(DepthState type) override { return depthStencilState[static_cast<int>(type)].get(); }

			// ラスタライザステート取得
			IRasterizer* GetRasterizerState(RasterizerState type) override { return rasterizerState[static_cast<int>(type)].get(); }

			// サンプラステート取得
			ISampler* GetSamplerState(SamplerState type) override { return samplerState[static_cast<int>(type)].get(); }

			// バッファコピー
			void CopyResource(IBuffer* destinationBuffer, IBuffer* sourceBuffer)   override;
			void CopyResourceDf(IBuffer* destinationBuffer, IBuffer* sourceBuffer) override;

			// ミップマップ作成
			void GenerateMips(ITexture* texture)   override;
			void GenerateMipsDf(ITexture* texture) override;
		};
	} // namespace Graphics
} // namespace Bread