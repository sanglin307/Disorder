#ifndef _DISORDER_DX11RENDERVIEW_H_
#define _DISORDER_DX11RENDERVIEW_H_


namespace Disorder
{
 
	class DX11RenderTargetView : public RenderView
	{
	public:
		virtual void* GetLowInterface()
		{
			return D3DInterface.get();
		}

	public:
		ID3D11RenderTargetViewPtr D3DInterface;
	};

	class DX11DepthStencilView : public RenderView
	{
	public:
		virtual void* GetLowInterface()
		{
			return D3DInterface.get();
		}

	public:
		ID3D11DepthStencilViewPtr D3DInterface;
	};

	class DX11ShaderResourceView : public RenderView
	{
	public:
		virtual void* GetLowInterface()
		{
			return D3DInterface.get();
		}

	    bool CreateTexture2DView(RenderTexture2DPtr const& textureResource);
		bool CreateTexture2DViewFromFile(std::string const& fileName);

	public:
		ID3D11ShaderResourceViewPtr D3DInterface;
	};

}


#endif
