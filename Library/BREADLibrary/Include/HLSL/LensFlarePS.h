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
//   float glowExtractionThreshold;     // Offset:    0 Size:     4 [unused]
//   float blurConvolutionIntensity;    // Offset:    4 Size:     4 [unused]
//   float lensFlareThreshold;          // Offset:    8 Size:     4
//   float lensFlareGhostDispersal;     // Offset:   12 Size:     4
//   int numberOfGhosts;                // Offset:   16 Size:     4
//   float lensFlareIntensity;          // Offset:   20 Size:     4
//   int2 options;                      // Offset:   24 Size:     8 [unused]
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// anisotropicSamplerState           sampler      NA          NA             s2      1 
// hdrColorMap                       texture  float4          2d             t0      1 
// gradientMap                       texture  float4          2d             t1      1 
// noiseMap                          texture  float4          2d             t2      1 
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
dcl_constantbuffer CB0[2], immediateIndexed
dcl_sampler s2, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_resource_texture2d (float,float,float,float) t1
dcl_resource_texture2d (float,float,float,float) t2
dcl_input_ps linear v1.xy
dcl_output o0.xyzw
dcl_temps 5
add r0.xy, -v1.xyxx, l(1.000000, 1.000000, 0.000000, 0.000000)
add r1.xyzw, -r0.xyxy, l(0.500000, 0.500000, 0.500000, 0.500000)
mul r0.zw, r1.xxxy, cb0[0].wwww
mov r2.xyz, l(0,0,0,0)
mov r1.x, l(0)
loop 
  ige r1.y, r1.x, cb0[1].x
  breakc_nz r1.y
  itof r1.y, r1.x
  mad r3.xy, r0.zwzz, r1.yyyy, r0.xyxx
  sample_indexable(texture2d)(float,float,float,float) r4.xyz, r3.xyxx, t0.xyzw, s2
  add r4.xyz, r4.xyzx, -cb0[0].zzzz
  max r4.xyz, r4.xyzx, l(0.000000, 0.000000, 0.000000, 0.000000)
  add r3.xy, r3.xyxx, l(-0.500000, -0.500000, 0.000000, 0.000000)
  dp2 r1.y, r3.xyxx, r3.xyxx
  sqrt r1.y, r1.y
  mul r1.y, r1.y, l(1.333333)
  min r1.y, r1.y, l(1.000000)
  mad r2.w, r1.y, l(-2.000000), l(3.000000)
  mul r1.y, r1.y, r1.y
  mad r1.y, -r2.w, r1.y, l(1.000000)
  mad r2.xyz, r4.xyzx, r1.yyyy, r2.xyzx
  iadd r1.x, r1.x, l(1)
endloop 
dp3 r0.x, r2.xyzx, l(0.212600, 0.715200, 0.072200, 0.000000)
dp2 r0.y, r1.zwzz, r1.zwzz
sqrt r0.y, r0.y
mul r0.y, r0.y, l(1.414214)
sample_indexable(texture2d)(float,float,float,float) r0.yzw, r0.yyyy, t1.wxyz, s2
mul r0.xyz, r0.yzwy, r0.xxxx
sample_indexable(texture2d)(float,float,float,float) r0.w, v1.xyxx, t2.yzwx, s2
mul r0.xyz, r0.wwww, r0.xyzx
mul o0.xyz, r0.xyzx, cb0[1].yyyy
mov o0.w, l(1.000000)
ret 
// Approximately 35 instruction slots used
#endif

