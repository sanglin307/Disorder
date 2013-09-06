#ifndef _DISORDER_RENDERRESOURCE_H_
#define _DISORDER_RENDERRESOURCE_H_

namespace Disorder
{

	class RenderResource
	{
	public:
		virtual void * GetLowInterface(){ return 0;}
	};

	class RenderBuffer :public RenderResource
	{
	public:
		virtual void CreateBuffer(RenderBufferType type,GeometryPtr const& data,unsigned int accessHint,ShaderObjectPtr const& vertexShader) = 0;
		virtual void CreateBuffer(RenderBufferType type,unsigned int accessHint,unsigned int elementSize,unsigned int size,void *pData) = 0;
		virtual void CreateVertexBuffer(GeometryPtr const& data,std::string const& sematic, unsigned int accessHint,ShaderObjectPtr const& vertexShader) = 0;
		virtual void CreateConstBuffer(unsigned int size, unsigned int accessHint)=0;
		unsigned int const& GetElementSize() const
	    {
		     return _elementSize;
		}

		virtual void Resize(unsigned int size) = 0;
		virtual void * GetLowInterface(){ return 0;}

	protected:
		unsigned int _accessHint;
		unsigned int _bindFlags;
		unsigned int _elementSize;
		unsigned int _bufferSize;
        RenderBufferType _type;
	};
 
	//----------Texture------------------------
	class RenderTexture : public RenderResource
	{
	};

	class RenderTexture1D: public RenderTexture
	{
	};

	class RenderTexture2D: public RenderTexture
	{
	public:
		virtual bool Create(PixelFormat pixelFormat,unsigned int width,unsigned int height,bool bMipmap,BufferInitData const* pData) = 0;
       

	public:
		UINT Width;
		UINT Height;
		PixelFormat Format;
        UINT MipLevel;
		SamplerStatePtr Sampler;
	};
 
}



#endif