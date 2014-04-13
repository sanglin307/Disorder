#ifndef _DISORDER_DX11RENDERSURFACE_H_
#define _DISORDER_DX11RENDERSURFACE_H_


namespace Disorder
{

	class DX11RenderSurface : public RenderSurface
	{
	public:
		~DX11RenderSurface();

		static DX11RenderSurfacePtr Create();
		static DX11RenderSurfacePtr Create(const std::vector<sRenderSurfaceDes>& surfaceDes);

	private:
		DX11RenderSurface(){}
	};

	class DX11RenderGBuffer : public RenderGBuffer
	{
	public:
		static DX11RenderGBufferPtr Create(unsigned int width,unsigned int height);
		virtual void DebugVisual();
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