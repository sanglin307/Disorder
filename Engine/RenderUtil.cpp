#include "EngineInclude.h"

namespace Disorder
{
	PixelFormatInfo	GPixelFormats[] =
	{
		// Name						BlockSizeX	BlockSizeY	BlockSizeZ	BlockBytes	NumComponents	PlatformFormat	Flags			Supported			PixelFormat

		{ TEXT("unknown"),			0,			0,			0,			0,			0,				0,				0,				false			},	//	PF_Unknown
		{ TEXT("R32G32B32A32F"),	1,			1,			1,			16,			4,				0,				0,				true			},	//	PF_R32G32B32A32F
		{ TEXT("R32G32B32F"),	    1,			1,			1,			12,			3,				0,				0,				true			},	//	PF_R32G32B32F
		{ TEXT("R8G8B8A8"),			1,			1,			1,			4,			4,				0,				0,				true			},	//	PF_R8G8B8A8
		{ TEXT("R32G32F"),	        1,			1,			1,			8,			2,				0,				0,				true			},	//	PF_R32G32F
		{ TEXT("R8G8"),	            1,			1,			1,			2,			2,				0,				0,				true			},	//	PF_R8G8F
		 
	};


}