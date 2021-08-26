
#include "EmitParticle.hlsli"

RWStructuredBuffer<ParticleHeader> particleHeaders : register(u0);


[numthreads(PARTICLE_PER_THREAD, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
    if (TotalParticleMax <= id.x) return;
    particleHeaders[id.x].tag = INVALID_TAG;
    particleHeaders[id.x].depth = 0.0f;
}