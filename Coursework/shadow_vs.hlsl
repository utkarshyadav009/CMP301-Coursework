
//shadow vertex shader 
//calculates the position of the vertices from lights point of veiw 
//used by directional and spotlight 
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix pointlightViewMatrix;
    matrix pointlightProjectionMatrix;
    matrix spotlightViewMatrix;
    matrix spotlightProjectionMatrix;
};
cbuffer CameraBuffer : register(b1)
{
    float3 cameraPosition;
    float padding;
};
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
    float4 lightViewPos1 : TEXCOORD1;
    float4 lightViewPos2 : TEXCOORD2;
    float3 worldPosition : TEXCOORD3;
    float3 viewVector : TEXCOORD4;
};


OutputType main(InputType input)
{
    OutputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Calculate the position of the vertice as viewed by the directional light source.
    output.lightViewPos1 = mul(input.position, worldMatrix);
    output.lightViewPos1 = mul(output.lightViewPos1, pointlightViewMatrix);
    output.lightViewPos1 = mul(output.lightViewPos1, pointlightProjectionMatrix);
 
    // Calculate the position of the vertice as viewed by the spotlight source.
    output.lightViewPos2 = mul(input.position, worldMatrix);
    output.lightViewPos2 = mul(output.lightViewPos2, spotlightViewMatrix);
    output.lightViewPos2 = mul(output.lightViewPos2, spotlightProjectionMatrix);

    output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only and normalise.
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);
	
	//calculate position of the vertex in the world 
    float4 worldPosition = mul(input.position, worldMatrix);
    output.viewVector = cameraPosition.xyz - worldPosition.xyz;
    output.viewVector = normalize(output.viewVector);
	
    output.worldPosition = mul(input.position, worldMatrix).xyz;
    
    return output;
}