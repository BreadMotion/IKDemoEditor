#pragma once

#include <memory>
#include <vector>
#include "Types.h"
#include "FND/Base.h"
#include "Graphics/Device.h"
#include "OS/Resource.h"
#include "Math/Color.h"
#include "Graphics/Buffer.h"


namespace Bread
{
	namespace Graphics
	{
		//****************************************************************************
		// テクスチャ次元タイプ
		//****************************************************************************
		enum class TextureDimension
		{
			Tex1D,		// 1Dテクスチャ
			Tex2D,		// 2Dテクスチャ
			Tex3D,		// 3Dテクスチャ
			Cube,		// キューブテクスチャ
		};

		enum class TextureDimensionDx
		{
			SRV_DIMENSION_UNKNOWN = 0,
			SRV_DIMENSION_BUFFER = 1,
			SRV_DIMENSION_TEXTURE1D = 2,
			SRV_DIMENSION_TEXTURE1DARRAY = 3,
			SRV_DIMENSION_TEXTURE2D = 4,
			SRV_DIMENSION_TEXTURE2DARRAY = 5,
			SRV_DIMENSION_TEXTURE2DMS = 6,
			SRV_DIMENSION_TEXTURE2DMSARRAY = 7,
			SRV_DIMENSION_TEXTURE3D = 8,
			SRV_DIMENSION_TEXTURECUBE = 9,
			SRV_DIMENSION_TEXTURECUBEARRAY = 10,
			SRV_DIMENSION_BUFFEREX = 11
		};

		//****************************************************************************
		// テクスチャフォーマット
		//****************************************************************************
		enum class TextureFormat
		{
			R8G8B8A8,		//!< 32bit R8G8B8A8
			B8G8R8A8,		//!< 32bit B8G8R8A8
			R24G8,			//!< 32bit R24G8
			R32,			//!< 32bit R32
			B4G4R4A4,		//!< 16bit B4G4R4A4
			B5G5R5A1,		//!< 16bit B5G5R5A1
			B5G6R5,			//!< 16bit B5G6R5
			R16,			//!< 16bit R16
			A8,				//!< 8bit  A8
			R8,				//!< 8bit  R8
			BC1,			//!< 4bit  RGB
			BC2,			//!< 8bit  RGBA
			BC3,			//!< 8bit  RGBA

			UNKNOWN			//!< プラットフォーム固有
		};

		enum class TextureFormatDx
		{
			UNKNOWN = 0,
			R32G32B32A32_TYPELESS = 1,
			R32G32B32A32_FLOAT = 2,
			R32G32B32A32_UINT = 3,
			R32G32B32A32_SINT = 4,
			R32G32B32_TYPELESS = 5,
			R32G32B32_FLOAT = 6,
			R32G32B32_UINT = 7,
			R32G32B32_SINT = 8,
			R16G16B16A16_TYPELESS = 9,
			R16G16B16A16_FLOAT = 10,
			R16G16B16A16_UNORM = 11,
			R16G16B16A16_UINT = 12,
			R16G16B16A16_SNORM = 13,
			R16G16B16A16_SINT = 14,
			R32G32_TYPELESS = 15,
			R32G32_FLOAT = 16,
			R32G32_UINT = 17,
			R32G32_SINT = 18,
			R32G8X24_TYPELESS = 19,
			D32_FLOAT_S8X24_UINT = 20,
			R32_FLOAT_X8X24_TYPELESS = 21,
			X32_TYPELESS_G8X24_UINT = 22,
			R10G10B10A2_TYPELESS = 23,
			R10G10B10A2_UNORM = 24,
			R10G10B10A2_UINT = 25,
			R11G11B10_FLOAT = 26,
			R8G8B8A8_TYPELESS = 27,
			R8G8B8A8_UNORM = 28,
			R8G8B8A8_UNORM_SRGB = 29,
			R8G8B8A8_UINT = 30,
			R8G8B8A8_SNORM = 31,
			R8G8B8A8_SINT = 32,
			R16G16_TYPELESS = 33,
			R16G16_FLOAT = 34,
			R16G16_UNORM = 35,
			R16G16_UINT = 36,
			R16G16_SNORM = 37,
			R16G16_SINT = 38,
			R32_TYPELESS = 39,
			D32_FLOAT = 40,
			R32_FLOAT = 41,
			R32_UINT = 42,
			R32_SINT = 43,
			R24G8_TYPELESS = 44,
			D24_UNORM_S8_UINT = 45,
			R24_UNORM_X8_TYPELESS = 46,
			X24_TYPELESS_G8_UINT = 47,
			R8G8_TYPELESS = 48,
			R8G8_UNORM = 49,
			R8G8_UINT = 50,
			R8G8_SNORM = 51,
			R8G8_SINT = 52,
			R16_TYPELESS = 53,
			R16_FLOAT = 54,
			D16_UNORM = 55,
			R16_UNORM = 56,
			R16_UINT = 57,
			R16_SNORM = 58,
			R16_SINT = 59,
			R8_TYPELESS = 60,
			R8_UNORM = 61,
			R8_UINT = 62,
			R8_SNORM = 63,
			R8_SINT = 64,
			A8_UNORM = 65,
			R1_UNORM = 66,
			R9G9B9E5_SHAREDEXP = 67,
			R8G8_B8G8_UNORM = 68,
			G8R8_G8B8_UNORM = 69,
			BC1_TYPELESS = 70,
			BC1_UNORM = 71,
			BC1_UNORM_SRGB = 72,
			BC2_TYPELESS = 73,
			BC2_UNORM = 74,
			BC2_UNORM_SRGB = 75,
			BC3_TYPELESS = 76,
			BC3_UNORM = 77,
			BC3_UNORM_SRGB = 78,
			BC4_TYPELESS = 79,
			BC4_UNORM = 80,
			BC4_SNORM = 81,
			BC5_TYPELESS = 82,
			BC5_UNORM = 83,
			BC5_SNORM = 84,
			B5G6R5_UNORM = 85,
			B5G5R5A1_UNORM = 86,
			B8G8R8A8_UNORM = 87,
			B8G8R8X8_UNORM = 88,
			R10G10B10_XR_BIAS_A2_UNORM = 89,
			B8G8R8A8_TYPELESS = 90,
			B8G8R8A8_UNORM_SRGB = 91,
			B8G8R8X8_TYPELESS = 92,
			B8G8R8X8_UNORM_SRGB = 93,
			BC6H_TYPELESS = 94,
			BC6H_UF16 = 95,
			BC6H_SF16 = 96,
			BC7_TYPELESS = 97,
			BC7_UNORM = 98,
			BC7_UNORM_SRGB = 99,
			AYUV = 100,
			Y410 = 101,
			Y416 = 102,
			NV12 = 103,
			P010 = 104,
			P016 = 105,
			D420_OPAQUE = 106,
			YUY2 = 107,
			Y210 = 108,
			Y216 = 109,
			NV11 = 110,
			AI44 = 111,
			IA44 = 112,
			P8 = 113,
			A8P8 = 114,
			B4G4R4A4_UNORM = 115,

