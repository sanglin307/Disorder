// simple fx for render simple brush component.

cbuffer Transforms
{
	matrix World;
	matrix View;
	matrix Projection;
	matrix WorldNormal;  // translate for normal.
}

cbuffer GobalSetting
{
	float4 AmbientColor;
	float4 DiffuseColor;
}

cbuffer LightSetting
{
	int    LightType;
	float  LightIntensity;
	float3 LightPos;
	float3 LightDir;
	float3 LightColor;
	
}

struct VS_INPUT
{
    float3 Pos  : POSITION;
    float3 Norm : NORMAL;
};

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 NormWorld: TEXCOORD0; 
	float4 PosWorld: TEXCOORD1;
};

//----------------------------------------------------------------------------------------------------------------------------
// FinalColor = AmbientColor* SurfaceDiffuseColor + Dot( LightDir,Normal) * LightColor * LightIntensity * SurfaceDiffuseColor;
//----------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input )
{
    VS_OUTPUT output;
    output.Pos = mul( float4(input.Pos,1.0f),World);
	output.PosWorld = output.Pos;
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.NormWorld = mul(float4(input.Norm,1.0),WorldNormal);
	
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
	float4 diffuseColor = (float4)0;
	if( LightType == 0 ) // parallel 
	{
		diffuseColor.xyz = saturate( dot(LightDir,normalize(input.NormWorld).xyz) * LightColor * LightIntensity);
	}
	else if( LightType == 1) // point 
	{
		float3 lightDir = LightPos - normalize(input.PosWorld).xyz;
		diffuseColor.xyz = saturate( dot(lightDir,normalize(input.NormWorld).xyz) * LightColor * LightIntensity);
	}

    return diffuseColor * DiffuseColor;
}