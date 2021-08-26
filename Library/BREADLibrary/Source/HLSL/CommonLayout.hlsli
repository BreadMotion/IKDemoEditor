#ifndef COMMON_LAYOUT_HLSLI
#define COMMON_LAYOUT_HLSLI

//--------------------------------------------------------------------------------------
// 頂点入力レイアウト
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 position : POSITION;
#if defined(USE_NORMAL)
	float3 normal       : NORMAL;
#endif
#if defined(USE_TANGENT)
	float3 tangent      : TANGENT;
#endif
#if defined(USE_COLOR)
	float4 color        : COLOR;
#endif
#if defined(USE_TEXCOORD0)
	float2 texcoord0    : TEXCOORD0;
#endif
#if defined(USE_TEXCOORD1)
	float2 texcoord1    : TEXCOORD1;
#endif
#if defined(USE_TEXCOORD2)
	float2 texcoord2    : TEXCOORD2;
#endif
#if defined(USE_TEXCOORD3)
	float2 texcoord3    : TEXCOORD3;
#endif
#if defined(USE_BONE)
	float4 blend_weight0 : BLENDWEIGHT0;
	float4 blend_weight1 : BLENDWEIGHT1;
	uint4  blend_index0  : BLENDINDICES0;
	uint4  blend_index1  : BLENDINDICES1;
#endif
};

//--------------------------------------------------------------------------------------
// 頂点出力レイアウト
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 sv_position : SV_POSITION;
#if defined(USE_POSITION)
	float3 position     : POSITION;
#endif
#if defined(USE_NORMAL)
	float3 normal       : NORMAL;
#endif
#if defined(USE_TANGENT)
	float3 tangent      : TANGENT;
#endif
#if defined(USE_COLOR)
	float4 color        : COLOR;
#endif
#if defined(USE_TEXCOORD0)
	float2 texcoord0    : TEXCOORD0;
#endif
#if defined(USE_TEXCOORD1)
	float2 texcoord1    : TEXCOORD1;
#endif
#if defined(USE_TEXCOORD2)
	float2 texcoord2    : TEXCOORD2;
#endif
#if defined(USE_TEXCOORD3)
	float2 texcoord3    : TEXCOORD3;
#endif
#if defined(USE_UVW)
	float3 uvw          : UVW;
#endif
#if defined(USE_SHADOW)
	float4 shadow       : TEXCOORD4;
#endif
};

#endif