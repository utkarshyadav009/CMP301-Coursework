
//bloom pixel shader adds up the main render texture and the blured and downsampled render texture for the final bloom result  
Texture2D screen : register(t0);
Texture2D bloom : register(t1);
SamplerState SampleType : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
	// Combine the two scenes together to create the final bloom effect
    float4 mainColour = screen.Sample(SampleType, input.tex);
    float4 bloomColour = bloom.Sample(SampleType, input.tex);
    return saturate(mainColour + bloomColour);
}
