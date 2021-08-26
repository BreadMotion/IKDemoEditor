
struct Particle
{
    float3 position;
    float mass;
    float3 force;
    float rotationalVelocity;
    float3 velocity;
    float maxLife;
    float2 sizeBeginEnd;
    float life;
    uint colorMirror;
    float4 motionVelocity;
    float4 originPosition;
};

struct ParticleCounters
{
    uint aliveCount;
    uint deadCount;
    uint realEmitCount;
    uint aliveCountAfterSimulation;
};
static const uint PARTICLECOUNTER_OFFSET_ALIVECOUNT = 0;
static const uint PARTICLECOUNTER_OFFSET_DEADCOUNT = PARTICLECOUNTER_OFFSET_ALIVECOUNT + 4;
static const uint PARTICLECOUNTER_OFFSET_REALEMITCOUNT = PARTICLECOUNTER_OFFSET_DEADCOUNT + 4;
static const uint PARTICLECOUNTER_OFFSET_ALIVECOUNT_AFTERSIMULATION = PARTICLECOUNTER_OFFSET_REALEMITCOUNT + 4;


cbuffer EmittedParticleCB : register(b0)
{
    float4x4 xEmitterWorld;
    
    uint xEmitCount;
    uint xEmitterMeshIndexCount;
    uint xEmitterMeshVertexPositionStride;
    float xEmitterRandomness;
    
    float xParticleSize;
    float xParticleScaling;
    float xParticleRotation;
    uint xParticleColor;
    
    float4 xParticleNormal;
    float4 xParticleMainColor;
    
    float xParticleRandomFactor;
    float xParticleNormalFactor;
    float xParticleLifeSpan;
    float xParticleLifeSpanRandomness;
    
    float xParticleMass;
    float xParticleMotionBlurAmount;
    float xEmitterOpacity;
    uint xEmitterMaxParticleCount;
    
    float xSPH_h; // smoothing radius
    float xSPH_h_rcp; // 1.0f / smoothing radius
    float xSPH_h2; // smoothing radius ^ 2
    float xSPH_h3; // smoothing radius ^ 3
    
    float xSPH_poly6_constant; // precomputed Poly6 kernel constant term
    float xSPH_spiky_constant; // precomputed Spiky kernel function constant term
    float xSPH_K; // pressure constant
    float xSPH_p0; // reference density
    
    float xSPH_e; // viscosity constant
    uint xSPH_ENABLED; // is SPH enabled?
    float xEmitterFixedTimestep; // we can force a fixed timestep (>0) onto the simulation to avoid blowing up
    float xParticleEmissive;
    
    float xSeed;
    float randU;
    float randV;
    float randW;
};

cbuffer FrameTimeCB : register(b1)
{
    float frameTime;
    float3 dummy;
};

#define THREADCOUNT_EMIT 256
#define THREADCOUNT_SIMULATION 256

static const uint ARGUMENTBUFFER_OFFSET_DISPATCHEMIT = 0;
static const uint ARGUMENTBUFFER_OFFSET_DISPATCHSIMULATION = ARGUMENTBUFFER_OFFSET_DISPATCHEMIT + (3 * 4);
static const uint ARGUMENTBUFFER_OFFSET_DRAWPARTICLES = ARGUMENTBUFFER_OFFSET_DISPATCHSIMULATION + (3 * 4);

struct VertextoPixel
{
    float4 pos : SV_POSITION;
    float4 pos2D : TEXCOORD0;
    float2 tex : TEXCOORD1;
    nointerpolation float size : TEXCOORD2;
    nointerpolation uint color : TEXCOORD3;
    float life : LIFE;
    float3 P : WORLDPOSITION;
};

// returns a random float in range (0, 1). seed must be >0!
inline float rand(inout float seed, in float2 uv)
{
    float result = frac(sin(seed * dot(uv, float2(12.9898f, 78.233f))) * 43758.5453f);
    seed += 1.0f;
    return result;
}

// https://www.shadertoy.com/view/llGSzw
float hash1(uint n)
{
	// integer hash copied from Hugo Elias
    n = (n << 13U) ^ n;
    n = n * (n * n * 15731U + 789221U) + 1376312589U;
    return float(n & 0x7fffffffU) / float(0x7fffffff);
}