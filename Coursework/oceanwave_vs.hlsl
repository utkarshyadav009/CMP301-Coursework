//ocean vertex shader 
//simulates the ocean wave using the gerstner wave formula for wave simulation 
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}

cbuffer CameraBuffer : register(b1)
{
    float3 cameraPosition;
    float padding;
};

cbuffer TimeBuffer : register(b2)
{
    float time;
    float Speed;
    float Amplitude;
    float Steepness;
    float2 Wavelength;
    float2 HeightMap;
    float4 direction[2];
}
struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition : TEXCOORD1;
    float3 viewVector : TEXCOORD2;
};
struct Waves
{
    float wave;
    float amplitude;
    float waveSpeed;
    float2 direction;
    float waveDir;
    float steepness;
    float3 wavePosition;
    float3 waveNormals;
};

OutputType main(InputType input)
{
    OutputType output;
   float4 textureColour = texture0.SampleLevel(sampler0, input.tex, 0);
    float PI = 3.141;
    //Initialising two wave objects
    Waves waves[2];
    //Initializing first wave
    waves[0].wave = (2 * PI) / Wavelength.x;
    waves[1].wave = (2 * PI) / Wavelength.y;
    for (int i = 0; i < 2;i++)
    {
        waves[i].amplitude = Amplitude;
        waves[i].waveSpeed = Speed * waves[i].wave;
        waves[i].direction = normalize(direction[i].xy);
        waves[i].waveDir = waves[i].wave * dot(waves[i].direction, input.position.xz) + (waves[i].waveSpeed * time);
        waves[i].steepness = Steepness / (waves[i].wave * waves[i].amplitude * 3);
    }
       
    //Initializing wave positions and normals with respect to the world position for the first wave 
    for (int i = 0; i < 2; i++)
    {
        waves[i].wavePosition.x = waves[i].steepness * waves[i].amplitude * waves[i].direction.x * cos(waves[i].waveDir);
        waves[i].wavePosition.y = waves[i].amplitude * sin(waves[i].waveDir);
        waves[i].wavePosition.z = waves[i].steepness * waves[i].amplitude * waves[i].direction.y * cos(waves[i].waveDir);
        
        float WA = (waves[i].wave * waves[i].amplitude);
        waves[i].waveDir = waves[i].wave * dot(waves[i].direction, input.position.xz) + (waves[i].waveSpeed * time);
        waves[i].waveNormals.x = waves[i].direction.x * (WA) * cos(waves[i].waveDir);
        waves[i].waveNormals.y = waves[i].steepness * (WA) * sin(waves[i].waveDir);
        waves[i].waveNormals.z = waves[i].direction.y * (WA) * cos(waves[i].waveDir);
    }
    //summing the positions and normals 
    float3 wavePositionSum = waves[0].wavePosition+ waves[1].wavePosition;
    float2 waveNormalSumXZ = -(waves[0].waveNormals.xz + waves[1].waveNormals.xz);
    float waveNormalsSumY = 1 - (waves[0].waveNormals.y + waves[1].waveNormals.y);
    
    //inputting normals and positions
    input.position.xyz += wavePositionSum;
    input.normal.xz = waveNormalSumXZ;
    input.normal.y = waveNormalsSumY;
    input.position.y += textureColour.r * HeightMap.x;
	// Calculate the position of the vertex against the worldirection, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader
    output.tex = input.tex;
    
    float4 worldPosition = mul(input.position, worldMatrix);
    output.viewVector = cameraPosition.xyz - worldPosition.xyz;
    output.viewVector = normalize(output.viewVector);
    output.worldPosition = mul(input.position, worldMatrix).xyz;
   
    // Calculate the normal vector against the world matrix only and normalize
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
    
    return output;
}