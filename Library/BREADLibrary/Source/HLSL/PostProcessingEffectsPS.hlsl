
struct VS_OUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

//Volumetric Fog
struct FOG
{
    float3 colour;
    float highlight_intensity;
    float3 highlight_colour;
    float highlight_power;
    float global_density;
    float height_falloff;
    float start_depth;
    float start_height;
};

//Bokeh Effect
//Max Blur: The maximum amount of blurring.Ranges from 0 to 1
//Aperture: Bigger values create a shallower depth of field
//Focus: Controls the focus of the effect
//Aspect: Controls the blurring effect
struct BOKEH
{
    float max_blur;
    float aperture;
    float focus;
    float aspect;
};

struct SHADOWMAP
{
    float3 colour;
    float bias;
};

cbuffer POST_PROCESSING_EFFECTS_CONSTANTS : register(b0)
{
	//view-projection matrix from the light's point of view
    column_major float4x4 light_view_projection;
    row_major float4x4 inverse_view_projection;
	//FOG fog;
	//BOKEH bokeh;
	SHADOWMAP shadowmap;
};

#define ENABLE_4X_MSAA_COL 0
#define ENABLE_4X_MSAA_DEP 0

#if ENABLE_4X_MSAA_COL
Texture2DMS<float4> colour_map : register(t0);
#else
Texture2D colour_map : register(t0);
#endif

#if ENABLE_4X_MSAA_DEP
Texture2DMS<float4> depth_map : register(t1);
#else
Texture2D depth_map : register(t1);
#endif

Texture2D shadow_map : register(t2);

SamplerState point_sampler_state : register(s0);
SamplerState linear_sampler_state : register(s1);
SamplerState anisotropic_sampler_state : register(s2);
SamplerComparisonState comparison_sampler_state : register(s3);


