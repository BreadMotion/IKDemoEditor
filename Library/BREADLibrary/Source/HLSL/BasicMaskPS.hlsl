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

cbuffer DissolveCB : register(b1)
{
    float dissolveThreshold; //透過閾値
    float dissolveEmissiveWidth; //発光閾値(ディゾルブ・エミッシブ)
    float2 dummy;
}

//--------------------------------------------------------------------------------------
// リソース
//--------------------------------------------------------------------------------------
Texture2D texture0 : register(t0);
Texture2D maskTexture : register(t10);
SamplerState sampler0 : register(s0);

//--------------------------------------------------------------------------------------
// ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    float4 ShadowPos = float4(input.position, 1.0);

    //マスクの明暗
    float mask = maskTexture.Sample(sampler0, input.texcoord0).r;

    //閾値がマスクの値を超えていたら完全透過、超えていなかったら不透過の2値判定
    //しきい値未満の場合は0.0を、それ以上の場合は1.0の二値化を行う関数
    float thresold = 1.0f - dissolveThreshold;
    float alpha = step(mask, thresold);

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

    float4 color = texture0.Sample(sampler0, input.texcoord0) * float4(shadow, 1.0f) * cbColor;
    color.a *= alpha;

    return color;
}