#include "bloom.hlsli"


Texture2D downsampledTextures[numberOfDownsampled] : register(t0);

SamplerState pointSamplerState : register(s0);
SamplerState linearSamplerState : register(s1);
SamplerState anisotropicSamplerState : register(s2);


float4 main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
    float3 fragmentColor = 0;
    
	[unroll]
    for (uint indexOfDownsampled = 0; indexOfDownsampled < numberOfDownsampled; ++indexOfDownsampled)
    {
        fragmentColor += downsampledTextures[indexOfDownsampled].Sample(anisotropicSamplerState, texcoord).xyz;
    }
    
    return float4(fragmentColor * blurConvolutionIntensity, 1);
}