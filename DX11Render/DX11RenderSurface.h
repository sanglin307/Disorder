#ifndef _DISORDER_DX11RENDERSURFACE_H_
#define _DISORDER_DX11RENDERSURFACE_H_


namespace Disorder
{

	class DX11RenderSurface : public RenderSurface
	{
	public:

		static DX11RenderSurfacePtr Create(const RenderTexture2DPtr& resource,ID3D11RenderTargetViewPtr RenerTarget,ID3D11ShaderResourceViewPtr ShaderResource,ID3D11DepthStencilViewPtr DepthStencil);
		static DX11RenderSurfacePtr Create(const RenderTexture2DPtr& resource,unsigned int usage);
		virtual void* GetLowInterface(RenderSurfaceUsage usage);

		ID3D11DepthStencilViewPtr DepthStencilView;
		ID3D11ShaderResourceViewPtr ShaderResourceView;
		ID3D11RenderTargetViewPtr RenderTargetView;
	private:
		DX11RenderSurface(){}
	};
}



#endif