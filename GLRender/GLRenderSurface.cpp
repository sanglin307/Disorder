#include "GLRenderInclude.h"


namespace Disorder
{
	GLSurfaceView::GLSurfaceView(ESurfaceViewType type, RenderTexture* resource, PixelFormat Format, unsigned int Flag)
	{
		Flag = Flag;
		Format = Format;
		Resource = resource;
		Type = type;
	}

	GLRenderSurface::GLRenderSurface(const std::map<ESurfaceLocation, SurfaceView*>& viewMap)
	 {
		 glGenFramebuffers(1, &_frameBuffer);
		 glBindFramebuffer(GL_FRAMEBUFFER, _frameBuffer);
		 std::map<ESurfaceLocation, SurfaceView*>::const_iterator iter = viewMap.cbegin();
		 bool bHaveColorAttach = false;
		 while (iter != viewMap.cend())
		 {
			 _surfacesViewArray[iter->first] = iter->second;

			 if (iter->first == SL_DepthStencil)
			 {
				 if (iter->second->Flag & SF_AsCubeMap)
				 {
					 GLuint handle = (GLuint)iter->second->Resource->GetHandle();
					 glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, handle, 0);
					 glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, handle, 0);
					 glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, handle, 0);
					 glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, handle, 0);
					 glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, handle, 0);
					 glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, handle, 0);
				 }
				 else
				    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, (GLuint)iter->second->Resource->GetHandle(), 0);
			 }
			 else if (iter->first >= SL_RenderTarget1 && iter->first <= SL_RenderTarget8)
			 {
				 GLenum loc = iter->first - SL_RenderTarget1 + GL_COLOR_ATTACHMENT0;

				 if (iter->second->Flag & SF_AsCubeMap)
				 {
					 GLuint handle = (GLuint)iter->second->Resource->GetHandle();
					 glFramebufferTexture2D(GL_FRAMEBUFFER, loc, GL_TEXTURE_CUBE_MAP_POSITIVE_X, handle, 0);
					 glFramebufferTexture2D(GL_FRAMEBUFFER, loc, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, handle, 0);
					 glFramebufferTexture2D(GL_FRAMEBUFFER, loc, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, handle, 0);
					 glFramebufferTexture2D(GL_FRAMEBUFFER, loc, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, handle, 0);
					 glFramebufferTexture2D(GL_FRAMEBUFFER, loc, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, handle, 0);
					 glFramebufferTexture2D(GL_FRAMEBUFFER, loc, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, handle, 0);
				 }
				 else
					 glFramebufferTexture(GL_FRAMEBUFFER, loc, (GLuint)iter->second->Resource->GetHandle(), 0);
				 bHaveColorAttach = true;
			 }

			 ++iter;
		 }

		 if (!bHaveColorAttach)
		 {
			 glDrawBuffer(GL_NONE);
			 glReadBuffer(GL_NONE);
		 }
 
		 GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		 BOOST_ASSERT(result == GL_FRAMEBUFFER_COMPLETE);
	 }

	void GLRenderSurface::GetGLDrawBuffers(std::vector<GLenum>& buffers)
	{
		for (size_t i = SL_RenderTarget1; i < SL_SurfaceLoactionMax; i++)
		{
			if (_surfacesViewArray[i] != NULL)
			{
				buffers.push_back(i - SL_RenderTarget1 + GL_COLOR_ATTACHMENT0);
			}
		}
 
	}

	 void* GLRenderSurface::GetHandle()
	 {
		 return (void*)_frameBuffer;
	 }

	 GLRenderSurface::~GLRenderSurface()
	 {
		 glDeleteFramebuffers(1, &_frameBuffer);
	 }
 
}