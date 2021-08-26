
#include "EmitParticle.hlsli"

RWStructuredBuffer<ParticleHeader> particleHeaders : register(u0);
RWStructuredBuffer<EmitterData> emitterData : register(u1);
RWStructuredBuffer<EmitterRange> emitterRange : register(u2);
RWByteAddressBuffer paticleIndexList : register(u3);


[numthreads(TotalEmitterMax, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    uint num = emitterData[id.x].spawnNum;
    uint particleNum = emitterData[id.x].particleNum;
    
    if (num <= 0) return;

    
}