#ifndef _DISORDER_GLRENDERSURFACE_H_
#define _DISORDER_GLRENDERSURFACE_H_


namespace Disorder
{

	class GLRenderSurface : public RenderSurface
	{
	public:
		~GLRenderSurface();

		static GLRenderSurfacePtr Create(const RenderTexture2DPtr& resource,unsigned int usage);
		virtual void* GetHandle();

	private:
		GLRenderSurface();

		GLuint _frameBuffer;
	};
 
}



#endif