
#include "EmitParticle.hlsli"

RWByteAddressBuffer indirectArgs : register(u0);


[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint prePtCount = indirectArgs.Load(IA_PARTICLE_COUNTER);
    uint curPtCount = min(prePtCount + TotalSpawnCount, TotalParticleMax);

    indirectArgs.Store(IA_PARTICLE_COUNTER, curPtCount);
    indirectArgs.Store(IA_PREV_PARTICLE_COUNTER, prePtCount);
}