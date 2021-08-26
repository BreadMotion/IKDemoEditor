#pragma once

#include <memory>

#include "Types.h"
#include "FND/Base.h"
#include "Graphics/Device.h"
#include "Graphics/Surface.h"
#include "Graphics/Viewport.h"
//#include "Graphics/Shader.h"
#include "Graphics/Buffer.h"
#include "Graphics/RenderState.h"
#include "Math/BreadMath.h"


namespace Bread
{
	namespace Graphics
	{
		// TODO : 移動
		typedef enum class ShaderType
		{
			Vertex,   // 頂点シェーダー
			Pixel,    // ピクセルシェーダー
			Compute,  // コンピュートシェーダー

			TypeNum
		} ShaderType;

		//****************************************************************************
		// コンテキスト操作インターフェース
		//****************************************************************************
		class IContext : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IContext> Create();

			// 初期化
			virtual bool Initialize(IDevice* device)    = 0;
			virtual bool Initialize_DF(IDevice* device) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// 描画
			virtual void Draw(u32 vertexCount, u32 startVertexLocation)       = 0;
			virtual void DrawDf(u32 vertexCount, u32 startVertexLocation)     = 0;
			virtual void DrawInstancedIndirect(IBuffer* buffer, u32 offset)   = 0;
			virtual void DrawInstancedIndirectDf(IBuffer* buffer, u32 offset) = 0;

			// 描画開始
			virtual void Begin() = 0;

			// 描画終了
			virtual void End() = 0;

			// 描画同期待ち
			virtual void WaitSync() = 0;

			// レンダーターゲットビュークリア
			virtual void ClearRenderTargetView(IRenderTargetSurface* renderTargetSurface, const f32* color)   = 0;
			virtual void ClearRenderTargetViewDf(IRenderTargetSurface* renderTargetSurface, const f32* color) = 0;

			// 深度ステンシルビュークリア
			virtual void ClearDepthStencilView(IDepthStencilSurface* depthStencilSurface, f32 depth, u8 stencil)   = 0;
			virtual void ClearDepthStencilViewDf(IDepthStencilSurface* depthStencilSurface, f32 depth, u8 stencil) = 0;

			// レンダーターゲット設定
			virtual void SetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface)   = 0;
			virtual void SetRenderTargetsDf(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) = 0;

			// レンダーターゲット取得
			virtual void GetRenderTargets(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface)   = 0;
			virtual void GetRenderTargetsDf(u32 count, IRenderTargetSurface* renderTargetSurface[], IDepthStencilSurface* depthStencilSurface) = 0;

			// ビューポート設定
			virtual void SetViewports(u32 count, const Viewport viewports[])   = 0;
			virtual void SetViewportsDf(u32 count, const Viewport viewports[]) = 0;

			// ビューポート取得
			virtual void GetViewports(u32 count, Viewport* viewports[])   = 0;
			virtual void GetViewportsDf(u32 count, Viewport* viewports[]) = 0;

			// シェーダーリソースビュー設定
			virtual void SetShaderResources(ShaderType shadowType, u32 startSlot, u32 numViews, ITexture* texture[])   = 0;
			virtual void SetShaderResourcesDf(ShaderType shadowType, u32 startSlot, u32 numViews, ITexture* texture[]) = 0;

			// アンオーダーアクセスビュー設定
			virtual void SetUnorderedAccess(u32 startSlot, u32 numViews, ITexture* texture[], u32* uavInitialCounts)   = 0;
			virtual void SetUnorderedAccessDf(u32 startSlot, u32 numViews, ITexture* texture[], u32* uavInitialCounts) = 0;

			// ブレンドステート設定
			virtual void SetBlend(IBlend* blend, const f32* blendFactor, u32 samplerMask)   = 0;
			virtual void SetBlendDf(IBlend* blend, const f32* blendFactor, u32 samplerMask) = 0;

			// 深度ステンシルステート設定
			virtual void SetDepthStencil(IDepthStencil* depthStencil, u32 stencilRef)   = 0;
			virtual void SetDepthStencilDf(IDepthStencil* depthStencil, u32 stencilRef) = 0;

			// 深度ステンシルステート取得
			virtual void GetDepthStencil(IDepthStencil* depthStencil, u32 stencilRef)   = 0;
			virtual void GetDepthStencilDf(IDepthStencil* depthStencil, u32 stencilRef) = 0;

