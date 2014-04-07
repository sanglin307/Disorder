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
		
		pState->Desc = *pSamplerDesc;

		glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_WRAP_S,GLRenderEngine::GetGLAddressMode(pSamplerDesc->AddressU));
		glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_WRAP_T,GLRenderEngine::GetGLAddressMode(pSamplerDesc->AddressV));
		glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_WRAP_R,GLRenderEngine::GetGLAddressMode(pSamplerDesc->AddressW));

	 
		if( pSamplerDesc->Filter == SF_Min_Mag_Mip_Point )
		{
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else if( pSamplerDesc->Filter == SF_Min_Mag_Point_Mip_Linear )
		{
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else if( pSamplerDesc->Filter == SF_Min_Point_Mag_Linear_Mip_Point )
		{
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else if( pSamplerDesc->Filter == SF_Min_Point_Mag_Mip_Linear )
		{
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else if( pSamplerDesc->Filter == SF_Min_Linear_Mag_Mip_Point )
		{
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else if( pSamplerDesc->Filter == SF_Min_Linear_Mag_Point_Mip_Linear )
		{
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else if( pSamplerDesc->Filter == SF_Min_Mag_Linear_Mip_Point )
		{
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else if( pSamplerDesc->Filter == SF_Min_Mag_Mip_Linear )
		{
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else if( pSamplerDesc->Filter == SF_Anisotropic )
		{
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_MAX_ANISOTROPY_EXT, pSamplerDesc->MaxAnisotropy);
		}

		glSamplerParameterfv(pState->_stateHandle,GL_TEXTURE_BORDER_COLOR,pSamplerDesc->BorderColor);
		glSamplerParameterf(pState->_stateHandle,GL_TEXTURE_MIN_LOD,pSamplerDesc->MinLOD);
		glSamplerParameterf(pState->_stateHandle,GL_TEXTURE_MAX_LOD,pSamplerDesc->MaxLOD);
		glSamplerParameterf(pState->_stateHandle,GL_TEXTURE_LOD_BIAS,pSamplerDesc->MipLODBias);

		if( pSamplerDesc->CompareFunc == CF_None )
		    glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_COMPARE_MODE, GL_NONE);
		else
		{
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			glSamplerParameteri(pState->_stateHandle,GL_TEXTURE_COMPARE_FUNC, GLRenderEngine::GetGLComparisonFunc(pSamplerDesc->CompareFunc));
		}
 
		return GLSamplerStatePtr(pState);

	}

	GLRasterizeStatePtr GLRasterizeState::Create(RasterizeDesc* pDesc)
	{
		GLRasterizeState* pState = new GLRasterizeState;
		pState->Desc = *pDesc;

		return GLRasterizeStatePtr(pState);
	}


	GLDepthStencilStatePtr GLDepthStencilState::Create(DepthStencilDesc *pDepthStencilDesc, unsigned int stencilRef)
	{
		GLDepthStencilState *pState = new GLDepthStencilState;
		pState->Desc = *pDepthStencilDesc;
		pState->StencilRef = stencilRef;

		return GLDepthStencilStatePtr(pState);
	}

	GLBlendStatePtr GLBlendState::Create(BlendDesc *pBlendDescArray, int BlendArraySize, bool AlphaToCoverageEnable, bool IndependentBlendEnable)
	{
		GLBlendState* pState = new GLBlendState;

		pState->Desc = *pBlendDescArray;

		return GLBlendStatePtr(pState);
	}

}