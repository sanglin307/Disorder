#ifndef _DISORDER_DX11RENDERSTATE_H_
#define _DISORDER_DX11RENDERSTATE_H_


namespace Disorder
{
	class DX11SamplerState : public SamplerState
	{
	public:
		static DX11SamplerStatePtr Create(SamplerDesc* pSamplerDesc);
		virtual void* GetHandle();


	protected:
		DX11SamplerState(){}
		ID3D11SamplerStatePtr D3DInterface;
	};

	class DX11RasterizeState : public RasterizeState
	{
	public:

		static DX11RasterizeStatePtr Create(RasterizeDesc *pDesc);
		virtual void* GetHandle();

	protected:
		DX11RasterizeState(){}

		ID3D11RasterizerStatePtr D3DInterface;

	};

	class DX11DepthStencilState : public DepthStencilState
	{
	public:
		static DX11DepthStencilStatePtr Create(DepthStencilDesc *pDepthStencilDesc,unsigned int stencilRef);
		virtual void* GetHandle();
 
	protected:
		DX11DepthStencilState(){}

		ID3D11DepthStencilStatePtr D3DInterface;
	};


	class DX11BlendState : public BlendState
	{
	public:
		static DX11BlendStatePtr Create(BlendDesc *pBlendDescArray,int BlendArraySize,bool AlphaToCoverageEnable = false,bool IndependentBlendEnable = false);
		virtual void* GetHandle();

	protected:
		DX11BlendState(){}

		ID3D11BlendStatePtr D3DInterface;
	};
}

#endif