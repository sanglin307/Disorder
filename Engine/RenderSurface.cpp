#include "EngineInclude.h"

namespace Disorder
{ 
	RenderSurface::RenderSurface()
	{
		for (size_t i = 0; i < SL_SurfaceLoactionMax; i++)
		{
			_surfacesViewArray.push_back(NULL);
		}
	}

	SurfaceView* RenderSurface::GetSurfaceView(ESurfaceLocation location)
	{
		return _surfacesViewArray[location];
	}
 
 

	RenderGBuffer::RenderGBuffer(unsigned int width, unsigned int height)
	{
		ShaderPropertyManager* propertyManager = GRenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);
		_GBufferDepthTexture = propertyManager->CreateProperty(ShaderPropertyManager::sGBufferDepthTexture,eSP_ShaderResource);
		_GBufferColorSpecIntTexture = propertyManager->CreateProperty(ShaderPropertyManager::sGBufferColorSpecIntTexture,eSP_ShaderResource);
		_GBufferNormalTexture = propertyManager->CreateProperty(ShaderPropertyManager::sGBufferNormalTexture,eSP_ShaderResource);
		_GBufferSpecPowTexture = propertyManager->CreateProperty(ShaderPropertyManager::sGBufferSpecPowTexture,eSP_ShaderResource);
		_SurfaceVisTex = propertyManager->CreateProperty(ShaderPropertyManager::sSurfaceVisTex,eSP_ShaderResource);
		_SurfaceSampler = propertyManager->CreateProperty(ShaderPropertyManager::sSurfaceSampler,eSP_SampleState);
 
		SamplerDesc sDesc;
		SamplerState* pointState = GRenderResourceMgr->CreateSamplerState(&sDesc);
		_SurfaceSampler->SetData(pointState);
 
		RenderTexture2D* depthStencilTex = GRenderResourceMgr->CreateTexture2D("GBufferDepth",NULL, PF_R24G8_TYPELESS, width, height, false, false, SV_DepthStencil | SV_ShaderResource,1, NULL,0);
		DepthBufferView = GRenderResourceMgr->CreateSurfaceView(SV_DepthStencil, depthStencilTex, PF_D24_UNORM_S8_UINT, SF_ReadOnlyDepth | SF_ReadOnlyStencil);
		DepthShaderView = GRenderResourceMgr->CreateSurfaceView(SV_ShaderResource, depthStencilTex, PF_R24_UNORM_X8_TYPELESS);

		RenderTexture2D* basicColorTex = GRenderResourceMgr->CreateTexture2D("GBufferBasicColor",NULL, PF_R8G8B8A8_UNORM, width, height, false, false, SV_RenderTarget | SV_ShaderResource,1, NULL,0);
		BasicColorShaderView = GRenderResourceMgr->CreateSurfaceView(SV_ShaderResource, basicColorTex, PF_R8G8B8A8_UNORM);
		BasicColorTargetView = GRenderResourceMgr->CreateSurfaceView(SV_RenderTarget, basicColorTex, PF_R8G8B8A8_UNORM);

		RenderTexture2D* normalTex = GRenderResourceMgr->CreateTexture2D("GBufferNormal",NULL, PF_R11G11B10_FLOAT, width, height, false, false, SV_RenderTarget | SV_ShaderResource, 1,NULL,0);
		NormalDataShaderView = GRenderResourceMgr->CreateSurfaceView(SV_ShaderResource, normalTex, PF_R11G11B10_FLOAT);
		NormalDataTargetView = GRenderResourceMgr->CreateSurfaceView(SV_RenderTarget, normalTex, PF_R11G11B10_FLOAT);

