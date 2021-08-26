
Texture2D originTexture : register(t0);
Texture2D bloomTexture : register(t1);

SamplerState pointSamplerState : register(s0);
SamplerState linearSamplerState : register(s1);
SamplerState anisotropicSamplerState : register(s2);


float4 main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
    return originTexture.Sample(pointSamplerState, texcoord) + bloomTexture.Sample(pointSamplerState, texcoord);
}