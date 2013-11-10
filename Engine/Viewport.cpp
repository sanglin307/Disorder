#include "EngineInclude.h"

namespace Disorder
{
	Viewport::Viewport(int x,int y,int sizeX,int sizeY)
		:X(x),Y(y),SizeX(sizeX),SizeY(sizeY)
	{
		_renderPath = RPT_ForwardLighting;
		_bActive = true;
	}

	void Viewport::Draw()
	{
		RenderPath::Render(_renderPath);
	}

	

}