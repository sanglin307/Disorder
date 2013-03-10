//-----------------------------------------------------------------------------------------
// Textures and Samplers
//-----------------------------------------------------------------------------------------
Texture2D    g_txDiffuse : register( t0 );
SamplerState g_samLinear : register( s0 );

//--------------------------------------------------------------------------------------
// shader input/output structure
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Position     : POSITION; // vertex position 
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
    
	Output.Position  = input.Position;
	Output.Color     = input.Color;
    Output.TextureUV = input.TextureUV; 
    
    return Output;    
}
 
float4 PS( VS_OUTPUT In ) : SV_TARGET
{ 
    // Lookup mesh texture and modulate it with diffuse
	In.Color.a *= g_txDiffuse.Sample( g_samLinear, In.TextureUV ).g;
	//return g_txDiffuse.Sample( g_samLinear, In.TextureUV ) ;
 

	return In.Color;
}
