
Texture2D textureMap : register(t0);
Texture2D maskTexture : register(t1);
Texture2D maskTexture02 : register(t2);
Texture2D emissiveTexture : register(t3);

SamplerState samplerState : register(s0);

struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};

cbuffer DissolveCB : register(b0)
{
    float dissolveThreshold; //透過閾値
    float dissolveEmissiveWidth; //発光閾値(ディゾルブ・エミッシブ)
    float2 dummy;
}

float4 main(VS_OUT pin) : SV_TARGET
{
    //テクスチャ画像
    float4 color = textureMap.Sample(samplerState, pin.texcoord) * pin.color;
    
    //マスクの明暗
    float mask = maskTexture.Sample(samplerState, pin.texcoord).r;
    float mask02 = maskTexture02.Sample(samplerState, pin.texcoord).r;
    mask *= mask02;
    
    //閾値がマスクの値を超えていたら完全透過、超えていなかったら不透過の2値判定
    //しきい値未満の場合は0.0を、それ以上の場合は1.0の二値化を行う関数
    float thresold = 1.0f - dissolveThreshold;
    float alpha = step(mask, thresold);
    
    // 発光箇所計算
    float3 emissive = (float3) 0;
    float3 emissive_color = emissiveTexture.Sample(samplerState, pin.texcoord).rgb;
    float emissive_value = (mask + dissolveEmissiveWidth > thresold) && (thresold > mask);
    emissive = emissive_color * emissive_value;
   
    color.rgb += emissive;
    color.a *= alpha;

    return color;
}