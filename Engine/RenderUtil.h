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
 

}


#endif