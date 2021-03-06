#define FXAA_PC 1
#define FXAA_HLSL_4 1
#define FXAA_QUALITY__PRESET 20
//#define FXAA_GREEN_AS_LUMA 1  // maybe later use RGBL texture format to avoid use this macro.

#include "Common.hlsl"
#include "Fxaa3_11.h"

Texture2D ScreenAATexture;
SamplerState ScreenAASampler;

struct AAVSInput
{
	float3 Position     : POSITION; // vertex position 
	float2 TextureUV    : TEXCOORD0;// vertex texture coords 
};

struct AAVSOutput
{
	float4 Position	: SV_Position; // vertex position 
	noperspective float2 UV : TEXCOORD0;   // vertex texture coords
 
};

AAVSOutput VS(AAVSInput input)
{
	AAVSOutput Output;

	Output.Position = float4(input.Position, 1.0f);
	Output.UV = input.TextureUV;

	//float4 frameRcpFrame;
	/*frameRcpFrame.x = ScreenWidth;
	frameRcpFrame.y = ScreenHeight;
	frameRcpFrame.z = 1.f / frameRcpFrame.x;
	frameRcpFrame.w = 1.f / frameRcpFrame.y;

	Output.FXAAUV = float4(input.TextureUV, input.TextureUV + float2(1.0, 1.0)*frameRcpFrame.zw);*/
	return Output;
}

float4 PrepareSurfacePS(AAVSOutput In) : SV_TARGET
{
	float4 finalColor = (float4)0;
	finalColor.rgb = ScreenAATexture.Load(In.Position);
	finalColor.a = dot(finalColor.rgb, float3(0.299, 0.587, 0.114));

	return finalColor;
}

float4 PS(AAVSOutput In) : SV_TARGET
{
	struct FxaaTex aaTexture;
	aaTexture.smpl = ScreenAASampler;
	aaTexture.tex = ScreenAATexture;

	float4 frameRcpFrame;
	frameRcpFrame.x = ScreenWidth;
	frameRcpFrame.y = ScreenHeight;
	frameRcpFrame.z = 1.f / frameRcpFrame.x;
	frameRcpFrame.w = 1.f / frameRcpFrame.y;
 
	float4 fxaa_ConsolePosPos = float4(In.UV.xy - float2(0.5, 0.5)*frameRcpFrame.zw, In.UV.xy + float2(0.5, 0.5)*frameRcpFrame.zw);

	float4 finalColor = FxaaPixelShader(
		/*FxaaFloat2 pos*/							In.UV.xy,
		/*FxaaFloat4 fxaaConsolePosPos*/			fxaa_ConsolePosPos,
		/*FxaaTex tex*/								aaTexture,
		/*FxaaTex fxaaConsole360TexExpBiasNegOne*/	aaTexture,
		/*FxaaTex fxaaConsole360TexExpBiasNegTwo*/	aaTexture,
		/*FxaaFloat2 fxaaQualityRcpFrame*/			frameRcpFrame.zw,
		/*FxaaFloat4 fxaaConsoleRcpFrameOpt*/		float4(-0.5, -0.5, 0.3, 0.3)*frameRcpFrame.zwzw,
		/*FxaaFloat4 fxaaConsoleRcpFrameOpt2*/		float4(-2.0, -2.0, 2.0, 2.0)*frameRcpFrame.zwzw,
		/*FxaaFloat4 fxaaConsole360RcpFrameOpt2*/	float4(8.0, 8.0, -4.0, -4.0)*frameRcpFrame.zwzw,
		/*FxaaFloat fxaaQualitySubpix*/				0.75,  //   1.00 - upper limit (softer) //   0.75 - default amount of filtering //   0.50 - lower limit (sharper, less sub-pixel aliasing removal) //   0.25 - almost off //   0.00 - completely off
		/*FxaaFloat fxaaQualityEdgeThreshold*/		0.166, ////   0.333 - too little (faster) //   0.250 - low quality //   0.166 - default //   0.125 - high quality  //   0.063 - overkill (slower)
		/*FxaaFloat fxaaQualityEdgeThresholdMin*/	0.0833, // //   0.0833 - upper limit (default, the start of visible unfiltered edges)//   0.0625 - high quality (faster) //   0.0312 - visible limit (slower)
		/*FxaaFloat fxaaConsoleEdgeSharpness*/		8,// //   8.0 is sharper (default!!!) //   4.0 is softer //   2.0 is really soft
		/*FxaaFloat fxaaConsoleEdgeThreshold*/		0.125, //  //   0.125 leaves less aliasing, but is softer (default!!!) //   0.25 leaves more aliasing, and is sharper
		/*FxaaFloat fxaaConsoleEdgeThresholdMin*/	0.05, // //   0.06 - faster but more aliasing in darks //   0.05 - default //   0.04 - slower and less aliasing in darks
		/*FxaaFloat4 fxaaConsole360ConstDir*/		float4(1.0, -1.0, 0.25, -0.25)
		);

	return finalColor;
}
