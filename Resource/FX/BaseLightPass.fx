cbuffer SceneProperty
{
	float3 AmbientLowColor;
	float3 AmbientUpperColor;
}

cbuffer CameraTransforms
{
	matrix CameraView;
	matrix CameraProjection;	
	float3 CameraPosition;
}

cbuffer ObjectTransform
{
	matrix WorldTransform;
	matrix WorldNormalTransform;  // translate for normal.
}

cbuffer MaterialProperty
{
	float3 AmbientColor;
	float3 DiffuseColor;
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
    output.NormWorld = mul(float4(input.Norm,1.0),WorldNormalTransform);
	
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

float3 CalculateAmbient(float3 normal, float3 color)
{
	// Convert from [-1, 1] to [0, 1]
	float up = normal.y * 0.5 + 0.5;

	// Calculate the ambient value
	float3 ambient = AmbientLowColor + up * (AmbientUpperColor - AmbientLowColor);

	// Apply the ambient value to the color
	return ambient * color;
}

float4 PS( VS_OUTPUT input ) : SV_Target
{
	float4 diffuseColor = (float4)0;
	float3 normal = normalize(input.NormWorld).xyz;

	// max 4 direct lights add
	//for( int i=0;i<LightNumber;i++)
	//{
		diffuseColor.xyz += saturate( max(dot(LightDirArray,normal),0) * LightColorArray * LightIntensityPack.x);
	//}
	
	diffuseColor.xyz += CalculateAmbient(normal,DiffuseColor);

	diffuseColor.a = 1.0f;

    return diffuseColor; //AmbientColor*DiffuseColor + diffuseColor * DiffuseColor;
}