float4 main(VS_OUT pin) : SV_TARGET
{
#if ENABLE_4X_MSAA_COL
	uint colour_map_width, colour_map_height, colour_map_number_of_samples;
	colour_map.GetDimensions(colour_map_width, colour_map_height, colour_map_number_of_samples);
	float4 colour_map_colour = colour_map.Load(uint2(pin.texcoord.x * colour_map_width, pin.texcoord.y * colour_map_height), sample_index);
#else
    uint colour_map_mip_level = 0, colour_map_width, colour_map_height, colour_map_number_of_levels;
    colour_map.GetDimensions(colour_map_mip_level, colour_map_width, colour_map_height, colour_map_number_of_levels);
    float4 colour_map_colour = colour_map.Sample(linear_sampler_state, pin.texcoord);
#endif

#if ENABLE_4X_MSAA_DEP
	uint depth_map_width, depth_map_height, depth_map_number_of_samples;
	depth_map.GetDimensions(depth_map_width, depth_map_height, depth_map_number_of_samples);
	float4 depth_map_colour = depth_map.Load(uint2(pin.texcoord.x * depth_map_width, pin.texcoord.y * depth_map_height), sample_index);
#else
    float4 depth_map_colour = depth_map.Sample(linear_sampler_state, pin.texcoord);
#endif
    float3 fragment_colour = colour_map_colour.rgb;
    float alpha = colour_map_colour.a;

	//bokeh effect
#if ENABLE_4X_MSAA_COL
	fragment_colour = bokeh_effect_ms(colour_map, sample_index, colour_map_width, colour_map_height, depth_map_colour.x, pin.texcoord);
#else
   // fragment_colour = bokeh_effect(colour_map, depth_map_colour.x, linear_sampler_state, pin.texcoord);
#endif

	//shadow mapping
    float4 position_in_ndc;
	//texture space to ndc
    position_in_ndc.x = pin.texcoord.x * 2 - 1;
    position_in_ndc.y = pin.texcoord.y * -2 + 1;
    position_in_ndc.z = depth_map_colour.x;
    position_in_ndc.w = 1;

	//ndc to world space
    float4 position_in_world_space = mul(position_in_ndc, inverse_view_projection);
    //float4 position_in_world_space = position_in_ndc;
    position_in_world_space /= position_in_world_space.w;

	//if (time_of_day > 5.5 && time_of_day < 18.5)
	{
		//world space to light view clip space, and to ndc
        float4 position_in_light_space = mul(position_in_world_space, light_view_projection);
        position_in_light_space /= position_in_light_space.w;
        //if (0 < position_in_light_space.z && position_in_light_space.z < 1) // TODO:?
        {
			//ndc to texture space
            position_in_light_space.x = position_in_light_space.x * 0.5 + 0.5;
            position_in_light_space.y = position_in_light_space.y * -0.5 + 0.5;
            
           // fragment_colour = float3(0.0f, 1.0f, 0.0f);

            float shadow_threshold = 0.0f;
			//float shadow_bias = 0.0008;
			//float3 shadow_colour = float3(0.65, 0.65, 0.65);
			//PCF:percentage closer filtering
			//http://developer.download.nvidia.com/presentations/2008/GDC/GDC08_SoftShadowMapping.pdf
            shadow_threshold = shadow_map.SampleCmpLevelZero(comparison_sampler_state, position_in_light_space.xy, position_in_light_space.z - shadowmap.bias);
            //shadow_threshold = shadow_map.GatherCmpRed(comparison_sampler_state, position_in_light_space.xy, position_in_light_space.z - shadowmap.bias);
#if 0
            int offset = 2;
            shadow_threshold += shadow_map.SampleCmpLevelZero(comparison_sampler_state, position_in_light_space.xy, position_in_light_space.z - shadowmap.bias, int2(-offset, -offset)) * 1;
            shadow_threshold += shadow_map.SampleCmpLevelZero(comparison_sampler_state, position_in_light_space.xy, position_in_light_space.z - shadowmap.bias, int2(-offset, 0)) * 2;
            shadow_threshold += shadow_map.SampleCmpLevelZero(comparison_sampler_state, position_in_light_space.xy, position_in_light_space.z - shadowmap.bias, int2(-offset, offset)) * 1;
            shadow_threshold += shadow_map.SampleCmpLevelZero(comparison_sampler_state, position_in_light_space.xy, position_in_light_space.z - shadowmap.bias, int2(0, -offset)) * 2;
            shadow_threshold += shadow_map.SampleCmpLevelZero(comparison_sampler_state, position_in_light_space.xy, position_in_light_space.z - shadowmap.bias, int2(0, 0)) * 4;
            shadow_threshold += shadow_map.SampleCmpLevelZero(comparison_sampler_state, position_in_light_space.xy, position_in_light_space.z - shadowmap.bias, int2(0, offset)) * 2;
            shadow_threshold += shadow_map.SampleCmpLevelZero(comparison_sampler_state, position_in_light_space.xy, position_in_light_space.z - shadowmap.bias, int2(offset, -offset)) * 1;
            shadow_threshold += shadow_map.SampleCmpLevelZero(comparison_sampler_state, position_in_light_space.xy, position_in_light_space.z - shadowmap.bias, int2(offset, 0)) * 2;
            shadow_threshold += shadow_map.SampleCmpLevelZero(comparison_sampler_state, position_in_light_space.xy, position_in_light_space.z - shadowmap.bias, int2(offset, offset)) * 1;
            shadow_threshold /= 16;
#endif

            shadow_threshold = lerp(shadowmap.colour, float3(1.0f, 1.0f, 1.0f), shadow_threshold).x;
            fragment_colour *= shadow_threshold;
        }
    }

	//fragment_colour = apply_fog(fragment_colour, position_in_world_space.xyz, camera.position.xyz);
    //fragment_colour = apply_fog(fragment_colour, position_in_world_space.xyz, operation_object.position.xyz);

    return float4(fragment_colour, alpha);
}