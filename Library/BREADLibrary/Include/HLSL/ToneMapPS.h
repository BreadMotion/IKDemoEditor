#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer ConstantBuffer
// {
//
//   float whitePoint;                  // Offset:    0 Size:     4
//      = 0x40400000 
//   float averageGray;                 // Offset:    4 Size:     4
//      = 0x3f92d0e5 
//   float brightness;                  // Offset:    8 Size:     4
//      = 0x00000000 
//   float contrast;                    // Offset:   12 Size:     4
//      = 0x00000000 
//   float hue;                         // Offset:   16 Size:     4
//      = 0x00000000 
//   float saturation;                  // Offset:   20 Size:     4
//      = 0x00000000 
//   float amount;                      // Offset:   24 Size:     4
//      = 0x00000000 
//   float offset;                      // Offset:   28 Size:     4
//      = 0x00000000 
//   float darkness;                    // Offset:   32 Size:     4
//      = 0x00000000 
//   float3 options;                    // Offset:   36 Size:    12 [unused]
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// pointSamplerState                 sampler      NA          NA             s0      1 
// hdrColorMap                       texture  float4          2d             t0      1 
// averageLuminance                  texture  float4          2d             t1      1 
// ConstantBuffer                    cbuffer      NA          NA            cb0      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float       
// TEXCOORD                 0   xy          1     NONE   float   xy  
// SV_SAMPLEINDEX           0   x           2   SAMPLE    uint       
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_TARGET                0   xyzw        0   TARGET   float   xyzw
//
ps_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB0[3], immediateIndexed
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_texture2d (float,float,float,float) t1
dcl_input_ps linear v1.xy
dcl_output o0.xyzw
dcl_temps 4
resinfo_indexable(texture2d)(float,float,float,float)_uint r0.x, l(0), t1.wxyz
iadd r0.x, r0.x, l(-1)
utof r0.x, r0.x
sample_l_indexable(texture2d)(float,float,float,float) r0.x, l(0.500000, 0.500000, 0.000000, 0.000000), t1.xyzw, s0, r0.x
div r0.x, cb0[0].y, r0.x
mul r0.y, r0.x, r0.x
mul r0.z, cb0[0].x, cb0[0].x
div r0.y, r0.y, r0.z
add r0.y, r0.y, r0.x
add r0.x, r0.x, l(1.000000)
div r0.x, r0.y, r0.x
sample_indexable(texture2d)(float,float,float,float) r0.yzw, v1.xyxx, t0.wxyz, s0
mad r0.xyz, r0.yzwy, r0.xxxx, cb0[0].zzzz
add r1.xyz, r0.xyzx, l(-0.500000, -0.500000, -0.500000, 0.000000)
add r0.w, cb0[0].w, l(1.000000)
mad r2.xyz, r1.xyzx, r0.wwww, l(0.500000, 0.500000, 0.500000, 0.000000)
lt r0.w, cb0[0].w, l(0.000000)
movc r0.xyz, r0.wwww, r2.xyzx, r0.xyzx
add r0.w, -cb0[0].w, l(1.000000)
div r1.xyz, r1.xyzx, r0.wwww
add r1.xyz, r1.xyzx, l(0.500000, 0.500000, 0.500000, 0.000000)
lt r0.w, l(0.000000), cb0[0].w
movc r0.xyz, r0.wwww, r1.xyzx, r0.xyzx
mul r0.w, cb0[1].x, l(3.141593)
sincos r1.x, r2.x, r0.w
add r3.z, r2.x, r2.x
mad r3.xy, r1.xxxx, l(-1.732051, 1.732051, 0.000000, 0.000000), -r2.xxxx
add r1.xyz, r3.xyzx, l(1.000000, 1.000000, 1.000000, 0.000000)
mul r1.xyz, r1.xyzx, l(0.333333, 0.333333, 0.333333, 0.000000)
dp3 r2.y, r0.yzxy, r1.xyzx
dp3 r2.z, r0.zxyz, r1.xyzx
dp3 r2.w, r0.xyzx, r1.xyzx
add r0.x, r2.z, r2.y
add r0.x, r2.w, r0.x
mad r0.xyz, r0.xxxx, l(0.333333, 0.333333, 0.333333, 0.000000), -r2.yzwy
add r0.w, -cb0[1].y, l(1.001000)
div r0.w, l(1.000000, 1.000000, 1.000000, 1.000000), r0.w
add r0.w, -r0.w, l(1.000000)
mad r1.xyz, r0.xyzx, r0.wwww, r2.yzwy
mad r0.xyz, r0.xyzx, -cb0[1].yyyy, r2.yzwy
lt r0.w, l(0.000000), cb0[1].y
movc r0.xzw, r0.wwww, r1.xxzy, r0.xxzy
mad r1.xyz, cb0[1].zzzz, l(-0.607000, 0.769000, 0.189000, 0.000000), l(1.000000, 0.000000, 0.000000, 0.000000)
dp3 r0.y, r0.xwzx, r1.xyzx
mad r1.xyz, cb0[1].zzzz, l(0.349000, -0.314000, 0.168000, 0.000000), l(0.000000, 1.000000, 0.000000, 0.000000)
dp3 r0.x, r0.ywzy, r1.xyzx
mad r1.xyz, cb0[1].zzzz, l(0.272000, 0.534000, -0.869000, 0.000000), l(0.000000, 0.000000, 1.000000, 0.000000)
dp3 r0.w, r0.yxzy, r1.xyzx
add r0.z, -cb0[2].x, l(1.000000)
add r1.xyz, -r0.yxwy, r0.zzzz
add r2.xy, v1.xyxx, l(-0.500000, -0.500000, 0.000000, 0.000000)
mul r2.xy, r2.xyxx, cb0[1].wwww
dp2 r0.z, r2.xyxx, r2.xyxx
mad o0.xyz, r0.zzzz, r1.xyzx, r0.yxwy
mov o0.w, l(1.000000)
ret 
// Approximately 56 instruction slots used
#endif

