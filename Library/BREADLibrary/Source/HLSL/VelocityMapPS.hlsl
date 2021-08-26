#define USE_POSITION
#define CB_SCENE	b0
#define kBlockK		(20)

#include "CommonCB.hlsli"
#include "CommonLayout.hlsli"

//--------------------------------------------------------------------------------------
// 入力レイアウト
//--------------------------------------------------------------------------------------
typedef VS_OUTPUT PS_INPUT;

//--------------------------------------------------------------------------------------
// 定数バッファ
//--------------------------------------------------------------------------------------
cbuffer CbVelocity : register(b1)
{
    float exposureTime;
    float screenWidth;
    float screenHeight;
    float frameRate;
};

float4 main(PS_INPUT input) : SV_Target
{
    float4 currentPos = mul(float4(input.position, 1.0f), cb_view_projection);
    float4 lastPos = mul(float4(input.position, 1.0f), cb_prev_view_projection);
    
	// (c * (0.5, -0.5) + 0.5) - (l * (0.5, -0.5) + 0.5)
	// = c * (0.5, -0.5) - l * (0.5, -0.5) = (c - l) * (0.5, -0.5)
    float2 current = currentPos.xy / currentPos.w;
    float2 last = lastPos.xy / lastPos.w;
    float2 vel = (current - last) * float2(0.5f, -0.5f) * float2(screenWidth, screenHeight); // pixelに変換する

    const float kFrameRate = frameRate;
    const float kEpsilon = 1e-4f;
    
    vel *= (0.5f * exposureTime * kFrameRate);
    
    float len = length(vel);
    float2 halfPix = float2(0.5f, 0.5f);
    float halfPixLen = length(halfPix);
    float2 KPix = float2(kBlockK, kBlockK);
    float KPixLen = kBlockK;
    
    vel *= max(halfPixLen, min(len, KPixLen));
    vel /= (len + kEpsilon);
    vel = (vel / KPix) * 0.5f + 0.5f;

    return float4(vel, 0.0f, 1.0f);
}