#ifndef _DISORDER_GLRENDERSURFACE_H_
#define _DISORDER_GLRENDERSURFACE_H_


namespace Disorder
{

	class GLRenderSurface : public RenderSurface
	{
	public:
		~GLRenderSurface();

		//static GLRenderSurfacePtr Create(const RenderTexture2DPtr& resource,ID3D11RenderTargetViewPtr RenerTarget,ID3D11ShaderResourceViewPtr ShaderResource,ID3D11DepthStencilViewPtr DepthStencil);
		static GLRenderSurfacePtr Create(const RenderTexture2DPtr& resource,unsigned int usage);
		virtual void* GetHandle();

	private:
		GLRenderSurface();

		GLuint _frameBuffer;
	};

	class GLRenderGBuffer : public RenderGBuffer
	{
	public:
		static GLRenderGBufferPtr Create(unsigned int width,unsigned int height);
		virtual void DebugVisual();
	protected:
		GLRenderGBuffer(unsigned int width,unsigned int height);
	};

	class GLRenderSurfaceCache : public RenderSurfaceCache
	{
	public:
		static GLRenderSurfaceCachePtr Create();

		virtual void InitGBuffer(unsigned int width,unsigned int height);

	protected:
		GLRenderSurfaceCache();
	};
}



#endif