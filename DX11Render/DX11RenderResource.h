#ifndef _DISORDER_DX11RENDERRESOURCE_H_
#define _DISORDER_DX11RENDERRESOURCE_H_

namespace Disorder
{
	class DX11RenderBuffer : public RenderBuffer
	{
	public:
		static DX11RenderBufferPtr Create(RenderBufferType type,GeometryPtr const& data,std::string const& sematic,BufferUsage bufferUsage,ShaderObjectPtr const& vertexShader);
		static DX11RenderBufferPtr Create(RenderBufferType type,BufferUsage bufferUsage,unsigned int elementSize,unsigned int size,void *pData);

		virtual void * GetHandle();
		virtual void Resize(unsigned int size);

	private:
		DX11RenderBuffer(){}

		void GetD3DFlags(void *pData,D3D11_USAGE& usage, UINT& cpuAccessFlags,UINT& miscFlags);
		void DoCreateBuffer(void *pData);

	public:
		ID3D11BufferPtr D3DInterface; 
	};


	class DX11RenderTexture2D : public RenderTexture2D
	{
	public:

		static DX11RenderTexture2DPtr Create(PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag);
		static DX11RenderTexture2DPtr Create(PixelFormat pixelFormat, bool bMultiSample,ImagePtr const& image);
		static DX11RenderTexture2DPtr Create(PixelFormat pixelFormat, bool bMultiSample, const std::vector<ImagePtr>& image, unsigned int flag);
		static DX11RenderTexture2DPtr Create(PixelFormat pixelFormat, unsigned int width, unsigned int height, unsigned int viewFlag, bool bmipmap, unsigned int multiSampleCount, unsigned int multiSampleQuality, ID3D11Texture2DPtr DXInterface);
		virtual void * GetHandle();
	protected:
		DX11RenderTexture2D(){};
		 
	public:
		ID3D11Texture2DPtr D3DInterface;
	};
	
 

}


#endif