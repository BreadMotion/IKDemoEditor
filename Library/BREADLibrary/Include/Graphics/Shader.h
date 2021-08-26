#pragma once

#include <memory>
#include "Types.h"
#include "Graphics/Device.h"
#include "Graphics/Context.h"


namespace Bread
{
	namespace Graphics
	{
        //****************************************************************************
        // シェーダー設定記述
        //****************************************************************************
        //typedef enum class ShaderType
        //{
        //    Vertex, // 頂点シェーダー
        //    Pixel,  // ピクセルシェーダー

        //    TypeNum
        //} ShaderType;

        enum PhoenixFormat
        {
            PHOENIX_FORMAT_UNKNOWN = 0,
            PHOENIX_FORMAT_R32G32B32A32_TYPELESS = 1,
            PHOENIX_FORMAT_R32G32B32A32_FLOAT = 2,
            PHOENIX_FORMAT_R32G32B32A32_UINT = 3,
            PHOENIX_FORMAT_R32G32B32A32_SINT = 4,
            PHOENIX_FORMAT_R32G32B32_TYPELESS = 5,
            PHOENIX_FORMAT_R32G32B32_FLOAT = 6,
            PHOENIX_FORMAT_R32G32B32_UINT = 7,
            PHOENIX_FORMAT_R32G32B32_SINT = 8,
            PHOENIX_FORMAT_R16G16B16A16_TYPELESS = 9,
            PHOENIX_FORMAT_R16G16B16A16_FLOAT = 10,
            PHOENIX_FORMAT_R16G16B16A16_UNORM = 11,
            PHOENIX_FORMAT_R16G16B16A16_UINT = 12,
            PHOENIX_FORMAT_R16G16B16A16_SNORM = 13,
            PHOENIX_FORMAT_R16G16B16A16_SINT = 14,
            PHOENIX_FORMAT_R32G32_TYPELESS = 15,
            PHOENIX_FORMAT_R32G32_FLOAT = 16,
            PHOENIX_FORMAT_R32G32_UINT = 17,
            PHOENIX_FORMAT_R32G32_SINT = 18,
            PHOENIX_FORMAT_R32G8X24_TYPELESS = 19,
            PHOENIX_FORMAT_D32_FLOAT_S8X24_UINT = 20,
            PHOENIX_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
            PHOENIX_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
            PHOENIX_FORMAT_R10G10B10A2_TYPELESS = 23,
            PHOENIX_FORMAT_R10G10B10A2_UNORM = 24,
            PHOENIX_FORMAT_R10G10B10A2_UINT = 25,
            PHOENIX_FORMAT_R11G11B10_FLOAT = 26,
            PHOENIX_FORMAT_R8G8B8A8_TYPELESS = 27,
            PHOENIX_FORMAT_R8G8B8A8_UNORM = 28,
            PHOENIX_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
            PHOENIX_FORMAT_R8G8B8A8_UINT = 30,
            PHOENIX_FORMAT_R8G8B8A8_SNORM = 31,
            PHOENIX_FORMAT_R8G8B8A8_SINT = 32,
            PHOENIX_FORMAT_R16G16_TYPELESS = 33,
            PHOENIX_FORMAT_R16G16_FLOAT = 34,
            PHOENIX_FORMAT_R16G16_UNORM = 35,
            PHOENIX_FORMAT_R16G16_UINT = 36,
            PHOENIX_FORMAT_R16G16_SNORM = 37,
            PHOENIX_FORMAT_R16G16_SINT = 38,
            PHOENIX_FORMAT_R32_TYPELESS = 39,
            PHOENIX_FORMAT_D32_FLOAT = 40,
            PHOENIX_FORMAT_R32_FLOAT = 41,
            PHOENIX_FORMAT_R32_UINT = 42,
            PHOENIX_FORMAT_R32_SINT = 43,
            PHOENIX_FORMAT_R24G8_TYPELESS = 44,
            PHOENIX_FORMAT_D24_UNORM_S8_UINT = 45,
            PHOENIX_FORMAT_R24_UNORM_X8_TYPELESS = 46,
            PHOENIX_FORMAT_X24_TYPELESS_G8_UINT = 47,
            PHOENIX_FORMAT_R8G8_TYPELESS = 48,
            PHOENIX_FORMAT_R8G8_UNORM = 49,
            PHOENIX_FORMAT_R8G8_UINT = 50,
            PHOENIX_FORMAT_R8G8_SNORM = 51,
            PHOENIX_FORMAT_R8G8_SINT = 52,
            PHOENIX_FORMAT_R16_TYPELESS = 53,
            PHOENIX_FORMAT_R16_FLOAT = 54,
            PHOENIX_FORMAT_D16_UNORM = 55,
            PHOENIX_FORMAT_R16_UNORM = 56,
            PHOENIX_FORMAT_R16_UINT = 57,
            PHOENIX_FORMAT_R16_SNORM = 58,
            PHOENIX_FORMAT_R16_SINT = 59,
            PHOENIX_FORMAT_R8_TYPELESS = 60,
            PHOENIX_FORMAT_R8_UNORM = 61,
            PHOENIX_FORMAT_R8_UINT = 62,
            PHOENIX_FORMAT_R8_SNORM = 63,
            PHOENIX_FORMAT_R8_SINT = 64,
            PHOENIX_FORMAT_A8_UNORM = 65,
            PHOENIX_FORMAT_R1_UNORM = 66,
            PHOENIX_FORMAT_R9G9B9E5_SHAREDEXP = 67,
            PHOENIX_FORMAT_R8G8_B8G8_UNORM = 68,
            PHOENIX_FORMAT_G8R8_G8B8_UNORM = 69,
            PHOENIX_FORMAT_BC1_TYPELESS = 70,
            PHOENIX_FORMAT_BC1_UNORM = 71,
            PHOENIX_FORMAT_BC1_UNORM_SRGB = 72,
            PHOENIX_FORMAT_BC2_TYPELESS = 73,
            PHOENIX_FORMAT_BC2_UNORM = 74,
            PHOENIX_FORMAT_BC2_UNORM_SRGB = 75,
            PHOENIX_FORMAT_BC3_TYPELESS = 76,
            PHOENIX_FORMAT_BC3_UNORM = 77,
            PHOENIX_FORMAT_BC3_UNORM_SRGB = 78,
            PHOENIX_FORMAT_BC4_TYPELESS = 79,
            PHOENIX_FORMAT_BC4_UNORM = 80,
            PHOENIX_FORMAT_BC4_SNORM = 81,
            PHOENIX_FORMAT_BC5_TYPELESS = 82,
            PHOENIX_FORMAT_BC5_UNORM = 83,
            PHOENIX_FORMAT_BC5_SNORM = 84,
            PHOENIX_FORMAT_B5G6R5_UNORM = 85,
            PHOENIX_FORMAT_B5G5R5A1_UNORM = 86,
            PHOENIX_FORMAT_B8G8R8A8_UNORM = 87,
            PHOENIX_FORMAT_B8G8R8X8_UNORM = 88,
            PHOENIX_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
            PHOENIX_FORMAT_B8G8R8A8_TYPELESS = 90,
            PHOENIX_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
            PHOENIX_FORMAT_B8G8R8X8_TYPELESS = 92,
            PHOENIX_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
            PHOENIX_FORMAT_BC6H_TYPELESS = 94,
            PHOENIX_FORMAT_BC6H_UF16 = 95,
            PHOENIX_FORMAT_BC6H_SF16 = 96,
            PHOENIX_FORMAT_BC7_TYPELESS = 97,
            PHOENIX_FORMAT_BC7_UNORM = 98,
            PHOENIX_FORMAT_BC7_UNORM_SRGB = 99,
            PHOENIX_FORMAT_AYUV = 100,
            PHOENIX_FORMAT_Y410 = 101,
            PHOENIX_FORMAT_Y416 = 102,
            PHOENIX_FORMAT_NV12 = 103,
            PHOENIX_FORMAT_P010 = 104,
            PHOENIX_FORMAT_P016 = 105,
            PHOENIX_FORMAT_420_OPAQUE = 106,
            PHOENIX_FORMAT_YUY2 = 107,
            PHOENIX_FORMAT_Y210 = 108,
            PHOENIX_FORMAT_Y216 = 109,
            PHOENIX_FORMAT_NV11 = 110,
            PHOENIX_FORMAT_AI44 = 111,
            PHOENIX_FORMAT_IA44 = 112,
            PHOENIX_FORMAT_P8 = 113,
            PHOENIX_FORMAT_A8P8 = 114,
            PHOENIX_FORMAT_B4G4R4A4_UNORM = 115,

