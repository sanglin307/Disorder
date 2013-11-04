#ifndef _DISORDER_DX11RENDERRESOURCE_H_
#define _DISORDER_DX11RENDERRESOURCE_H_

namespace Disorder
{
	class DX11RenderBuffer : public RenderBuffer
	{
	public:
		virtual void CreateVertexBuffer(GeometryPtr const& data,std::string const& sematic,unsigned int accessHint,ShaderObjectPtr const& vertexShader);
		virtual void CreateBuffer(RenderBufferType type,GeometryPtr const& data,unsigned int accessHint,ShaderObjectPtr const& vertexShader);
		virtual void CreateBuffer(RenderBufferType type,unsigned int accessHint,unsigned int elementSize,unsigned int size,void *pData);
		virtual void CreateConstBuffer(unsigned int size, unsigned int accessHint);
		void GetD3DFlags(D3D11_USAGE& usage, UINT& cpuAccessFlags, UINT& bindFlags, UINT& miscFlags);
		virtual void * GetLowInterface();
		virtual void Resize(unsigned int size);

	private:
		void DoCreateBuffer(void *pData);

	public:
		ID3D11BufferPtr D3DInterface; 
	};


	class DX11RenderTexture2D : public RenderTexture2D
	{
	public:
		DX11RenderTexture2D(){};
		DX11RenderTexture2D(PixelFormat pixelFormat,unsigned int width,unsigned int height,ID3D11Texture2DPtr DXInterface);
		virtual bool Create(PixelFormat pixelFormat,unsigned int width,unsigned int height,bool bMipmap,BufferInitData const* pData);
		virtual bool Create(PixelFormat pixelFormat,ImagePtr const& image);
	 
		virtual void * GetLowInterface();

	public:
		ID3D11Texture2DPtr D3DInterface;
	};
	
 

}


#endif