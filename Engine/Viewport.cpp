#include "EngineInclude.h"

namespace Disorder
{
	Viewport::Viewport(int x,int y,int sizeX,int sizeY)
		:X(x),Y(y),SizeX(sizeX),SizeY(sizeY)
	{
		SetRenderPath(RPT_ForwardLighting);
		_bActive = true;
	}

	void Viewport::Render()
	{
		if( _renderPath != NULL )
			_renderPath->Render();
	}

	void Viewport::SetRenderPath(RenderPathType type)
	{
		if( type == RPT_ForwardLighting )
			_renderPath = ForwardRenderPath::Create();
		else
			BOOST_ASSERT(0);
	}

    RenderPathPtr Viewport::GetRenderPath()
	{
		return _renderPath;
	}
	

}