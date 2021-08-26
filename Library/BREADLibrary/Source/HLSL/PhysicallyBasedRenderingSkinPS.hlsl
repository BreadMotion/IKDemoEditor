#define USE_POSITION
#define USE_NORMAL
#define USE_TANGENT
#define USE_TEXCOORD0
#define USE_PBR
#include "CommonLayout.hlsli"
#include "ShadowMap.hlsli"

// defines
#define PI 3.14159265359
#define PI2 6.28318530718
#define RECIPROCAL_PI 0.31830988618
#define RECIPROCAL_PI2 0.15915494
#define LOG2 1.442695
#define EPSILON 1e-6

#define LIGHT_MAX 4

struct GeometricContext
{
    float3 position;
    float3 normal;
    float3 viewDir;
};

struct Material
{
    float3 diffuseColor;
    float3 specularColor;
    float specularRoughness;
};

struct ReflectedLight
{
    float3 directDiffuse;
    float3 directSpecular;
    float3 indirectDiffuse;
    float3 indirectSpecular;
};

struct IncidentLight
{
    float3 direction;
    float3 color;
    bool visible;
};

struct DirectionalLight
{
    float4 direction;
    float4 color;
};

struct PointLight
{
    float3 position;
    float3 color;
    float distance;
    float decay;
};

struct SpotLight
{
    float3 position;
    float3 direction;
    float3 color;
    float distance;
    float decay;
    float coneCos;
    float penumbraCos;
};

//--------------------------------------------------------------------------------------
// 入力レイアウト
//--------------------------------------------------------------------------------------
typedef VS_OUTPUT PS_INPUT;

//--------------------------------------------------------------------------------------
// 定数バッファ
//--------------------------------------------------------------------------------------
cbuffer CbMaterial : register(b0)
{
    float4 albedo;
    float cbMetallic;
    float roughness;
    float padding01;
    float padding02;
};

cbuffer CbScene : register(b1)
{
    DirectionalLight dirLight[2];
    float4 cbEye;
};

//--------------------------------------------------------------------------------------
// リソース
//--------------------------------------------------------------------------------------
Texture2D albedoTex : register(t0);
Texture2D specularTex : register(t2);
Texture2D normalTex : register(t3);
Texture2D emissiveTex : register(t5);

SamplerState sampler0 : register(s0);

// Normalized Lambert
float3 DiffuseBRDF(float3 diffuseColor)
{
    return diffuseColor / PI;
}

float D_GGX(float a, float dotNH)
{
    float a2 = a * a;
    float dotNH2 = dotNH * dotNH;
    float d = dotNH2 * (a2 - 1.0) + 1.0;
    return a2 / (PI * d * d);
}

float G_Smith_Schlick_GGX(float a, float dotNV, float dotNL)
{
    float k = a * a * 0.5 + EPSILON;
    float gl = dotNL / (dotNL * (1.0 - k) + k);
    float gv = dotNV / (dotNV * (1.0 - k) + k);
    return gl * gv;
}

float3 F_Schlick(float3 specularColor, float3 H, float3 V)
{
    return (specularColor + (1.0 - specularColor) * pow(1.0 - saturate(dot(V, H)), 5.0));
}

// Cook-Torrance
float3 SpecularBRDF(const in IncidentLight directLight, const in GeometricContext geometry, float3 specularColor, float roughnessFactor)
{
    float3 N = geometry.normal;
    float3 V = geometry.viewDir;
    float3 L = directLight.direction;

    float dotNL = saturate(dot(N, L));
    float dotNV = saturate(dot(N, V));
    float3 H = normalize(L + V);
    float dotNH = saturate(dot(N, H));
    float dotVH = saturate(dot(V, H));
    float dotLV = saturate(dot(L, V));
    float a = roughnessFactor * roughnessFactor;

    float D = D_GGX(a, dotNH);
    float G = G_Smith_Schlick_GGX(a, dotNV, dotNL);
    float3 F = F_Schlick(specularColor, V, H);
    return (F * (G * D)) / (4.0 * dotNL * dotNV + EPSILON);
}

