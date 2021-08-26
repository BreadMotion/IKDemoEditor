
Texture2D hdrColorMap : register(t0);

SamplerState pointSamplerState : register(s0);
SamplerState linearSamplerState : register(s1);
SamplerState anisotropicSamplerState : register(s2);


float4 main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
    uint mipLevel = 0, width, height, numberOfLevels;
    
    hdrColorMap.GetDimensions(mipLevel, width, height, numberOfLevels);
    float4 hdrColor = hdrColorMap.Sample(anisotropicSamplerState, texcoord);
    
    return float4(max(0.0, dot(hdrColor.rgb, float3(0.2126, 0.7152, 0.0722))), 0, 0, 1);
}