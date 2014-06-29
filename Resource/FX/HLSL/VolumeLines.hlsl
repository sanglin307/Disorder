#include "Common.hlsl"

//--------------------------------------------------------------------------------------
// shader input/output structure
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 Position      : POSITION0;  
	float3 PositionOther : POSITION1; 
	float4 OffsetUV :      POSITION2;
	float3 Color :         COLOR;
};

struct VS_OUTPUT
{
	float4 Position     : SV_POSITION;  
	float2 Texcoord     : TEXCOORD0;  
	float3 Color        : COLOR;
};

Texture2D LineTexture;
SamplerState LineSampler;

//--------------------------------------------------------------------------------------
// This shader computes standard transform
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT Output;
	Output.Texcoord = float2(input.OffsetUV.z,1.f - input.OffsetUV.w);

	//compute vertices position in clip space
	float4 vMVP = mul(CameraViewProj,float4(input.Position, 1.0));
	float4 otherMVP = mul(CameraViewProj, float4(input.PositionOther, 1.0));

	//  line direction in screen space (perspective division required)
	float2 lineDirProj = LineRadius * normalize(vMVP.xy / vMVP.ww - otherMVP.xy / otherMVP.ww);

	// small trick to avoid inversed line condition when points are not on the same side of Z plane
	if (sign(otherMVP.w) != sign(vMVP.w))
		lineDirProj = -lineDirProj;

	float invSrcRatio = ScreenWidth * 1.f / ScreenHeight;
	// offset position in screen space along line direction and orthogonal direction
	vMVP.xy += lineDirProj.xy					* input.OffsetUV.xx *float2 (1.0, invSrcRatio);
	vMVP.xy += lineDirProj.yx*float2(1.0, -1.0)	* input.OffsetUV.yy *float2 (1.0, invSrcRatio);

	Output.Position = vMVP;
	Output.Color = input.Color;

	return Output;
}

float4 PS(VS_OUTPUT In) : SV_TARGET
{
	float4 finalColor = LineTexture.Sample(LineSampler, In.Texcoord);
	finalColor.a = (finalColor.r + finalColor.g + finalColor.b) / 3.f;
	finalColor.xyz *= In.Color;
	return finalColor;
}
