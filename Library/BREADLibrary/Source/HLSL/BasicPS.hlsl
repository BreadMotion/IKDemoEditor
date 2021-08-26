#define USE_POSITION
#define USE_TEXCOORD0
#include "CommonLayout.hlsli"
#include "ShadowMap.hlsli"

//--------------------------------------------------------------------------------------
// 入力レイアウト
//--------------------------------------------------------------------------------------
typedef VS_OUTPUT PS_INPUT;

//--------------------------------------------------------------------------------------
// 定数バッファ
//--------------------------------------------------------------------------------------
cbuffer CbMaterial : register(b0)
{
    float4 cbColor;
};

//--------------------------------------------------------------------------------------
// リソース
//--------------------------------------------------------------------------------------
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

//--------------------------------------------------------------------------------------
// ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    float4 ShadowPos = float4(input.position, 1.0);
    
	//シャドー空間座標
    ShadowPos = mul(ShadowPos, lightViewProjection);
    ShadowPos.xyz /= ShadowPos.w;
    ShadowPos.y = -ShadowPos.y;
    
	//シャドーマップ空間
    ShadowPos.xy = ShadowPos.xy * 0.5 + 0.5;
    
    float3 shadow = (float3) 1.0;
    if (0.0f < ShadowPos.x && ShadowPos.x < 1.0f && 0.0f < ShadowPos.y && ShadowPos.y < 1.0f)
    {
        //シャードーマップの深度
        float depth = shadowMap.Sample(sampler0, ShadowPos.xy).r;

        if (ShadowPos.z > depth + 0.0005)
        {
            shadow = /*1.0 - (ShadowPos.z - depth) / ShadowPos.z*/0.5f;
        }
    }
    
    return texture0.Sample(sampler0, input.texcoord0) * float4(shadow, 1.0f) * cbColor;
}