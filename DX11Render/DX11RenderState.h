#ifndef _DISORDER_DX11RENDERSTATE_H_
#define _DISORDER_DX11RENDERSTATE_H_


namespace Disorder
{
	class DX11SamplerState : public SamplerState
	{
	public:
		DX11SamplerState(SamplerDesc* pSamplerDesc);
		virtual void* GetHandle();

	protected:
		ID3D11SamplerState* D3DInterface;
	};

	class DX11RasterizeState : public RasterizeState
	{
	public:
		DX11RasterizeState(RasterizeDesc *pDesc);
		virtual void* GetHandle();

	protected:
		ID3D11RasterizerState* D3DInterface;

	};

	class DX11DepthStencilState : public DepthStencilState
	{
	public:
		DX11DepthStencilState(DepthStencilDesc *pDepthStencilDesc, unsigned int stencilRef);
		virtual void* GetHandle();
 
	protected:
		ID3D11DepthStencilState* D3DInterface;
	};


	class DX11BlendState : public BlendState
	{
	public:
		DX11BlendState(BlendDesc *pBlendDescArray, int BlendArraySize, bool AlphaToCoverageEnable = false, bool IndependentBlendEnable = false);
		virtual void* GetHandle();

	protected:
		ID3D11BlendState* D3DInterface;
	};
}

#endif