cbuffer CameraTransforms
{
	matrix CameraView;
	matrix CameraProjection;	
	float3 CameraPosition;
}

cbuffer ObjectTransform
{
	matrix WorldTransform;
	matrix WorldNormal;  // translate for normal.
}

cbuffer MaterialProperty
{
	float4 AmbientColor;
	float4 DiffuseColor;
}

// support max 4 direct lights now
cbuffer LightProperty
{
	int    LightNumber;
	float4 LightIntensityPack;
	float3 LightDirArray;
	float3 LightColorArray;	
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
    output.Pos = mul( float4(input.Pos,1.0f),WorldTransform);
	output.PosWorld = output.Pos;
    output.Pos = mul( output.Pos, CameraView );
    output.Pos = mul( output.Pos, CameraProjection );
    output.NormWorld = mul(float4(input.Norm,1.0),WorldNormal);
	
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
	float4 diffuseColor = (float4)0;
	float4 specularColor = (float4)0;
	float3 reflectVector = (float3)0;
	float3 cameraVector = normalize(CameraPosition - input.PosWorld).xyz;
	float3 normal = normalize(input.NormWorld).xyz;

	// max 4 direct lights add
	//for( int i=0;i<LightNumber;i++)
	//{
		diffuseColor.xyz += saturate( dot(LightDirArray,normal) * LightColorArray * LightIntensityPack);

		reflectVector = reflect(LightDirArray,normal);
		specularColor.xyz += saturate( pow(max(dot(reflectVector,cameraVector),0),Shininess) * LightColorArray * LightIntensityPack);
	//}
	
	diffuseColor.a = 1.0f;
	specularColor.a = 1.0f;

    return AmbientColor*DiffuseColor + diffuseColor * DiffuseColor + specularColor * SpecularColor;
}