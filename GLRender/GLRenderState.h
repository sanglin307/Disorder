#ifndef _DISORDER_GLRENDERSTATE_H_
#define _DISORDER_GLRENDERSTATE_H_

namespace Disorder
{
	class GLSamplerState : public SamplerState
	{
	public:
		~GLSamplerState();
		GLSamplerState(SamplerDesc* pSamplerDesc);
		virtual void* GetHandle();


	protected:
		GLuint _stateHandle;
	};

	class GLRasterizeState : public RasterizeState
	{
	public:
		GLRasterizeState(RasterizeDesc* pDesc);
	};

	class GLDepthStencilState : public DepthStencilState
	{
	public:
	    GLDepthStencilState(DepthStencilDesc *pDepthStencilDesc, unsigned int stencilRef);
	};

	class GLBlendState : public BlendState
	{
	public:
		GLBlendState(BlendDesc *pBlendDescArray, int BlendArraySize, bool AlphaToCoverageEnable = false, bool IndependentBlendEnable = false);
	};

}

#endif