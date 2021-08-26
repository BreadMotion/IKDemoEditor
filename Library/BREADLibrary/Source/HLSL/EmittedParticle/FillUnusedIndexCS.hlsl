
#include "EmitParticle.hlsli"

RWByteAddressBuffer indirectArgs : register(u0);
RWByteAddressBuffer paticleIndexList : register(u1);


[numthreads(1, 1, 1)]
void main()
{
    uint spawnCount = currentEmitterSpawnCount;
    
    for (uint i = 0; i < spawnCount; ++i)
    {
        paticleIndexList.Store(previousEmitterSpawnCount + i, INVALID_TAG);
    }
}