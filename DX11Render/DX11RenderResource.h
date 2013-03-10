#ifndef _DISORDER_DX11RENDERRESOURCE_H_
#define _DISORDER_DX11RENDERRESOURCE_H_

namespace Disorder
{
	class DX11RenderBuffer : public RenderBuffer
	{
	public:
		virtual void CreateBuffer(RenderBufferType type,unsigned int elementSize,unsigned int bufferSize,unsigned int accessHint,BufferInitData const* pData);
		void GetD3DFlags(D3D11_USAGE& usage, UINT& cpuAccessFlags, UINT& bindFlags, UINT& miscFlags);
		virtual void * GetLowInterface();
		virtual void Resize(unsigned int size);

	private:
		void DoCreateBuffer(BufferInitData const* pData);

	public:
		ID3D11BufferPtr D3DInterface; 
	};


	class DX11RenderTexture2D : public RenderTexture2D
	{
	public:
		virtual bool Create(PixelFormat pixelFormat,unsigned int width,unsigned int height,bool bMipmap,BufferInitData const* pData);
	
		virtual void * GetLowInterface();

	public:
		ID3D11Texture2DPtr D3DInterface;
	};
	
 

}


#endif