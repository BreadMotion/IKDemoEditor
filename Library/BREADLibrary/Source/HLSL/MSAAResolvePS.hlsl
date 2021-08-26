
struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};


Texture2DMS<float> depthMap  : register(t0);


float main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD0) : SV_DEPTH
{
	uint width, height, numberOfSamples;
	
    depthMap.GetDimensions(width, height, numberOfSamples);
	
    return depthMap.Load(uint2(texcoord.x * width, texcoord.y * height), 0).r;
}