 
cbuffer Transforms : register( b0 )
{
	matrix WorldViewProjMatrix;
}
//-----------------------------------------------------------------------------------------
// Textures and Samplers
//-----------------------------------------------------------------------------------------
Texture2D    DiffuseTexture : register( t0 );
SamplerState LinearSampler : register( s0 );

//--------------------------------------------------------------------------------------
// shader input/output structure
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 Position     : POSITION; // vertex position 
    float2 TextureUV    : TEXCOORD0;// vertex texture coords 
};

struct VS_OUTPUT
{
    float4 Position     : SV_POSITION; // vertex position
    float2 TextureUV    : TEXCOORD0;   // vertex texture coords 
};


 
//--------------------------------------------------------------------------------------
// This shader computes standard transform
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input )
{
    VS_OUTPUT Output;
    
    Output.Position = float4(0.5,0.5,0,1);//mul(float4(input.Position,1.0f), WorldViewProjMatrix );
	Output.TextureUV = input.TextureUV; 
    
    return Output;    
}

float4 PS( VS_OUTPUT In ) : SV_TARGET
{ 
    return float4(1,1,1,1); //DiffuseTexture.Sample( LinearSampler, In.TextureUV );
}