void getDirectionalDirectLightIrradiance(const in DirectionalLight directionalLight, const in GeometricContext geometry, out IncidentLight directLight)
{
    directLight.color = directionalLight.color.xyz;
    directLight.direction = directionalLight.direction.xyz;
    directLight.visible = true;
}

// RenderEquations(RE)
void RE_Direct(const in IncidentLight directLight, const in GeometricContext geometry, const in Material material, inout ReflectedLight reflectedLight)
{
    float dotNL = saturate(dot(geometry.normal, directLight.direction));
    float3 irradiance = dotNL * directLight.color;

    // punctual light
    irradiance *= PI;

    reflectedLight.directDiffuse += irradiance * DiffuseBRDF(material.diffuseColor);
    reflectedLight.directSpecular += irradiance * SpecularBRDF(directLight, geometry, material.specularColor, material.specularRoughness);
}

//--------------------------------------------------------------------------------------
// ピクセルシェーダー
//--------------------------------------------------------------------------------------
float4 main(PS_INPUT input) : SV_Target
{
    // 接空間軸
    float3 vB = normalize(cross(input.tangent, input.normal));
  
    float3 vx = normalize(input.tangent);
    float3 vy = normalize(vB);
    float3 vz = normalize(input.normal);
  
    // テクスチャ色取得
    float3 N = normalTex.Sample(sampler0, input.texcoord0).xyz;
    N = N * 2.0f - 1.0f;
  
    // 法線ワールド変換
    float3x3 mat = { vx, vy, vz };
    N = normalize(mul(N, mat));
  
    GeometricContext geometry;
    //geometry.position = -lighPos.xyz;
    //geometry.normal = normalize(input.normal);
    geometry.normal = N;
    geometry.viewDir = normalize(cbEye.xyz - input.position);
  
    //float3 albedoColor = pow(albedoTex.Sample(sampler0, input.texcoord0).rgb, 2.2f) * albedo.xyz;
    float3 albedoColor = albedoTex.Sample(sampler0, input.texcoord0).rgb * albedo.xyz;
    float rough = /*specularTex.Sample(sampler0, input.texcoord0).x * */roughness;
  
    Material material;
    material.diffuseColor = lerp(albedoColor, albedoColor * 2.0f, float3(cbMetallic, cbMetallic, cbMetallic));
    material.specularColor = lerp(float3(0.04f, 0.04f, 0.04f), albedoColor, float3(cbMetallic, cbMetallic, cbMetallic));
    material.specularRoughness = rough;

    // Lighting
    ReflectedLight reflectedLight;
    reflectedLight.directDiffuse = float3(0.0f, 0.0f, 0.0f);
    reflectedLight.directSpecular = float3(0.0f, 0.0f, 0.0f);
    reflectedLight.indirectDiffuse = float3(0.0f, 0.0f, 0.0f);
    reflectedLight.indirectSpecular = float3(0.0f, 0.0f, 0.0f);
  
    //float3 emissive = float3(0.0f, 0.0f, 0.0f);
    float3 emissive = emissiveTex.Sample(sampler0, input.texcoord0).xyz/* * float3(10.0f, 10.0f, 10.0f)*/;
    float opacity = 1.0;

    IncidentLight directLight;

#if 0
    // point light
    for (int i = 0; i < LIGHT_MAX; ++i)
    {
        if (i >= numPointLights)
            break;
        getPointDirectLightIrradiance(pointLights[i], geometry, directLight);
        if (directLight.visible)
        {
            RE_Direct(directLight, geometry, material, reflectedLight);
        }
    }

    // spot light
    for (int i = 0; i < LIGHT_MAX; ++i)
    {
        if (i >= numSpotLights)
            break;
        getSpotDirectLightIrradiance(spotLights[i], geometry, directLight);
        if (directLight.visible)
        {
            RE_Direct(directLight, geometry, material, reflectedLight);
        }
    }
  
    // directional light
    for (int i=0; i<LIGHT_MAX; ++i)
    {
        if (i >= numDirectionalLights) break;
        getDirectionalDirectLightIrradiance(directionalLights[i], geometry, directLight);
        RE_Direct(directLight, geometry, material, reflectedLight);
    }
#endif
  
    // directional light
    //getDirectionalDirectLightIrradiance(dirLight[0], geometry, directLight);
    //RE_Direct(directLight, geometry, material, reflectedLight);
    
    getDirectionalDirectLightIrradiance(dirLight[1], geometry, directLight);
    RE_Direct(directLight, geometry, material, reflectedLight);
    
    getDirectionalDirectLightIrradiance(dirLight[0], geometry, directLight);
    {
        float dotNL = saturate(dot(geometry.normal, directLight.direction));
        float3 irradiance = dotNL * directLight.color;

        // punctual light
        irradiance *= PI;

        reflectedLight.directDiffuse += (irradiance * DiffuseBRDF(material.diffuseColor)) * 1.50f;
    }
    
    float4 ShadowPos = float4(input.position, 1.0);
    
	//シャドー空間座標
    ShadowPos = mul(ShadowPos, lightViewProjection);
    ShadowPos.xyz /= ShadowPos.w;
    ShadowPos.y = -ShadowPos.y;
    
	//シャドーマップ空間
    ShadowPos.xy = ShadowPos.xy * 0.5 + 0.5;
    
    float3 shadow = (float3) 1.0;
    if (0.0f < ShadowPos.x && ShadowPos.x < 1.0f && 0.0f < ShadowPos.y && ShadowPos.y < 1.0f)
    {
        //シャードーマップの深度
        float depth = shadowMap.Sample(sampler0, ShadowPos.xy).r;

        if (ShadowPos.z > depth + 0.0005)
        {
            shadow = /*1.0 - (ShadowPos.z - depth) / ShadowPos.z*/0.5f;
        }
    }
	
    reflectedLight.directDiffuse *= shadow;
    reflectedLight.indirectDiffuse *= shadow;

    float3 outgoingLight = emissive + reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse + reflectedLight.indirectSpecular /*+ float3(0.15f, 0.15f, 0.015f)*/;

    return float4(outgoingLight, opacity);
}

