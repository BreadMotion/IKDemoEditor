
 #define sizeX 32
 #define sizeY 1
 #define sizeZ 1

struct CSInput
{
    uint3 groupThread : SV_GroupThreadID;
    uint3 group : SV_GroupID;
    uint groupIndex : SV_GroupIndex;
    uint3 dispatch : SV_DispatchThreadID;
};

struct ParticleParameter
{
    float3 pos;
};

StructuredBuffer<ParticleParameter> InputBuffer : register(t0);
RWStructuredBuffer<ParticleParameter> OutputBuffer : register(u0);

[numthreads(sizeX, sizeY, sizeZ)]
void main(const CSInput input)
{
    int index = input.dispatch.x;
    
    OutputBuffer[index].pos.x += float(index);
}