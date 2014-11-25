#ifndef _DISORDER_GLRENDERSURFACE_H_
#define _DISORDER_GLRENDERSURFACE_H_


namespace Disorder
{

	class GLSurfaceView : public SurfaceView
	{
	public:
		GLSurfaceView(ESurfaceViewType type, RenderTexture* resource, PixelFormat Format, unsigned int Flag);

	};

	class GLRenderSurface : public RenderSurface
	{
		friend class GLRenderEngine;

	public:
		~GLRenderSurface();
		GLRenderSurface(const std::map<ESurfaceLocation, SurfaceView*>& viewMap);

		virtual void* GetHandle();

		void GetGLDrawBuffers(std::vector<GLenum>& buffers);

	private:
		GLuint _frameBuffer;
		 
	};
 
}



#endif