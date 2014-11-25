#ifndef _DISORDER_DX11RENDERRESOURCE_H_
#define _DISORDER_DX11RENDERRESOURCE_H_

namespace Disorder
{
	class DX11RenderBuffer : public RenderBuffer
	{
	public:
		DX11RenderBuffer(const std::string& bufferName, RenderBufferType type, Geometry* data, std::string const& sematic, BufferUsage bufferUsage, ShaderObject* vertexShader);
		DX11RenderBuffer(const std::string& bufferName, RenderBufferType type, BufferUsage bufferUsage, unsigned int elementSize, unsigned int size, void *pData);

		virtual void * GetHandle();
		virtual void Resize(unsigned int size);

	private:
		void GetD3DFlags(void *pData,D3D11_USAGE& usage, UINT& cpuAccessFlags,UINT& miscFlags);
		void DoCreateBuffer(void *pData);

	public:
		ID3D11Buffer* D3DInterface; 
	};


	class DX11RenderTexture2D : public RenderTexture2D
	{
	public:
		DX11RenderTexture2D(PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag);
		DX11RenderTexture2D(PixelFormat pixelFormat, bool bMultiSample, Image* image);
		DX11RenderTexture2D(PixelFormat pixelFormat, bool bMultiSample, const std::vector<Image*>& image, unsigned int flag);
		DX11RenderTexture2D(PixelFormat pixelFormat, unsigned int width, unsigned int height, unsigned int viewFlag, bool bmipmap, unsigned int multiSampleCount, unsigned int multiSampleQuality, ID3D11Texture2D* DXInterface);
		virtual void * GetHandle();
	public:
		ID3D11Texture2D* D3DInterface;

	private:
		void InternalCreate(PixelFormat pixelFormat, unsigned int width, unsigned int height, bool bMipmap, bool bMultiSample, unsigned int viewFlag, int arraySize, BufferInitData const* pData, unsigned int flag);
	};
}


#endif