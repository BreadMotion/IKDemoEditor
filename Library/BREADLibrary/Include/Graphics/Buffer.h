#pragma once

#include <memory>
#include "Types.h"
#include "FND/Base.h"
#include "Graphics/Device.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// バッファー設定記述
		//****************************************************************************
		typedef
		enum class PhoenixUsage
		{
			Default = 0,
			Immutable = 1,
			Dynamic = 2,
			Staging = 3
		} 	PhoenixUsage;

		typedef
		enum class PhoenixBindFlag
		{
			VertexBuffer = 0x1L,
			IndexBuffer = 0x2L,
			ConstantBuffer = 0x4L,
			ShaderResource = 0x8L,
			StreamOutput = 0x10L,
			RenderTarget = 0x20L,
			DepthStencil = 0x40L,
			UnorderedAccess = 0x80L,
			Decoder = 0x200L,
			VideoEncoder = 0x400L
		} 	PhoenixBindFlag;

		typedef
		enum PhoenixResouceMiscFlag
		{
			ResouceMiscGenerateMips = 0x1L,
			ResouceMiscShader = 0x2L,
			ResouceMiscTextureCube = 0x4L,
			ResouceMiscDrawindIrectArgs = 0x10L,
			ResouceMiscBufferAllowsRAWViews = 0x20L,
			ResouceMiscBufferStructured = 0x40L,
			ResouceMiscResouceClamp = 0x80L,
			ResouceMiscShaderKeyesMutex = 0x100L,
			ResouceMiscGDICompatible = 0x200L,
			ResouceMiscShaderNthandle = 0x800L,
			ResouceMiscRestrictedContext = 0x1000L,
			ResouceMiscRestrictShaderResouce = 0x2000L,
			ResouceMiscRestrictShaderResouceDrive = 0x4000L,
			ResouceMiscGuarded = 0x8000L,
			ResouceMiscTilePool = 0x20000L,
			ResouceMiscTiled = 0x40000L,
			ResouceMiscHWProtected = 0x80000L
		} 	PhoenixResouceMiscFlag;

		typedef
		enum PhoenixCPUAccessFlag
		{
			CPUAccessWrite = 0x10000L,
			CPUAccessRead = 0x20000L
		} 	PhoenixCPUAccessFlag;

		typedef
		enum class PhoenixMap
		{
			Read = 1,
			Write = 2,
			ReadWrite = 3,
			WriteDiscard = 4,
			WriteNoOverWrite = 5
		} 	PhoenixMap;

		typedef
		struct PhoenixBufferDesc
		{
			u32 byteWidth;
			PhoenixUsage usage;
			u32 bindFlags;
			u32 cpuAccessFlags;
			u32 miscFlags;
			u32 structureByteStride;
		} 	PhoenixBufferDesc;

		typedef
		struct PhoenixBox
		{
			u32 left;
			u32 top;
			u32 front;
			u32 right;
			u32 bottom;
			u32 back;
		} 	PhoenixBox;

		typedef
		struct PhoenixSubresourceData
		{
			const void* sysMem;
			UINT sysMemPitch;
			UINT sysMemSlicePitch;
		} PhoenixSubresourceData;

		typedef
		struct PhoenixMappedSubresource
		{
			void* data;
			u32 rowPitch;
			u32 depthPitch;
		} PhoenixMappedSubresource;

		//****************************************************************************
		// バッファー操作インターフェース
		//****************************************************************************
		class IBuffer : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IBuffer> Create();

			// 初期化
			virtual bool Initialize(IDevice* device, const PhoenixBufferDesc& desc) = 0;
			virtual bool Initialize(IDevice* device, const PhoenixBufferDesc& desc, const PhoenixSubresourceData& data) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// 情報取得
			virtual void GetDesc(PhoenixBufferDesc* desc) = 0;
		};
	} // namespace Graphics
} // namespace Bread