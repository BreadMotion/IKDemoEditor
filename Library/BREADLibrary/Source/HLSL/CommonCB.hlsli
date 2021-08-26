#ifndef COMMON_CB_HLSLI
#define COMMON_CB_HLSLI

//--------------------------------------------------------------------------------------
// 定数バッファ
//--------------------------------------------------------------------------------------
#if defined(CB_SCENE)
cbuffer CbScene : register(CB_SCENE)
{
	float4x4    cb_view;
	float4x4    cb_projection;
	float4x4    cb_view_projection;
	float4x4    cb_viewInv;
	float4x4    cb_projectionInv;
	float4x4    cb_view_projectionInv;
	float4x4    cb_prev_view;
	float4x4    cb_prev_projection;
	float4x4    cb_prev_view_projection;
	float4x4    cb_prev_viewInv;
	float4x4    cb_prev_projectionInv;
	float4x4    cb_prev_view_projectionInv;
	float4      cb_viewport;
};
#endif

#if defined(CB_MESH)
cbuffer CbMesh : register(CB_MESH)
{
	float4x4    cb_world;
	float4x4    cb_world_inverse;
	float4x4	cb_texture;
};
#endif

#if defined(CB_BONE)
cbuffer CbBone : register(CB_BONE)
{
	float4x4    cb_bones[512];
};
#endif

#if defined(CB_SHADOW)
cbuffer CbShadow : register(CB_SHADOW)
{
	float4x4    cb_shadow;
	float		cb_texel_size;
};
#endif

#endif	// COMMON_CB_HLSLI