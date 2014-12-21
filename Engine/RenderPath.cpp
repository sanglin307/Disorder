#include "EngineInclude.h"
 
namespace Disorder
{
	RenderPath::RenderPath()
	{
		_DirectionLightPropertyManager = GRenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerDirectionLight);
		_DirectionLightIntensityProperty = _DirectionLightPropertyManager->CreateProperty(ShaderPropertyManager::sDirectionLightIntensity,eSP_Float,1);
		_DirectionLightDirProperty = _DirectionLightPropertyManager->CreateProperty(ShaderPropertyManager::sDirectionLightDir,eSP_Float,3);
		_DirectionLightColorProperty = _DirectionLightPropertyManager->CreateProperty(ShaderPropertyManager::sDirectionLightColor,eSP_Float,3);
		_DirectionLightCastShadowProperty = _DirectionLightPropertyManager->CreateProperty(ShaderPropertyManager::sDirectionLightCastShadow, eSP_Bool,1);

		_PointLightPropertyManager = GRenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerPointLight);
		_PointLightPosProperty = _PointLightPropertyManager->CreateProperty(ShaderPropertyManager::sPointLightPos,eSP_Float,3);
		_PointLightColorProperty = _PointLightPropertyManager->CreateProperty(ShaderPropertyManager::sPointLightColor, eSP_Float, 3);
		_PointLightRangeRcpProperty = _PointLightPropertyManager->CreateProperty(ShaderPropertyManager::sPointLightRangeRcp, eSP_Float, 1);
		_PointLightIntensityProperty = _PointLightPropertyManager->CreateProperty(ShaderPropertyManager::sPointLightIntensity, eSP_Float, 1);
		_PointLightCastShadowProperty = _PointLightPropertyManager->CreateProperty(ShaderPropertyManager::sPointLightCastShadow, eSP_Bool, 1);

		_SpotLightPropertyManager = GRenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerSpotLight);
		_SpotLightPosProperty = _SpotLightPropertyManager->CreateProperty(ShaderPropertyManager::sSpotLightPos,eSP_Float,3);
		_SpotLightDirProperty = _SpotLightPropertyManager->CreateProperty(ShaderPropertyManager::sSpotLightDir, eSP_Float, 3);
		_SpotLightColorProperty = _SpotLightPropertyManager->CreateProperty(ShaderPropertyManager::sSpotLightColor, eSP_Float, 3);
		_SpotLightIntensityProperty = _SpotLightPropertyManager->CreateProperty(ShaderPropertyManager::sSpotLightIntensity, eSP_Float, 1);
		_SpotLightRangeRcpProperty = _SpotLightPropertyManager->CreateProperty(ShaderPropertyManager::sSpotLightRangeRcp, eSP_Float, 1);
		_SpotLightCosOuterConeProperty = _SpotLightPropertyManager->CreateProperty(ShaderPropertyManager::sSpotLightCosOuterCone, eSP_Float, 1);
		_SpotLightCosInnerConeProperty = _SpotLightPropertyManager->CreateProperty(ShaderPropertyManager::sSpotLightCosInnerCone, eSP_Float, 1);
		_SpotLightCastShadowProperty = _SpotLightPropertyManager->CreateProperty(ShaderPropertyManager::sSpotLightCastShadow, eSP_Bool, 1);


	}

	void RenderPath::SetDirectionLight(DirectionLight* directionLight)
	{
		_DirectionLightPropertyManager->ClearShaderPropertyValue();
		_DirectionLightIntensityProperty->SetData(&(directionLight->Intensity));
		glm::vec3 dir = directionLight->GetDirection();
		_DirectionLightDirProperty->SetData(glm::value_ptr(dir));
		_DirectionLightColorProperty->SetData(glm::value_ptr(directionLight->Color));
		_DirectionLightCastShadowProperty->SetData(&(directionLight->CastShadows));

		_DirectionLightPropertyManager->UpdateShaderProperty();
	}

	void RenderPath::SetPointLight(PointLight* pointLight)
	{
		glm::vec3 pos = pointLight->GetPosition();
		_PointLightPosProperty->SetData(glm::value_ptr(pos));
		_PointLightColorProperty->SetData(glm::value_ptr(pointLight->Color));
		float rangeRcp = 1.0f / pointLight->Range;
		_PointLightRangeRcpProperty->SetData(&rangeRcp);
		_PointLightIntensityProperty->SetData(&pointLight->Intensity);
		_PointLightCastShadowProperty->SetData(&(pointLight->CastShadows));
		_PointLightPropertyManager->UpdateShaderProperty();
	}

	void RenderPath::SetSpotLight(SpotLight* spotLight)
	{
		glm::vec3 spotPos = spotLight->GetPosition();
		glm::vec3 spotDir = spotLight->GetDirection();

		float rangeRcp = 1.0f / spotLight->Range;
		float inner = Math::Cosf(spotLight->SpotInnerAngle);
		float outerCone = Math::Cosf(spotLight->SpotOuterAngle);

		_SpotLightPosProperty->SetData(glm::value_ptr(spotPos));
		_SpotLightDirProperty->SetData(glm::value_ptr(spotDir));
		_SpotLightColorProperty->SetData(glm::value_ptr(spotLight->Color));
		_SpotLightRangeRcpProperty->SetData(&rangeRcp);
		_SpotLightCosOuterConeProperty->SetData(&outerCone);
		_SpotLightCosInnerConeProperty->SetData(&inner);
		_SpotLightIntensityProperty->SetData(&spotLight->Intensity);
		_SpotLightCastShadowProperty->SetData(&(spotLight->CastShadows));
		_SpotLightPropertyManager->UpdateShaderProperty();

	}

	 

	//////////////////////////////////////////////////////////////////////////////////////////
 
	void ForwardRenderPath::RenderLights(Camera* camera, const std::vector<GeometryRenderer*>& renderList)
	{
		const std::vector<Light*>& lightList = GSceneManager->GetLightsList();
		if (lightList.size() == 0)
			return;
 
		std::vector<GeometryRenderer*> allGeometryList;
		GSceneManager->GetRendererList(allGeometryList);

		for (size_t iLight = 0; iLight < lightList.size(); iLight++)
		{
			Light* light = lightList[iLight];

			// render shadow
			if (light->CastShadows)
			{
				light->CalculateShadowMatrix();
				// begin render depth
				GRenderSurface->ShadowMapBuffer->RenderDepth(camera, allGeometryList, light);

				// render object
				GRenderEngine->SetRenderTarget(GRenderSurface->MainTarget->RenderTargetSurface);
				GRenderEngine->SetViewport((float)GConfig->pRenderConfig->SizeX, (float)GConfig->pRenderConfig->SizeY, 0.f, 1.f, 0.f, 0.f);
			}

			GRenderSurface->ShadowMapBuffer->PrepareRenderLight(light); 
			if (light->LightType == LT_Directional)
			{
				DirectionLight* dirLight = (DirectionLight*)light;
				SetDirectionLight(dirLight);
			}
			else if (light->LightType == LT_Point)
			{
				PointLight* pointLight = (PointLight*)light;
				SetPointLight(pointLight);
			}
			else if (light->LightType == LT_Spot)
			{
				SpotLight* spotLight = (SpotLight*)light;
				SetSpotLight(spotLight);
			}

			for (unsigned int iObj = 0; iObj < renderList.size(); iObj++)
			{
				GeometryRenderer* obj = renderList[iObj];
				if (!light->Touch(obj))
					continue;
				obj->UpdateShaderProperty();
				if (light->LightType == LT_Directional)
					obj->SetRenderEffect(_DirectionLightEffect);
				else if (light->LightType == LT_Point)
					obj->SetRenderEffect(_PointLightEffect);
				else if (light->LightType == LT_Spot)
					obj->SetRenderEffect(_SpotLightEffect);

				obj->Render(camera);
 
			}
		}
 
	}

	void ForwardRenderPath::BasePassRender(Camera* camera, const std::vector<GeometryRenderer*>& renderList)
	{
		if (renderList.size() == 0)
			return;

		for (unsigned int i = 0; i< renderList.size(); i++)
		{
			GeometryRenderer* obj = renderList[i];
			obj->BuildRenderLayout(_BasePassEffect, false);
			obj->UpdateShaderProperty();
 
			obj->SetRenderEffect(_BasePassEffect);
			obj->Render(camera);
		}
	}

	void ForwardRenderPath::Render()
	{
		Camera* mainCamera = GSceneManager->GetDefaultCamera();

		if( mainCamera == NULL )
			return;

		GSceneManager->UpdateBoundingBox();

		GRenderEngine->OnFrameBegin();

		GRenderEngine->SetRenderTarget(GRenderSurface->MainTarget->RenderTargetSurface);
		GRenderEngine->ClearRenderSurface(GRenderSurface->MainTarget->RenderTargetSurface, glm::vec4(0.f, 0.f, 0.f, 1.0f), true, 1.0f, false, 0);
 
		GSceneManager->UpdateShaderProperty();
		mainCamera->UpdateShaderProperty();
 
		// skybox
		GSceneManager->GetSkybox()->Render();

		std::vector<GeometryRenderer*> rendererList;
		GSceneManager->GetRendererList(mainCamera,rendererList);

		// base pass for ambient light and diffuse texture etc
		BasePassRender(mainCamera, rendererList);
		RenderLights(mainCamera, rendererList);
 

		GCanvas->DrawString(5, 35, "Forward Lighting Mode");
		GSceneManager->DebugDraw();
		GEngine->Stat.DrawStat();
		GCanvas->RenderLines(mainCamera);

		// postprocess not include the string renders.
		if (GConfig->pRenderConfig->FXAA)
		{
			_aaRender->Render(mainCamera, GRenderSurface->MainTarget->RenderTargetShaderView);
			GRenderEngine->SetEffect(NULL);
			GRenderEngine->SetRenderTarget(GRenderSurface->MainTarget->RenderTargetSurface);
		}
		// the last ... render string.
		GCanvas->RenderStrings(mainCamera);

		GRenderEngine->OnFrameEnd();
	}
 
	ForwardRenderPath::ForwardRenderPath()
	{
		_type = RPT_ForwardLighting;

	 
		_BasePassEffect = GRenderResourceMgr->CreateRenderEffect();
		ShaderObject* vertexShader = GRenderResourceMgr->CreateShader(ST_VertexShader, "ForwardLighting", SM_4_0, "SceneVS");
		ShaderObject* pixelShader = GRenderResourceMgr->CreateShader(ST_PixelShader, "ForwardLighting", SM_4_0, "BasePassPS");
		 
		_BasePassEffect->BindShader(vertexShader);
		_BasePassEffect->BindShader(pixelShader);
		_BasePassEffect->LinkShaders();

		_DirectionLightEffect = GRenderResourceMgr->CreateRenderEffect();
		ShaderObject* dvertexShader = GRenderResourceMgr->CreateShader(ST_VertexShader,"ForwardLighting",SM_4_0,"SceneVS");
		ShaderObject* dpixelShader = GRenderResourceMgr->CreateShader(ST_PixelShader,"ForwardLighting",SM_4_0,"DirectionLightingPS");
		_DirectionLightEffect->BindShader(dvertexShader);
		_DirectionLightEffect->BindShader(dpixelShader);
		_DirectionLightEffect->LinkShaders();
 
		_PointLightEffect = GRenderResourceMgr->CreateRenderEffect();
		ShaderObject* pVertexShader = GRenderResourceMgr->CreateShader(ST_VertexShader,"ForwardLighting",SM_4_0,"SceneVS");
		ShaderObject* pPixelShader = GRenderResourceMgr->CreateShader(ST_PixelShader,"ForwardLighting",SM_4_0,"PointLightingPS");
		_PointLightEffect->BindShader(pVertexShader);
		_PointLightEffect->BindShader(pPixelShader);
		_PointLightEffect->LinkShaders();

		_SpotLightEffect = GRenderResourceMgr->CreateRenderEffect();
		ShaderObject* sVertexShader = GRenderResourceMgr->CreateShader(ST_VertexShader, "ForwardLighting", SM_4_0, "SceneVS");
		ShaderObject* sPixelShader = GRenderResourceMgr->CreateShader(ST_PixelShader, "ForwardLighting", SM_4_0, "SpotLightingPS");
		_SpotLightEffect->BindShader(sVertexShader);
		_SpotLightEffect->BindShader(sPixelShader);
		_SpotLightEffect->LinkShaders();

		BlendDesc bDesc;
		bDesc.BlendEnable = true;
		bDesc.BlendOp = BLEND_OP_ADD;
		bDesc.SrcBlend = BLEND_ONE;
		bDesc.DestBlend = BLEND_ONE;
		bDesc.BlendOpAlpha = BLEND_OP_ADD;
		bDesc.SrcBlendAlpha = BLEND_ONE;
		bDesc.DestBlendAlpha = BLEND_ONE;
		BlendState* blendState = GRenderResourceMgr->CreateBlendState(&bDesc,1);
		_DirectionLightEffect->BindBlendState(blendState);
		_PointLightEffect->BindBlendState(blendState);
		_SpotLightEffect->BindBlendState(blendState);
 
		DepthStencilDesc dsDesc;
		dsDesc.DepthFunc = CF_Less_Equal;
		dsDesc.DepthWrite = false;
		DepthStencilState* noDepthState = GRenderResourceMgr->CreateDepthStencilState(&dsDesc,0);
		_DirectionLightEffect->BindDepthStencilState(noDepthState);
		_PointLightEffect->BindDepthStencilState(noDepthState);
		_SpotLightEffect->BindDepthStencilState(noDepthState);
 
		_aaRender = new FXAA;
 
	}

	////////////////////////////////////////////////////////////////////////////////////////
 

	DeferredShading::DeferredShading()
	{
		_type = RPT_DeferredShading;

	 
		_RenderSceneEffect = new RenderEffect;
		ShaderObject* vertexShader = GRenderResourceMgr->CreateShader(ST_VertexShader,"DeferredShading",SM_4_0,"SceneVS");
		ShaderObject* pixelShader = GRenderResourceMgr->CreateShader(ST_PixelShader,"DeferredShading",SM_4_0,"ScenePS");
		_RenderSceneEffect->BindShader(vertexShader);
		_RenderSceneEffect->BindShader(pixelShader);
		_RenderSceneEffect->LinkShaders();

		DepthStencilDesc depthDesc;
		depthDesc.DepthEnable = true;
		depthDesc.DepthWrite = true;
		depthDesc.DepthFunc = CF_Less;
		depthDesc.StencilEnable = true;
		depthDesc.BackFaceStencilDepthFailOp = depthDesc.FrontFaceStencilDepthFailOp = STENCIL_OP_REPLACE;
		depthDesc.BackFaceStencilFailOp = depthDesc.FrontFaceStencilFailOp = STENCIL_OP_REPLACE;
		depthDesc.BackFaceStencilPassOp = depthDesc.FrontFaceStencilPassOp = STENCIL_OP_REPLACE;
		depthDesc.BackFaceStencilFunc = depthDesc.BackFaceStencilFunc = CF_Always;
		DepthStencilState* _DepthWriteState = GRenderResourceMgr->CreateDepthStencilState(&depthDesc,0);
		_RenderSceneEffect->BindDepthStencilState(_DepthWriteState);

		_BasePassEffect = GRenderResourceMgr->CreateRenderEffect();
	     vertexShader = GRenderResourceMgr->CreateShader(ST_VertexShader,"DeferredShading",SM_4_0,"LightingVS");
		 pixelShader = GRenderResourceMgr->CreateShader(ST_PixelShader,"DeferredShading",SM_4_0,"BasePassPS");
		 _BasePassEffect->BindShader(vertexShader);
		 _BasePassEffect->BindShader(pixelShader);
		 _BasePassEffect->LinkShaders();

		 _DirectionLightEffect = GRenderResourceMgr->CreateRenderEffect();
		 vertexShader = GRenderResourceMgr->CreateShader(ST_VertexShader, "DeferredShading", SM_4_0, "LightingVS");
		 pixelShader = GRenderResourceMgr->CreateShader(ST_PixelShader, "DeferredShading", SM_4_0, "DirectionLightingPS");
		 _DirectionLightEffect->BindShader(vertexShader);
		 _DirectionLightEffect->BindShader(pixelShader);
		 _DirectionLightEffect->LinkShaders();

		 _PointLightEffect = GRenderResourceMgr->CreateRenderEffect();
		 vertexShader = GRenderResourceMgr->CreateShader(ST_VertexShader, "DeferredShading", SM_4_0, "LightingVS");
		 pixelShader = GRenderResourceMgr->CreateShader(ST_PixelShader, "DeferredShading", SM_4_0, "PointLightingPS");
		 _PointLightEffect->BindShader(vertexShader);
		 _PointLightEffect->BindShader(pixelShader);
		 _PointLightEffect->LinkShaders();

		 _SpotLightEffect = GRenderResourceMgr->CreateRenderEffect();
		 vertexShader = GRenderResourceMgr->CreateShader(ST_VertexShader, "DeferredShading", SM_4_0, "LightingVS");
		 pixelShader = GRenderResourceMgr->CreateShader(ST_PixelShader, "DeferredShading", SM_4_0, "SpotLightingPS");
		 _SpotLightEffect->BindShader(vertexShader);
		 _SpotLightEffect->BindShader(pixelShader);
		 _SpotLightEffect->LinkShaders();

		 
		DepthStencilDesc nodepthWriteDesc;
		nodepthWriteDesc.DepthEnable = true;
		nodepthWriteDesc.DepthWrite = false;
		nodepthWriteDesc.DepthFunc = CF_Less;
		nodepthWriteDesc.StencilEnable = true;
		nodepthWriteDesc.BackFaceStencilDepthFailOp = nodepthWriteDesc.FrontFaceStencilDepthFailOp = STENCIL_OP_KEEP;
		nodepthWriteDesc.BackFaceStencilFailOp = nodepthWriteDesc.FrontFaceStencilFailOp = STENCIL_OP_KEEP;
		nodepthWriteDesc.BackFaceStencilPassOp = nodepthWriteDesc.FrontFaceStencilPassOp = STENCIL_OP_KEEP;
		nodepthWriteDesc.BackFaceStencilFunc = nodepthWriteDesc.BackFaceStencilFunc = CF_Equal;
		DepthStencilState* _noDepthWriteState = GRenderResourceMgr->CreateDepthStencilState(&nodepthWriteDesc, 0);
		_BasePassEffect->BindDepthStencilState(_noDepthWriteState);
 
		BlendDesc bBaseDesc;
		bBaseDesc.BlendEnable = true;
		bBaseDesc.BlendOp = BLEND_OP_ADD;
		bBaseDesc.SrcBlend = BLEND_ONE;
		bBaseDesc.DestBlend = BLEND_INV_SRC_ALPHA;
		bBaseDesc.BlendOpAlpha = BLEND_OP_ADD;
		bBaseDesc.SrcBlendAlpha = BLEND_ONE;
		bBaseDesc.DestBlendAlpha = BLEND_ONE;
		BlendState* blendStateBase = GRenderResourceMgr->CreateBlendState(&bBaseDesc, 1);
		_BasePassEffect->BindBlendState(blendStateBase);

		BlendDesc bDesc;
		bDesc.BlendEnable = true;
		bDesc.BlendOp = BLEND_OP_ADD;
		bDesc.SrcBlend = BLEND_ONE;
		bDesc.DestBlend = BLEND_ONE;
		bDesc.BlendOpAlpha = BLEND_OP_ADD;
		bDesc.SrcBlendAlpha = BLEND_ONE;
		bDesc.DestBlendAlpha = BLEND_ONE;
		BlendState* blendState = GRenderResourceMgr->CreateBlendState(&bDesc,1);
		
		_DirectionLightEffect->BindBlendState(blendState);
		_PointLightEffect->BindBlendState(blendState);
		_SpotLightEffect->BindBlendState(blendState);
 
		DepthStencilDesc dsDesc;
		dsDesc.DepthFunc = CF_Less_Equal;
		DepthStencilState* noDepthState = GRenderResourceMgr->CreateDepthStencilState(&dsDesc,0);
		_DirectionLightEffect->BindDepthStencilState(noDepthState);
		_PointLightEffect->BindDepthStencilState(noDepthState);
		_SpotLightEffect->BindDepthStencilState(noDepthState);


		TileTexVertex vertex[4];
		vertex[0].position = glm::vec3(-1.f, 1.f, 0.f);
		vertex[0].texcoord = glm::vec2(0.f, 1.f); // used for openGL, not used for dx.
		vertex[1].position = glm::vec3(-1.0f, -1.0f, 0.0f);
		vertex[1].texcoord = glm::vec2(0.f, 0.f);
		vertex[2].position = glm::vec3(1.0f, 1.0f, 0.0f);
		vertex[2].texcoord = glm::vec2(1.f, 1.0f);
		vertex[3].position = glm::vec3(1.0f, -1.0f, 0.0f);
		vertex[3].texcoord = glm::vec2(1.0f, 0.0f);
 
	
		_LightingTile = SimpleTile("DeferLightingTile", vertex, _BasePassEffect);
 
		GRenderSurface->GBuffer = new RenderGBuffer(GConfig->pRenderConfig->SizeX,GConfig->pRenderConfig->SizeY);

		_aaRender = new FXAA;
	}

	void DeferredShading::RenderLights(Camera* camera, const std::vector<GeometryRenderer*>& renderList)
	{
		const std::vector<Light*>& lightList = GSceneManager->GetLightsList();
		if (lightList.size() == 0)
			return;

		std::vector<GeometryRenderer*> allGeometryList;
		GSceneManager->GetRendererList(allGeometryList);

		for (size_t i = 0; i < lightList.size(); i++)
		{
			Light* light = lightList[i];

			// render shadow
			if (light->CastShadows)
			{
				light->CalculateShadowMatrix();
				// begin render depth
				GRenderSurface->ShadowMapBuffer->RenderDepth(camera, allGeometryList, light);

				// render object
				GRenderEngine->SetRenderTarget(GRenderSurface->MainTarget->RenderTargetSurface);
				GRenderEngine->SetViewport((float)GConfig->pRenderConfig->SizeX, (float)GConfig->pRenderConfig->SizeY, 0.f, 1.f, 0.f, 0.f);
			}
			

			GRenderSurface->ShadowMapBuffer->PrepareRenderLight(light);

			if (light->LightType == LT_Directional)
			{
				DirectionLight* dirLight = (DirectionLight*)light;
				SetDirectionLight(dirLight);
				_LightingTile.SetRenderEffect(_DirectionLightEffect);
				_LightingTile.Render(camera);
			}
			else if (light->LightType == LT_Point)
			{
				PointLight* pointLight = (PointLight*)light;
				SetPointLight(pointLight);
				_LightingTile.SetRenderEffect(_PointLightEffect);
				_LightingTile.Render(camera);
			}
			else if (light->LightType == LT_Spot)
			{
				SpotLight* spotLight =  (SpotLight*)light;
				SetSpotLight(spotLight);
				_LightingTile.SetRenderEffect(_SpotLightEffect);
				_LightingTile.Render(camera);
			}
 
		}
	}

	void DeferredShading::Render()
	{
		Camera* mainCamera = GSceneManager->GetDefaultCamera();

		if( mainCamera == NULL )
			return;

		GSceneManager->UpdateBoundingBox();
		GRenderEngine->OnFrameBegin();

		GSceneManager->UpdateShaderProperty();
		mainCamera->UpdateShaderProperty();

		GRenderEngine->SetRenderTarget(GRenderSurface->MainTarget->RenderTargetSurface);
		GRenderEngine->ClearRenderSurface(GRenderSurface->MainTarget->RenderTargetSurface, glm::vec4(0.f, 0.f, 0.f, 1.0f), true, 1.0f, false, 0);
		
		GSceneManager->GetSkybox()->Render();

		// draw to GBuffer
		RenderSurface* renderTargetPtr = GRenderSurface->GBuffer->RenderTargetBuffer;
		GRenderEngine->SetRenderTarget(renderTargetPtr);
		GRenderEngine->ClearRenderSurface(renderTargetPtr, glm::vec4(0.f),true, 1.f, true, 0);
 
		std::vector<GeometryRenderer*> rendererList;
		GSceneManager->GetRendererList(mainCamera, rendererList);
		
		RenderScene(mainCamera,rendererList);

		GRenderSurface->GBuffer->UpdateShaderProperty();
	
		//lighting pass, draw to main render target
		GRenderEngine->SetRenderTarget(GRenderSurface->MainTarget->RenderTargetSurface);

		//base pass light
		_LightingTile.SetRenderEffect(_BasePassEffect);
		_LightingTile.Render(mainCamera);

		RenderLights(mainCamera, rendererList);
 
		// the last hud things, must use GBuffer's depth buffer surface view
		GRenderEngine->SetEffect(NULL);
		GRenderEngine->SetRenderTarget(GRenderSurface->GBuffer->MainTargetGDepth, true);
		GCanvas->DrawString(5,35,"Deferred Shading Mode");
		GSceneManager->DebugDraw();
		GEngine->Stat.DrawStat();

		GCanvas->RenderLines(mainCamera);
 

		// postprocess not include the string renders.
		if (GConfig->pRenderConfig->FXAA)
		{
			_aaRender->Render(mainCamera, GRenderSurface->MainTarget->RenderTargetShaderView);
			GRenderEngine->SetEffect(NULL);
			GRenderEngine->SetRenderTarget(GRenderSurface->GBuffer->MainTargetGDepth);
		}
		// the last ... render string.

		GCanvas->RenderStrings(mainCamera);

		GRenderSurface->GBuffer->DebugVisual();

		GRenderEngine->SetEffect(NULL);
		GRenderEngine->OnFrameEnd();
	}

	void DeferredShading::RenderScene(Camera* mainCamera,const std::vector<GeometryRenderer*>& rendererList)
	{	
		for(size_t i=0;i< rendererList.size(); i++ )
		{
			GeometryRenderer* obj = rendererList[i];
			obj->BuildRenderLayout(_RenderSceneEffect,false);
			obj->UpdateShaderProperty();
			obj->SetRenderEffect(_RenderSceneEffect);
			obj->Render(mainCamera);
			 
		}
	}
 
}