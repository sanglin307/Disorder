//-----------------------------------------------------------------------------------------
// Textures and Samplers
//-----------------------------------------------------------------------------------------
Texture2D    TextTexture : register( t0 );
SamplerState TextSampler : register( s0 );

//--------------------------------------------------------------------------------------
// shader input/output structure
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 Position     : POSITION; // vertex position 
	float4 Color        : COLOR;
    float2 TextureUV    : TEXCOORD0;// vertex texture coords 
};

struct VS_OUTPUT
{
    float4 Position     : SV_POSITION; // vertex position
    float4 Color        : COLOR;
    float2 TextureUV    : TEXCOORD0;   // vertex texture coords 
};

//--------------------------------------------------------------------------------------
// This shader computes standard transform
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input )
{
    VS_OUTPUT Output;
    
	Output.Position  = float4(input.Position,1);
	Output.Color     = input.Color;
    Output.TextureUV = input.TextureUV; 
    
    return Output;    
}
 
float4 PS( VS_OUTPUT In ) : SV_TARGET
{ 
	In.Color.a *= TextTexture.Sample( TextSampler, In.TextureUV ).g;
	return In.Color;
}