const BYTE LensFlarePS[] =
{
     68,  88,  66,  67, 246, 228, 
     18,  30, 168,  74, 139, 248, 
    192, 212, 226,  59,  97, 216, 
     83, 229,   1,   0,   0,   0, 
    104,   9,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    188,   3,   0,   0,  20,   4, 
      0,   0,  72,   4,   0,   0, 
    204,   8,   0,   0,  82,  68, 
     69,  70, 128,   3,   0,   0, 
      1,   0,   0,   0,  36,   1, 
      0,   0,   5,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    255, 255,   0,   1,   0,   0, 
     88,   3,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    220,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   2,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 244,   0,   0,   0, 
      2,   0,   0,   0,   5,   0, 
      0,   0,   4,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     13,   0,   0,   0,   0,   1, 
      0,   0,   2,   0,   0,   0, 
      5,   0,   0,   0,   4,   0, 
      0,   0, 255, 255, 255, 255, 
      1,   0,   0,   0,   1,   0, 
      0,   0,  13,   0,   0,   0, 
     12,   1,   0,   0,   2,   0, 
      0,   0,   5,   0,   0,   0, 
      4,   0,   0,   0, 255, 255, 
    255, 255,   2,   0,   0,   0, 
      1,   0,   0,   0,  13,   0, 
      0,   0,  21,   1,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  97, 110, 
    105, 115, 111, 116, 114, 111, 
    112, 105,  99,  83,  97, 109, 
    112, 108, 101, 114,  83, 116, 
     97, 116, 101,   0, 104, 100, 
    114,  67, 111, 108, 111, 114, 
     77,  97, 112,   0, 103, 114, 
     97, 100, 105, 101, 110, 116, 
     77,  97, 112,   0, 110, 111, 
    105, 115, 101,  77,  97, 112, 
      0,  67, 111, 110, 115, 116, 
     97, 110, 116,  66, 117, 102, 
    102, 101, 114,   0,  21,   1, 
      0,   0,   7,   0,   0,   0, 
     60,   1,   0,   0,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  84,   2, 
      0,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   0,   0, 
      0,   0, 116,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 152,   2,   0,   0, 
      4,   0,   0,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
    116,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    177,   2,   0,   0,   8,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0, 116,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 196,   2, 
      0,   0,  12,   0,   0,   0, 
      4,   0,   0,   0,   2,   0, 
      0,   0, 116,   2,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 220,   2,   0,   0, 
     16,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
    240,   2,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     20,   3,   0,   0,  20,   0, 
      0,   0,   4,   0,   0,   0, 
      2,   0,   0,   0, 116,   2, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0,  39,   3, 
      0,   0,  24,   0,   0,   0, 
      8,   0,   0,   0,   0,   0, 
      0,   0,  52,   3,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 103, 108, 111, 119, 
     69, 120, 116, 114,  97,  99, 
    116, 105, 111, 110,  84, 104, 
    114, 101, 115, 104, 111, 108, 
    100,   0, 102, 108, 111,  97, 
    116,   0, 171, 171,   0,   0, 
      3,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    108,   2,   0,   0,  98, 108, 
    117, 114,  67, 111, 110, 118, 
    111, 108, 117, 116, 105, 111, 
    110,  73, 110, 116, 101, 110, 
    115, 105, 116, 121,   0, 108, 
    101, 110, 115,  70, 108,  97, 
    114, 101,  84, 104, 114, 101, 
    115, 104, 111, 108, 100,   0, 
    108, 101, 110, 115,  70, 108, 
     97, 114, 101,  71, 104, 111, 
    115, 116,  68, 105, 115, 112, 
    101, 114, 115,  97, 108,   0, 
    110, 117, 109,  98, 101, 114, 
     79, 102,  71, 104, 111, 115, 
    116, 115,   0, 105, 110, 116, 
      0, 171,   0,   0,   2,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 235,   2, 
      0,   0, 108, 101, 110, 115, 
     70, 108,  97, 114, 101,  73, 
    110, 116, 101, 110, 115, 105, 
    116, 121,   0, 111, 112, 116, 
    105, 111, 110, 115,   0, 105, 
    110, 116,  50,   0,   1,   0, 
      2,   0,   1,   0,   2,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     47,   3,   0,   0,  77, 105, 
     99, 114, 111, 115, 111, 102, 
    116,  32,  40,  82,  41,  32, 
     72,  76,  83,  76,  32,  83, 
    104,  97, 100, 101, 114,  32, 
     67, 111, 109, 112, 105, 108, 
    101, 114,  32,  49,  48,  46, 
     49,   0,  73,  83,  71,  78, 
     80,   0,   0,   0,   2,   0, 
      0,   0,   8,   0,   0,   0, 
     56,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     68,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   3,   3,   0,   0, 
     83,  86,  95,  80,  79,  83, 
     73,  84,  73,  79,  78,   0, 
     84,  69,  88,  67,  79,  79, 
     82,  68,   0, 171, 171, 171, 
     79,  83,  71,  78,  44,   0, 
      0,   0,   1,   0,   0,   0, 
      8,   0,   0,   0,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0,  83,  86, 
     95,  84,  65,  82,  71,  69, 
     84,   0, 171, 171,  83,  72, 
     69,  88, 124,   4,   0,   0, 
     80,   0,   0,   0,  31,   1, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0,  90,   0, 
      0,   3,   0,  96,  16,   0, 
      2,   0,   0,   0,  88,  24, 
      0,   4,   0, 112,  16,   0, 
      0,   0,   0,   0,  85,  85, 
      0,   0,  88,  24,   0,   4, 
      0, 112,  16,   0,   1,   0, 
      0,   0,  85,  85,   0,   0, 
     88,  24,   0,   4,   0, 112, 
     16,   0,   2,   0,   0,   0, 
     85,  85,   0,   0,  98,  16, 
      0,   3,  50,  16,  16,   0, 
      1,   0,   0,   0, 101,   0, 
      0,   3, 242,  32,  16,   0, 
      0,   0,   0,   0, 104,   0, 
      0,   2,   5,   0,   0,   0, 
      0,   0,   0,  11,  50,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  16,  16, 128,  65,   0, 
      0,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
    128,  63,   0,   0, 128,  63, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,  11, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70,   4,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,  63,   0,   0, 
      0,  63,   0,   0,   0,  63, 
      0,   0,   0,  63,  56,   0, 
      0,   8, 194,   0,  16,   0, 
      0,   0,   0,   0,   6,   4, 
     16,   0,   1,   0,   0,   0, 
    246, 143,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     54,   0,   0,   8, 114,   0, 
     16,   0,   2,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
     18,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  48,   0, 
      0,   1,  33,   0,   0,   8, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,  10, 128, 
     32,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      4,   3,  26,   0,  16,   0, 
      1,   0,   0,   0,  43,   0, 
      0,   5,  34,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,   9,  50,   0, 
     16,   0,   3,   0,   0,   0, 
    230,  10,  16,   0,   0,   0, 
      0,   0,  86,   5,  16,   0, 
      1,   0,   0,   0,  70,   0, 
     16,   0,   0,   0,   0,   0, 
     69,   0,   0, 139, 194,   0, 
      0, 128,  67,  85,  21,   0, 
    114,   0,  16,   0,   4,   0, 
      0,   0,  70,   0,  16,   0, 
      3,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   2,   0, 
      0,   0,   0,   0,   0,   9, 
    114,   0,  16,   0,   4,   0, 
      0,   0,  70,   2,  16,   0, 
      4,   0,   0,   0, 166, 138, 
     32, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  52,   0,   0,  10, 
    114,   0,  16,   0,   4,   0, 
      0,   0,  70,   2,  16,   0, 
      4,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,  10,  50,   0, 
     16,   0,   3,   0,   0,   0, 
     70,   0,  16,   0,   3,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0, 191,   0,   0, 
      0, 191,   0,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   7,  34,   0,  16,   0, 
      1,   0,   0,   0,  70,   0, 
     16,   0,   3,   0,   0,   0, 
     70,   0,  16,   0,   3,   0, 
      0,   0,  75,   0,   0,   5, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   7,  34,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0, 171, 170, 
    170,  63,  51,   0,   0,   7, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     50,   0,   0,   9, 130,   0, 
     16,   0,   2,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0, 192,   1,  64, 
      0,   0,   0,   0,  64,  64, 
     56,   0,   0,   7,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10,  34,   0,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16, 128,  65,   0,   0,   0, 
      2,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  50,   0,   0,   9, 
    114,   0,  16,   0,   2,   0, 
      0,   0,  70,   2,  16,   0, 
      4,   0,   0,   0,  86,   5, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   2,   0, 
      0,   0,  30,   0,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   1,   0,   0,   0, 
     22,   0,   0,   1,  16,   0, 
      0,  10,  18,   0,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   2,   0,   0,   0, 
      2,  64,   0,   0, 208, 179, 
     89,  62,  89,  23,  55,  63, 
    152, 221, 147,  61,   0,   0, 
      0,   0,  15,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0, 230,  10,  16,   0, 
      1,   0,   0,   0, 230,  10, 
     16,   0,   1,   0,   0,   0, 
     75,   0,   0,   5,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0, 243,   4, 181,  63, 
     69,   0,   0, 139, 194,   0, 
      0, 128,  67,  85,  21,   0, 
    226,   0,  16,   0,   0,   0, 
      0,   0,  86,   5,  16,   0, 
      0,   0,   0,   0,  54, 121, 
     16,   0,   1,   0,   0,   0, 
      0,  96,  16,   0,   2,   0, 
      0,   0,  56,   0,   0,   7, 
    114,   0,  16,   0,   0,   0, 
      0,   0, 150,   7,  16,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     69,   0,   0, 139, 194,   0, 
      0, 128,  67,  85,  21,   0, 
    130,   0,  16,   0,   0,   0, 
      0,   0,  70,  16,  16,   0, 
      1,   0,   0,   0, 150, 115, 
     16,   0,   2,   0,   0,   0, 
      0,  96,  16,   0,   2,   0, 
      0,   0,  56,   0,   0,   7, 
    114,   0,  16,   0,   0,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   8, 114,  32, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  86, 133,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
    130,  32,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    148,   0,   0,   0,  35,   0, 
      0,   0,   5,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  22,   0,   0,   0, 
      2,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
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
      0,   0
};
