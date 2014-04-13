#include "DX11RenderInclude.h"

namespace Disorder
{
	DX11RenderSurfacePtr DX11RenderSurface::Create()
	{
		DX11RenderSurface *pSurface = new DX11RenderSurface;
		return DX11RenderSurfacePtr(pSurface);
	}

	DX11RenderSurfacePtr DX11RenderSurface::Create(const std::vector<sRenderSurfaceDes>& surfaceDes)
	{
		BOOST_ASSERT(surfaceDes.size() > 0);
		DX11RenderSurface *pSurface = new DX11RenderSurface;
		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine);

		for (size_t i = 0; i < surfaceDes.size(); i++)
		{
			sRenderSurfaceDes des = surfaceDes[i];
			if (des.Location == SL_ShaderResource)
			{
				ID3D11ShaderResourceView* pShaderResourceView = NULL;
				D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
				SRVDesc.Format = DX11RenderEngine::GetPixelFormat(des.Format);
				RenderTexture2DPtr tex = boost::dynamic_pointer_cast<RenderTexture2D>(des.Resource);
				if (tex->MultiSampleCount > 1)
					SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
				else
					SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				SRVDesc.Texture2D.MostDetailedMip = 0;
				SRVDesc.Texture2D.MipLevels = 1;
				renderEngine->D3DDevice()->CreateShaderResourceView((ID3D11Resource *)des.Resource->GetHandle(), &SRVDesc, &pShaderResourceView);
				des.Handler = pShaderResourceView;			
			}
			else if (des.Location == SL_DepthStencil)
			{
				ID3D11DepthStencilView* pDepthStencilView = NULL;
				D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
				ZeroMemory(&descDSV, sizeof(descDSV));
				descDSV.Format = DX11RenderEngine::GetPixelFormat(des.Format);
				RenderTexture2DPtr tex = boost::dynamic_pointer_cast<RenderTexture2D>(des.Resource);
				if (tex->MultiSampleCount > 1)
					descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
				else
					descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				descDSV.Texture2D.MipSlice = 0;
				if (renderEngine->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0)
				{
					if (des.Flag & SF_ReadOnlyDepth)
						descDSV.Flags |= D3D11_DSV_READ_ONLY_DEPTH;
					if (des.Flag & SF_ReadOnlyStencil )
						descDSV.Flags |= D3D11_DSV_READ_ONLY_STENCIL;
				}

				renderEngine->D3DDevice()->CreateDepthStencilView((ID3D11Resource *)des.Resource->GetHandle(), &descDSV, &pDepthStencilView);
				des.Handler = pDepthStencilView;
			}
			else if (des.Location >= SL_RenderTarget1)
			{
				ID3D11RenderTargetView* pRenderTargetView = NULL;
				D3D11_RENDER_TARGET_VIEW_DESC SRVDesc;
				SRVDesc.Format = DX11RenderEngine::GetPixelFormat(des.Format);
				RenderTexture2DPtr tex = boost::dynamic_pointer_cast<RenderTexture2D>(des.Resource);
				if (tex->MultiSampleCount > 1)
					SRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
				else
					SRVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				SRVDesc.Texture2D.MipSlice = 0;
				renderEngine->D3DDevice()->CreateRenderTargetView((ID3D11Resource *)des.Resource->GetHandle(), &SRVDesc, &pRenderTargetView);
				des.Handler = pRenderTargetView;
			}

			pSurface->SurfaceDes.push_back(des);
		}

