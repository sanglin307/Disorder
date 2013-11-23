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
    // Lookup mesh texture and modulate it with diffuse
	//In.Color.r = DiffuseTexture.Sample( LinearSampler, In.TextureUV ).g;
	//In.Color.g = DiffuseTexture.Sample( LinearSampler, In.TextureUV ).g;
	//In.Color.b = DiffuseTexture.Sample( LinearSampler, In.TextureUV ).g;

	In.Color.a *= DiffuseTexture.Sample( LinearSampler, In.TextureUV ).g;
	//return g_txDiffuse.Sample( g_samLinear, In.TextureUV ) ;
 

	return In.Color;
}