            PHOENIX_FORMAT_P208 = 130,
            PHOENIX_FORMAT_V208 = 131,
            PHOENIX_FORMAT_V408 = 132,


            PHOENIX_FORMAT_FORCE_UINT = 0xffffffff
        };

        enum PhoenixInputClassIfaication
        {
            PHOENIX_INPUT_PER_VERTEX_DATA = 0,
            PHOENIX_INPUT_PER_INSTANCE_DATA = 1
        };

#define	PHOENIX_APPEND_ALIGNED_ELEMENT	( 0xffffffff )

		typedef struct PhoenixInputElementDesc
		{
			const s8* semanticName;
			u32 semanticIndex;
            PhoenixFormat format;
			u32 inputSlot;
			u32 alignedByteOffset;
            PhoenixInputClassIfaication inputSlotClass;
            u32 instanceDataStepRate;
		} PhoenixInputElementDesc;

        //****************************************************************************
        // シェーダー操作インターフェース
        //****************************************************************************
		class IShader : public FND::Base
		{
		public:
            // 生成
            static std::unique_ptr<IShader> Create();

			// 初期化
			virtual bool Initialize() = 0;

			// 終了化
			virtual void Finalize() = 0;

			// 頂点シェーダー読み込み
            virtual void LoadVS(IDevice* device, const char* csoNameOfVertexShader, PhoenixInputElementDesc* inputElementDesc, u32 numElements) = 0;

