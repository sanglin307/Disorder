cbuffer Transforms
{
	matrix World;
	matrix View;
	matrix Projection;
}
 

//--------------------------------------------------------------------------------------
// shader input/output structure
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 Position     : POSITION; // vertex position 
    float4 Color        : COLOR;//  
};

struct VS_OUTPUT
{
    float4 Position     : SV_POSITION; // vertex position
    float4 Color        : COLOR;   //  
};
 
//--------------------------------------------------------------------------------------
// This shader computes standard transform
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input )
{
    VS_OUTPUT Output;
    
	Output.Position = mul( float4(input.Position,1.0f),World);
    Output.Position = mul( Output.Position, View );
    Output.Position = mul( Output.Position, Projection );
 
	Output.Color = input.Color; 
    
    return Output;    
}

float4 PS( VS_OUTPUT In ) : SV_TARGET
{ 
    return In.Color;
}