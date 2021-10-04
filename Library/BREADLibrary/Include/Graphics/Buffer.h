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
		enum class BreadUsage
		{
			Default = 0,
			Immutable = 1,
			Dynamic = 2,
			Staging = 3
		} 	BreadUsage;

		typedef
		enum class BreadBindFlag
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
		} 	BreadBindFlag;

		typedef
		enum BreadResouceMiscFlag
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
		} 	BreadResouceMiscFlag;

		typedef
		enum BreadCPUAccessFlag
		{
			CPUAccessWrite = 0x10000L,
			CPUAccessRead = 0x20000L
		} 	BreadCPUAccessFlag;

		typedef
		enum class BreadMap
		{
			Read = 1,
			Write = 2,
			ReadWrite = 3,
			WriteDiscard = 4,
			WriteNoOverWrite = 5
		} 	BreadMap;

		typedef
		struct BreadBufferDesc
		{
			u32 byteWidth;
			BreadUsage usage;
			u32 bindFlags;
			u32 cpuAccessFlags;
			u32 miscFlags;
			u32 structureByteStride;
		} 	BreadBufferDesc;

		typedef
		struct BreadBox
		{
			u32 left;
			u32 top;
			u32 front;
			u32 right;
			u32 bottom;
			u32 back;
		} 	BreadBox;

		typedef
		struct BreadSubresourceData
		{
			const void* sysMem;
			UINT sysMemPitch;
			UINT sysMemSlicePitch;
		} BreadSubresourceData;

		typedef
		struct BreadMappedSubresource
		{
			void* data;
			u32 rowPitch;
			u32 depthPitch;
		} BreadMappedSubresource;

		//****************************************************************************
		// バッファー操作インターフェース
		//****************************************************************************
		class IBuffer : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<IBuffer> Create();

			// 初期化
			virtual bool Initialize(IDevice* device, const BreadBufferDesc& desc) = 0;
			virtual bool Initialize(IDevice* device, const BreadBufferDesc& desc, const BreadSubresourceData& data) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// 情報取得
			virtual void GetDesc(BreadBufferDesc* desc) = 0;
		};
	} // namespace Graphics
} // namespace Bread