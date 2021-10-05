#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer EmittedParticleCB
// {
//
//   float4x4 xEmitterWorld;            // Offset:    0 Size:    64 [unused]
//   uint xEmitCount;                   // Offset:   64 Size:     4 [unused]
//   uint xEmitterMeshIndexCount;       // Offset:   68 Size:     4 [unused]
//   uint xEmitterMeshVertexPositionStride;// Offset:   72 Size:     4 [unused]
//   float xEmitterRandomness;          // Offset:   76 Size:     4 [unused]
//   float xParticleSize;               // Offset:   80 Size:     4 [unused]
//   float xParticleScaling;            // Offset:   84 Size:     4 [unused]
//   float xParticleRotation;           // Offset:   88 Size:     4 [unused]
//   uint xParticleColor;               // Offset:   92 Size:     4 [unused]
//   float4 xParticleNormal;            // Offset:   96 Size:    16 [unused]
//   float4 xParticleMainColor;         // Offset:  112 Size:    16 [unused]
//   float xParticleRandomFactor;       // Offset:  128 Size:     4 [unused]
//   float xParticleNormalFactor;       // Offset:  132 Size:     4 [unused]
//   float xParticleLifeSpan;           // Offset:  136 Size:     4 [unused]
//   float xParticleLifeSpanRandomness; // Offset:  140 Size:     4 [unused]
//   float xParticleMass;               // Offset:  144 Size:     4 [unused]
//   float xParticleMotionBlurAmount;   // Offset:  148 Size:     4 [unused]
//   float xEmitterOpacity;             // Offset:  152 Size:     4 [unused]
//   uint xEmitterMaxParticleCount;     // Offset:  156 Size:     4 [unused]
//   float xSPH_h;                      // Offset:  160 Size:     4 [unused]
//   float xSPH_h_rcp;                  // Offset:  164 Size:     4 [unused]
//   float xSPH_h2;                     // Offset:  168 Size:     4 [unused]
//   float xSPH_h3;                     // Offset:  172 Size:     4 [unused]
//   float xSPH_poly6_constant;         // Offset:  176 Size:     4 [unused]
//   float xSPH_spiky_constant;         // Offset:  180 Size:     4 [unused]
//   float xSPH_K;                      // Offset:  184 Size:     4 [unused]
//   float xSPH_p0;                     // Offset:  188 Size:     4 [unused]
//   float xSPH_e;                      // Offset:  192 Size:     4 [unused]
//   uint xSPH_ENABLED;                 // Offset:  196 Size:     4 [unused]
//   float xEmitterFixedTimestep;       // Offset:  200 Size:     4
//   float xParticleEmissive;           // Offset:  204 Size:     4 [unused]
//   float xSeed;                       // Offset:  208 Size:     4 [unused]
//   float randU;                       // Offset:  212 Size:     4 [unused]
//   float randV;                       // Offset:  216 Size:     4 [unused]
//   float randW;                       // Offset:  220 Size:     4 [unused]
//
// }
//
// Resource bind info for particleBuffer
// {
//
//   struct Particle
//   {
//       
//       float3 position;               // Offset:    0
//       float mass;                    // Offset:   12
//       float3 force;                  // Offset:   16
//       float rotationalVelocity;      // Offset:   28
//       float3 velocity;               // Offset:   32
//       float maxLife;                 // Offset:   44
//       float2 sizeBeginEnd;           // Offset:   48
//       float life;                    // Offset:   56
//       uint colorMirror;              // Offset:   60
//       float4 motionVelocity;         // Offset:   64
//       float4 originPosition;         // Offset:   80
//
//   } $Element;                        // Offset:    0 Size:    96
//
// }
//
// Resource bind info for aliveBuffer_CURRENT
// {
//
//   uint $Element;                     // Offset:    0 Size:     4
//
// }
//
// Resource bind info for aliveBuffer_NEW
// {
//
//   uint $Element;                     // Offset:    0 Size:     4
//
// }
//
// Resource bind info for deadBuffer
// {
//
//   uint $Element;                     // Offset:    0 Size:     4
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// particleBuffer                        UAV  struct         r/w             u0      1 
// aliveBuffer_CURRENT                   UAV  struct         r/w             u1      1 
// aliveBuffer_NEW                       UAV  struct         r/w             u2      1 
// deadBuffer                            UAV  struct         r/w             u3      1 
// counterBuffer                         UAV    byte         r/w             u4      1 
// EmittedParticleCB                 cbuffer      NA          NA            cb0      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Input
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Output
cs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB0[13], immediateIndexed
dcl_uav_structured u0, 96
dcl_uav_structured u1, 4
dcl_uav_structured u2, 4
dcl_uav_structured u3, 4
dcl_uav_raw u4
dcl_input vThreadID.x
dcl_temps 4
dcl_thread_group 256, 1, 1
ld_raw_indexable(raw_buffer)(mixed,mixed,mixed,mixed) r0.x, l(0), u4.xxxx
ult r0.x, vThreadID.x, r0.x
if_nz r0.x
  ld_structured_indexable(structured_buffer, stride=4)(mixed,mixed,mixed,mixed) r0.x, vThreadID.x, l(0), u1.xxxx
  ld_structured_indexable(structured_buffer, stride=96)(mixed,mixed,mixed,mixed) r0.yzw, r0.x, l(0), u0.xxyz
  ld_structured_indexable(structured_buffer, stride=96)(mixed,mixed,mixed,mixed) r1.xyz, r0.x, l(16), u0.xyzx
  ld_structured_indexable(structured_buffer, stride=96)(mixed,mixed,mixed,mixed) r2.xyzw, r0.x, l(32), u0.xyzw
  ld_structured_indexable(structured_buffer, stride=96)(mixed,mixed,mixed,mixed) r1.w, r0.x, l(56), u0.xxxx
  lt r3.x, l(0.000000), r1.w
  if_nz r3.x
    mul r1.xyz, r1.xyzx, r2.xyzx
    mul r1.xyz, r1.xyzx, cb0[12].zzzz
    mad r3.xyz, r1.xyzx, l(7.250000, 1.250000, 7.250000, 0.000000), r0.yzwy
    add r0.y, -r1.w, r2.w
    mul r0.y, r0.y, r0.y
    mul r0.y, |r2.y|, r0.y
    mad r3.w, -r0.y, l(0.250000), r3.y
    mad r0.y, -cb0[12].z, l(2.000000), r1.w
    mul r0.z, r0.y, l(5.000000)
    store_structured u0.xyz, r0.x, l(0), r3.xwzx
    store_structured u0.xyz, r0.x, l(16), r0.zzzz
    store_structured u0.x, r0.x, l(56), r0.y
    imm_atomic_iadd r1.x, u4, l(12), l(1)
    store_structured u2.x, r1.x, l(0), r0.x
  else 
    imm_atomic_iadd r1.x, u4, l(4), l(1)
    store_structured u3.x, r1.x, l(0), r0.x
  endif 
