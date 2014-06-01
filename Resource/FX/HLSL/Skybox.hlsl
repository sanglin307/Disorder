#include "Common.hlsl"

TextureCube  SkyboxTexture;
SamplerState SkyboxSampler;

struct VS_INPUT
{
	float3 Position     : POSITION; // vertex position 
};

struct VS_OUTPUT
{
	float4 Position     : SV_POSITION; // vertex position
	float3 CubeUVW      : TEXCOORD0;   // vertex texture coords 
};


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT Output;

	float4x4 worldMat = 
	{ 
		1, 0, 0, CameraPosition.x,
		0, 1, 0, CameraPosition.y,
		0, 0, 1, CameraPosition.z,
		0, 0,0,1
	};

	Output.Position = float4(input.Position, 1.0f);
	Output.Position = mul(worldMat, Output.Position);
	Output.Position = mul(CameraViewProj, Output.Position);
	Output.CubeUVW = input.Position;

	return Output;
}

float4 PS(VS_OUTPUT In) : SV_TARGET
{
	float4 finalColor =  SkyboxTexture.Sample(SkyboxSampler, In.CubeUVW);
	return finalColor;
}
