#include "bloom.hlsli"


Texture2D hdrColorMap : register(t0);
SamplerState linearBorderSamplerState : register(s0);


float4 main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
    uint mipLevel = 0, width, height, numberOfLevels;
    hdrColorMap.GetDimensions(mipLevel, width, height, numberOfLevels);
    //const float aspectRatio = width / height;

#if 1
	//http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
    const float offset[3] = { 0.0, 1.3846153846, 3.2307692308 };
    const float weight[3] = { 0.2270270270, 0.3162162162, 0.0702702703 };

    float4 fragmentColor = hdrColorMap.Sample(linearBorderSamplerState, texcoord) * weight[0];
    for (int i = 1; i < 3; i++)
    {
        fragmentColor += hdrColorMap.Sample(linearBorderSamplerState, texcoord + float2(0.0, offset[i] / height)) * weight[i];
        fragmentColor += hdrColorMap.Sample(linearBorderSamplerState, texcoord - float2(0.0, offset[i] / height)) * weight[i];
    }
#else
	//https://software.intel.com/en-us/blogs/2014/07/15/an-investigation-of-fast-real-time-gpu-based-image-blur-algorithms
	const float offset[2] = { 0.53805, 2.06278 };
	const float weight[2] = { 0.44908, 0.05092 };
	float4 fragmentColor = 0;
    
	for (int i = 0; i < 2; i++)
	{
		fragmentColor += hdrColorMap.Sample(linearBorderSamplerState, texcoord + float2(0.0, offset[i]) / height) * weight[i];
		fragmentColor += hdrColorMap.Sample(linearBorderSamplerState, texcoord - float2(0.0, offset[i]) / height) * weight[i];
	}
#endif
    return fragmentColor;
}