			P208 = 130,
			V208 = 131,
			V408 = 132,


			//FORCE_UINT = 0xffffffff
		};

		enum BufferExSRVFlag
		{
			BufferExSRVFlagRAW = 0x1
		};

		enum BufferUAVFlag
		{
			BufferUAVFlagRAW = 0x1,
			BufferUAVFlagAppEnd = 0x2,
			BufferUAVFlagCounter = 0x4
		};

		enum UAVDimension
		{
			UNKNOWN = 0,
			BUFFER = 1,
			TEXTURE1D = 2,
			TEXTURE1DARRAY = 3,
			TEXTURE2D = 4,
			TEXTURE2DARRAY = 5,
			TEXTURE3D = 8
		};

		//****************************************************************************
		// テクスチャ設定記述
		//****************************************************************************
		struct TextureDesc
		{
			u32 width;						// 幅。
			u32 height;						// 高さ(1Dテクスチャの場合は1)
			u32 depth;						// 深さ(1D,2Dテクスチャの場合は1)
			u32 arraySize;					// 配列数
			u32 mipLevels;					// ミップマップレベルの最大数(0の場合は最大になる)
			TextureDimension dimension;		// テクスチャ次元タイプ
			TextureFormat format;			// テクスチャフォーマット
			const void* sysMem;				// 初期化データへのポインタ
			u32	 sysMemSize;				// 初期化データサイズ
		};

		struct TextureDescDx
		{
			u32 width;						// 幅。
			u32 height;						// 高さ(1Dテクスチャの場合は1)
			u32 depth;						// 深さ(1D,2Dテクスチャの場合は1)
			u32 arraySize;					// 配列数
			u32 mipLevels;					// ミップマップレベルの最大数(0の場合は最大になる)
			TextureDimensionDx dimension;	// テクスチャ次元タイプ
			TextureFormat format;			// テクスチャフォーマット
			const void* sysMem;				// 初期化データへのポインタ
			u32	 sysMemSize;				// 初期化データサイズ
		};

		struct BufferSRV
		{
			union
			{
				u32 firstElement;
				u32 elementOffset;
			};
			union
			{
				u32 numElements;
				u32 elementWidth;
			};
		};

		struct BufferExSRV
		{
			u32 firstElement;
			u32 numElements;
			u32 flags;
		};

		struct Tex1DSRV
		{
			u32 mostDetailedMip;
			u32 mipLevels;
		};

		struct Tex1DArraySRV
		{
			u32 mostDetailedMip;
			u32 mipLevels;
			u32 firstArraySlice;
			u32 arraySize;
		};

		struct Tex2DSRV
		{
			u32 mostDetailedMip;
			u32 mipLevels;
		};

		struct Tex2DArraySRV
		{
			u32 mostDetailedMip;
			u32 mipLevels;
			u32 firstArraySlice;
			u32 arraySize;
		};

		struct Tex3DSRV
		{
			u32 mostDetailedMip;
			u32 mipLevels;
		};