		return DX11RenderSurfacePtr(pSurface);
	}

	DX11RenderSurface::~DX11RenderSurface()
	{
		for (size_t i = 0; i < SurfaceDes.size(); i++)
		{
			if (SurfaceDes[i].Handler != NULL)
			{
				((ID3D11View *)SurfaceDes[i].Handler)->Release();
				SurfaceDes[i].Handler = NULL;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	DX11RenderGBuffer::DX11RenderGBuffer(unsigned int width,unsigned int height)
	{
 
		SamplerDesc sDesc;
		sDesc.AddressU = sDesc.AddressV = sDesc.AddressW = TAM_Wrap;
		SamplerStatePtr linearSampleState = GEngine->RenderResourceMgr->CreateSamplerState(&sDesc);

		{
			std::vector<ESurfaceLocation> vlocation;
			vlocation.push_back(SL_ShaderResource);
			vlocation.push_back(SL_DepthStencil);
			RenderTexture2DPtr depthStencilTex = GEngine->RenderResourceMgr->CreateTexture2D(linearSampleState, PF_R24G8_TYPELESS, width, height, false, false, vlocation, NULL);
			std::vector<sRenderSurfaceDes> depthSurfaceDes;
			sRenderSurfaceDes des;
			des.Flag = SF_ReadOnlyDepth | SF_ReadOnlyStencil;
			des.Format = PF_D24_UNORM_S8_UINT;
			des.Location = SL_DepthStencil;
			des.Resource = depthStencilTex;
			depthSurfaceDes.push_back(des);

			des.Flag = 0;
			des.Format = PF_R24_UNORM_X8_TYPELESS;
			des.Location = SL_ShaderResource;
			depthSurfaceDes.push_back(des);

			DepthStencilBuffer = GEngine->RenderResourceMgr->CreateRenderSurface(depthSurfaceDes);
		}

		{
			std::vector<ESurfaceLocation> vlocation;
			vlocation.push_back(SL_ShaderResource);
			vlocation.push_back(SL_RenderTarget1);
			RenderTexture2DPtr basicColorTex = GEngine->RenderResourceMgr->CreateTexture2D(linearSampleState, PF_R8G8B8A8_UNORM, width, height, false, false, vlocation, NULL);
			std::vector<sRenderSurfaceDes> vDes;
			sRenderSurfaceDes des;
			des.Format = PF_R8G8B8A8_UNORM;
			des.Location = SL_ShaderResource;
			des.Resource = basicColorTex;
			vDes.push_back(des);

			des.Format = PF_R8G8B8A8_UNORM;
			des.Location = SL_RenderTarget1;
			vDes.push_back(des);
			BasicColorBuffer = GEngine->RenderResourceMgr->CreateRenderSurface(vDes);

			vDes.clear();
			RenderTexture2DPtr normalTex = GEngine->RenderResourceMgr->CreateTexture2D(linearSampleState, PF_R11G11B10_FLOAT, width, height, false, false, vlocation, NULL);
			des.Format = PF_R11G11B10_FLOAT;
			des.Location = SL_ShaderResource;
			des.Resource = basicColorTex;
			vDes.push_back(des);

			des.Format = PF_R11G11B10_FLOAT;
			des.Location = SL_RenderTarget1;
			vDes.push_back(des);
			NormalDataBuffer = GEngine->RenderResourceMgr->CreateRenderSurface(vDes);

			vDes.clear();
			RenderTexture2DPtr specularTex = GEngine->RenderResourceMgr->CreateTexture2D(linearSampleState, PF_R8G8B8A8_UNORM, width, height, false, false, vlocation, NULL);
			des.Format = PF_R8G8B8A8_UNORM;
			des.Location = SL_ShaderResource;
			des.Resource = basicColorTex;
			vDes.push_back(des);

			des.Format = PF_R8G8B8A8_UNORM;
			des.Location = SL_RenderTarget1;
			vDes.push_back(des);
			SpecularDataBuffer = GEngine->RenderResourceMgr->CreateRenderSurface(vDes);
		}
		
		_GBufferVisualEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
		ShaderObjectPtr vertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader,"SurfaceVisual",SM_4_0,"VS");
		ShaderObjectPtr pixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader,"SurfaceVisual",SM_4_0,"PS");
		_GBufferVisualEffect->BindShader(vertexShader);
		_GBufferVisualEffect->BindShader(pixelShader);

		DepthStencilDesc depthDesc;
		depthDesc.DepthEnable = false;
		DepthStencilStatePtr _DepthWriteState = GEngine->RenderResourceMgr->CreateDepthStencilState(&depthDesc,1);
		_GBufferVisualEffect->BindDepthStencilState(_DepthWriteState);

		TileTexVertex vertex[4];
		vertex[0].position = glm::vec3(-0.55f, -0.55f, 0.f);
		vertex[0].texcoord = glm::vec2(1.f, 0.f);
 
		vertex[1].position = glm::vec3(-0.95f, -0.55f, 0.0f);
		vertex[1].texcoord = glm::vec2(0.f, 0.f);
 
		vertex[2].position = glm::vec3(-0.55f, -0.95f, 0.0f);
		vertex[2].texcoord = glm::vec2(1.f, 1.0f);
 
		vertex[3].position = glm::vec3(-0.95f, -0.95f, 0.0f);
		vertex[3].texcoord = glm::vec2(0.0f, 1.0f);
 
		_DepthVisTile = SimpleTile("GBufferDepthTile",vertex,_GBufferVisualEffect);

		vertex[0].position = glm::vec3(-0.05f, -0.55f, 0.0f);
		vertex[1].position = glm::vec3(-0.45f, -0.55f, 0.0f);
		vertex[2].position = glm::vec3(-0.05f, -0.95f, 0.0f);
		vertex[3].position = glm::vec3(-0.45f, -0.95f, 0.0f);
		_ColorVisTile = SimpleTile("GBufferColorTile",vertex,_GBufferVisualEffect);

		vertex[0].position = glm::vec3(0.45f, -0.55f, 0.0f);
		vertex[1].position = glm::vec3(0.05f, -0.55f, 0.0f);
		vertex[2].position = glm::vec3(0.45f, -0.95f, 0.0f);
		vertex[3].position = glm::vec3(0.05f, -0.95f, 0.0f);
		_NormalVisTile = SimpleTile("GBufferNormalTile",vertex,_GBufferVisualEffect);
	
		vertex[0].position = glm::vec3(0.95f, -0.55f, 0.0f);
		vertex[1].position = glm::vec3(0.55f, -0.55f, 0.0f);
		vertex[2].position = glm::vec3(0.95f, -0.95f, 0.0f);
		vertex[3].position = glm::vec3(0.55f, -0.95f, 0.0f);
		_SpecPowVisTile = SimpleTile("GBufferSpeclTile",vertex,_GBufferVisualEffect);

		

 
	}

	void DX11RenderGBuffer::DebugVisual()
	{
		return;
		CameraPtr mainCamera = GSceneManager->GetDefaultCamera();

		//_GBufferVisTex->SetData(DepthStencilBuffer);
		//_DepthVisTile.Render(mainCamera);
		_SurfaceVisTex->SetData(BasicColorBuffer);
		_ColorVisTile.Render(mainCamera);
		_SurfaceVisTex->SetData(NormalDataBuffer);
		_NormalVisTile.Render(mainCamera);
		_SurfaceVisTex->SetData(SpecularDataBuffer);
		_SpecPowVisTile.Render(mainCamera);
	}

	DX11RenderGBufferPtr DX11RenderGBuffer::Create(unsigned int width,unsigned int height)
	{
 
		DX11RenderGBuffer *pGbuffer = new DX11RenderGBuffer(width,height);
		return DX11RenderGBufferPtr(pGbuffer);
	}
 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	DX11RenderSurfaceCache::DX11RenderSurfaceCache()
	{
	}

	DX11RenderSurfaceCachePtr DX11RenderSurfaceCache::Create()
	{
		DX11RenderSurfaceCache *mgr = new DX11RenderSurfaceCache;
		return DX11RenderSurfaceCachePtr(mgr);
	}

	void DX11RenderSurfaceCache::InitGBuffer(unsigned int width,unsigned int height)
	{
		GBuffer = DX11RenderGBuffer::Create(width,height);
	}


}