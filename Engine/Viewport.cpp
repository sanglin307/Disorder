#include "EngineInclude.h"

namespace Disorder
{
	Viewport::Viewport(int sizeX,int sizeY)
		:SizeX(sizeX),SizeY(sizeY)
	{	
		_bActive = true;
		_renderPath = NULL;
	}

	Viewport::~Viewport()
	{
		if (_renderPath != NULL)
		{
			delete _renderPath;
			_renderPath = NULL;
		}
	}

	void Viewport::Render()
	{
		if( _renderPath != NULL )
			_renderPath->Render();
	}

	void Viewport::SetRenderPath(RenderPathType type)
	{
		if (GConfig->pRenderConfig->MultiSampleCount > 1 && type == RPT_DeferredShading)
		{
			GLogger->Error("Don't support DeferredShading for multiSample surface, so force to forward Rendering!");
			type = RPT_ForwardLighting;
		}

		if (_renderPath != NULL)
		{
			delete _renderPath;
			_renderPath = NULL;
		}

		if( type == RPT_ForwardLighting )
			_renderPath = new ForwardRenderPath;
		else if(type == RPT_DeferredShading )
			_renderPath = new DeferredShading;
		else
			BOOST_ASSERT(0);
	}

	 void Viewport::ToggleRenderPath()
	 {
		 if( _renderPath->GetType() == RPT_ForwardLighting )
			 SetRenderPath(RPT_DeferredShading);
		 else if(_renderPath->GetType() == RPT_DeferredShading )
			 SetRenderPath(RPT_ForwardLighting);
	 }

    RenderPath* Viewport::GetRenderPath() const
	{
		return _renderPath;
	}
	

}