// Texture and sampler registers
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);


cbuffer thresholdBuffer : register(b0)
{
    float4 thresholds;
}

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

//returns the entire texture as it is 
//threshold detection causes the ocean to be not bloomed 
//as its texture is just a red png without any dark or light pixels 
//could be used if provided the right texture 
float4 main(InputType input) : SV_TARGET
{
    float4 pixel = texture0.Sample(Sampler0, input.tex);
    return pixel;
  // // If the pixels are above the given threshold they'll get bloomed 
  // if (pixel.x >= thresholds.x && pixel.y >= thresholds.y && pixel.z >= thresholds.z)
  // {
  //     return pixel;
  // }
  // 
  // return float4(0.0f, 0.0f, 0.0f, 1.0f);

}
