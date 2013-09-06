#ifndef _DISORDER_RENDERUTIL_H_
#define _DISORDER_RENDERUTIL_H_


namespace Disorder
{

	

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