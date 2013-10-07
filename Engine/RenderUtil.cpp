#include "EngineInclude.h"

namespace Disorder
{
	PixelFormatInfo	GPixelFormats[] =
	{
		// Name						BlockBytes	NumChannels	 PlatformFormat
		{ TEXT("unknown"),			0,			0,				0          },	//	PF_Unknown
		{ TEXT("R32G32B32A32F"),	16,			4,				0 		   },	//	PF_R32G32B32A32F
		{ TEXT("R32G32B32F"),	    12,			3,				0 		   },	//	PF_R32G32B32F
		{ TEXT("R8G8B8A8"),			4,			4,				0          },	//	PF_R8G8B8A8
		{ TEXT("R32G32F"),	        8,			2,				0		   },	//	PF_R32G32F
		{ TEXT("R8G8"),	            2,			2,				0          },	//	PF_R8G8F
	};
}