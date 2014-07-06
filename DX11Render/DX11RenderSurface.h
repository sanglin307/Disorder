#ifndef _DISORDER_DX11RENDERSURFACE_H_
#define _DISORDER_DX11RENDERSURFACE_H_


namespace Disorder
{

	class DX11SurfaceView : public SurfaceView
	{
	public:
		static DX11SurfaceViewPtr Create(ESurfaceViewType type, RenderTexturePtr resource, PixelFormat format, unsigned int flag);
	
		virtual void* GetHandle();

		ID3D11DepthStencilViewPtr DepthStencilHandle;
		std::vector<ID3D11DepthStencilViewPtr> DepthStencilHandleArray;

		ID3D11DepthStencilViewPtr ReadonlyDepthStencil;
		ID3D11ShaderResourceViewPtr ShaderResourceHandle;
		ID3D11RenderTargetViewPtr RenderTargetHandle;
		std::vector<ID3D11RenderTargetViewPtr> RenderTargetHandleArray;


	private:
		DX11SurfaceView(){};
 
	};

	class DX11RenderSurface : public RenderSurface
	{
		friend class DX11RenderEngine;

	public:
		static DX11RenderSurfacePtr Create(const std::map<ESurfaceLocation, SurfaceViewPtr>& viewMap);

	private:
		DX11RenderSurface(){}
	};
}



#endif