			// ラスタライザーステート設定
			virtual void SetRasterizer(IRasterizer* rasterizer)   = 0;
			virtual void SetRasterizerDf(IRasterizer* rasterizer) = 0;

			// ラスタライザーステート取得
			virtual void GetRasterizer(IRasterizer* rasterizer)   = 0;
			virtual void GetRasterizerDf(IRasterizer* rasterizer) = 0;

			// サンプラーステート設定
			virtual void SetSamplers(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[])   = 0;
			virtual void SetSamplersDf(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[]) = 0;

			// サンプラーステート取得
			virtual void GetSamplers(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[])   = 0;
			virtual void GetSamplersDf(ShaderType shadowType, u32 startSlot, u32 numViews, ISampler* sampler[]) = 0;

			// バッファ更新
			virtual void UpdateSubresource(IBuffer* buffer, u32 dstSubresource, const PhoenixBox* dstBox, const void* drcData, u32 srcRowPitch, u32 srcDepthPitch)   = 0;
			virtual void UpdateSubresourceDf(IBuffer* buffer, u32 dstSubresource, const PhoenixBox* dstBox, const void* drcData, u32 srcRowPitch, u32 srcDepthPitch) = 0;

			// バッファ更新開始
			virtual void Map(IBuffer* buffer, u32 subresource, PhoenixMap mapType, u32 mapFlags, PhoenixMappedSubresource* mappedSubresource)   = 0;
			virtual void MapDf(IBuffer* buffer, u32 subresource, PhoenixMap mapType, u32 mapFlags, PhoenixMappedSubresource* mappedSubresource) = 0;

			// バッファ更新終了
			virtual void Unmap(IBuffer* buffer, u32 subresource)   = 0;
			virtual void UnmapDf(IBuffer* buffer, u32 subresource) = 0;

			// バッファ設定
			virtual void SetConstantBuffers(ShaderType shadowType, u32 startSlot, u32 numViews, IBuffer* buffer[])   = 0;
			virtual void SetConstantBuffersDf(ShaderType shadowType, u32 startSlot, u32 numViews, IBuffer* buffer[]) = 0;

			// シーン定数バッファ更新
			virtual void UpdateConstantBufferScene(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform)   = 0;
			virtual void UpdateConstantBufferSceneDf(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform) = 0;

			// 前フレームのシーン定数バッファ更新
			virtual void UpdateConstantBufferPrevScene(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform)   = 0;
			virtual void UpdateConstantBufferPrevSceneDf(const Math::Matrix& viewTransform, const Math::Matrix& projectionTransform) = 0;

			// メッシュ定数バッファ更新
			virtual void UpdateConstantBufferMesh(const Math::Matrix& worldTransform)   = 0;
			virtual void UpdateConstantBufferMeshDf(const Math::Matrix& worldTransform) = 0;

			// ボーン定数バッファ更新
			virtual void UpdateConstantBufferBone(const Math::Matrix boneTransforms[], s32 boneTransformCount)   = 0;
			virtual void UpdateConstantBufferBoneDf(const Math::Matrix boneTransforms[], s32 boneTransformCount) = 0;

			// 定数バッファ(シーン毎)取得
			virtual IBuffer* GetConstantBufferScene() = 0;

			// 定数バッファ(メッシュ毎)取得
			virtual IBuffer* GetConstantBufferMesh()  = 0;

			// 定数バッファ(ボーン変換行列)取得
			virtual IBuffer* GetConstantBufferBone()  = 0;

			// ブレンドステート取得
			virtual IBlend* GetBlendState(BlendState type) = 0;

			// 深度ステンシルステート取得
			virtual IDepthStencil* GetDepthStencilState(DepthState type)  = 0;

			// ラスタライザステート取得
			virtual IRasterizer* GetRasterizerState(RasterizerState type) = 0;

			// サンプラステート取得
			virtual ISampler* GetSamplerState(SamplerState type) = 0;

			// バッファコピー
			virtual void CopyResource(IBuffer* destinationBuffer, IBuffer* sourceBuffer)   = 0;
			virtual void CopyResourceDf(IBuffer* destinationBuffer, IBuffer* sourceBuffer) = 0;

			// ミップマップ作成
			virtual void GenerateMips(ITexture* texture)   = 0;
			virtual void GenerateMipsDf(ITexture* texture) = 0;
		};
	} // namespace Graphics
} // namespace Bread