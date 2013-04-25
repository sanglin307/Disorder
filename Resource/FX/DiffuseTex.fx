 
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


//VS_OUTPUT VS( float4 Position:POSITION,float4 TextureUV :TEXCOORD0 )
//{
//    VS_OUTPUT Output;
//    
//    Output.Position = mul(float4(Position.xyz,1.0f), WorldViewProjMatrix );
// 
//	Output.TextureUV = TextureUV.xy; 
//    
//    return Output;    
//}

 
//--------------------------------------------------------------------------------------
// This shader computes standard transform
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input )
{
    VS_OUTPUT Output;
    
    Output.Position = mul(float4(input.Position.xyz,1.0f), WorldViewProjMatrix );
 
	Output.TextureUV = input.TextureUV; 
    
    return Output;    
}

float4 PS( VS_OUTPUT In ) : SV_TARGET
{ 
    return DiffuseTexture.Sample( LinearSampler, In.TextureUV );
}
