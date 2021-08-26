
#include "EmittedParticle.hlsli"

Texture2D texture0 : register(t0);
Texture2D maskTexture : register(t1);
SamplerState sampler_linear_clamp : register(s0);


float4 main(VertextoPixel input) : SV_TARGET
{
    float4 color = texture0.Sample(sampler_linear_clamp, input.tex);
    float alpha = color.a;
    
    float mask = maskTexture.Sample(sampler_linear_clamp, input.tex).r;
    float thresold = 1.0f - input.life;
    mask = step(thresold, mask);
    
    color.a *= mask;
    
    return color;
}