//float NormalDistributionGGXTR(float3 normalVec, float3 halfwayVec, float roughness)
//{
//    float a = roughness * roughness;
//    float a2 = a * a; // a2 = a^2
//    float NdotH = max(dot(normalVec, halfwayVec), 0.0); // NdotH = normalVec.halfwayVec
//    float NdotH2 = NdotH * NdotH; // NdotH2 = NdotH^2
//    float nom = a2;
//    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
//    denom = PI * denom * denom;

//    return nom / denom;
//}


//float GeometrySchlickGGX(float NdotV, float roughness)  // k is a remapping of roughness based on direct lighting or IBL lighting
//{
//    float r = roughness + 1.0f;
//    float k = (r * r) / 8.0f;

//    float nom = NdotV;
//    float denom = NdotV * (1.0f - k) + k;

//    return nom / denom;
//}


//float GeometrySmith(float3 normalVec, float3 viewDir, float3 lightDir, float k)
//{
//    float NdotV = max(dot(normalVec, viewDir), 0.0f);
//    float NdotL = max(dot(normalVec, lightDir), 0.0f);
//    float ggx1 = GeometrySchlickGGX(NdotV, k);
//    float ggx2 = GeometrySchlickGGX(NdotL, k);

//    return ggx1 * ggx2;
//}

//float3 FresnelSchlick(float cosTheta, float3 F0)   // cosTheta is n.v and F0 is the base reflectivity
//{
//    return (F0 + (1.0f - F0) * pow(1.0 - cosTheta, 5.0f));
//}

//float3 FresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)   // cosTheta is n.v and F0 is the base reflectivity
//{
//    return F0 + (max(float3(1.0f - roughness, 1.0f - roughness, 1.0f - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0f);
//}

//void CalcRadiance(PS_INPUT input, float3 viewDir, float3 normalVec, float3 albedo, float roughness, float metallic, float3 lightPos, float3 lightCol, float3 F0, out float3 rad)
//{
//    //static const float PI = 3.14159265359;

//	//calculate light radiance
//    float3 lightDir = normalize(lightPos - input.position);
//    float3 halfwayVec = normalize(viewDir + lightDir);
//    float distance = length(lightPos - input.position);
//    float attenuation = 1.0f / (distance * distance);
//    float3 radiance = lightCol * attenuation;

