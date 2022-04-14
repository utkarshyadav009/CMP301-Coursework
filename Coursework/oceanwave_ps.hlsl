// Ocean pixel shader
// Calculates directional and spotlight 
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 ambient[2];
    float4 diffuse[2];
    float4 direction[2];
    float4 position[2];
    float4 cutoff;
    float4 specularPower;
    float4 specular;
    bool showNormals;
    float3 padding;
};
struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
    float3 viewVector : TEXCOORD2;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
    float intensity = saturate(dot(normal, lightDirection));
    float4 colour = saturate(diffuse * intensity);
    return colour;
}
float4 calcSpecular(float3 lightDirection, float3 normal, float3 viewVector, float4 specularColour, float specularPower)
{
    //blinn-phong specular calculation
    float3 halfway = normalize(lightDirection + viewVector);
    float specularIntensity = pow(max(dot(normal, halfway), 0.0), specularPower);
    return saturate(specularColour * specularIntensity);
}

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour;
    float4 lightColour;
    float4 lightColourResult = float(0);

	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
    textureColour = texture0.Sample(sampler0, input.tex);
  
    if(showNormals)
    {
        textureColour.x = input.normal.x;
        textureColour.y = input.normal.y;
        textureColour.z = input.normal.z;
        textureColour.a = 0.0f;
        return textureColour;
    }
    
    float3 dir = normalize(-direction[0].xyz);
    
    float3 lightVector = normalize(position[1].xyz - input.worldPosition);
    float theta = dot(normalize(-direction[1].xyz), lightVector);
    //if theta is larger than the cutoff value it'll get lit by the let other wise its the ambient colour 
    if (theta > cutoff.x)
    {
        float4 specularColour2 = calcSpecular(normalize(-direction[1].xyz), input.normal, input.viewVector, specular, specularPower.x);
        lightColourResult += calculateLighting(-direction[1].xyz, input.normal, diffuse[1]) + specularColour2 + ambient[1];
    }
    else
    {
        lightColourResult += ambient[1];
    }
    
    float4 specularColour = calcSpecular(dir, input.normal, input.viewVector, specular, specularPower.x);
  
    lightColourResult += specularColour + ambient[0] + calculateLighting(dir, input.normal, diffuse[0]);
   
    //returning the sum of both direction and spot light 
    return lightColourResult * textureColour;
}
