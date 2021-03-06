Texture2D SurfaceVisTex;
SamplerState SurfaceSampler;
 
struct VisualVSInput
{
    float3 Position     : POSITION; // vertex position 
    float2 TextureUV    : TEXCOORD0;// vertex texture coords 
};

struct VisualVSOutput
{
    float4 Position	: SV_Position; // vertex position 
    float2 UV		: TEXCOORD0;   // vertex texture coords
};

VisualVSOutput VS( VisualVSInput input )
{
    VisualVSOutput Output;

    Output.Position = float4(input.Position,1.0f);
    Output.UV = input.TextureUV;
 
    return Output;    
}


float4 PS( VisualVSOutput In ) : SV_TARGET
{
	float4 finalColor = SurfaceVisTex.Sample(SurfaceSampler, In.UV.xy);
 
	return finalColor;
}
