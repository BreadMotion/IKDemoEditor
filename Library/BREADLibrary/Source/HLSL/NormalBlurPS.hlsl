#define kBlockK		(20)

Texture2D texColor : register(t0);
Texture2D texVelocity : register(t1);

SamplerState pointSamplerState : register(s0);
SamplerState linearSamplerState : register(s1);
SamplerState anisotropicSamplerState : register(s2);

//--------------------------------------------------------------------------------------
// 定数バッファ
//--------------------------------------------------------------------------------------
cbuffer CbBlur : register(b0)
{
    uint loop;
    float softZExtent;
    float div;  // 1.0 / (loop + 1)
    int jitter; // ジッタのON/OFF
    int mix;    // 速度のミックスを行うかどうか
    float3 padding;
};

struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

float2 ReconstVel(float2 tv)
{
    return (tv * 2.0 - 1.0) * kBlockK;
}


float4 main(VS_OUT pin) : SV_TARGET
{
    uint width, height;
    texColor.GetDimensions(width, height);
    const float2 uvPerPix = float2(1.0 / (float) width, 1.0 / (float) height);
    const float2 halfPix = float2(0.5, 0.5);
    const float halfPixLenSq = dot(halfPix, halfPix);

    float2 vel = ReconstVel(texVelocity.SampleLevel(linearSamplerState, pin.texcoord, 0).xy);
    float lenSq = dot(vel, vel);
    if (lenSq <= halfPixLenSq)
    {
        return texColor.SampleLevel(pointSamplerState, pin.texcoord, 0);
    }
    vel *= uvPerPix;
    float2 uvHalfPix = halfPix * uvPerPix;

    float4 color = (float4) 0;
	[loop]
    for (uint i = 0; i < loop; ++i)
    {
        float t = lerp(-1.0, 1.0, (float) (i + 1) * div);
        float2 Y = pin.texcoord + vel * t + uvHalfPix;
        color += texColor.SampleLevel(pointSamplerState, Y, 0);
    }
    color /= (float)loop;
    return color;
}