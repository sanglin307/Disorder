#include "DX11RenderInclude.h"

namespace Disorder
{
	void* DX11RenderSurface::GetLowInterface(RenderSurfaceUsage usage)
	{
		if( usage == RSU_RenderTarget )
			return RenderTargetView.get();
		else if( usage == RSU_DepthStencil )
			return DepthStencilView.get();
		else if( usage == RSU_ShaderResource )
			return ShaderResourceView.get();

		return NULL;
	}

	void DX11RenderSurface::Create(const RenderTexture2DPtr& resource,unsigned int usage)
	{
		Tex2DResource = resource;
		Usage = usage;

		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 
		if( usage & RSU_RenderTarget )
		{
			ID3D11RenderTargetView* pRenderTargetView = NULL;
			D3D11_RENDER_TARGET_VIEW_DESC SRVDesc;
			SRVDesc.Format = (DXGI_FORMAT)GPixelFormats[resource->Format].PlatformFormat;
			SRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MipSlice = 0;
			renderEngine->D3DDevice()->CreateRenderTargetView((ID3D11Resource *)resource->GetLowInterface(),&SRVDesc,&pRenderTargetView);
			RenderTargetView = MakeComPtr<ID3D11RenderTargetView>(pRenderTargetView);
		}

		if( usage & RSU_DepthStencil )
		{
			ID3D11DepthStencilView* pDepthStencilView = NULL;
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory( &descDSV, sizeof(descDSV) );
			descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;  // fix now 
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;
			renderEngine->D3DDevice()->CreateDepthStencilView( (ID3D11Resource *)resource->GetLowInterface(), &descDSV, &pDepthStencilView );
			DepthStencilView = MakeComPtr<ID3D11DepthStencilView>(pDepthStencilView);
		}

		if( usage & RSU_ShaderResource )
		{
			ID3D11ShaderResourceView* pShaderResourceView = NULL;
			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
			SRVDesc.Format = (DXGI_FORMAT)GPixelFormats[resource->Format].PlatformFormat;
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MostDetailedMip = 0;
			SRVDesc.Texture2D.MipLevels = 1;
			renderEngine->D3DDevice()->CreateShaderResourceView((ID3D11Resource *)resource->GetLowInterface(),&SRVDesc,&pShaderResourceView );
			ShaderResourceView = MakeComPtr<ID3D11ShaderResourceView>(pShaderResourceView);
		}

	}

}