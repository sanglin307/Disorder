#ifndef _DISORDER_RENDERUTIL_H_
#define _DISORDER_RENDERUTIL_H_


namespace Disorder
{
	struct PixelFormatInfo
	{
		const TCHAR*	Name;
		int				BlockBytes;
		int				NumChannels;
		DWORD			PlatformFormat;
	 
	};

	extern PixelFormatInfo GPixelFormats[];
 

}


#endif