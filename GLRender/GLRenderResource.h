#ifndef _DISORDER_GLRENDERRESOURCE_H_
#define _DISORDER_GLRENDERRESOURCE_H_

namespace Disorder
{
	class GLRenderBuffer : public RenderBuffer
	{
	public:
		~GLRenderBuffer();

		static GLRenderBufferPtr Create(RenderBufferType type,GeometryPtr const& data,GLint location,BufferUsage bufferUsage,RenderEffectPtr const& effect);
		static GLRenderBufferPtr Create(RenderBufferType type, BufferUsage bufferUsage, unsigned int elementSize, unsigned int size, void *pData, int bindingPoint);

		virtual void * GetHandle();
		virtual void Resize(unsigned int size);

		GLuint GetBindingPoint()
		{
			return _bindingPoint;
		}

	private:
		GLRenderBuffer();
		void DoCreateBuffer(void *pData);

		GLenum GetGLBufferUsage(BufferUsage bufferUsage);

		GLuint _bufferHandle;
		GLuint _bindingPoint;
 
	};
 
	class GLRenderTexture2D : public RenderTexture2D
	{
	public:
		~GLRenderTexture2D();

		static GLRenderTexture2DPtr Create(PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag);
		static GLRenderTexture2DPtr Create(PixelFormat pixelFormat, bool bMultiSample,ImagePtr const& image);
		static GLRenderTexture2DPtr Create(PixelFormat pixelFormat, bool bMultiSample, const std::vector<ImagePtr>& image, unsigned int flag);

		virtual void * GetHandle();
	protected:
		GLRenderTexture2D(int arraySize);
 
		GLuint _texFormat;
		GLuint _texHandle;
		 
	};
 
}

#endif