endif 
ret 
// Approximately 30 instruction slots used
#endif

const BYTE PetalEffectCS[] =
{
     68,  88,  66,  67, 235, 143, 
     44,  87, 149, 142, 105,  87, 
    240, 233,  98,   2,  32, 216, 
     62, 183,   1,   0,   0,   0, 
    248,  17,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
     32,  13,   0,   0,  48,  13, 
      0,   0,  64,  13,   0,   0, 
     92,  17,   0,   0,  82,  68, 
     69,  70, 228,  12,   0,   0, 
      5,   0,   0,   0,  92,   1, 
      0,   0,   6,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
     83,  67,   0,   1,   0,   0, 
    188,  12,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    252,   0,   0,   0,   6,   0, 
      0,   0,   6,   0,   0,   0, 
      1,   0,   0,   0,  96,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  11,   1,   0,   0, 
      6,   0,   0,   0,   6,   0, 
      0,   0,   1,   0,   0,   0, 
      4,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  31,   1, 
      0,   0,   6,   0,   0,   0, 
      6,   0,   0,   0,   1,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     47,   1,   0,   0,   6,   0, 
      0,   0,   6,   0,   0,   0, 
      1,   0,   0,   0,   4,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  58,   1,   0,   0, 
      8,   0,   0,   0,   6,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  72,   1, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
    112,  97, 114, 116, 105,  99, 
    108, 101,  66, 117, 102, 102, 
    101, 114,   0,  97, 108, 105, 
    118, 101,  66, 117, 102, 102, 
    101, 114,  95,  67,  85,  82, 
     82,  69,  78,  84,   0,  97, 
    108, 105, 118, 101,  66, 117, 
    102, 102, 101, 114,  95,  78, 
     69,  87,   0, 100, 101,  97, 
    100,  66, 117, 102, 102, 101, 
    114,   0,  99, 111, 117, 110, 
    116, 101, 114,  66, 117, 102, 
    102, 101, 114,   0,  69, 109, 
    105, 116, 116, 101, 100,  80, 
     97, 114, 116, 105,  99, 108, 
    101,  67,  66,   0, 171, 171, 
     72,   1,   0,   0,  35,   0, 
      0,   0, 212,   1,   0,   0, 
    224,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    252,   0,   0,   0,   1,   0, 
      0,   0,  32,  10,   0,   0, 
     96,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     11,   1,   0,   0,   1,   0, 
      0,   0,  68,  12,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     31,   1,   0,   0,   1,   0, 
      0,   0, 108,  12,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     47,   1,   0,   0,   1,   0, 
      0,   0, 148,  12,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
     76,   7,   0,   0,   0,   0, 
      0,   0,  64,   0,   0,   0, 
      0,   0,   0,   0, 100,   7, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 136,   7, 
      0,   0,  64,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0, 156,   7,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 192,   7,   0,   0, 
     68,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
    156,   7,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    215,   7,   0,   0,  72,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0, 156,   7, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 248,   7, 
      0,   0,  76,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  20,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  56,   8,   0,   0, 
     80,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     20,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     70,   8,   0,   0,  84,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  20,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  87,   8, 
      0,   0,  88,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  20,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 105,   8,   0,   0, 
     92,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
    156,   7,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    120,   8,   0,   0,  96,   0, 
      0,   0,  16,   0,   0,   0, 
      0,   0,   0,   0, 144,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 180,   8, 
      0,   0, 112,   0,   0,   0, 
     16,   0,   0,   0,   0,   0, 
      0,   0, 144,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 199,   8,   0,   0, 
    128,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     20,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    221,   8,   0,   0, 132,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  20,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 243,   8, 
      0,   0, 136,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  20,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,   5,   9,   0,   0, 
    140,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     20,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     33,   9,   0,   0, 144,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  20,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  47,   9, 
      0,   0, 148,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  20,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  73,   9,   0,   0, 
    152,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     20,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     89,   9,   0,   0, 156,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0, 156,   7, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 114,   9, 
      0,   0, 160,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  20,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 121,   9,   0,   0, 
    164,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     20,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    132,   9,   0,   0, 168,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  20,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 140,   9, 
      0,   0, 172,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  20,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 148,   9,   0,   0, 
    176,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     20,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    168,   9,   0,   0, 180,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  20,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 188,   9, 
      0,   0, 184,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  20,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 195,   9,   0,   0, 
    188,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     20,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    203,   9,   0,   0, 192,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  20,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 210,   9, 
      0,   0, 196,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0, 156,   7,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 223,   9,   0,   0, 
    200,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
     20,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    245,   9,   0,   0, 204,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  20,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   7,  10, 
      0,   0, 208,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  20,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  13,  10,   0,   0, 
    212,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
     20,   8,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     19,  10,   0,   0, 216,   0, 
      0,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,  20,   8, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  25,  10, 
      0,   0, 220,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,  20,   8,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 120,  69, 109, 105, 
    116, 116, 101, 114,  87, 111, 
    114, 108, 100,   0, 102, 108, 
    111,  97, 116,  52, 120,  52, 
      0, 171,   3,   0,   3,   0, 
      4,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  90,   7, 
      0,   0, 120,  69, 109, 105, 
    116,  67, 111, 117, 110, 116, 
      0, 100, 119, 111, 114, 100, 
      0, 171, 171, 171,   0,   0, 
     19,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    147,   7,   0,   0, 120,  69, 
    109, 105, 116, 116, 101, 114, 
     77, 101, 115, 104,  73, 110, 
    100, 101, 120,  67, 111, 117, 
    110, 116,   0, 120,  69, 109, 
    105, 116, 116, 101, 114,  77, 
    101, 115, 104,  86, 101, 114, 
    116, 101, 120,  80, 111, 115, 
    105, 116, 105, 111, 110,  83, 
    116, 114, 105, 100, 101,   0, 
    120,  69, 109, 105, 116, 116, 
    101, 114,  82,  97, 110, 100, 
    111, 109, 110, 101, 115, 115, 
      0, 102, 108, 111,  97, 116, 
      0, 171, 171, 171,   0,   0, 
      3,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     11,   8,   0,   0, 120,  80, 
     97, 114, 116, 105,  99, 108, 
    101,  83, 105, 122, 101,   0, 
    120,  80,  97, 114, 116, 105, 
     99, 108, 101,  83,  99,  97, 
    108, 105, 110, 103,   0, 120, 
     80,  97, 114, 116, 105,  99, 
    108, 101,  82, 111, 116,  97, 
    116, 105, 111, 110,   0, 120, 
     80,  97, 114, 116, 105,  99, 
    108, 101,  67, 111, 108, 111, 
    114,   0, 120,  80,  97, 114, 
    116, 105,  99, 108, 101,  78, 
    111, 114, 109,  97, 108,   0, 
    102, 108, 111,  97, 116,  52, 
      0, 171,   1,   0,   3,   0, 
      1,   0,   4,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 136,   8, 
      0,   0, 120,  80,  97, 114, 
    116, 105,  99, 108, 101,  77, 
     97, 105, 110,  67, 111, 108, 
    111, 114,   0, 120,  80,  97, 
    114, 116, 105,  99, 108, 101, 
     82,  97, 110, 100, 111, 109, 
     70,  97,  99, 116, 111, 114, 
      0, 120,  80,  97, 114, 116, 
    105,  99, 108, 101,  78, 111, 
    114, 109,  97, 108,  70,  97, 
     99, 116, 111, 114,   0, 120, 
     80,  97, 114, 116, 105,  99, 
    108, 101,  76, 105, 102, 101, 
     83, 112,  97, 110,   0, 120, 
     80,  97, 114, 116, 105,  99, 
    108, 101,  76, 105, 102, 101, 
     83, 112,  97, 110,  82,  97, 
    110, 100, 111, 109, 110, 101, 
    115, 115,   0, 120,  80,  97, 
    114, 116, 105,  99, 108, 101, 
     77,  97, 115, 115,   0, 120, 
     80,  97, 114, 116, 105,  99, 
    108, 101,  77, 111, 116, 105, 
    111, 110,  66, 108, 117, 114, 
     65, 109, 111, 117, 110, 116, 
      0, 120,  69, 109, 105, 116, 
    116, 101, 114,  79, 112,  97, 
     99, 105, 116, 121,   0, 120, 
     69, 109, 105, 116, 116, 101, 
    114,  77,  97, 120,  80,  97, 
    114, 116, 105,  99, 108, 101, 
     67, 111, 117, 110, 116,   0, 
    120,  83,  80,  72,  95, 104, 
      0, 120,  83,  80,  72,  95, 
    104,  95, 114,  99, 112,   0, 
    120,  83,  80,  72,  95, 104, 
     50,   0, 120,  83,  80,  72, 
     95, 104,  51,   0, 120,  83, 
     80,  72,  95, 112, 111, 108, 
    121,  54,  95,  99, 111, 110, 
    115, 116,  97, 110, 116,   0, 
    120,  83,  80,  72,  95, 115, 
    112, 105, 107, 121,  95,  99, 
    111, 110, 115, 116,  97, 110, 
    116,   0, 120,  83,  80,  72, 
     95,  75,   0, 120,  83,  80, 
     72,  95, 112,  48,   0, 120, 
     83,  80,  72,  95, 101,   0, 
    120,  83,  80,  72,  95,  69, 
     78,  65,  66,  76,  69,  68, 
      0, 120,  69, 109, 105, 116, 
    116, 101, 114,  70, 105, 120, 
    101, 100,  84, 105, 109, 101, 
    115, 116, 101, 112,   0, 120, 
     80,  97, 114, 116, 105,  99, 
    108, 101,  69, 109, 105, 115, 
    115, 105, 118, 101,   0, 120, 
     83, 101, 101, 100,   0, 114, 
     97, 110, 100,  85,   0, 114, 
     97, 110, 100,  86,   0, 114, 
     97, 110, 100,  87,   0, 171, 
     72,  10,   0,   0,   0,   0, 
      0,   0,  96,   0,   0,   0, 
      2,   0,   0,   0,  32,  12, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  36,  69, 
    108, 101, 109, 101, 110, 116, 
      0,  80,  97, 114, 116, 105, 
     99, 108, 101,   0, 112, 111, 
    115, 105, 116, 105, 111, 110, 
      0, 102, 108, 111,  97, 116, 
     51,   0, 171, 171,   1,   0, 
      3,   0,   1,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     99,  10,   0,   0, 109,  97, 
    115, 115,   0, 171, 171, 171, 
      0,   0,   3,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  11,   8,   0,   0, 
    102, 111, 114,  99, 101,   0, 
    114, 111, 116,  97, 116, 105, 
    111, 110,  97, 108,  86, 101, 
    108, 111,  99, 105, 116, 121, 
      0, 118, 101, 108, 111,  99, 
    105, 116, 121,   0, 109,  97, 
    120,  76, 105, 102, 101,   0, 
    115, 105, 122, 101,  66, 101, 
    103, 105, 110,  69, 110, 100, 
      0, 102, 108, 111,  97, 116, 
     50,   0, 171, 171,   1,   0, 
      3,   0,   1,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    243,  10,   0,   0, 108, 105, 
    102, 101,   0,  99, 111, 108, 
    111, 114,  77, 105, 114, 114, 
    111, 114,   0, 171, 171, 171, 
      0,   0,  19,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 147,   7,   0,   0, 
    109, 111, 116, 105, 111, 110, 
     86, 101, 108, 111,  99, 105, 
    116, 121,   0, 171,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    136,   8,   0,   0, 111, 114, 
    105, 103, 105, 110,  80, 111, 
    115, 105, 116, 105, 111, 110, 
      0, 171,  90,  10,   0,   0, 
    108,  10,   0,   0,   0,   0, 
      0,   0, 144,  10,   0,   0, 
    152,  10,   0,   0,  12,   0, 
      0,   0, 188,  10,   0,   0, 
    108,  10,   0,   0,  16,   0, 
      0,   0, 194,  10,   0,   0, 
    152,  10,   0,   0,  28,   0, 
      0,   0, 213,  10,   0,   0, 
    108,  10,   0,   0,  32,   0, 
      0,   0, 222,  10,   0,   0, 
    152,  10,   0,   0,  44,   0, 
      0,   0, 230,  10,   0,   0, 
    252,  10,   0,   0,  48,   0, 
      0,   0,  32,  11,   0,   0, 
    152,  10,   0,   0,  56,   0, 
      0,   0,  37,  11,   0,   0, 
     52,  11,   0,   0,  60,   0, 
      0,   0,  88,  11,   0,   0, 
    104,  11,   0,   0,  64,   0, 
      0,   0, 140,  11,   0,   0, 
    104,  11,   0,   0,  80,   0, 
      0,   0,   5,   0,   0,   0, 
      1,   0,  24,   0,   0,   0, 
     11,   0, 156,  11,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  81,  10, 
      0,   0,  72,  10,   0,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
     52,  11,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     72,  10,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0,  52,  11, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  72,  10, 
      0,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0,  52,  11,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  49,  48,  46,  49,   0, 
     73,  83,  71,  78,   8,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  79,  83, 
     71,  78,   8,   0,   0,   0, 
      0,   0,   0,   0,   8,   0, 
      0,   0,  83,  72,  69,  88, 
     20,   4,   0,   0,  80,   0, 
      5,   0,   5,   1,   0,   0, 
    106,   8,   0,   1,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      0,   0,   0,   0,  13,   0, 
      0,   0, 158,   0,   0,   4, 
      0, 224,  17,   0,   0,   0, 
      0,   0,  96,   0,   0,   0, 
    158,   0,   0,   4,   0, 224, 
     17,   0,   1,   0,   0,   0, 
      4,   0,   0,   0, 158,   0, 
      0,   4,   0, 224,  17,   0, 
      2,   0,   0,   0,   4,   0, 
      0,   0, 158,   0,   0,   4, 
      0, 224,  17,   0,   3,   0, 
      0,   0,   4,   0,   0,   0, 
    157,   0,   0,   3,   0, 224, 
     17,   0,   4,   0,   0,   0, 
     95,   0,   0,   2,  18,   0, 
      2,   0, 104,   0,   0,   2, 
      4,   0,   0,   0, 155,   0, 
      0,   4,   0,   1,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 165,   0,   0, 137, 
    194,   2,   0, 128, 131, 153, 
     25,   0,  18,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 224,  17,   0,   4,   0, 
      0,   0,  79,   0,   0,   6, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,   2,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  31,   0,   4,   3, 
     10,   0,  16,   0,   0,   0, 
      0,   0, 167,   0,   0, 138, 
      2,  35,   0, 128, 131, 153, 
     25,   0,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
      2,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,   6, 224, 
     17,   0,   1,   0,   0,   0, 
    167,   0,   0, 139,   2,   3, 
      3, 128, 131, 153,  25,   0, 
    226,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
      6, 233,  17,   0,   0,   0, 
      0,   0, 167,   0,   0, 139, 
      2,   3,   3, 128, 131, 153, 
     25,   0, 114,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,  16,   0, 
      0,   0,  70, 226,  17,   0, 
      0,   0,   0,   0, 167,   0, 
      0, 139,   2,   3,   3, 128, 
    131, 153,  25,   0, 242,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
     32,   0,   0,   0,  70, 238, 
     17,   0,   0,   0,   0,   0, 
    167,   0,   0, 139,   2,   3, 
      3, 128, 131, 153,  25,   0, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,  56,   0,   0,   0, 
      6, 224,  17,   0,   0,   0, 
      0,   0,  49,   0,   0,   7, 
     18,   0,  16,   0,   3,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     31,   0,   4,   3,  10,   0, 
     16,   0,   3,   0,   0,   0, 
     56,   0,   0,   7, 114,   0, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,  56,   0, 
      0,   8, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
    166, 138,  32,   0,   0,   0, 
      0,   0,  12,   0,   0,   0, 
     50,   0,   0,  12, 114,   0, 
     16,   0,   3,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 232,  64,   0,   0, 
    160,  63,   0,   0, 232,  64, 
      0,   0,   0,   0, 150,   7, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,   8,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16, 128,  65,   0, 
      0,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   2,   0, 
      0,   0,  56,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   8,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16, 128, 129,   0, 
      0,   0,   2,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  10, 
    130,   0,  16,   0,   3,   0, 
      0,   0,  26,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  62,  26,   0, 
     16,   0,   3,   0,   0,   0, 
     50,   0,   0,  11,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     42, 128,  32, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
     12,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,  64, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  56,   0,   0,   7, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 160,  64, 
    168,   0,   0,   9, 114, 224, 
     17,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0, 198,   2, 
     16,   0,   3,   0,   0,   0, 
    168,   0,   0,   9, 114, 224, 
     17,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
     16,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
    168,   0,   0,   9,  18, 224, 
     17,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
     56,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
    180,   0,   0,   9,  18,   0, 
     16,   0,   1,   0,   0,   0, 
      0, 224,  17,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
     12,   0,   0,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
    168,   0,   0,   9,  18, 224, 
     17,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     18,   0,   0,   1, 180,   0, 
      0,   9,  18,   0,  16,   0, 
      1,   0,   0,   0,   0, 224, 
     17,   0,   4,   0,   0,   0, 
      1,  64,   0,   0,   4,   0, 
      0,   0,   1,  64,   0,   0, 
      1,   0,   0,   0, 168,   0, 
      0,   9,  18, 224,  17,   0, 
      3,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  21,   0, 
      0,   1,  21,   0,   0,   1, 
     62,   0,   0,   1,  83,  84, 
     65,  84, 148,   0,   0,   0, 
     30,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,  10,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   2,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      6,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
      5,   0,   0,   0
};
