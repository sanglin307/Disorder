#include "GLRenderInclude.h"

namespace Disorder
{
	GLSamplerState::GLSamplerState()
	{
		glGenSamplers(1, &_stateHandle);
	}

	GLSamplerState::~GLSamplerState()
	{
		glDeleteSamplers(1,&_stateHandle);
	}

	void* GLSamplerState::GetLowInterface()
	{
		return (void*)_stateHandle;
	}

	GLSamplerStatePtr GLSamplerState::Create(SamplerDesc* pSamplerDesc)
	{
		GLSamplerState *pState = new GLSamplerState;

		/*GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_R, 
		GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, 
		GL_TEXTURE_BORDER_COLOR, 
		GL_TEXTURE_MIN_LOD, GL_TEXTURE_MAX_LOD, GL_TEXTURE_LOD_BIAS 
		GL_TEXTURE_COMPARE_MODE, GL_TEXTURE_COMPARE_FUNC.*/

		return NULL;

	}
}