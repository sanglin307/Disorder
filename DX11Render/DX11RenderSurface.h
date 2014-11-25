#ifndef _DISORDER_DX11RENDERSURFACE_H_
#define _DISORDER_DX11RENDERSURFACE_H_


namespace Disorder
{

	class DX11SurfaceView : public SurfaceView
	{
	public:
		DX11SurfaceView(ESurfaceViewType type, RenderTexture* resource, PixelFormat format, unsigned int flag);
		ID3D11DepthStencilView* DepthStencilHandle;
		std::vector<ID3D11DepthStencilView*> DepthStencilHandleArray;
		ID3D11DepthStencilView* ReadonlyDepthStencil;
		ID3D11ShaderResourceView* ShaderResourceHandle;
		ID3D11RenderTargetView* RenderTargetHandle;
		std::vector<ID3D11RenderTargetView*> RenderTargetHandleArray;

		virtual void* GetHandle();
	private:
		DX11SurfaceView(){};
 
	};

	class DX11RenderSurface : public RenderSurface
	{
		friend class DX11RenderEngine;

	public:
		DX11RenderSurface(const std::map<ESurfaceLocation, SurfaceView*>& viewMap);
	};
}



#endif