#ifndef _DISORDER_RENDERRESOURCE_H_
#define _DISORDER_RENDERRESOURCE_H_

namespace Disorder
{

	class RenderResource
	{
	public:

		virtual void * GetHandle(){ return 0;}
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
		virtual void * GetHandle(){ return 0;}

	protected:
		RenderBuffer(){}

		BufferUsage  _bufferUsage;
		unsigned int _bindFlags;
		unsigned int _elementSize;
		unsigned int _bufferSize;
        RenderBufferType _type;
	};
 
	//----------Texture------------------------
	class RenderTexture : public RenderResource
	{
	public:
		unsigned int Width;
		unsigned int Height;
		PixelFormat Format;
		unsigned int ViewFlag;
		unsigned int MipLevel;
		unsigned int MultiSampleCount;
		unsigned int MultiSampleQuality;
		int ArraySize;
		SamplerStatePtr Sampler;
	};

	class RenderTexture1D: public RenderTexture
	{
	};

	class RenderTexture2D: public RenderTexture
	{
	public:
		
	protected:

		RenderTexture2D(){}
	};
 
	class RenderTexture3D: public RenderTexture
	{
	};
 
}



#endif