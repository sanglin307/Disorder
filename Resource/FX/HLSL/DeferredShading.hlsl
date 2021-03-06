#include "Common.hlsl"

Texture2D<float> GBufferDepthTexture;
Texture2D<float4> GBufferColorSpecIntTexture;
Texture2D<float3> GBufferNormalTexture;
Texture2D<float4> GBufferSpecPowTexture;


struct GBufferData
{
	float Depth;
	float3 Color;
	float3 Normal;
	float3 SpecularColor;
	float SpecularExp;
};
 
float3 CalculateWorldPos(float2 csPos, float depth)
{
	float4 position = float4(csPos.x,csPos.y,depth,1.0f);
	position = mul(CameraViewProjInv,position);
	return float3(position.x/position.w,position.y/position.w,position.z/position.w);
}

struct SceneVSInput
{
    float3 Position  : POSITION;
    float3 Normal : NORMAL;
	float2 UV0  : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
struct SceneVSOutput
{
    float4 Position : SV_POSITION;
    float3 NormalWorld: COLOR0; 
	float3 PositionWorld: COLOR1;
	float2 UV0 : TEXCOORD0;
};

SceneVSOutput SceneVS( SceneVSInput input )
{   
    SceneVSOutput output;
    output.Position = mul( WorldTransform,float4(input.Position,1.0f));
	output.PositionWorld = output.Position.xyz;
    output.Position = mul( CameraViewProj,output.Position);
	  
    output.NormalWorld = mul(WorldNormalTransform,float4(input.Normal,1.0)).xyz;
    output.UV0 = input.UV0;

    return output;
}

/////////////////////////////////////////////////////////////////////////////
// Pixel shader
/////////////////////////////////////////////////////////////////////////////

struct ScenePSOutput
{
	float4 ColorShiness : SV_TARGET0;
	float4 Normal : SV_TARGET1;
	float4 SpecColor : SV_TARGET2;
};

ScenePSOutput PackGBuffer(float3 BaseColor, float3 Normal, float3 SpecColor, float SpecularExp)
{
	ScenePSOutput Out;
 
	// Pack all the data into the GBuffer structure
	Out.ColorShiness = float4(BaseColor.rgb, SpecularExp / 10.f);  // map to [0,1]
	Out.Normal = float4(Normal * 0.5 + 0.5, 0.0);
	Out.SpecColor = float4(SpecColor,0.0);

	return Out;
}

ScenePSOutput ScenePS( SceneVSOutput In )
{ 
    // Lookup mesh texture and modulate it with diffuse
    float3 dc = DiffuseTexture.Sample( DiffuseSampler, In.UV0 ).xyz;
	dc *= DiffuseColor;

	return PackGBuffer(dc, normalize(In.NormalWorld), SpecularColor, SpecularExp);
}
 
GBufferData UnpackGBuffer(float2 location)
{
	GBufferData Out;
	int3 location3 = int3(location, 0);

	float depth = GBufferDepthTexture.Load(location3).x;
	Out.Depth = depth;
	float4 baseColorSpecInt = GBufferColorSpecIntTexture.Load(location3);
	Out.Color = baseColorSpecInt.xyz;
	Out.SpecularExp = baseColorSpecInt.w * 10.f;
	Out.Normal = GBufferNormalTexture.Load(location3).xyz;
	Out.Normal = normalize(Out.Normal * 2.0 - 1.0);
	Out.SpecularColor = GBufferSpecPowTexture.Load(location3).xyz;

	return Out;
}

void MaterialFromGBuffer(GBufferData gbd, inout Material mat)
{
	mat.Normal = gbd.Normal;
	mat.DiffuseColor.xyz = gbd.Color;
	mat.SpecularColor = gbd.SpecularColor;
	mat.SpecularExp = gbd.SpecularExp;
}

/////////////////////////Lighting///////////////////////////////////////////////

struct LightVSInput
{
    float3 Position     : POSITION; // vertex position 
    float2 TextureUV    : TEXCOORD0;// vertex texture coords 
};

struct LightVSOutput
{
	float4 Position : SV_Position; // vertex position 
	float2 ScreenPos: TEXCOORD0;
};

LightVSOutput LightingVS( LightVSInput input )
{
	LightVSOutput Output;

	Output.Position = float4(input.Position,1.0f);
	Output.ScreenPos = Output.Position.xy;

	return Output;    
}

float4 BasePassPS(LightVSOutput In) : SV_TARGET
{
	// Unpack the GBuffer
	GBufferData gbd = UnpackGBuffer(In.Position.xy);

	if (gbd.Depth == 1.0)
		return float4(0,0,0,0);

	// Convert the data into the material structure
	Material mat;
	MaterialFromGBuffer(gbd, mat); 

	// Reconstruct the world position
	float3 position = CalculateWorldPos(In.ScreenPos, gbd.Depth);

	// Calculate the ambient color
	float3 finalColor = CalculateAmbient(mat.Normal, mat.DiffuseColor);

	
	// Return the final color
	 

	return float4(finalColor, 1.0);
}

float4 DirectionLightingPS(LightVSOutput In) : SV_TARGET
{
	// Unpack the GBuffer
	GBufferData gbd = UnpackGBuffer(In.Position.xy);
	
	// Convert the data into the material structure
	Material mat;
	MaterialFromGBuffer(gbd, mat);

	// Reconstruct the world position
	float3 position = CalculateWorldPos(In.ScreenPos, gbd.Depth);

	// Calculate the directional light
	float3 finalColor = CalculateDirectionLight(position, mat);

	// Return the final color
	float shadowValue = 1.f;

	if (DirectionLightCastShadow)
		shadowValue = PCFShadow(position);

	return float4(finalColor * shadowValue, 1.0f);
}

float4 SpotLightingPS(LightVSOutput In) : SV_TARGET
{
	// Unpack the GBuffer
	GBufferData gbd = UnpackGBuffer(In.Position.xy);

	// Convert the data into the material structure
	Material mat;
	MaterialFromGBuffer(gbd, mat);

	// Reconstruct the world position
	float3 position = CalculateWorldPos(In.ScreenPos, gbd.Depth);
 
	float3 finalColor = CalculateSpotLight(position, mat);

		// Return the final color
	float shadowValue = 1.f;
	if (SpotLightCastShadow)
		shadowValue = PCFShadow(position);

	return float4(finalColor * shadowValue, 1.0f);
}

float4 PointLightingPS(LightVSOutput In) : SV_TARGET
{
	// Unpack the GBuffer
	GBufferData gbd = UnpackGBuffer(In.Position.xy);

	// Convert the data into the material structure
	Material mat;
	MaterialFromGBuffer(gbd, mat);

	// Reconstruct the world position
	float3 position = CalculateWorldPos(In.ScreenPos, gbd.Depth);

	float3 finalColor = CalculatePointLight(position, mat);

	float shadowValue = 1.f;
	if (PointLightCastShadow)
		shadowValue = PCFShadowCubeMap(position);

	return float4(finalColor*shadowValue, 1.0f);
}