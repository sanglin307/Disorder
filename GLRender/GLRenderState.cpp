#include "GLRenderInclude.h"

namespace Disorder
{
 

	GLSamplerState::~GLSamplerState()
	{
		glDeleteSamplers(1,&_stateHandle);
	}

	void* GLSamplerState::GetHandle()
	{
		return (void*)_stateHandle;
	}

	GLSamplerState::GLSamplerState(SamplerDesc* pSamplerDesc)
	{
		glGenSamplers(1, &_stateHandle);
		Desc = *pSamplerDesc;

		glSamplerParameteri(_stateHandle,GL_TEXTURE_WRAP_S,GLRenderEngine::GetGLAddressMode(pSamplerDesc->AddressU));
		glSamplerParameteri(_stateHandle,GL_TEXTURE_WRAP_T,GLRenderEngine::GetGLAddressMode(pSamplerDesc->AddressV));
		glSamplerParameteri(_stateHandle,GL_TEXTURE_WRAP_R,GLRenderEngine::GetGLAddressMode(pSamplerDesc->AddressW));

		 
		if( pSamplerDesc->Filter == SF_Min_Mag_Mip_Point )
		{
			glSamplerParameteri(_stateHandle,GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glSamplerParameteri(_stateHandle,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else if( pSamplerDesc->Filter == SF_Min_Mag_Point_Mip_Linear )
		{
			glSamplerParameteri(_stateHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glSamplerParameteri(_stateHandle,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else if( pSamplerDesc->Filter == SF_Min_Point_Mag_Linear_Mip_Point )
		{
			glSamplerParameteri(_stateHandle,GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			glSamplerParameteri(_stateHandle,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else if( pSamplerDesc->Filter == SF_Min_Point_Mag_Mip_Linear )
		{
			glSamplerParameteri(_stateHandle, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			glSamplerParameteri(_stateHandle,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else if( pSamplerDesc->Filter == SF_Min_Linear_Mag_Mip_Point )
		{
			glSamplerParameteri(_stateHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glSamplerParameteri(_stateHandle,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else if( pSamplerDesc->Filter == SF_Min_Linear_Mag_Point_Mip_Linear )
		{
			glSamplerParameteri(_stateHandle,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glSamplerParameteri(_stateHandle,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else if( pSamplerDesc->Filter == SF_Min_Mag_Linear_Mip_Point )
		{
			glSamplerParameteri(_stateHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glSamplerParameteri(_stateHandle,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else if( pSamplerDesc->Filter == SF_Min_Mag_Mip_Linear )
		{
			glSamplerParameteri(_stateHandle,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glSamplerParameteri(_stateHandle,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else if( pSamplerDesc->Filter == SF_Anisotropic )
		{
			glSamplerParameteri(_stateHandle,GL_TEXTURE_MAX_ANISOTROPY_EXT, pSamplerDesc->MaxAnisotropy);
		}

		glSamplerParameterfv(_stateHandle,GL_TEXTURE_BORDER_COLOR,pSamplerDesc->BorderColor);
		glSamplerParameterf(_stateHandle,GL_TEXTURE_MIN_LOD,pSamplerDesc->MinLOD);
		glSamplerParameterf(_stateHandle,GL_TEXTURE_MAX_LOD,pSamplerDesc->MaxLOD);
		glSamplerParameterf(_stateHandle,GL_TEXTURE_LOD_BIAS,pSamplerDesc->MipLODBias);

		if( pSamplerDesc->CompareTypeSampler == false || pSamplerDesc->CompareFunc == CF_None )
		    glSamplerParameteri(_stateHandle,GL_TEXTURE_COMPARE_MODE, GL_NONE);
		else
		{
			glSamplerParameteri(_stateHandle,GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			glSamplerParameteri(_stateHandle,GL_TEXTURE_COMPARE_FUNC, GLRenderEngine::GetGLComparisonFunc(pSamplerDesc->CompareFunc));
		}

	}

	GLRasterizeState::GLRasterizeState(RasterizeDesc* pDesc)
	{
		Desc = *pDesc;
	}


	GLDepthStencilState::GLDepthStencilState(DepthStencilDesc *pDepthStencilDesc, unsigned int stencilRef)
	{
		Desc = *pDepthStencilDesc;
		StencilRef = stencilRef;
	}

	GLBlendState::GLBlendState(BlendDesc *pBlendDescArray, int BlendArraySize, bool AlphaToCoverageEnable, bool IndependentBlendEnable)
	{	 
		AlphaToCoverageEnable = AlphaToCoverageEnable;
		IndependentBlendEnable = IndependentBlendEnable;

		for (int index = 0; index < BlendArraySize; index++)
		{
			Desc[index] = pBlendDescArray[index];
		}
		
	}

}