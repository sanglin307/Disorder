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

// support max 4 direct lights now
cbuffer LightSetting
{
	int    LightNumber;
	float4 LightIntensityPack;
	float3 LightDirArray[4];
	float3 LightColorArray[4];	
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
	float3 normal = normalize(input.NormWorld).xyz;

	// max 4 direct lights add
	for( int i=0;i<LightNumber;i++)
	{
		diffuseColor.xyz += saturate( max(dot(LightDirArray[i],normal),0) * LightColorArray[i] * LightIntensityPack[i]);
	}
	
	diffuseColor.a = 1.0f;

    return AmbientColor*DiffuseColor + diffuseColor * DiffuseColor;
}