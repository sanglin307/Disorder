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

	DX11RenderSurfacePtr DX11RenderSurface::Create(const RenderTexture2DPtr& resource,ID3D11RenderTargetViewPtr RenerTarget,ID3D11ShaderResourceViewPtr ShaderResource,ID3D11DepthStencilViewPtr DepthStencil)
	{
		DX11RenderSurface *pSurface = new DX11RenderSurface;
		pSurface->Tex2DResource = resource;
		pSurface->Usage = 0;
		if( RenerTarget != NULL )
		{
			pSurface->Usage |= RSU_RenderTarget;
			pSurface->RenderTargetView = RenerTarget;
		}

		if( ShaderResource != NULL )
		{
			pSurface->Usage |= RSU_ShaderResource;
			pSurface->ShaderResourceView = ShaderResource;
		}

		if( DepthStencil != NULL )
		{
			pSurface->Usage |= RSU_DepthStencil;
			pSurface->DepthStencilView = DepthStencil;
		}

		return DX11RenderSurfacePtr(pSurface);

	}

	DX11RenderSurfacePtr DX11RenderSurface::Create(const RenderTexture2DPtr& resource,unsigned int usage)
	{
		DX11RenderSurface *pSurface = new DX11RenderSurface;
		pSurface->Tex2DResource = resource;
		pSurface->Usage = usage;

		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 
		if( usage & RSU_RenderTarget )
		{
			ID3D11RenderTargetView* pRenderTargetView = NULL;
			D3D11_RENDER_TARGET_VIEW_DESC SRVDesc;
			SRVDesc.Format = (DXGI_FORMAT)GPixelFormats[resource->Format].PlatformFormat;
			SRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MipSlice = 0;
			renderEngine->D3DDevice()->CreateRenderTargetView((ID3D11Resource *)resource->GetLowInterface(),&SRVDesc,&pRenderTargetView);
			pSurface->RenderTargetView = MakeComPtr<ID3D11RenderTargetView>(pRenderTargetView);
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
			pSurface->DepthStencilView = MakeComPtr<ID3D11DepthStencilView>(pDepthStencilView);
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
			pSurface->ShaderResourceView = MakeComPtr<ID3D11ShaderResourceView>(pShaderResourceView);
		}

		return DX11RenderSurfacePtr(pSurface);
	}

}