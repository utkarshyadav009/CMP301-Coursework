//black fragment shader for rendering black geometry 
//for sea pass so that bloom doesn't get in the main render scene 
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};


float4 main(InputType input) : SV_TARGET
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    return float4(0.0f, 0.0f, 0.0f, 1.0f);

}