//	//Cook-Torrance BRDF
//    float D = NormalDistributionGGXTR(normalVec, halfwayVec, roughness);
//    float G = GeometrySmith(normalVec, viewDir, lightDir, roughness);
//    float3 F = FresnelSchlick(max(dot(halfwayVec, viewDir), 0.0f), F0);

//    float3 kS = F;
//    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
//    kD *= 1.0 - metallic;

//    float3 nom = D * G * F;
//    float denom = 4 * max(dot(normalVec, viewDir), 0.0f) * max(dot(normalVec, lightDir), 0.0) + 0.001f; // 0.001f just in case product is 0
//    float3 specular = nom / denom;

//	//Add to outgoing radiance Lo
//    float NdotL = max(dot(normalVec, lightDir), 0.0f);
//    rad = (((kD * albedo / PI) + specular) * radiance * NdotL);
//}

//float4 main(PS_INPUT input) : SV_Target
//{
//    //Albedo
//    float3 albedo = pow(albedoTex.Sample(sampler0, input.texcoord0).rgb, 2.2f);
//    //float3 albedo = albedoTex.Sample(sampler0, input.texcoord0).rgb;
    
//    //Normal
//    input.normal = normalize(input.normal);
//    input.tangent = normalize(input.tangent);
    
//    float3 normalFromMap = normalTex.Sample(sampler0, input.texcoord0).xyz * 2 - 1;

//    float3 N = input.normal;
//    float3 T = normalize(input.tangent - N * dot(input.tangent, N));
//    float3 B = cross(T, N);

//    float3x3 TBN = float3x3(T, B, N);
//    input.normal = normalize(mul(normalFromMap, TBN));
    
//    float3 normalVec = input.normal;
	
//	//Metallic
//    //float metallic = specularTex.Sample(sampler0, input.texcoord0).r;
//    float metallic = cbMetallic;

//	//Rough
//    //float rough = roughSRV.Sample(basicSampler, input.uv).r;
//    float rough = roughness;
	
//    float3 viewDir = normalize(cbEye.xyz - input.position);
	
//    float3 R = reflect(-viewDir, normalVec);

//    float3 F0 = float3(0.04f, 0.04f, 0.04f);
//    F0 = lerp(F0, albedo, metallic);

//    float3 rad = float3(0.0f, 0.0f, 0.0f);
//	//reflectance equation
//    float3 Lo = float3(0.0f, 0.0f, 0.0f);
    
//    //CalcRadiance(input, viewDir, normalVec, albedo, rough, metallic, lightPos1, lightCol, F0, rad);
//    //Lo += rad;

//    //CalcRadiance(input, viewDir, normalVec, albedo, rough, metallic, lightPos2, lightCol, F0, rad);
//    //Lo += rad;

//    //CalcRadiance(input, viewDir, normalVec, albedo, rough, metallic, lightPos3, lightCol, F0, rad);
//    //Lo += rad;

//    //CalcRadiance(input, viewDir, normalVec, albedo, rough, metallic, lightPos4, lightCol, F0, rad);
//    //Lo += rad;
    
//    float3 kS = FresnelSchlickRoughness(max(dot(normalVec, viewDir), 0.0f), F0, rough);
//    float3 kD = float3(1.0f, 1.0f, 1.0f) - kS;
//    kD *= 1.0 - metallic;
    
//    //float3 irradiance = skyIR.Sample(basicSampler, normalVec).rgb;
//    float3 diffuse = albedo /** irradiance*/;
    
//    const float MAX_REF_LOD = 4.0f;
//    //float3 prefilteredColor = skyPrefilter.SampleLevel(basicSampler, R, rough * MAX_REF_LOD).rgb;
//    //float2 brdf = brdfLUT.Sample(basicSampler, float2(max(dot(normalVec, viewDir), 0.0f), rough)).rg;
//    float3 specular = /*prefilteredColor * */(kS/* * brdf.x + brdf.y*/);

//    float3 ambient = (kD * diffuse + specular)/* * ao*/;
//    float3 color = ambient + Lo;
    
//    color = color / (color + float3(1.0f, 1.0f, 1.0f));
//    color = pow(color, float3(1.0f / 2.2f, 1.0f / 2.2f, 1.0f / 2.2f));

//    return float4(color, 1.0f);
//}