#ifndef _DISORDER_GLRENDERRESOURCE_H_
#define _DISORDER_GLRENDERRESOURCE_H_

namespace Disorder
{
	class GLRenderBuffer : public RenderBuffer
	{
	public:
		~GLRenderBuffer();

		static GLRenderBufferPtr Create(RenderBufferType type,GeometryPtr const& data,GLint location,BufferUsage bufferUsage,RenderEffectPtr const& effect);
		static GLRenderBufferPtr Create(RenderBufferType type,BufferUsage bufferUsage,unsigned int elementSize,unsigned int size,void *pData);

		virtual void * GetLowInterface();
		virtual void Resize(unsigned int size);

		

	private:
		GLRenderBuffer();
		void DoCreateBuffer(void *pData);

		GLenum GetGLBufferUsage(BufferUsage bufferUsage);

		GLuint _bufferHandle;
 
	};
 
	class GLRenderTexture2D : public RenderTexture2D
	{
	public:
		~GLRenderTexture2D();

		static GLRenderTexture2DPtr Create(PixelFormat pixelFormat,unsigned int width,unsigned int height,bool bMipmap,unsigned int bindFlag,BufferInitData const* pData);
		static GLRenderTexture2DPtr Create(PixelFormat pixelFormat,ImagePtr const& image);
	   
		virtual void * GetLowInterface();
	protected:
		GLRenderTexture2D();
 
		GLuint _texHandle;
		 
	};
 
}

#endif
