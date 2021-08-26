#define ENABLE_MSAA 0

#if ENABLE_MSAA
Texture2DMS<float4> hdrColorMap : register(t0);
#else
Texture2D hdrColorMap : register(t0);
#endif

Texture2D averageLuminance : register(t1);
SamplerState pointSamplerState : register(s0);
SamplerState linearSamplerState : register(s1);
SamplerState anisotropicSamplerState : register(s2);


cbuffer ConstantBuffer : register(b0)
{
    float whitePoint = 3.0f; //The luminance level to use as the upper end of a tone mapping curve.
    float averageGray = 1.147f; //The luminance level to use as the midpoint of a tone mapping curve.

	//Brightness - Contrast Effect
	//The brightness - contrast effect allows you to modify the brightness and contrast of the rendered image.
	//Brightness: The brighness of the image.Ranges from - 1 to 1 (-1 is solid black, 0 no change, 1 solid white).
	//Contrast : The contrast of the image.Ranges from - 1 to 1 (-1 is solid gray, 0 no change, 1 maximum contrast).
    float brightness = 0.0;
    float contrast = 0.0;

	//Hue - Saturation Effect
	//The hue - saturation effect allows you to modify the hue and saturation of the rendered image.
	//Hue: The hue of the image.Ranges from - 1 to 1 (-1 is 180 degrees in the negative direction, 0 no change, 1 is 180 degrees in the postitive direction).
	//Saturation : The saturation of the image.Ranges from - 1 to 1 (-1 is solid gray, 0 no change, 1 maximum saturation).
    float hue = 0.0;
    float saturation = 0.0;

	//Sepia Effect
	//The Sepia effect makes the image look like an old photograph.
	//Amount: Controls the intensity of the effect. Ranges from 0 to 1.
    float amount = 0.0;

	//Vignette Effect
	//In photography and optics, vignetting is a reduction of an image's brightness or saturation at the periphery compared to the image center.
	//You can use it to draw attention to the center of the frame. (from Wikipedia)
	//Offset: Controls the offset of the effect.
	//Darkness : Controls the darkness of the effect.
    float offset = 0.0;
    float darkness = 0.0;

    float3 options;
}

//https://developer.playcanvas.com/en/user-manual/graphics/posteffects/
float3 BrightnessContrastEffect(float3 fragmentColor, float brightness, float contrast)
{
	//Brightness - Contrast Effect
	//The brightness - contrast effect allows you to modify the brightness and contrast of the rendered image.
	//Brightness: The brighness of the image.Ranges from - 1 to 1 (-1 is solid black, 0 no change, 1 solid white).
	//Contrast : The contrast of the image.Ranges from - 1 to 1 (-1 is solid gray, 0 no change, 1 maximum contrast).
    fragmentColor += brightness;
    if (contrast > 0.0)
    {
        fragmentColor = (fragmentColor - 0.5) / (1.0 - contrast) + 0.5;
    }
    else if (contrast < 0.0)
    {
        fragmentColor = (fragmentColor - 0.5) * (1.0 + contrast) + 0.5;
    }
    return fragmentColor;
}

float3 HueSaturationEffect(float3 fragmentColor, float hue, float saturation)
{
	//Hue - Saturation Effect
	//The hue - saturation effect allows you to modify the hue and saturation of the rendered image.
	//Hue: The hue of the image.Ranges from - 1 to 1 (-1 is 180 degrees in the negative direction, 0 no change, 1 is 180 degrees in the postitive direction).
	//Saturation : The saturation of the image.Ranges from - 1 to 1 (-1 is solid gray, 0 no change, 1 maximum saturation).
    float angle = hue * 3.14159265;
    float s = sin(angle), c = cos(angle);
    float3 weights = (float3(2.0 * c, -sqrt(3.0) * s - c, sqrt(3.0) * s - c) + 1.0) / 3.0;
    float len = length(fragmentColor);
    fragmentColor = float3(dot(fragmentColor, weights.xyz), dot(fragmentColor, weights.zxy), dot(fragmentColor, weights.yzx));
    float average = (fragmentColor.r + fragmentColor.g + fragmentColor.b) / 3.0;
    if (saturation > 0.0)
    {
        fragmentColor += (average - fragmentColor) * (1.0 - 1.0 / (1.001 - saturation));
    }
    else
    {
        fragmentColor += (average - fragmentColor) * (-saturation);
    }
    return fragmentColor;
}

