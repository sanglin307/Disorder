#include "DX11RenderInclude.h"

namespace Disorder
{
	bool DX11ShaderResourceView::CreateTexture2DView(RenderTexture2DPtr const& textureResource)
	{
		//_width = textureResource->Width;
		//_height = textureResource->Height;

		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
        ZeroMemory( &SRVDesc, sizeof( SRVDesc ) );
		SRVDesc.Format =  (DXGI_FORMAT)GPixelFormats[textureResource->Format].PlatformFormat;
        SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = -1;//textureResource->MipLevel;

		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 
		ID3D11ShaderResourceView* pShaderView;
		HRESULT hr = renderEngine->D3DDevice()->CreateShaderResourceView( (ID3D11Resource*)(textureResource->GetLowInterface()), &SRVDesc, &pShaderView );
		BOOST_ASSERT(hr==S_OK);

		D3DInterface = MakeComPtr<ID3D11ShaderResourceView>(pShaderView);

		return true;
	}

	bool DX11ShaderResourceView::CreateTexture2DViewFromFile(std::string const& fileName)
	{
		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 
		ID3D11ShaderResourceView* pShaderView;
		D3DX11CreateShaderResourceViewFromFileA( renderEngine->D3DDevice().get(), fileName.c_str(), NULL, NULL, &pShaderView, NULL );
		D3DInterface = MakeComPtr<ID3D11ShaderResourceView>(pShaderView);

		return true;

	}
	 
}