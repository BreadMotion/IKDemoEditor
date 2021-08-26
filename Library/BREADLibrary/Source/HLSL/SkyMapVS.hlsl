//#define USE_POSITION
//#define USE_NORMAL
#define USE_UVW
//#define USE_TEXCOORD0

#define CB_SCENE	b0
#define CB_MESH		b1

#include "CommonCB.hlsli"
#include "CommonLayout.hlsli"


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    output.sv_position = mul(input.position, mul(cb_world, cb_view_projection));
    output.uvw = normalize(input.position.xyz);

    return output;
}