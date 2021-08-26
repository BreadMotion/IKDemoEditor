#ifndef SKINNING_HLSLI
#define SKINNING_HLSLI

#if defined(USE_BONE)

float3 SkinningPosition(float4 position, uint4 blend_index0, uint4 blend_index1, float4 blend_weight0, float4 blend_weight1)
{
	float3 o  = mul(position, cb_bones[blend_index0.x]).xyz * blend_weight0.x;
	o += mul(position, cb_bones[blend_index0.y]).xyz * blend_weight0.y;
	o += mul(position, cb_bones[blend_index0.z]).xyz * blend_weight0.z;
	o += mul(position, cb_bones[blend_index0.w]).xyz * blend_weight0.w;

	o += mul(position, cb_bones[blend_index1.x]).xyz * blend_weight1.x;
	o += mul(position, cb_bones[blend_index1.y]).xyz * blend_weight1.y;
	o += mul(position, cb_bones[blend_index1.z]).xyz * blend_weight1.z;
	o += mul(position, cb_bones[blend_index1.w]).xyz * blend_weight1.w;

	return o;
}

float3 SkinningVector(float3 vec, uint4 blend_index0, uint4 blend_index1, float4 blend_weight0, float4 blend_weight1)
{
	float4 v = float4(vec, 0.0f);
	float3 o = mul(v, cb_bones[blend_index0.x]).xyz * blend_weight0.x;
	o += mul(v, cb_bones[blend_index0.y]).xyz * blend_weight0.y;
	o += mul(v, cb_bones[blend_index0.z]).xyz * blend_weight0.z;
	o += mul(v, cb_bones[blend_index0.w]).xyz * blend_weight0.w;

	o += mul(v, cb_bones[blend_index1.x]).xyz * blend_weight1.x;
	o += mul(v, cb_bones[blend_index1.y]).xyz * blend_weight1.y;
	o += mul(v, cb_bones[blend_index1.z]).xyz * blend_weight1.z;
	o += mul(v, cb_bones[blend_index1.w]).xyz * blend_weight1.w;

	return o;
}

#endif

#endif