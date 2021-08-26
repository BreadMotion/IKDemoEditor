#define USE_POSITION
#define USE_NORMAL
#define USE_TANGENT
#define USE_TEXCOORD0
#define USE_BONE
//#define USE_SHADOW

#include "CommonLayout.hlsli"

typedef VS_OUTPUT PS_INPUT;

//#define CB_SHADOW b0

#include "CommonCB.hlsli"
#include "CommonPS.hlsli"


//--------------------------------------------------------------------------------------
// 定数バッファ
//--------------------------------------------------------------------------------------
#if 0
cbuffer CbScene : register(b1)
{
    ExpFog cb_fog;
    float4 cb_eye;
    int cb_lit;
    int cb_textured;
    int cb_shadowed;
    int padding_scene;
};
#else
cbuffer CbScene : register(b0)
{
    float4 cb_eye;
};
#endif

#if 0
cbuffer CbMaterial : register(b2)
{
    LightEnvironment cb_light_env;
    float4 cb_emissive;
    float4 cb_diffuse;
    float4 cb_specular;
    int cb_has_diffusemap;
    int cb_has_normalmap;
    int ch_has_specularmap;
    int padding_material;
};
#else
cbuffer CbMaterial : register(b1)
{
    LightEnvironment cb_light_env;
    float4 cb_emissive;
    float4 cb_diffuse;
    float4 cb_specular;
};
#endif

//--------------------------------------------------------------------------------------
// リソース
//--------------------------------------------------------------------------------------
Texture2D diffusemap : register(t0);
Texture2D specularmap : register(t1);
Texture2D normalmap : register(t2);
//Texture2D shadowmap : register(t3);
SamplerState sampler0 : register(s0);
//SamplerComparisonState shadow_sampler_cmp : register(s1);

//--------------------------------------------------------------------------------------
// ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
//    float4 diffuse_color = cb_diffuse;
//    float4 specular_color = cb_specular;
//    float4 fc = diffuse_color;

//#if defined(USE_TEXCOORD0)
//    float4 texcolor = diffusemap.Sample(sampler0, input.texcoord0);
//    clip(texcolor.a - 0.5f);
//    diffuse_color *= texcolor;
//    fc = diffuse_color;
//#endif

//#if defined(USE_NORMAL)
//	// ライティング
//    float3 normal = normalize(input.normal);

//#if defined(USE_TANGENT)
//	// 法線マップ
//    float3 tangent = normalize(input.tangent - dot(input.tangent, normal) * normal);
//    float3 binormal = cross(normal, tangent);
//    texcolor.xyz = normalmap.Sample(sampler0, input.texcoord0).xyz;
//    texcolor.xyz = ((2.0f * texcolor.xyz) - 1.0f);

//    float3x3 tbn = float3x3(tangent, binormal, normal);
//    normal = normalize(mul(texcolor.xyz, tbn));
//#endif

//    float shadow_factor = 1.0f;
//#if defined(USE_SHADOW)
//	// 影
//    shadow_factor = ComputeShadowFactor(shadowmap, shadow_sampler_cmp, input.shadow.xyz, cb_texel_size);
//#endif
//	// ライト計算
//    float3 ambient, diffuse, specular;
//    float specular_power = max(1.0f, specular_color.a);

//    ComputeLighting(input.position, normal, cb_eye.xyz, specular_power, shadow_factor, cb_light_env, ambient, diffuse, specular);

//    fc.xyz = cb_emissive.xyz + diffuse_color.xyz * (ambient + diffuse) + specular_color.xyz * specular;
//#endif

//	// フォグ
//    //if (cb_fog.enabled)
//    //{
//    //    float dist = distance(cb_eye.xyz, input.position);
//    //    float foglerp = ComputeFogFactor(cb_fog, dist);
//    //    fc.xyz = lerp(cb_fog.color.xyz, fc.xyz, foglerp);
//    //}

//    return fc;
    
    float4 diffuse_map_colour = diffusemap.Sample(sampler0, input.texcoord0);
    float alpha = diffuse_map_colour.a;

    float4 specular_map_colour = specularmap.Sample(sampler0, input.texcoord0);

    float4 normal_map_colour = normalmap.Sample(sampler0, input.texcoord0);
    normal_map_colour = (normal_map_colour * 2.0) - 1.0;
    normal_map_colour.w = 0;
	// transform to world space from tangent space
	//                 |Tx Ty Tz|
	// normal = |x y z||Bx By Bz|
	//                 |Nx Ny Nz|
    float3 N = normalize(input.normal.xyz);
    float3 T = normalize(input.tangent.xyz);
    //float sigma = input.tangent.w;
    T = normalize(T - dot(N, T));
    float3 B = normalize(cross(N, T)/* * pin.tangent.w*/);
    N = normalize((normal_map_colour.x * T) + (normal_map_colour.y * B) + (normal_map_colour.z * N));

    float3 L = normalize(-cb_light_env.dirLight[0].dir.xyz); // TODO : temp[0]
#if 0
	float diffuse_factor = max(dot(L, N), 0);
#else
	//half lambert 
	//float diffuse_factor = dot(L, N) * 0.5 + 0.5;
    float diffuse_factor = max(0, dot(L, N)) * 0.5 + 0.5;
    diffuse_factor = diffuse_factor * diffuse_factor;
#endif

    float specular_factor = 0.0;
    float3 E = normalize(cb_eye.xyz - input.position.xyz);
#if 0
	float3 H = normalize(E + L);
	specular_factor = max(dot(H, N), 0);
#else
    float3 R = normalize(reflect(-L, N));
    specular_factor = max(dot(R, E), 0);
#endif
    specular_factor = pow(specular_factor, 180) * 0.5;

    float3 fragment_colour = float3(0, 0, 0);

	//directional lighting
    float3 Kd = diffuse_map_colour.rgb * diffuse_factor * cb_diffuse.xyz;
    float3 Ks = /*specular_map_colour.rgb * */specular_factor * cb_specular.xyz;
    fragment_colour = (Kd + Ks) * cb_light_env.dirLight[0].diffuse.xyz * cb_light_env.dirLight[0].diffuse.w; // TODO : temp[0]
    //fragment_colour *= cb_diffuse.xyz;
    return float4(fragment_colour, alpha);
}