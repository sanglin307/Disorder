#include "Common.hlsl"

 
Material PrepareMaterial(float3 normal, float2 UV)
{
	Material material;

	// Normalize the interpulated vertex normal
	material.Normal = normalize(normal);

	// Sample the texture and convert to linear space
    material.DiffuseColor = DiffuseColor * DiffuseTexture.Sample(DiffuseSampler,UV).xyz;
	material.SpecularColor = SpecularColor;
	material.SpecularExp = SpecularExp;
 
	return material;
} 
 
struct VS_INPUT
{
    float3 Position  : POSITION;
    float3 Normal : NORMAL;
	float2 UV0  : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float3 NormalWorld: COLOR0; 
	float3 PositionWorld: COLOR1;
	float2 UV0 : TEXCOORD0;
};
 
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT SceneVS( VS_INPUT input )
{
    VS_OUTPUT output;
    output.Position = mul(WorldTransform, float4(input.Position,1.0f));
	output.PositionWorld = output.Position.xyz;
    output.Position = mul( CameraViewProj,output.Position);
    output.NormalWorld = mul(WorldNormalTransform,float4(input.Normal,1.0)).xyz;
	output.UV0 = input.UV0;

    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------

  // no light ,use ambient only.
float4 BasePassPS( VS_OUTPUT input) : SV_Target
{
	Material mat = PrepareMaterial(input.NormalWorld,input.UV0);
	float3 finalColor = CalculateAmbient(mat.Normal,mat.DiffuseColor);

	return float4(finalColor,1.0f);
}

float4 DirectionLightingPS( VS_OUTPUT input ) : SV_Target
{
    Material mat = PrepareMaterial(input.NormalWorld,input.UV0);

	float3 finalColor = CalculateDirectionLight(input.PositionWorld,mat);

	float shadowValue = PCFShadow(input.PositionWorld);
	return float4(finalColor * shadowValue, 1.0f);
}

float4 PointLightingPS( VS_OUTPUT In ) : SV_TARGET
{
	// Prepare the material structure
	Material material = PrepareMaterial(In.NormalWorld, In.UV0);

	// Calculate the spot light color
	float3 finalColor = CalculatePointLight(In.PositionWorld, material);

	// Return the final color
	return float4(finalColor, 1.0);
}

float4 SpotLightingPS( VS_OUTPUT In ) : SV_TARGET
{
	// Prepare the material structure
	Material material = PrepareMaterial(In.NormalWorld, In.UV0);

	// Calculate the spot light color
	float3 finalColor = CalculateSpotLight(In.PositionWorld, material);

	float shadowValue = PCFShadow(In.PositionWorld);
	// Return the final color
	return float4(finalColor * shadowValue, 1.0);
}