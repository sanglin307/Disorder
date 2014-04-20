#ifndef _DISORDER_GLRENDERSURFACE_H_
#define _DISORDER_GLRENDERSURFACE_H_


namespace Disorder
{

	class GLSurfaceView : public SurfaceView
	{
	public:
		static GLSurfaceViewPtr Create(ESurfaceViewType type, RenderTexturePtr resource, PixelFormat Format, unsigned int Flag);

	private:
		GLSurfaceView(){}

	};

	class GLRenderSurface : public RenderSurface
	{
		friend class GLRenderEngine;

	public:
		~GLRenderSurface();

		static GLRenderSurfacePtr Create(const std::map<ESurfaceLocation, SurfaceViewPtr>& viewMap);
		virtual void* GetHandle();

	private:
		GLRenderSurface();

		GLuint _frameBuffer;
	};
 
}



#endif