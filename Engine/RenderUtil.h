#ifndef _DISORDER_RENDERUTIL_H_
#define _DISORDER_RENDERUTIL_H_


namespace Disorder
{

	enum ShaderModel
	{
		SM_4_0
	};

	enum ShaderType
	{
		ST_VertexShader,
		ST_PixelShader,
		ST_GeometryShader,
		ST_ComputeShader,
		ST_HullShader,
		ST_DomainShader,

		ST_NumShaderTypes
	};


	enum PixelFormat
	{
		PF_Unknown              =0,
		PF_R32G32B32A32F        =1,
		PF_R32G32B32F           =2,
		PF_R8G8B8A8             =3,
		PF_R32G32F              =4,
		PF_R8G8                 =5,
		PF_MAX                  =6
	};

	enum TopologyType
	{
		TT_PointList,
		TT_LineList,
		TT_LineStrip,
		TT_TriangleList,
		TT_TriangleStrip,
		TT_LineList_Adj,
		TT_LineStrip_Adj,
		TT_TriangleList_Adj,
		TT_TriangleStrip_Adj,
		TT_1_Ctrl_Pt_PatchList,
		TT_2_Ctrl_Pt_PatchList,
		TT_3_Ctrl_Pt_PatchList,
		TT_4_Ctrl_Pt_PatchList,
		TT_5_Ctrl_Pt_PatchList,
		TT_6_Ctrl_Pt_PatchList,
		TT_7_Ctrl_Pt_PatchList,
		TT_8_Ctrl_Pt_PatchList,
		TT_9_Ctrl_Pt_PatchList,
		TT_10_Ctrl_Pt_PatchList,
		TT_11_Ctrl_Pt_PatchList,
		TT_12_Ctrl_Pt_PatchList,
		TT_13_Ctrl_Pt_PatchList,
		TT_14_Ctrl_Pt_PatchList,
		TT_15_Ctrl_Pt_PatchList,
		TT_16_Ctrl_Pt_PatchList,
		TT_17_Ctrl_Pt_PatchList,
		TT_18_Ctrl_Pt_PatchList,
		TT_19_Ctrl_Pt_PatchList,
		TT_20_Ctrl_Pt_PatchList,
		TT_21_Ctrl_Pt_PatchList,
		TT_22_Ctrl_Pt_PatchList,
		TT_23_Ctrl_Pt_PatchList,
		TT_24_Ctrl_Pt_PatchList,
		TT_25_Ctrl_Pt_PatchList,
		TT_26_Ctrl_Pt_PatchList,
		TT_27_Ctrl_Pt_PatchList,
		TT_28_Ctrl_Pt_PatchList,
		TT_29_Ctrl_Pt_PatchList,
		TT_30_Ctrl_Pt_PatchList,
		TT_31_Ctrl_Pt_PatchList,
		TT_32_Ctrl_Pt_PatchList
	};

	struct PixelFormatInfo
	{
		const TCHAR*	Name;
		int				BlockSizeX,
						BlockSizeY,
						BlockSizeZ,
						BlockBytes,
						NumComponents;
		/** Platform specific token, e.g. D3DFORMAT with D3DDrv										*/
		DWORD			PlatformFormat;
		/** Format specific internal flags, e.g. whether SRGB is supported with this format		*/
		DWORD			Flags;
		/** Whether the texture format is supported on the current platform/ rendering combination	*/
		bool			Supported;
	};

	extern PixelFormatInfo GPixelFormats[];


	struct RenderConfig
	{
		bool	FullScreen;
		int		X;
		int		Y;
		int		SizeX;
		int		SizeY;
		PixelFormat ColorFormat;
		//EPixelFormat DepthStencilFormat;
		unsigned int SampleCount;
		unsigned int SampleQuality;
		unsigned int SyncInterval;
		unsigned int MotionFrames;
		bool HDR;
		bool Gamma;

		RenderConfig::RenderConfig()
		{
			FullScreen = false;
			X = 0;
			Y = 0;
			SizeX = 640;
			SizeY = 480;
			ColorFormat = PF_R8G8B8A8;
			//DepthStencilFormat = PF_DepthStencil;
			SampleCount = 0;
			SampleQuality = 0;
			SyncInterval = 0;
			MotionFrames = 0;
			HDR = false;
			Gamma = true;
		};
	};
 

}


#endif