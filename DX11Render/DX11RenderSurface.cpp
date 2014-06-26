#include "DX11RenderInclude.h"

namespace Disorder
{
	void* DX11SurfaceView::GetHandle()
	{
		if (Type == SV_DepthStencil)
			return DepthStencilHandle.get();

		if (Type == SV_RenderTarget)
			return RenderTargetHandle.get();

		if (Type == SV_ShaderResource)
			return ShaderResourceHandle.get();

		return NULL;
	}

	DX11SurfaceViewPtr DX11SurfaceView::Create(ESurfaceViewType type, RenderTexturePtr resource, PixelFormat format, unsigned int flag)
	{
		DX11SurfaceView *pSurface = new DX11SurfaceView;
		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine);
		pSurface->Type = type;
		pSurface->Resource = resource;
		pSurface->Format = format;
		pSurface->Flag = flag;

		if (type == SV_ShaderResource)
		{
			ID3D11ShaderResourceView* pShaderResourceView = NULL;
			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
			SRVDesc.Format = DX11RenderEngine::GetPixelFormat(format);
			RenderTexture2DPtr tex = boost::dynamic_pointer_cast<RenderTexture2D>(resource);
			
			if (tex->ArraySize == 6 && flag & SF_AsCubeMap)
			{
				SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				SRVDesc.TextureCube.MostDetailedMip = 0;
				SRVDesc.TextureCube.MipLevels = 1;
			}
			else
			{
				if (tex->MultiSampleCount > 1)
					SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
				else
				{
					SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					SRVDesc.Texture2D.MostDetailedMip = 0;
					SRVDesc.Texture2D.MipLevels = 1;
				}
			}
			
			
			HRESULT hr = renderEngine->D3DDevice()->CreateShaderResourceView((ID3D11Resource *)resource->GetHandle(), &SRVDesc, &pShaderResourceView);
			BOOST_ASSERT(SUCCEEDED(hr));
			pSurface->ShaderResourceHandle = MakeComPtr<ID3D11ShaderResourceView>(pShaderResourceView);
		}
		else if (type == SV_DepthStencil)
		{
			ID3D11DepthStencilView* pDepthStencilView = NULL;
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = DX11RenderEngine::GetPixelFormat(format);
			RenderTexture2DPtr tex = boost::dynamic_pointer_cast<RenderTexture2D>(resource);
			if (tex->ArraySize > 1)
			{
				if (tex->MultiSampleCount > 1)
				{
					descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
					descDSV.Texture2DMSArray.ArraySize = tex->ArraySize;
					descDSV.Texture2DMSArray.FirstArraySlice = 0;
				}
				else
				{
					descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
					descDSV.Texture2DArray.ArraySize = tex->ArraySize;
					descDSV.Texture2DArray.FirstArraySlice = 0;
					descDSV.Texture2DArray.MipSlice = 0;
				}
			}
			else
			{
				if (tex->MultiSampleCount > 1)
					descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
				else
					descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

				descDSV.Texture2D.MipSlice = 0;
			}
			
			
 
			HRESULT hr = renderEngine->D3DDevice()->CreateDepthStencilView((ID3D11Resource *)resource->GetHandle(), &descDSV, &pDepthStencilView);
			BOOST_ASSERT(SUCCEEDED(hr));
			pSurface->DepthStencilHandle = MakeComPtr<ID3D11DepthStencilView>(pDepthStencilView);

			ID3D11DepthStencilView* pDepthStencilViewReadOnly = NULL;
			if (flag != 0 && renderEngine->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0)
			{
				if (flag & SF_ReadOnlyDepth)
					descDSV.Flags |= D3D11_DSV_READ_ONLY_DEPTH;
				if (flag & SF_ReadOnlyStencil)
					descDSV.Flags |= D3D11_DSV_READ_ONLY_STENCIL;

				hr = renderEngine->D3DDevice()->CreateDepthStencilView((ID3D11Resource *)resource->GetHandle(), &descDSV, &pDepthStencilViewReadOnly);
				BOOST_ASSERT(SUCCEEDED(hr));
				pSurface->ReadonlyDepthStencil = MakeComPtr<ID3D11DepthStencilView>(pDepthStencilViewReadOnly);
			}
			else
				pSurface->ReadonlyDepthStencil = NULL;
			
		}
		else if (type == SV_RenderTarget)
		{
			ID3D11RenderTargetView* pRenderTargetView = NULL;
			D3D11_RENDER_TARGET_VIEW_DESC SRVDesc;
			SRVDesc.Format = DX11RenderEngine::GetPixelFormat(format);
			RenderTexture2DPtr tex = boost::dynamic_pointer_cast<RenderTexture2D>(resource);

			if (tex->ArraySize > 1)
			{
				if (tex->MultiSampleCount > 1)
					SRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
				else
					SRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			}
			else
			{
				if (tex->MultiSampleCount > 1)
					SRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
				else
					SRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			}
			
			SRVDesc.Texture2D.MipSlice = 0;
			HRESULT hr = renderEngine->D3DDevice()->CreateRenderTargetView((ID3D11Resource *)resource->GetHandle(), &SRVDesc, &pRenderTargetView);
			BOOST_ASSERT(SUCCEEDED(hr));
			pSurface->RenderTargetHandle = MakeComPtr<ID3D11RenderTargetView>(pRenderTargetView);
		}
		else
		{
			BOOST_ASSERT(0);
		}

		return DX11SurfaceViewPtr(pSurface);
	}

	DX11RenderSurfacePtr DX11RenderSurface::Create(const std::map<ESurfaceLocation, SurfaceViewPtr>& viewMap)
	{
		DX11RenderSurface *pSurface = new DX11RenderSurface;
		std::map<ESurfaceLocation, SurfaceViewPtr>::const_iterator iter = viewMap.cbegin();
		while (iter != viewMap.cend())
		{
			pSurface->_surfacesViewArray[iter->first] = iter->second;
			++iter;
		}
		return DX11RenderSurfacePtr(pSurface);
	}
 
}