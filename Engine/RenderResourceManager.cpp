#include "EngineInclude.h"

namespace Disorder
{
	RasterizeStatePtr RenderResourceManager::DefaultRasterizeState = 0;
	BlendStatePtr RenderResourceManager::DefaultBlentState = 0;

	void RenderResourceManager::Init()
	{
		RasterizeDesc rasterDesc;
		DefaultRasterizeState = CreateRasterizeState(&rasterDesc);

	    BlendDesc blendDesc;
		DefaultBlentState = CreateBlendState(&blendDesc,1);

	}

}