float3 SepiaEffect(float3 fragmentColor, float amount)
{
	//Sepia Effect
	//The Sepia effect makes the image look like an old photograph.
	//Amount: Controls the intensity of the effect. Ranges from 0 to 1.
    fragmentColor.r = dot(fragmentColor, float3(1.0 - 0.607 * amount, 0.769 * amount, 0.189 * amount));
    fragmentColor.g = dot(fragmentColor, float3(0.349 * amount, 1.0 - 0.314 * amount, 0.168 * amount));
    fragmentColor.b = dot(fragmentColor, float3(0.272 * amount, 0.534 * amount, 1.0 - 0.869 * amount));
    return fragmentColor;
}

float3 VignetteEffect(float3 fragmentColor, float2 uv, float offset, float darkness)
{
	//Vignette Effect
	//In photography and optics, vignetting is a reduction of an image's brightness or saturation at the periphery compared to the image center.
	//You can use it to draw attention to the center of the frame. (from Wikipedia)
	//Offset: Controls the offset of the effect.
	//Darkness : Controls the darkness of the effect.
    uv = (uv - 0.5) * offset;
    fragmentColor = lerp(fragmentColor, (1.0 - darkness), dot(uv, uv));
    return fragmentColor;
}

float3 GrayscaleEffect(float3 fragmentColor)
{
    return dot(fragmentColor, float3(0.299, 0.587, 0.114));
}

float3 ScanlineEffect(float3 fragmentColor, uint screenHeight, float2 texcoord, float pitch, float ratio = 0)
{
    return fragmentColor * ((fmod(texcoord.y * screenHeight, pitch) > 1) ? ratio : 1);
}

float4 main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD, uint sampleIndex : SV_SAMPLEINDEX) : SV_TARGET
{
#if ENABLE_MSAA
	uint hdrColorMapWidth, hdrColorMapHeight, numberOfSamples;
	hdrColorMap.GetDimensions(hdrColorMapWidth, hdrColorMapHeight, numberOfSamples);
	float4 colorMapColor = hdrColorMap.Load(uint2(texcoord.x * hdrColorMapWidth, texcoord.y * hdrColorMapHeight), sampleIndex);
#else
    float4 colorMapColor = hdrColorMap.Sample(pointSamplerState, texcoord);
#endif
    float luminance = dot(colorMapColor.rgb, float3(0.2126, 0.7152, 0.0722));

    uint mipLevel = 0, width, height, numberOfLevels;
    averageLuminance.GetDimensions(mipLevel, width, height, numberOfLevels);
    float averageLuminanceSample = averageLuminance.SampleLevel(pointSamplerState, float2(0.5, 0.5), numberOfLevels - 1).x;

    float scaledLuminance = averageGray / averageLuminanceSample;
    float mappedColorScale = (scaledLuminance + (scaledLuminance * scaledLuminance) / (whitePoint * whitePoint)) / (1.0 + scaledLuminance);
    float3 fragmentColor = colorMapColor.rgb * mappedColorScale;

    fragmentColor = BrightnessContrastEffect(fragmentColor, brightness, contrast);
    fragmentColor = HueSaturationEffect(fragmentColor, hue, saturation);
    fragmentColor = SepiaEffect(fragmentColor, amount);
    fragmentColor = VignetteEffect(fragmentColor, texcoord, offset, darkness);

	//fragmentColor = GrayscaleEffect(fragmentColor);
	//fragmentColor = ScanlineEffect(fragmentColor, height, texcoord, 5, 0.5);

    return float4(fragmentColor, 1);
}