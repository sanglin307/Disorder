#ifndef _DISORDER_DX11RENDERRESOURCE_H_
#define _DISORDER_DX11RENDERRESOURCE_H_

namespace Disorder
{
	class DX11RenderBuffer : public RenderBuffer
	{
	public:
		static DX11RenderBufferPtr Create(RenderBufferType type,GeometryPtr const& data,std::string const& sematic,BufferUsage bufferUsage,ShaderObjectPtr const& vertexShader);
		static DX11RenderBufferPtr Create(RenderBufferType type,BufferUsage bufferUsage,unsigned int elementSize,unsigned int size,void *pData);

		virtual void * GetLowInterface();
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

		static DX11RenderTexture2DPtr Create(PixelFormat pixelFormat,unsigned int width,unsigned int height,bool bMipmap,unsigned int bindFlag,BufferInitData const* pData);
		static DX11RenderTexture2DPtr Create(PixelFormat pixelFormat,ImagePtr const& image);
	    static DX11RenderTexture2DPtr Create(PixelFormat pixelFormat,unsigned int width,unsigned int height,ID3D11Texture2DPtr DXInterface);
		virtual void * GetLowInterface();
	protected:
		DX11RenderTexture2D(){};
		 
	public:
		ID3D11Texture2DPtr D3DInterface;
	};
	
 

}


#endif