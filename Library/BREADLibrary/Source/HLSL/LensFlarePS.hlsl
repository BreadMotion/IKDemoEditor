#include "bloom.hlsli"


Texture2D hdrColorMap : register(t0);
Texture2D gradientMap : register(t1);
Texture2D noiseMap : register(t2);

SamplerState pointSamplerState : register(s0);
SamplerState linearSamplerState : register(s1);
SamplerState anisotropicSamplerState : register(s2);


//https://john-chapman.github.io/2017/11/05/pseudo-lens-flare.html
float4 main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
    float2 mimicTexcoord = -texcoord + float2(1, 1);

    uint mipLevel = 0, width, height, numberOfLevels;
    hdrColorMap.GetDimensions(mipLevel, width, height, numberOfLevels);
    float2 texelSize = 1.0 / float2(width, height);

	//ghost vector to image centre
    float2 ghostVector = float2(0.5 - mimicTexcoord.x, 0.5 - mimicTexcoord.y) * lensFlareGhostDispersal;

	// sample ghosts;
    float3 fragmentColor = 0;

	//int numberOfGhosts = 15; //number of ghost samples
    for (int i = 0; i < numberOfGhosts; ++i)
    {
        float2 offset = mimicTexcoord + ghostVector * i;
        float3 colour = hdrColorMap.Sample(anisotropicSamplerState, offset).rgb;
        colour = max(colour - lensFlareThreshold, 0);
        
#if 1
        float d = distance(offset, float2(0.5, 0.5));
        float weight = 1.0 - smoothstep(0.0, 0.75, d); // reduce contributions from samples at the screen edge
            
        fragmentColor += colour * weight;
#else
		fragmentColor += colour;
#endif
    }
    
    float luminance = dot(fragmentColor.xyz, float3(0.2126, 0.7152, 0.0722));
    fragmentColor = luminance;
    fragmentColor *= gradientMap.Sample(anisotropicSamplerState, length(float2(0.5, 0.5) - mimicTexcoord) / length(float2(0.5, 0.5))).rgb;
    fragmentColor *= noiseMap.Sample(anisotropicSamplerState, texcoord).r;
	
    return float4(fragmentColor * lensFlareIntensity, 1.0);
}