#include "GLRenderInclude.h"


namespace Disorder
{
	 GLRenderSurfacePtr GLRenderSurface::Create(const RenderTexture2DPtr& resource,unsigned int usage)
	 {
		 GLRenderSurface *pSurface = new GLRenderSurface;

		 //pSurface->Tex2DResource = resource;
		 //pSurface->Usage = usage;

		 //if (usage == RSU_ShaderResource)
		 //{
			// //don't use frame object handle it
			// glDeleteFramebuffers(1, &pSurface->_frameBuffer);
			// pSurface->_frameBuffer = 0;
			// return GLRenderSurfacePtr(pSurface);
		 //}

		 //glBindFramebuffer(GL_FRAMEBUFFER, pSurface->_frameBuffer);
		 //
		 //if (usage & RSU_RenderTarget)
		 //{
			// glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, (GLuint)resource->GetHandle(), 0);
		 //}

		 //if (usage & RSU_DepthStencil)
		 //{
			// glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, (GLuint)resource->GetHandle(), 0);
		 //}

		 //static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
		 //glDrawBuffers(1, draw_buffers);

		 return GLRenderSurfacePtr(pSurface);
	 }

	 void* GLRenderSurface::GetHandle()
	 {
		 return (void*)_frameBuffer;
	 }

	 GLRenderSurface::GLRenderSurface()
	 {
		 glGenFramebuffers(1, &_frameBuffer);
	 }

	 GLRenderSurface::~GLRenderSurface()
	 {
		 glDeleteFramebuffers(1, &_frameBuffer);
	 }
 
}