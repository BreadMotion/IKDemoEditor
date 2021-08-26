#ifndef COMMON_VS_HLSLI
#define COMMON_VS_HLSLI

#define CB_SCENE	b0
#define CB_MESH		b1
#define CB_BONE		b2
//#define CB_SHADOW	b3

#include "CommonCB.hlsli"
#include "CommonLayout.hlsli"
#include "Skinning.hlsli"

float4x4 inverse(float4x4 m)
{
    float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
    float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
    float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
    float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

    float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    float idet = 1.0f / det;

    float4x4 ret;

    ret[0][0] = t11 * idet;
    ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
    ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
    ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

    ret[1][0] = t12 * idet;
    ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
    ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
    ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

    ret[2][0] = t13 * idet;
    ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
    ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
    ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

    ret[3][0] = t14 * idet;
    ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
    ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
    ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

    return ret;
}

//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------
VS_OUTPUT VSCommon(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

#if defined(USE_BONE)
	input.position.xyz = SkinningPosition(input.position, input.blend_index0, input.blend_index1, input.blend_weight0, input.blend_weight1);
#if defined(USE_NORMAL)
	input.normal = SkinningVector(input.normal, input.blend_index0, input.blend_index1, input.blend_weight0, input.blend_weight1);
#endif
#if defined(USE_TANGENT)
	input.tangent = SkinningVector(input.tangent, input.blend_index0, input.blend_index1, input.blend_weight0, input.blend_weight1);
#endif
#endif
    
    float3 position = mul(input.position, cb_world).xyz;
    output.sv_position = mul(float4(position, 1.0f), cb_view_projection);
	
#if defined(USE_POSITION)
	output.position = position;
#endif
#if defined(USE_NORMAL)
	output.normal = mul(float4(input.normal, 0.0f), cb_world).xyz;
#endif
#if defined(USE_TANGENT)
	output.tangent = mul(float4(input.tangent, 0.0f), cb_world).xyz;
#endif

#if defined(USE_COLOR)
	output.color = input.color;
#endif

#if defined(USE_TEXCOORD0)
	//output.texcoord0 = mul(float4(input.texcoord0.x, 1.0f - input.texcoord0.y, 0.0f, 1.0f), cb_texture).xy;
	output.texcoord0.x = input.texcoord0.x;
	output.texcoord0.y = 1.0f - input.texcoord0.y;
#endif
#if defined(USE_TEXCOORD1)
	output.texcoord1 = float2(input.texcoord1.x, 1.0f - input.texcoord1.y);
#endif
#if defined(USE_TEXCOORD2)
	output.texcoord2 = float2(input.texcoord2.x, 1.0f - input.texcoord2.y);
#endif
#if defined(USE_TEXCOORD3)
	output.texcoord3 = float2(input.texcoord3.x, 1.0f - input.texcoord3.y);
#endif

#if defined(USE_SHADOW)
	output.shadow = output.sv_position.z / output.sv_position.w;
#endif
    return output;
}

#endif