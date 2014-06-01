#include "Common.hlsl"
 
struct VS_INPUT
{
    float3 Position  : POSITION;
    float3 Normal : NORMAL;
	float2 UV0  : TEXCOORD0;
};
 

struct CubeMapGSOut
{
	float4 Position : SV_POSITION;     // Projection coord
	uint RTIndex : SV_RenderTargetArrayIndex;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
float4 DepthVertexShader(VS_INPUT input) : SV_Position
{ 
	// Calculate the position of the vertex against the world, view, and projection matrices.
    float4 position = float4(input.Position,1.f); //mul(WorldTransform,input.Position);
    position = mul(ShadowMapView,position);
    position = mul(ShadowMapProj,position);

 
	return position;
}
 

float4 DepthCubeMapVS(VS_INPUT input) : SV_Position
{
	float4 position = float4(input.Position, 1.f);
	position = mul(WorldTransform, position);
	return position;
}

[maxvertexcount(18)]
void DepthCubeMapGS(triangle float4 input[3] : SV_POSITION, inout TriangleStream<CubeMapGSOut> CubeMapStream)
{
	for (int f = 0; f < 6; ++f)
	{
		// Compute screen coordinates
		CubeMapGSOut output;
		output.RTIndex = f;
		for (int v = 0; v < 3; v++)
		{
			output.Position = mul(ShadowMapViewArray[f], input[v]);
			output.Position = mul(ShadowMapProj, output.Position);
			CubeMapStream.Append(output);
		}

		CubeMapStream.RestartStrip();
	}
}