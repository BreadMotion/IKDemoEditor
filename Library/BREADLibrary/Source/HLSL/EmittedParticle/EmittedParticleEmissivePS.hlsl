
#include "EmittedParticle.hlsli"

Texture2D<float4> texture0 : register(t0);
SamplerState sampler_linear_clamp : register(s0);


float4 main(VertextoPixel input) : SV_TARGET
{
    float4 color = texture0.Sample(sampler_linear_clamp, input.tex);
    float alpha = color.a;
    
    color *= xParticleMainColor;
    color.xyz *= alpha * 5.0f;
    return color;
}