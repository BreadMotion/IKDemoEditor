
Texture2D textureMap : register( t0 );
SamplerState samplerState : register( s0 );

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
};

float4 main(VS_OUT pin) : SV_TARGET
{
    return textureMap.Sample(samplerState, pin.texcoord) * pin.color;
}