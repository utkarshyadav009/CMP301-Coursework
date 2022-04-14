//shadow pixel shader
//calculates shadows and light for both directional as well as spotlight 
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture1 : register(t1);
Texture2D depthMapTexture2 : register(t2);


SamplerState diffuseSampler : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
    float4 ambient[2];
    float4 diffuse[2];
    float4 direction[2];
    float4 position[2];
    float4 cutoff;
    float4 specularPower;
    float4 specular;
    bool spotlightToggle;
    float3 padding;
    bool moonlightToggle;
    float3 padding1;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 lightViewPos1 : TEXCOORD1;
    float4 lightViewPos2 : TEXCOORD2;
    float3 worldPosition : TEXCOORD3;
    float3 viewVector : TEXCOORD4;
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

// Is the gemoetry in our shadow map
bool hasDepthData(float2 uv)
{
    if (uv.x < 0.f || uv.x > 1.f || uv.y < 0.f || uv.y > 1.f)
    {
        return false;
    }
    return true;
}

bool isInShadow(Texture2D sMap, float2 uv, float4 lightViewPosition, float bias)
{
    // Sample the shadow map (get depth of geometry)
    float depthValue = sMap.Sample(shadowSampler, uv).r;
	// Calculate the depth from the light.
    float lightDepthValue = lightViewPosition.z / lightViewPosition.w;
    lightDepthValue -= bias;

	// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
    if (lightDepthValue < depthValue)
    {
        return false;
    }
    return true;
}

float2 getProjectiveCoords(float4 lightViewPosition)
{
    // Calculate the projected texture coordinates.
    float2 projTex = lightViewPosition.xy / lightViewPosition.w;
    projTex *= float2(0.5, -0.5);
    projTex += float2(0.5f, 0.5f);
    return projTex;
}

float4 main(InputType input) : SV_TARGET
{
    float shadowMapBias = 0.005f;
    float4 textureColour = shaderTexture.Sample(diffuseSampler, input.tex);
    
	// Calculate the projected texture coordinates.
    float2 pTexCoord1 = getProjectiveCoords(input.lightViewPos1);
    float2 pTexCoord2 = getProjectiveCoords(input.lightViewPos2);

	//texture colour variables for different lights 
    float4 c1 = float4(0.f, 0.f, 0.f, 1.f);
    float4 c2 = float4(0.f, 0.f, 0.f, 1.f);
    
    float3 lightVector = normalize(position[1].xyz - input.worldPosition);
    
    float theta = dot(normalize(-direction[1].xyz), lightVector);
    float4 spotlight, moonlight;
    float4 lightcolour;
    {   //direction light calculation 
        float4 specularColour1 = calcSpecular(normalize(-direction[0].xyz), input.normal, input.viewVector, specular, specularPower.x);
        c1 = calculateLighting(-direction[0].xyz, input.normal, diffuse[0]) + specularColour1 + ambient[0];
        //spotlight calculation
        if (theta > cutoff.x)
        {
            float4 specularColour2 = calcSpecular(normalize(-direction[1].xyz), input.normal, input.viewVector, specular, specularPower.y);
            c2 = calculateLighting(-direction[1].xyz, input.normal, diffuse[1]) + specularColour2 + ambient[1];
        }
        else
        {
            c2 = ambient[1];
        }
        if (spotlightToggle)
        {
            spotlight = c2;
        }
        else
        {
            spotlight = ambient[1];
        }
        if (moonlightToggle)
        {
            moonlight = c1;
        }
        else
        {
            moonlight = ambient[0];
        }
        
        lightcolour = lerp(moonlight, spotlight, 0.5);
    }
    // Shadow test. Is or isn't in shadow
    if (moonlightToggle)
    {
        if (!hasDepthData(pTexCoord1) ||
        isInShadow(depthMapTexture1, pTexCoord1, input.lightViewPos1, shadowMapBias))
        {

            lightcolour -= float4(0.3, 0.3, 0.3, 1);
        }
    }
    // shadow test for spotlight is in the spotlight or not 
    if (spotlightToggle)
    {
        if (theta > cutoff.x)
        {
            if (!hasDepthData(pTexCoord2) ||
        isInShadow(depthMapTexture2, pTexCoord2, input.lightViewPos2, shadowMapBias))
            {

                lightcolour -= float4(0.3, 0.3, 0.3, 1);
            }
        }
    }
        
    float4 finalambient = lerp(ambient[0], ambient[1], 0.5);

   
    lightcolour = saturate(lightcolour + finalambient);
    return saturate(lightcolour) * textureColour;
}