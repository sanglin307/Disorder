#include "EngineInclude.h"

namespace Disorder
{
	void RenderEffect::UpdateShaderParameter()
	{
		if( _vertexShader != NULL )
			_vertexShader->UpdateShaderParameter();
		if( _pixelShader != NULL )
			_pixelShader->UpdateShaderParameter();
	}

	RenderEffectPtr RenderEffect::Create()
	{
		RenderEffect *pEffect = new RenderEffect;
		return RenderEffectPtr(pEffect);
	}
}