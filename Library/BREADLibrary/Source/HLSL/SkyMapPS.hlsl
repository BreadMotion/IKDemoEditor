#define USE_UVW

#include "CommonLayout.hlsli"

typedef VS_OUTPUT PS_INPUT;

#include "CommonCB.hlsli"

// External texture-related data
TextureCube Sky : register(t0);
SamplerState basicSampler : register(s0);

cbuffer CbScene : register(b0)
{
    float4 cbColor;
};


// Entry point for this pixel shader
float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(Sky.Sample(basicSampler, input.uvw).rgb * 1.5f/* * directional_light.colour.w*/, 1.0f);
}