const BYTE ToneMapPS[] =
{
     68,  88,  66,  67,  64,  55, 
      0, 164, 252, 155, 145, 133, 
     91,   9, 177, 237,  68,  37, 
    243,  47,   1,   0,   0,   0, 
     56,  13,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    200,   3,   0,   0,  68,   4, 
      0,   0, 120,   4,   0,   0, 
    156,  12,   0,   0,  82,  68, 
     69,  70, 140,   3,   0,   0, 
      1,   0,   0,   0, 252,   0, 
      0,   0,   4,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    255, 255,   0,   1,   0,   0, 
    100,   3,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    188,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 206,   0,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0,   4,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     13,   0,   0,   0, 218,   0, 
      0,   0,   2,   0,   0,   0, 
      5,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  13,   0,   0,   0, 
    235,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 112, 111, 105, 110, 
    116,  83,  97, 109, 112, 108, 
    101, 114,  83, 116,  97, 116, 
    101,   0, 104, 100, 114,  67, 
    111, 108, 111, 114,  77,  97, 
    112,   0,  97, 118, 101, 114, 
     97, 103, 101,  76, 117, 109, 
    105, 110,  97, 110,  99, 101, 
      0,  67, 111, 110, 115, 116, 
     97, 110, 116,  66, 117, 102, 
    102, 101, 114,   0, 171, 171, 
    235,   0,   0,   0,  10,   0, 
      0,   0,  20,   1,   0,   0, 
     48,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    164,   2,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0, 184,   2, 
      0,   0, 220,   2,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 224,   2, 
      0,   0,   4,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0, 184,   2,   0,   0, 
    236,   2,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 240,   2,   0,   0, 
      8,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
    184,   2,   0,   0, 252,   2, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      0,   3,   0,   0,  12,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0, 184,   2, 
      0,   0, 252,   2,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,   9,   3, 
      0,   0,  16,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0, 184,   2,   0,   0, 
    252,   2,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  13,   3,   0,   0, 
     20,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
    184,   2,   0,   0, 252,   2, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     24,   3,   0,   0,  24,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0, 184,   2, 
      0,   0, 252,   2,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  31,   3, 
      0,   0,  28,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0, 184,   2,   0,   0, 
    252,   2,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,  38,   3,   0,   0, 
     32,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
    184,   2,   0,   0, 252,   2, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     47,   3,   0,   0,  36,   0, 
      0,   0,  12,   0,   0,   0, 
      0,   0,   0,   0,  64,   3, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 119, 104, 
    105, 116, 101,  80, 111, 105, 
    110, 116,   0, 102, 108, 111, 
     97, 116,   0, 171, 171, 171, 
      0,   0,   3,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 175,   2,   0,   0, 
      0,   0,  64,  64,  97, 118, 
    101, 114,  97, 103, 101,  71, 
    114,  97, 121,   0, 229, 208, 
    146,  63,  98, 114, 105, 103, 
    104, 116, 110, 101, 115, 115, 
      0, 171,   0,   0,   0,   0, 
     99, 111, 110, 116, 114,  97, 
    115, 116,   0, 104, 117, 101, 
      0, 115,  97, 116, 117, 114, 
     97, 116, 105, 111, 110,   0, 
     97, 109, 111, 117, 110, 116, 
      0, 111, 102, 102, 115, 101, 
    116,   0, 100,  97, 114, 107, 
    110, 101, 115, 115,   0, 111, 
    112, 116, 105, 111, 110, 115, 
      0, 102, 108, 111,  97, 116, 
     51,   0, 171, 171,   1,   0, 
      3,   0,   1,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     55,   3,   0,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  49,  48,  46, 
     49,   0,  73,  83,  71,  78, 
    116,   0,   0,   0,   3,   0, 
      0,   0,   8,   0,   0,   0, 
     80,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     92,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   3,   3,   0,   0, 
    101,   0,   0,   0,   0,   0, 
      0,   0,  10,   0,   0,   0, 
      1,   0,   0,   0,   2,   0, 
      0,   0,   1,   0,   0,   0, 
     83,  86,  95,  80,  79,  83, 
     73,  84,  73,  79,  78,   0, 
     84,  69,  88,  67,  79,  79, 
     82,  68,   0,  83,  86,  95, 
     83,  65,  77,  80,  76,  69, 
     73,  78,  68,  69,  88,   0, 
     79,  83,  71,  78,  44,   0, 
      0,   0,   1,   0,   0,   0, 
      8,   0,   0,   0,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0,  83,  86, 
     95,  84,  65,  82,  71,  69, 
     84,   0, 171, 171,  83,  72, 
     69,  88,  28,   8,   0,   0, 
     80,   0,   0,   0,   7,   2, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  90,   0, 
      0,   3,   0,  96,  16,   0, 
      0,   0,   0,   0,  88,  24, 
      0,   4,   0, 112,  16,   0, 
      0,   0,   0,   0,  85,  85, 
      0,   0,  88,  24,   0,   4, 
      0, 112,  16,   0,   1,   0, 
      0,   0,  85,  85,   0,   0, 
     98,  16,   0,   3,  50,  16, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   0,   0,   0,   0, 
    104,   0,   0,   2,   4,   0, 
      0,   0,  61,  16,   0, 137, 
    194,   0,   0, 128,  67,  85, 
     21,   0,  18,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     54, 121,  16,   0,   1,   0, 
      0,   0,  30,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0, 255, 255, 255, 255, 
     86,   0,   0,   5,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  72,   0,   0, 144, 
    194,   0,   0, 128,  67,  85, 
     21,   0,  18,   0,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,  63, 
      0,   0,   0,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     70, 126,  16,   0,   1,   0, 
      0,   0,   0,  96,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     14,   0,   0,   8,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     26, 128,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   9,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     10, 128,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     10, 128,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     14,   0,   0,   7,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   7,  34,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     14,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  69,   0, 
      0, 139, 194,   0,   0, 128, 
     67,  85,  21,   0, 226,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  16,  16,   0,   1,   0, 
      0,   0,  54, 121,  16,   0, 
      0,   0,   0,   0,   0,  96, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  10, 114,   0, 
     16,   0,   0,   0,   0,   0, 
    150,   7,  16,   0,   0,   0, 
      0,   0,   6,   0,  16,   0, 
      0,   0,   0,   0, 166, 138, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,  10, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0, 191,   0,   0,   0, 191, 
      0,   0,   0, 191,   0,   0, 
      0,   0,   0,   0,   0,   8, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  58, 128,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  50,   0, 
      0,  12, 114,   0,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,  63,   0,   0, 
      0,  63,   0,   0,   0,  63, 
      0,   0,   0,   0,  49,   0, 
      0,   8, 130,   0,  16,   0, 
      0,   0,   0,   0,  58, 128, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     55,   0,   0,   9, 114,   0, 
     16,   0,   0,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,   9, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58, 128,  32, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     14,   0,   0,   7, 114,   0, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,  10, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,  63,   0,   0,   0,  63, 
      0,   0,   0,  63,   0,   0, 
      0,   0,  49,   0,   0,   8, 
    130,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  58, 128, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  55,   0, 
      0,   9, 114,   0,  16,   0, 
      0,   0,   0,   0, 246,  15, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8, 130,   0,  16,   0, 
      0,   0,   0,   0,  10, 128, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0, 219,  15,  73,  64, 
     77,   0,   0,   7,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     18,   0,  16,   0,   2,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   7,  66,   0,  16,   0, 
      3,   0,   0,   0,  10,   0, 
     16,   0,   2,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  13, 
     50,   0,  16,   0,   3,   0, 
      0,   0,   6,   0,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0, 215, 179, 221, 191, 
    215, 179, 221,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      6,   0,  16, 128,  65,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,  10, 114,   0, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   3,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
    128,  63,   0,   0, 128,  63, 
      0,   0,   0,   0,  56,   0, 
      0,  10, 114,   0,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0, 171, 170, 
    170,  62, 171, 170, 170,  62, 
    171, 170, 170,  62,   0,   0, 
      0,   0,  16,   0,   0,   7, 
     34,   0,  16,   0,   2,   0, 
      0,   0, 150,   4,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     16,   0,   0,   7,  66,   0, 
     16,   0,   2,   0,   0,   0, 
     38,   9,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  16,   0, 
      0,   7, 130,   0,  16,   0, 
      2,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,   0,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      2,   0,   0,   0,  26,   0, 
     16,   0,   2,   0,   0,   0, 
      0,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   2,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,  13, 114,   0,  16,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0, 171, 170, 
    170,  62, 171, 170, 170,  62, 
    171, 170, 170,  62,   0,   0, 
      0,   0, 150,   7,  16, 128, 
     65,   0,   0,   0,   2,   0, 
      0,   0,   0,   0,   0,   9, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  26, 128,  32, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,  64,   0,   0, 197,  32, 
    128,  63,  14,   0,   0,  10, 
    130,   0,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
    128,  63,   0,   0, 128,  63, 
      0,   0, 128,  63,  58,   0, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,   8, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  50,   0,   0,   9, 
    114,   0,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0, 246,  15, 
     16,   0,   0,   0,   0,   0, 
    150,   7,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,  11, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  86, 133, 
     32, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0, 150,   7,  16,   0, 
      2,   0,   0,   0,  49,   0, 
      0,   8, 130,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0,   0, 
     26, 128,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     55,   0,   0,   9, 210,   0, 
     16,   0,   0,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,   6,   6,  16,   0, 
      1,   0,   0,   0,   6,   6, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  16, 114,   0, 
     16,   0,   1,   0,   0,   0, 
    166, 138,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,  90, 100, 
     27, 191,  47, 221,  68,  63, 
     55, 137,  65,  62,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  16,   0, 
      0,   7,  34,   0,  16,   0, 
      0,   0,   0,   0, 198,   2, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  16, 
    114,   0,  16,   0,   1,   0, 
      0,   0, 166, 138,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   2,  64,   0,   0, 
     33, 176, 178,  62, 156, 196, 
    160, 190,  49,   8,  44,  62, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     16,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
    214,   6,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  16, 114,   0,  16,   0, 
      1,   0,   0,   0, 166, 138, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0, 150,  67, 139,  62, 
     57, 180,   8,  63, 201, 118, 
     94, 191,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 128,  63,   0,   0, 
      0,   0,  16,   0,   0,   7, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  22,   6,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
      0,   0,   0,   9,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     10, 128,  32, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
      0,   0,   0,   8, 114,   0, 
     16,   0,   1,   0,   0,   0, 
     22,   7,  16, 128,  65,   0, 
      0,   0,   0,   0,   0,   0, 
    166,  10,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,  10, 
     50,   0,  16,   0,   2,   0, 
      0,   0,  70,  16,  16,   0, 
      1,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0, 191, 
      0,   0,   0, 191,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     56,   0,   0,   8,  50,   0, 
     16,   0,   2,   0,   0,   0, 
     70,   0,  16,   0,   2,   0, 
      0,   0, 246, 143,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  15,   0,   0,   7, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  70,   0,  16,   0, 
      2,   0,   0,   0,  70,   0, 
     16,   0,   2,   0,   0,   0, 
     50,   0,   0,   9, 114,  32, 
     16,   0,   0,   0,   0,   0, 
    166,  10,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  22,   7, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5, 130,  32, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  62,   0,   0,   1, 
     83,  84,  65,  84, 148,   0, 
      0,   0,  56,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
     45,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0
};
