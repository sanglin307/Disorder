#include "EngineInclude.h"

namespace Disorder
{
	RasterizeStatePtr RenderResourceManager::DefaultRasterizeState = 0;
	BlendStatePtr RenderResourceManager::DefaultBlentState = 0;
	DepthStencilStatePtr RenderResourceManager::DefaultDepthStencilState = 0;


	void RenderResourceManager::Init()
	{
		RasterizeDesc rasterDesc;
		DefaultRasterizeState = CreateRasterizeState(&rasterDesc);

	    BlendDesc blendDesc;
		DefaultBlentState = CreateBlendState(&blendDesc,1);

		DepthStencilDesc dsDesc;
		DefaultDepthStencilState = CreateDepthStencilState(&dsDesc,0);

	}

}