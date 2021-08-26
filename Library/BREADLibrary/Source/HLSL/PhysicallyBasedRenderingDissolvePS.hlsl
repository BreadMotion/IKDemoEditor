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
    float4 position;
    float4 color;
    float distance;
    float decay;
    float2 dummy;
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
    PointLight pointLight;
    float4 cbEye;
};

cbuffer DissolveCB : register(b2)
{
    float dissolveThreshold; //透過閾値
    float dissolveEmissiveWidth; //発光閾値(ディゾルブ・エミッシブ)
    float2 dummy;
}

//--------------------------------------------------------------------------------------
// リソース
//--------------------------------------------------------------------------------------
Texture2D albedoTex : register(t0);
Texture2D specularTex : register(t2);
Texture2D normalTex : register(t3);
Texture2D emissiveTex : register(t5);

Texture2D albedoTex2 : register(t6);
Texture2D maskTexture : register(t7);
Texture2D emissiveTexture : register(t8);

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

bool testLightInRange(const in float lightDistance, const in float cutoffDistance)
{
    return any(bool2(cutoffDistance == 0.0, lightDistance < cutoffDistance));
}

float punctualLightIntensityToIrradianceFactor(const in float lightDistance, const in float cutoffDistance, const in float decayExponent)
{
    if (decayExponent > 0.0f)
    {
        return pow(saturate(-lightDistance / cutoffDistance + 1.0f), decayExponent);
    }

    return 1.0f;
}

void getDirectionalDirectLightIrradiance(const in DirectionalLight directionalLight, const in GeometricContext geometry, out IncidentLight directLight)
{
    directLight.color = directionalLight.color.xyz;
    directLight.direction = directionalLight.direction.xyz;
    directLight.visible = true;
}

void getPointDirectLightIrradiance(const in PointLight pointLight, const in GeometricContext geometry, out IncidentLight directLight)
{
    float3 L = pointLight.position.xyz - geometry.position;
    directLight.direction = normalize(L);

    float lightDistance = length(L);
    if (testLightInRange(lightDistance, pointLight.distance))
    {
        directLight.color = pointLight.color.xyz;
        directLight.color *= punctualLightIntensityToIrradianceFactor(lightDistance, pointLight.distance, pointLight.decay);
        directLight.visible = true;
    }
    else
    {
        directLight.color = float3(0.0f, 0.0f, 0.0f);
        directLight.visible = false;
    }
}

void getSpotDirectLightIrradiance(const in SpotLight spotLight, const in GeometricContext geometry, out IncidentLight directLight)
{
    float3 L = spotLight.position - geometry.position;
    directLight.direction = normalize(L);

    float lightDistance = length(L);
    float angleCos = dot(directLight.direction, spotLight.direction);

    if (all(bool2(angleCos > spotLight.coneCos, testLightInRange(lightDistance, spotLight.distance))))
    {
        float spotEffect = smoothstep(spotLight.coneCos, spotLight.penumbraCos, angleCos);
        directLight.color = spotLight.color;
        directLight.color *= spotEffect * punctualLightIntensityToIrradianceFactor(lightDistance, spotLight.distance, spotLight.decay);
        directLight.visible = true;
    }
    else
    {
        directLight.color = float3(0.0f, 0.0f, 0.0f);
        directLight.visible = false;
    }
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
    //マスクの明暗
    //float limitPosY = 5.0f;
    //float2 maskTex = float2(0.0f, input.position.y / limitPosY);
    //float mask = maskTexture.Sample(sampler0, maskTex).r;
    float mask = maskTexture.Sample(sampler0, input.texcoord0).r;
    
    //閾値がマスクの値を超えていたら完全透過、超えていなかったら不透過の2値判定
    //しきい値未満の場合は0.0を、それ以上の場合は1.0の二値化を行う関数
    float thresold = 1.0f - dissolveThreshold;
    float alpha = step(mask, thresold);
    
    // 発光箇所計算
    float3 emis = (float3) 0;
    float3 emissive_color = emissiveTexture.Sample(sampler0, input.texcoord0).rgb;
    float emissive_value = (mask + dissolveEmissiveWidth > thresold) && (thresold > mask);
    emis = emissive_color * emissive_value;
    
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
    geometry.position = input.position;
    //geometry.normal = normalize(input.normal);
    geometry.normal = N;
    geometry.viewDir = normalize(cbEye.xyz - input.position);
  
    //float3 albedoColor = pow(albedoTex.Sample(sampler0, input.texcoord0).rgb, 2.2f) * albedo.xyz;
    float3 albedoColor = alpha == 0.0f ? albedoTex.Sample(sampler0, input.texcoord0).rgb : albedoTex2.Sample(sampler0, input.texcoord0).rgb;
    albedoColor *= albedo.xyz;
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
    
    // point light
    getPointDirectLightIrradiance(pointLight, geometry, directLight);
    if (directLight.visible)
    {
        RE_Direct(directLight, geometry, material, reflectedLight);
    }
  
    // directional light
    getDirectionalDirectLightIrradiance(dirLight[0], geometry, directLight);
    RE_Direct(directLight, geometry, material, reflectedLight);
    
    getDirectionalDirectLightIrradiance(dirLight[1], geometry, directLight);
    RE_Direct(directLight, geometry, material, reflectedLight);
    
    float3 outgoingLight = emissive + reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse + reflectedLight.indirectSpecular /*+ float3(0.15f, 0.15f, 0.015f)*/;
    outgoingLight += emis;

    return float4(outgoingLight, opacity);
}