		RenderTexture2D* specularTex = GRenderResourceMgr->CreateTexture2D("GBufferSpecular",NULL, PF_R8G8B8A8_UNORM, width, height, false, false, SV_RenderTarget | SV_ShaderResource, 1,NULL,0);
		SpecularDataShaderView = GRenderResourceMgr->CreateSurfaceView(SV_ShaderResource, specularTex, PF_R8G8B8A8_UNORM);
		SpecularDataTargetView = GRenderResourceMgr->CreateSurfaceView(SV_RenderTarget, specularTex, PF_R8G8B8A8_UNORM);

		
		std::map<ESurfaceLocation, SurfaceView*> viewMap;
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceView*>(SL_DepthStencil, DepthBufferView));
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceView*>(SL_RenderTarget1, BasicColorTargetView));
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceView*>(SL_RenderTarget2, NormalDataTargetView));
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceView*>(SL_RenderTarget3, SpecularDataTargetView));
		RenderTargetBuffer = GRenderResourceMgr->CreateRenderSurface(viewMap);

		viewMap.clear();
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceView*>(SL_DepthStencil, DepthBufferView));
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceView*>(SL_RenderTarget1, GRenderSurface->MainTarget->RenderTargetView));
		MainTargetGDepth = GRenderResourceMgr->CreateRenderSurface(viewMap);

		_GBufferVisualEffect = GRenderResourceMgr->CreateRenderEffect();
		ShaderObject* vertexShader = GRenderResourceMgr->CreateShader(ST_VertexShader, "SurfaceVisual", SM_4_0, "VS");
		ShaderObject* pixelShader = GRenderResourceMgr->CreateShader(ST_PixelShader, "SurfaceVisual", SM_4_0, "PS");
		_GBufferVisualEffect->BindShader(vertexShader);
		_GBufferVisualEffect->BindShader(pixelShader);
		_GBufferVisualEffect->LinkShaders();

 
		DepthStencilDesc depthDesc;
		depthDesc.DepthEnable = false;
		DepthStencilState* _DepthWriteState = GRenderResourceMgr->CreateDepthStencilState(&depthDesc, 0);
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

		_DepthVisTile = SimpleTile("GBufferDepthTile", vertex, _GBufferVisualEffect);

		vertex[0].position = glm::vec3(-0.05f, -0.55f, 0.0f);
		vertex[1].position = glm::vec3(-0.45f, -0.55f, 0.0f);
		vertex[2].position = glm::vec3(-0.05f, -0.95f, 0.0f);
		vertex[3].position = glm::vec3(-0.45f, -0.95f, 0.0f);
		_ColorVisTile = SimpleTile("GBufferColorTile", vertex, _GBufferVisualEffect);

		vertex[0].position = glm::vec3(0.45f, -0.55f, 0.0f);
		vertex[1].position = glm::vec3(0.05f, -0.55f, 0.0f);
		vertex[2].position = glm::vec3(0.45f, -0.95f, 0.0f);
		vertex[3].position = glm::vec3(0.05f, -0.95f, 0.0f);
		_NormalVisTile = SimpleTile("GBufferNormalTile", vertex, _GBufferVisualEffect);

		vertex[0].position = glm::vec3(0.95f, -0.55f, 0.0f);
		vertex[1].position = glm::vec3(0.55f, -0.55f, 0.0f);
		vertex[2].position = glm::vec3(0.95f, -0.95f, 0.0f);
		vertex[3].position = glm::vec3(0.55f, -0.95f, 0.0f);
		_SpecPowVisTile = SimpleTile("GBufferSpeclTile", vertex, _GBufferVisualEffect);

	}



	void RenderGBuffer::UpdateShaderProperty()
	{
		_GBufferDepthTexture->SetData(DepthShaderView);
		_GBufferColorSpecIntTexture->SetData(BasicColorShaderView);
		_GBufferNormalTexture->SetData(NormalDataShaderView);
		_GBufferSpecPowTexture->SetData(SpecularDataShaderView);
	}

	void RenderGBuffer::DebugVisual()
	{
		if (!GSceneManager->EnableDebugDraw)
			return;

		Camera* mainCamera = GSceneManager->GetDefaultCamera();

		//_GBufferVisTex->SetData(DepthStencilBuffer);
		//_DepthVisTile.Render(mainCamera);
		_SurfaceVisTex->SetData(BasicColorShaderView);
		_ColorVisTile.Render(mainCamera);
		_SurfaceVisTex->SetData(NormalDataShaderView);
		_NormalVisTile.Render(mainCamera);
		_SurfaceVisTex->SetData(SpecularDataShaderView);
		_SpecPowVisTile.Render(mainCamera);
	}
	
	RenderSurfaceCache *GRenderSurface;

	RenderSurfaceCache::~RenderSurfaceCache()
	{
		delete ShadowMapBuffer;
		ShadowMapBuffer = NULL;

		delete MainTarget;
		MainTarget = NULL;

		delete GBuffer;
		GBuffer = NULL;
	}

	RenderSurfaceCache::RenderSurfaceCache()
	{
		unsigned int shadowSize = GConfig->pRenderConfig->ShadowMapSize;
		ShadowMapBuffer = new ShadowMap(shadowSize, shadowSize);
	}
}
