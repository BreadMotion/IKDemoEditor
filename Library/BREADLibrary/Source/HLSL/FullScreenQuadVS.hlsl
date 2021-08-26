
struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};


VS_OUT main(in uint vertexID : SV_VERTEXID)
{
	VS_OUT vout;
	
    vout.texcoord = float2((vertexID << 1) & 2, vertexID & 2);
	vout.position = float4(vout.texcoord * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
	
	return vout;
}