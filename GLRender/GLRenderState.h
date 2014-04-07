#ifndef _DISORDER_GLRENDERSTATE_H_
#define _DISORDER_GLRENDERSTATE_H_

namespace Disorder
{
	class GLSamplerState : public SamplerState
	{
	public:
		~GLSamplerState();
		static GLSamplerStatePtr Create(SamplerDesc* pSamplerDesc);
		virtual void* GetLowInterface();


	protected:
		GLSamplerState();
		GLuint _stateHandle;
	};

	class GLRasterizeState : public RasterizeState
	{
	public:
		static GLRasterizeStatePtr Create(RasterizeDesc* pDesc);
	protected:
		GLRasterizeState(){}
	};

	class GLDepthStencilState : public DepthStencilState
	{
	public:
		static GLDepthStencilStatePtr Create(DepthStencilDesc *pDepthStencilDesc, unsigned int stencilRef);
	protected:
		GLDepthStencilState(){}
	};

	class GLBlendState : public BlendState
	{
	public:
		static GLBlendStatePtr Create(BlendDesc *pBlendDescArray, int BlendArraySize, bool AlphaToCoverageEnable = false, bool IndependentBlendEnable = false);
	protected:
		GLBlendState()
		{
		}
	};

}

#endif