		struct TexCubeSRV
		{
			u32 mostDetailedMip;
			u32 mipLevels;
		};

		struct TexCubeArraySRV
		{
			u32 mostDetailedMip;
			u32 mipLevels;
			u32 first2DArrayFace;
			u32 numCubes;
		};

		struct Tex2DMSSRV
		{
			u32 unusedFieldNothingToDefine;
		};

		struct Tex2DMSArraySRV
		{
			u32 firstArraySlice;
			u32 arraySize;
		};

		struct ShaderResouceDesc
		{
			TextureFormatDx format;
			TextureDimensionDx viewDimension;
			union
			{
				BufferSRV buffer;
				Tex1DSRV texture1D;
				Tex1DArraySRV texture1DArray;
				Tex2DSRV texture2D;
				Tex2DArraySRV texture2DArray;
				Tex2DMSSRV texture2DMS;
				Tex2DMSArraySRV texture2DMSArray;
				Tex3DSRV texture3D;
				TexCubeSRV textureCube;
				TexCubeArraySRV textureCubeArray;
				BufferExSRV bufferEx;
			};
		};

		struct BufferUAV
		{
			u32 firstElement;
			u32 numElements;
			u32 flags;
		};

		struct Tex1DUAV
		{
			u32 mipSlice;
		};

		struct Tex1DArrayUAV
		{
			u32 mipSlice;
			u32 firstArraySlice;
			u32 arraySize;
		};

		struct Tex2DUAV
		{
			u32 mipSlice;
		};

		struct Tex2DArrayUAV
		{
			u32 mipSlice;
			u32 firstArraySlice;
			u32 arraySize;
		};

		struct Tex3DUAV
		{
			u32 mipSlice;
			u32 firstWSlice;
			u32 wSize;
		};

		struct UnorderedAccessViewDesc
		{
			TextureFormatDx format;
			UAVDimension viewDimension;
			union
			{
				BufferUAV buffer;
				Tex1DUAV texture1D;
				Tex1DArrayUAV texture1DArray;
				Tex2DUAV texture2D;
				Tex2DArrayUAV texture2DArray;
				Tex3DUAV texture3D;
			};
		};

		//****************************************************************************
		// マテリアル種類
		//****************************************************************************
		enum class MaterialType
		{
			Diffuse,
			Ambient,
			Specular,
			NormalMap,
			Bump,
			Emissive,
		};

		//****************************************************************************
		// テクスチャ操作インターフェース
		//****************************************************************************
		class ITexture : public FND::Base
		{
		public:
			// 生成
			static std::unique_ptr<ITexture> Create();

			// 初期化
			virtual bool Initialize(IDevice* device, const TextureDesc& desc) = 0;

			// 初期化
			virtual bool Initialize(IDevice* device, const ShaderResouceDesc& desc, IBuffer* buffer) = 0;

			// 初期化
			virtual bool Initialize(IDevice* device, const UnorderedAccessViewDesc& desc, IBuffer* buffer) = 0;

			// 初期化
			virtual bool Initialize(IDevice* device, const char* filename, MaterialType materialType, const Math::Color& color) = 0;

			// 終了化
			virtual void Finalize() = 0;

			// ハンドル取得
			virtual void* Handle() = 0;

			// テクスチャ設定の取得
			virtual void GetTextureDesc(TextureDesc* desc) = 0;

			// テクスチャ設定の取得
			virtual void GetShaderResourceViewDesc(TextureDescDx* desc) = 0;
		};

		//****************************************************************************
		// テクスチャリソース操作インターフェース
		//****************************************************************************
		class ITextureResource : public OS::Resource
		{
		public:
			// テクスチャ取得
			virtual Graphics::ITexture* GetTexture() = 0;

		};

		//****************************************************************************
		// テクスチャリソース操作オブジェクト生成インターフェース
		//****************************************************************************
		class ITextureResourceFactory : public OS::IResourceFactory
		{
		public:
			// 生成
			static std::unique_ptr<ITextureResourceFactory> Create(Graphics::IDevice* device);
		};

		//****************************************************************************
		// STBテクスチャリソース操作オブジェクト生成インターフェース
		//****************************************************************************
		class IStbTextureResourceFactory : public OS::IResourceFactory
		{
		public:
			// 生成
			static std::unique_ptr<IStbTextureResourceFactory> Create(Graphics::IDevice* device);
		};

		//****************************************************************************
		// DDSテクスチャリソース操作オブジェクト生成インターフェース
		//****************************************************************************
		class IDdsTextureResourceFactory : public OS::IResourceFactory
		{
		public:
			// 生成
			static std::unique_ptr<IDdsTextureResourceFactory> Create(Graphics::IDevice* device);
		};

		//****************************************************************************
		// テクスチャデータ
		//****************************************************************************
		struct TextureData
		{
			u32 width;
			u32 height;
			u32 depth;
			u32 arraySize;
			u32 mipLevels;
			Graphics::TextureFormat format;
			Graphics::TextureDimension dimension;

			std::vector<u8> pixels;
		};
	} // namespace Graphics
} // namespace Bread