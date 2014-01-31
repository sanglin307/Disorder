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

	DX11RenderSurfacePtr DX11RenderSurface::Create(const RenderTexture2DPtr& resource,unsigned int usage,PixelFormat RenderTargetFormat,PixelFormat DepthFormat,PixelFormat ShaderResFormat,bool readOnlyDepth,bool readOnlyStencil)
	{
		DX11RenderSurface *pSurface = new DX11RenderSurface;
		pSurface->Tex2DResource = resource;
		pSurface->Usage = usage;

		DX11RenderEnginePtr renderEngine = boost::dynamic_pointer_cast<DX11RenderEngine>(GEngine->RenderEngine); 
		if( usage & RSU_RenderTarget )
		{
			ID3D11RenderTargetView* pRenderTargetView = NULL;
			D3D11_RENDER_TARGET_VIEW_DESC SRVDesc;
			SRVDesc.Format = DX11RenderEngine::GetPixelFormat(RenderTargetFormat);
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
			descDSV.Format = DX11RenderEngine::GetPixelFormat(DepthFormat);   
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;
			renderEngine->D3DDevice()->CreateDepthStencilView( (ID3D11Resource *)resource->GetLowInterface(), &descDSV, &pDepthStencilView );
			pSurface->DepthStencilView = MakeComPtr<ID3D11DepthStencilView>(pDepthStencilView);

			// only feature level > 11 support it.
			if( renderEngine->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 && ( readOnlyDepth || readOnlyStencil ) )
			{
				if( readOnlyDepth )
				    descDSV.Flags |= D3D11_DSV_READ_ONLY_DEPTH ;
				if( readOnlyStencil )
					descDSV.Flags |= D3D11_DSV_READ_ONLY_STENCIL;

				renderEngine->D3DDevice()->CreateDepthStencilView( (ID3D11Resource *)resource->GetLowInterface(), &descDSV, &pDepthStencilView );
			    pSurface->DepthStencilReadOnlyView = MakeComPtr<ID3D11DepthStencilView>(pDepthStencilView);
			}
		}

		if( usage & RSU_ShaderResource )
		{
			ID3D11ShaderResourceView* pShaderResourceView = NULL;
			D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
			SRVDesc.Format = DX11RenderEngine::GetPixelFormat(ShaderResFormat);
			SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			SRVDesc.Texture2D.MostDetailedMip = 0;
			SRVDesc.Texture2D.MipLevels = 1;
			renderEngine->D3DDevice()->CreateShaderResourceView((ID3D11Resource *)resource->GetLowInterface(),&SRVDesc,&pShaderResourceView );
			pSurface->ShaderResourceView = MakeComPtr<ID3D11ShaderResourceView>(pShaderResourceView);
		}

		return DX11RenderSurfacePtr(pSurface);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	DX11RenderGBuffer::DX11RenderGBuffer(unsigned int width,unsigned int height)
	{
		SamplerStatePtr linearSampleState = GEngine->RenderResourceMgr->CreateSamplerState(SF_Linear,TAM_Wrap,1);

		unsigned int surfaceUsage = RSU_DepthStencil|RSU_ShaderResource;
		RenderTexture2DPtr depthStencilTex = GEngine->RenderResourceMgr->CreateRenderTexture2D(linearSampleState,PF_R24G8_TYPELESS,width,height,false,surfaceUsage,NULL);
		DepthStencilBuffer = GEngine->RenderResourceMgr->CreateRenderSurface(depthStencilTex,surfaceUsage,PF_UNKNOWN,PF_D24_UNORM_S8_UINT,PF_R24_UNORM_X8_TYPELESS,true,true);

		surfaceUsage = RSU_RenderTarget | RSU_ShaderResource;
		RenderTexture2DPtr basicColorTex = GEngine->RenderResourceMgr->CreateRenderTexture2D(linearSampleState,PF_R8G8B8A8_UNORM,width,height,false,surfaceUsage,NULL);
		BasicColorBuffer = GEngine->RenderResourceMgr->CreateRenderSurface(basicColorTex,surfaceUsage,PF_R8G8B8A8_UNORM,PF_UNKNOWN,PF_R8G8B8A8_UNORM);

		RenderTexture2DPtr normalTex = GEngine->RenderResourceMgr->CreateRenderTexture2D(linearSampleState,PF_R11G11B10_FLOAT,width,height,false,surfaceUsage,NULL);
		NormalDataBuffer = GEngine->RenderResourceMgr->CreateRenderSurface(normalTex,surfaceUsage,PF_R11G11B10_FLOAT,PF_UNKNOWN,PF_R11G11B10_FLOAT);

		RenderTexture2DPtr specularTex = GEngine->RenderResourceMgr->CreateRenderTexture2D(linearSampleState,PF_R8G8B8A8_UNORM,width,height,false,surfaceUsage,NULL);
		SpecularDataBuffer = GEngine->RenderResourceMgr->CreateRenderSurface(specularTex,surfaceUsage,PF_R8G8B8A8_UNORM,PF_UNKNOWN,PF_R8G8B8A8_UNORM);

		
		_GBufferVisualEffect = RenderEffect::Create();
		ShaderObjectPtr vertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader,"SurfaceVisual",SM_4_0,"SurfaceVisualVS");
		ShaderObjectPtr pixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader,"SurfaceVisual",SM_4_0,"SurfaceVisualPS");
		_GBufferVisualEffect->BindShader(vertexShader);
		_GBufferVisualEffect->BindShader(pixelShader);

		std::vector<TileTexVertex> tilePos;
		TileTexVertex vertex;
		vertex.position = Vector3(-0.55f, -0.55f,0.f);
		vertex.texcoord = Vector2(1.f,0.f);
		tilePos.push_back(vertex);
		vertex.position = Vector3(-0.95f, -0.55f,0.0f);
		vertex.texcoord = Vector2(0.f,0.f);
		tilePos.push_back(vertex);
		vertex.position = Vector3(-0.55f, -0.95f,0.0f);
		vertex.texcoord = Vector2(1.f,1.0f);
		tilePos.push_back(vertex);
		vertex.position = Vector3(-0.95f, -0.95f,0.0f);
		vertex.texcoord = Vector2(0.0f,1.0f);
		tilePos.push_back(vertex);
		_DepthVisTile = SimpleTile("GBufferDepthTile",tilePos,_GBufferVisualEffect);

		tilePos[0].position = Vector3(-0.05f,-0.55f,0.0f);
		tilePos[1].position = Vector3(-0.45f,-0.55f,0.0f);
		tilePos[2].position = Vector3(-0.05f,-0.95f,0.0f);
		tilePos[3].position = Vector3(-0.45f,-0.95f,0.0f);
		_ColorVisTile = SimpleTile("GBufferColorTile",tilePos,_GBufferVisualEffect);

		tilePos[0].position = Vector3(0.45f,-0.55f,0.0f);
		tilePos[1].position = Vector3(0.05f,-0.55f,0.0f);
		tilePos[2].position = Vector3(0.45f,-0.95f,0.0f);
		tilePos[3].position = Vector3(0.05f,-0.95f,0.0f);
		_NormalVisTile = SimpleTile("GBufferNormalTile",tilePos,_GBufferVisualEffect);
	
		tilePos[0].position = Vector3(0.95f,-0.55f,0.0f);
		tilePos[1].position = Vector3(0.55f,-0.55f,0.0f);
		tilePos[2].position = Vector3(0.95f,-0.95f,0.0f);
		tilePos[3].position = Vector3(0.55f,-0.95f,0.0f);
		_SpecPowVisTile = SimpleTile("GBufferSpeclTile",tilePos,_GBufferVisualEffect);
 
	}

	void DX11RenderGBuffer::DebugVisual()
	{
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