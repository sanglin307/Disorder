#include "GLRenderInclude.h"


namespace Disorder
{
	GLSurfaceViewPtr GLSurfaceView::Create(ESurfaceViewType type, RenderTexturePtr resource, PixelFormat Format, unsigned int Flag)
	{
		GLSurfaceView *pView = new GLSurfaceView;
		pView->Flag = Flag;
		pView->Format = Format;
		pView->Resource = resource;
		pView->Type = type;

		return GLSurfaceViewPtr(pView);
	}

	GLRenderSurfacePtr GLRenderSurface::Create(const std::map<ESurfaceLocation, SurfaceViewPtr>& viewMap)
	 {
		 GLRenderSurface *pSurface = new GLRenderSurface;

		 glBindFramebuffer(GL_FRAMEBUFFER, pSurface->_frameBuffer);
		 std::map<ESurfaceLocation, SurfaceViewPtr>::const_iterator iter = viewMap.cbegin();
		 std::vector<GLenum> bufferLoc;
		 while (iter != viewMap.cend())
		 {
			 pSurface->_surfacesViewArray[iter->first] = iter->second;

			 if ( iter->first == SL_DepthStencil )
				 glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, (GLuint)iter->second->Resource->GetHandle(), 0);
			 else if (iter->first >= SL_RenderTarget1 && iter->first <= SL_RenderTarget8)
			 {
				 GLenum loc = iter->first - SL_RenderTarget1 + GL_COLOR_ATTACHMENT0;
				 bufferLoc.push_back(loc);
				 glFramebufferTexture(GL_FRAMEBUFFER, loc, (GLuint)(GLuint)iter->second->Resource->GetHandle(), 0);
			 }
			 ++iter;
		 }
 
		 glDrawBuffers(bufferLoc.size(), bufferLoc.data());

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