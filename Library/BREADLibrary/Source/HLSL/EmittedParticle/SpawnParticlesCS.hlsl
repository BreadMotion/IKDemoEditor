
#include "EmitParticle.hlsli"

RWStructuredBuffer<ParticleHeader> particleHeaders : register(u0);
RWStructuredBuffer<EmitterData> emitterData : register(u1);
RWStructuredBuffer<EmitterRange> emitterRange : register(u2);


[numthreads(TotalEmitterMax, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    uint head = emitterData[id.x].spawnHead;
    uint num = emitterData[id.x].spawnNum;
    
    for (uint i = 0; i < num; ++i)
    {
        particleHeaders[i].tag = INVALID_TAG;
    }
    
    emitterRange[id.x].aliveHead = 0;
    emitterRange[id.x].aliveEnd = 0;
    emitterRange[id.x].deadHead = 0;
    emitterRange[id.x].deadEnd = 0;
}