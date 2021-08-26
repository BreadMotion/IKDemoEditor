#include "bloom.hlsli"


Texture2D hdrColorMap : register(t0);
SamplerState linearBorderSamplerState : register(s0);


float4 main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
    return hdrColorMap.Sample(linearBorderSamplerState, texcoord, 0.0);
}
