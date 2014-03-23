#include "GLRenderInclude.h"


namespace Disorder
{
	 GLRenderSurfacePtr GLRenderSurface::Create(const RenderTexture2DPtr& resource,unsigned int usage,PixelFormat RenderTargetFormat,PixelFormat DepthFormat,PixelFormat ShaderResFormat,bool readOnlyDepth,bool readOnlyStencil)
	 {
		 GLRenderSurface *pSurface = new GLRenderSurface;
		 return GLRenderSurfacePtr(pSurface);
	 }

	 void* GLRenderSurface::GetLowInterface(RenderSurfaceUsage usage)
	 {
		 return NULL;
	 }


	GLRenderGBufferPtr GLRenderGBuffer::Create(unsigned int width,unsigned int height)
	{
		GLRenderGBuffer *pBuffer = new GLRenderGBuffer(width,height);
		return GLRenderGBufferPtr(pBuffer);
	}

	void GLRenderGBuffer::DebugVisual()
	{
	}

	GLRenderGBuffer::GLRenderGBuffer(unsigned int width,unsigned int height)
	{
	}


	GLRenderSurfaceCachePtr GLRenderSurfaceCache::Create()
	{
		GLRenderSurfaceCache *pCache = new GLRenderSurfaceCache;
		return GLRenderSurfaceCachePtr(pCache);
	}

	void GLRenderSurfaceCache::InitGBuffer(unsigned int width,unsigned int height)
	{
	}

	GLRenderSurfaceCache::GLRenderSurfaceCache()
	{
	}
}