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
		 
		unsigned int const& GetElementSize() const
	    {
		     return _elementSize;
		}

		RenderBufferType GetBufferType()
		{
			return _type;
		}

		virtual void Resize(unsigned int size) = 0;
		virtual void * GetLowInterface(){ return 0;}

	protected:
		RenderBuffer(){}

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
		UINT Width;
		UINT Height;
		PixelFormat Format;
        UINT MipLevel;
		SamplerStatePtr Sampler;
	protected:

		RenderTexture2D(){}
	};
 
	class RenderTexture3D: public RenderTexture
	{
	};
}



#endif