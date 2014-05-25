#include "Common.hlsl"
 
struct VS_INPUT
{
    float3 Position  : POSITION;
    float3 Normal : NORMAL;
	float2 UV0  : TEXCOORD0;
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
 