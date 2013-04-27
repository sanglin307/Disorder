// simple fx for render simple brush component.

cbuffer Transforms : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
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
    float4 Pos  : POSITION;
    float3 Norm : NORMAL;
};

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 NormView: TEXCOORD0; 
	float4 PosView: TEXCOORD1;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input )
{
    VS_OUTPUT output;
    output.Pos = mul( input.Pos, World );
    output.Pos = mul( output.Pos, View );
	output.PosView = output.Pos;
    output.Pos = mul( output.Pos, Projection );
    output.NormView = mul(float4(input.Norm,1.0),World);
	output.NormView = mul(output.NormView,View);
	 
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
		diffuseColor.xyz = saturate( dot(normalize(LightDir),normalize(input.NormView).xyz) * LightColor * LightIntensity);
	}
	else if( LightType == 1) // point 
	{
		float3 lightDir = LightPos - input.PosView;
		lightDir = normalize(lightDir);
		diffuseColor.xyz = saturate( dot(lightDir,normalize(input.NormView).xyz) * LightColor * LightIntensity);
	}

    return AmbientColor + diffuseColor * DiffuseColor;
}