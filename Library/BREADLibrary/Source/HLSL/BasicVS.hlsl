#define USE_POSITION
#define USE_TEXCOORD0
#define USE_SHADOW
#include "CommonVS.hlsli"

//--------------------------------------------------------------------------------------
// 頂点シェーダー
//--------------------------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT input)
{
    return VSCommon(input); 
}
