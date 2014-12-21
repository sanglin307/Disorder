#include "EngineInclude.h"

namespace Disorder
{
 
	FXAA::FXAA()
	{
		ShaderPropertyManager* globalMgr = GRenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerGlobal);

		_ScreenAATexture = globalMgr->CreateProperty(ShaderPropertyManager::sScreenAATexture, eSP_ShaderResource, 1);
		_ScreenAASampler = globalMgr->CreateProperty(ShaderPropertyManager::sScreenAASampler, eSP_SampleState, 1);

		_AAPrepareEffect = GRenderResourceMgr->CreateRenderEffect();
		ShaderObject* vertexShader = GRenderResourceMgr->CreateShader(ST_VertexShader, "FXAA", SM_4_0, "VS");
		ShaderObject* pixelShader = GRenderResourceMgr->CreateShader(ST_PixelShader, "FXAA", SM_4_0, "PrepareSurfacePS");
		_AAPrepareEffect->BindShader(vertexShader);
		_AAPrepareEffect->BindShader(pixelShader);
		_AAPrepareEffect->LinkShaders();

		_AAEffect = GRenderResourceMgr->CreateRenderEffect();
		vertexShader = GRenderResourceMgr->CreateShader(ST_VertexShader, "FXAA", SM_4_0, "VS");
		pixelShader = GRenderResourceMgr->CreateShader(ST_PixelShader, "FXAA", SM_4_0, "PS");
		_AAEffect->BindShader(vertexShader);
		_AAEffect->BindShader(pixelShader);
		_AAEffect->LinkShaders();

		SamplerDesc sDesc;
		sDesc.Filter = SF_Min_Mag_Mip_Linear;
		SamplerState* sState = GRenderResourceMgr->CreateSamplerState(&sDesc);
		_ScreenAASampler->SetData(sState);

		TileTexVertex vertex[4];
		vertex[0].position = glm::vec3(-1.f, 1.f, 0.f);
		vertex[0].texcoord = glm::vec2(0.f, 0.f); 
		vertex[1].position = glm::vec3(-1.0f, -1.0f, 0.0f);
		vertex[1].texcoord = glm::vec2(0.f, 1.f);
		vertex[2].position = glm::vec3(1.0f, 1.0f, 0.0f);
		vertex[2].texcoord = glm::vec2(1.f, 0.0f);
		vertex[3].position = glm::vec3(1.0f, -1.0f, 0.0f);
		vertex[3].texcoord = glm::vec2(1.0f, 1.0f);

		_ScreenTile = SimpleTile("FXAATile", vertex, _AAEffect);

		_PrepareBufferTexture = GRenderResourceMgr->CreateTexture2D("AATex0",NULL, GConfig->pRenderConfig->ColorFormat, GConfig->pRenderConfig->SizeX,
			GConfig->pRenderConfig->SizeY, false, false, SV_RenderTarget | SV_ShaderResource, 1, NULL, 0);
		_PrepareShaderResource = GRenderResourceMgr->CreateSurfaceView(SV_ShaderResource, _PrepareBufferTexture, GConfig->pRenderConfig->ColorFormat);
		SurfaceView* svPrepareRes = GRenderResourceMgr->CreateSurfaceView(SV_RenderTarget, _PrepareBufferTexture, GConfig->pRenderConfig->ColorFormat);
		std::map<ESurfaceLocation, SurfaceView*> viewMap;
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceView*>(SL_RenderTarget1, svPrepareRes));
		_PrepareBufferSurface = GRenderResourceMgr->CreateRenderSurface(viewMap);


		viewMap.clear();
		_TargetBufferTexture = GRenderResourceMgr->CreateTexture2D("AATex1",NULL, GConfig->pRenderConfig->ColorFormat, GConfig->pRenderConfig->SizeX,
			GConfig->pRenderConfig->SizeY, false, false, SV_RenderTarget | SV_ShaderResource, 1, NULL, 0);
		_TargetShaderResource = GRenderResourceMgr->CreateSurfaceView(SV_ShaderResource, _TargetBufferTexture, GConfig->pRenderConfig->ColorFormat);
		SurfaceView* svTargetRes = GRenderResourceMgr->CreateSurfaceView(SV_RenderTarget, _TargetBufferTexture, GConfig->pRenderConfig->ColorFormat); 
		viewMap.insert(std::pair<ESurfaceLocation, SurfaceView*>(SL_RenderTarget1, svTargetRes));
		_TargetBufferSurface = GRenderResourceMgr->CreateRenderSurface(viewMap);

	}

	void FXAA::Render(Camera* camera,SurfaceView* aaRenderView)
	{
		GCanvas->DrawString(5, 63, "FXAA: On");

		//prepare surface first
		GRenderEngine->SetRenderTarget(_PrepareBufferSurface);
		GRenderEngine->ClearRenderSurface(_PrepareBufferSurface, glm::vec4(0, 0, 0, 1.f), false, 0, false, 0);
		_ScreenAATexture->SetData(aaRenderView);
		_ScreenTile.SetRenderEffect(_AAPrepareEffect);
		_ScreenTile.Render(camera);

		// AA pass
		GRenderEngine->SetRenderTarget(_TargetBufferSurface);
		GRenderEngine->ClearRenderSurface(_TargetBufferSurface, glm::vec4(0, 0, 0, 1.f), false, 0, false, 0);
		_ScreenAATexture->SetData(_PrepareShaderResource);
		_ScreenTile.SetRenderEffect(_AAEffect);
		_ScreenTile.Render(camera);

		RenderTexture2D* dstTexture = (RenderTexture2D*)GRenderSurface->MainTarget->RenderTargetView->Resource;
		GRenderEngine->CopyTexture2D(_TargetBufferTexture, dstTexture);
	}
}