			// ジオメトリシェーダー読み込み
			virtual void LoadGS(IDevice* device, const char* csoNameOfGeometryShader) = 0;

			// ピクセルシェーダー読み込み
			virtual void LoadPS(IDevice* device, const char* csoNameOfPixelShader) = 0;

			// シェーダー開始
			virtual void Activate(IDevice* device) = 0;
            virtual void ActivateVS(IDevice* device) = 0;
            virtual void ActivateGS(IDevice* device) = 0;
            virtual void ActivatePS(IDevice* device) = 0;

			// シェーダー終了
			virtual void Deactivate(IDevice* device) = 0;
            virtual void DeactivateVS(IDevice* device) = 0;
            virtual void DeactivateGS(IDevice* device) = 0;
            virtual void DeactivatePS(IDevice* device) = 0;
		};

        //****************************************************************************
        // コンピュートシェーダー操作インターフェース
        //****************************************************************************
        class IComputeShader : public FND::Base
        {
        public:
            // 生成
            static std::unique_ptr<IComputeShader> Create();

            // 初期化
            virtual bool Initialize() = 0;

            // 終了化
            virtual void Finalize() = 0;

            // コンピュートシェーダー読み込み
            virtual void Load(IDevice* device, const char* csoNameOfComputeShader) = 0;

            // シェーダー開始
            virtual void Activate(IDevice* device) = 0;

            // シェーダー終了
            virtual void Deactivate(IDevice* device) = 0;

            // コンピュートシェーダー実行
            virtual void Dispatch(IDevice* device, u32 x, u32 y, u32 z) = 0;

            // コンピュートシェーダー実行
            virtual void DispatchIndirect(IDevice* device, IBuffer* buffer, u32 offset) = 0;
        };
    } // namespace Graphics
} // namespace Bread