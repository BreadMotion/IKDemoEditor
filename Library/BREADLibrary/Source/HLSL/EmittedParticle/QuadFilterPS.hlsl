
Texture2D textureMap : register(t0);
SamplerState samplerState : register(s0);

struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};

cbuffer FilterCB : register(b0)
{
    float bright; //明度
    float contrast; //濃淡
    float saturate; //彩度
    float option;
    float4 screenColor; //色調
}

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = textureMap.Sample(samplerState, pin.texcoord) * pin.color;
    
    // 明度調整
    color.rgb += bright;
    
    // コントラスト調整
    color.rgb = ((color.rgb - 0.5f) * contrast) + 0.5f;
    
    // 彩度調整
    float avr = (color.r + color.g + color.b) / 3;
    color.rgb = (color.rgb - avr) * saturate + avr;
    
    // カラーバランス調整
    color.rgb *= screenColor.rgb;
    
    return color;
}