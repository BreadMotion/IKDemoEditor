
#include "EmitParticle.hlsli"

RWByteAddressBuffer indirectArgs : register(u0);


[numthreads(1, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    indirectArgs.Store(IA_PARTICLE_COUNTER, 0);
    indirectArgs.Store(IA_PREV_PARTICLE_COUNTER, 0);
}