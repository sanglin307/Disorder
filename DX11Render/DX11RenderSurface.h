#ifndef _DISORDER_DX11RENDERSURFACE_H_
#define _DISORDER_DX11RENDERSURFACE_H_


namespace Disorder
{

	class DX11RenderSurface : public RenderSurface
	{
	public:

		static DX11RenderSurfacePtr Create(const RenderTexture2DPtr& resource,ID3D11RenderTargetViewPtr RenerTarget,ID3D11ShaderResourceViewPtr ShaderResource,ID3D11DepthStencilViewPtr DepthStencil);
		static DX11RenderSurfacePtr Create(const RenderTexture2DPtr& resource,unsigned int usage,PixelFormat RenderTargetFormat,PixelFormat DepthFormat,PixelFormat ShaderResFormat);
		virtual void* GetLowInterface(RenderSurfaceUsage usage);

		ID3D11DepthStencilViewPtr DepthStencilView;
		ID3D11ShaderResourceViewPtr ShaderResourceView;
		ID3D11RenderTargetViewPtr RenderTargetView;

	private:
		DX11RenderSurface(){}
	};

	class DX11RenderGBuffer : public RenderGBuffer
	{
	public:
		static DX11RenderGBufferPtr Create(unsigned int width,unsigned int height);

	protected:
		DX11RenderGBuffer(unsigned int width,unsigned int height);
	};

	class DX11RenderSurfaceCache : public RenderSurfaceCache
	{
	public:
		static DX11RenderSurfaceCachePtr Create();

		virtual void InitGBuffer(unsigned int width,unsigned int height);

	protected:
		DX11RenderSurfaceCache();
	};
}



#endif