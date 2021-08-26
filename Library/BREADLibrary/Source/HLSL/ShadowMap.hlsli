
cbuffer CBShadow : register(b3)
{
    float4x4 lightViewProjection;
    float3 shadowColor;
    float shadowBias;
};

Texture2D shadowMap : register(t8);
SamplerComparisonState comparisonSamplerState : register(s3);

float Shadow(float4 positionInLightSpace)
{
	//http://developer.download.nvidia.com/presentations/2008/GDC/GDC08_SoftShadowMapping.pdf
    
    float shadow_threshold = 0.0f;
    shadow_threshold = shadowMap.SampleCmpLevelZero(comparisonSamplerState, positionInLightSpace.xy, positionInLightSpace.z - shadowBias).r;
#if 0
    int offset = 2;
    shadow_threshold += shadowMap.SampleCmpLevelZero(comparisonSamplerState, positionInLightSpace.xy, positionInLightSpace.z - shadowBias, int2(-offset, -offset)) * 1;
    shadow_threshold += shadowMap.SampleCmpLevelZero(comparisonSamplerState, positionInLightSpace.xy, positionInLightSpace.z - shadowBias, int2(-offset, 0)) * 2;
    shadow_threshold += shadowMap.SampleCmpLevelZero(comparisonSamplerState, positionInLightSpace.xy, positionInLightSpace.z - shadowBias, int2(-offset, offset)) * 1;
    shadow_threshold += shadowMap.SampleCmpLevelZero(comparisonSamplerState, positionInLightSpace.xy, positionInLightSpace.z - shadowBias, int2(0, -offset)) * 2;
    shadow_threshold += shadowMap.SampleCmpLevelZero(comparisonSamplerState, positionInLightSpace.xy, positionInLightSpace.z - shadowBias, int2(0, 0)) * 4;
    shadow_threshold += shadowMap.SampleCmpLevelZero(comparisonSamplerState, positionInLightSpace.xy, positionInLightSpace.z - shadowBias, int2(0, offset)) * 2;
    shadow_threshold += shadowMap.SampleCmpLevelZero(comparisonSamplerState, positionInLightSpace.xy, positionInLightSpace.z - shadowBias, int2(offset, -offset)) * 1;
    shadow_threshold += shadowMap.SampleCmpLevelZero(comparisonSamplerState, positionInLightSpace.xy, positionInLightSpace.z - shadowBias, int2(offset, 0)) * 2;
    shadow_threshold += shadowMap.SampleCmpLevelZero(comparisonSamplerState, positionInLightSpace.xy, positionInLightSpace.z - shadowBias, int2(offset, offset)) * 1;
    shadow_threshold /= 16;
#endif

    return lerp(shadowColor, float3(1.0f, 1.0f, 1.0f), shadow_threshold).x;
}