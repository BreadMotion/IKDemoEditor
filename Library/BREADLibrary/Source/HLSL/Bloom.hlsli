
cbuffer ConstantBuffer : register(b0)
{
    float glowExtractionThreshold;
    float blurConvolutionIntensity;
    float lensFlareThreshold;
    float lensFlareGhostDispersal;
    int numberOfGhosts;
    float lensFlareIntensity;
    int2 options;
}

// if you change value of 'number_of_downsampled', you must change 'number_of_downsampled' in bloom.h to this same value.
const static uint numberOfDownsampled = 6;