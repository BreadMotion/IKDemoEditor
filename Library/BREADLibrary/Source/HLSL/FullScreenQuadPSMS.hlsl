
Texture2DMS<float4> textureMap : register(t0);


struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};


float4 main(VS_OUT pin, uint sampleIndex : SV_SAMPLEINDEX) : SV_TARGET
{
	uint width, height, numberOfSamples;
	
    textureMap.GetDimensions(width, height, numberOfSamples);
	
    return textureMap.Load(uint2(pin.texcoord.x * width, pin.texcoord.y * height), sampleIndex);
}