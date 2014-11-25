#ifndef _DISORDER_GLRENDERRESOURCE_H_
#define _DISORDER_GLRENDERRESOURCE_H_

namespace Disorder
{
	class GLRenderBuffer : public RenderBuffer
	{
	public:
		~GLRenderBuffer();
		GLRenderBuffer(RenderBufferType type,Geometry* data,GLint location,BufferUsage bufferUsage,RenderEffect* effect);
		GLRenderBuffer(RenderBufferType type, BufferUsage bufferUsage, unsigned int elementSize, unsigned int size, void *pData, int bindingPoint);

		virtual void * GetHandle();
		virtual void Resize(unsigned int size);

		GLuint GetBindingPoint()
		{
			return _bindingPoint;
		}

	private:
		
		void DoCreateBuffer(void *pData);

		GLenum GetGLBufferUsage(BufferUsage bufferUsage);

		GLuint _bufferHandle;
		GLuint _bindingPoint;
 
	};
 
	class GLRenderTexture2D : public RenderTexture2D
	{
	public:
		~GLRenderTexture2D();

		GLRenderTexture2D(PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag);
		GLRenderTexture2D(PixelFormat pixelFormat, bool bMultiSample,Image* image);
		GLRenderTexture2D(PixelFormat pixelFormat, bool bMultiSample, const std::vector<Image*>& image, unsigned int flag);
		GLRenderTexture2D(int arraySize);
		void InnerCreator(PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag);

		virtual void * GetHandle();
		const GLenum GetGLFormat() const
		{
			return _texFormat;
		}

	protected:
		GLenum _texFormat;
		GLuint _texHandle;
		 
	};
 
}

#endif
