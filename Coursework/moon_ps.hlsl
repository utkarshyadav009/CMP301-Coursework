
//moon pixel shader 
//returns the texture of the moon without any lighting 
//moon does not need any lighting as it emits light 

Texture2D texture0 : register(t0);


SamplerState Sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(InputType input) : SV_TARGET
{
    float4 textureColor;
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = texture0.Sample(Sampler0, input.tex);
	 
    return textureColor;
} 