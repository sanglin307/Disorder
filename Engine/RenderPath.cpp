#include "EngineInclude.h"
 
namespace Disorder
{
	RenderPath::RenderPath()
	{
		_DirectionLightPropertyManager = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerDirectionLight);
		_DirectionLightIntensityProperty = _DirectionLightPropertyManager->CreateProperty(ShaderPropertyManager::sDirectionLightIntensity,eSP_Float,1);
		_DirectionLightDirProperty = _DirectionLightPropertyManager->CreateProperty(ShaderPropertyManager::sDirectionLightDir,eSP_Float,3);
		_DirectionLightColorProperty = _DirectionLightPropertyManager->CreateProperty(ShaderPropertyManager::sDirectionLightColor,eSP_Float,3);

		_PointLightPropertyManager = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerPointLight);
		_PointLightPosProperty = _PointLightPropertyManager->CreateProperty(ShaderPropertyManager::sPointLightPos,eSP_Float,3);
		_PointLightColorProperty = _PointLightPropertyManager->CreateProperty(ShaderPropertyManager::sPointLightColor, eSP_Float, 3);
		_PointLightRangeRcpProperty = _PointLightPropertyManager->CreateProperty(ShaderPropertyManager::sPointLightRangeRcp, eSP_Float, 1);
		_PointLightIntensityProperty = _PointLightPropertyManager->CreateProperty(ShaderPropertyManager::sPointLightIntensity, eSP_Float, 1);

		_SpotLightPropertyManager = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerSpotLight);
		_SpotLightPosProperty = _SpotLightPropertyManager->CreateProperty(ShaderPropertyManager::sSpotLightPos,eSP_Float,3);
		_SpotLightDirProperty = _SpotLightPropertyManager->CreateProperty(ShaderPropertyManager::sSpotLightDir, eSP_Float, 3);
		_SpotLightColorProperty = _SpotLightPropertyManager->CreateProperty(ShaderPropertyManager::sSpotLightColor, eSP_Float, 3);
		_SpotLightIntensityProperty = _SpotLightPropertyManager->CreateProperty(ShaderPropertyManager::sSpotLightIntensity, eSP_Float, 1);
		_SpotLightRangeRcpProperty = _SpotLightPropertyManager->CreateProperty(ShaderPropertyManager::sSpotLightRangeRcp, eSP_Float, 1);
		_SpotLightCosOuterConeProperty = _SpotLightPropertyManager->CreateProperty(ShaderPropertyManager::sSpotLightCosOuterCone, eSP_Float, 1);
		_SpotLightCosInnerConeRcpProperty = _SpotLightPropertyManager->CreateProperty(ShaderPropertyManager::sSpotLightCosInnerConeRcp, eSP_Float, 1);


	}

	void RenderPath::SetDirectionLight(const DirectionLightPtr& directionLight)
	{
		_DirectionLightPropertyManager->ClearShaderPropertyValue();
		_DirectionLightIntensityProperty->SetData(&(directionLight->Intensity));
		glm::vec3 dir = directionLight->GetDirection();
		_DirectionLightDirProperty->SetData(glm::value_ptr(dir));
		_DirectionLightColorProperty->SetData(glm::value_ptr(directionLight->Color));

		_DirectionLightPropertyManager->UpdateShaderProperty();
	}

	void RenderPath::SetPointLight(const PointLightPtr& pointLight)
	{
		glm::vec3 pos = pointLight->GetPosition();
		_PointLightPosProperty->SetData(glm::value_ptr(pos));
		_PointLightColorProperty->SetData(glm::value_ptr(pointLight->Color));
		float rangeRcp = 1.0f / pointLight->Range;
		_PointLightRangeRcpProperty->SetData(&rangeRcp);
		_PointLightIntensityProperty->SetData(&pointLight->Intensity);
		_PointLightPropertyManager->UpdateShaderProperty();
	}

	void RenderPath::SetSpotLight(const SpotLightPtr& spotLight)
	{
		glm::vec3 spotPos = spotLight->GetPosition();
		glm::vec3 spotDir = spotLight->GetDirection();

		float rangeRcp = 1.0f / spotLight->Range;
		float innerRcp = 1.0f / Math::Cosf(spotLight->SpotInnerAngle);
		float outerCone = Math::Cosf(spotLight->SpotOuterAngle);

		_SpotLightPosProperty->SetData(glm::value_ptr(spotPos));
		_SpotLightDirProperty->SetData(glm::value_ptr(spotDir));
		_SpotLightColorProperty->SetData(glm::value_ptr(spotLight->Color));
		_SpotLightRangeRcpProperty->SetData(&rangeRcp);
		_SpotLightCosOuterConeProperty->SetData(&outerCone);
		_SpotLightCosInnerConeRcpProperty->SetData(&innerRcp);
		_SpotLightIntensityProperty->SetData(&spotLight->Intensity);
		_SpotLightPropertyManager->UpdateShaderProperty();

	}

	 

	//////////////////////////////////////////////////////////////////////////////////////////
 
	void ForwardRenderPath::RenderLights(const CameraPtr& camera, const std::vector<GeometryRendererPtr>& renderList)
	{
		const std::vector<LightPtr>& lightList = GSceneManager->GetLightsList();
		if (lightList.size() == 0)
			return;
 
		std::vector<GeometryRendererPtr> allGeometryList;
		GSceneManager->GetRendererList(allGeometryList);

		for (size_t i = 0; i < lightList.size(); i++)
		{
			LightPtr light = lightList[i];

			// render shadow
			if (!light->CastShadows)
				continue;

			light->CalculateShadowMatrix();
			// begin render depth
			GEngine->RenderSurfaceCache->ShadowMapBuffer->RenderDepth(camera,allGeometryList,light);	

			// render object
			GEngine->RenderEngine->SetRenderTarget(GEngine->RenderSurfaceCache->MainTarget->RenderTargetSurface);
			GEngine->RenderEngine->SetViewport((float)GConfig->pRenderConfig->SizeX, (float)GConfig->pRenderConfig->SizeY, 0.f, 1.f, 0.f, 0.f);
			GEngine->RenderSurfaceCache->ShadowMapBuffer->PrepareRenderLight(light);

			if (light->LightType == LT_Directional)
			{
				DirectionLightPtr dirLight = boost::dynamic_pointer_cast<DirectionLight>(light);
				SetDirectionLight(dirLight);
			}
			else if (light->LightType == LT_Point)
			{
				PointLightPtr pointLight = boost::dynamic_pointer_cast<PointLight>(light);
				SetPointLight(pointLight);
			}
			else if (light->LightType == LT_Spot)
			{
				SpotLightPtr spotLight = boost::dynamic_pointer_cast<SpotLight>(light);
				SetSpotLight(spotLight);
			}

			for (unsigned int i = 0; i < renderList.size(); i++)
			{
				GeometryRendererPtr obj = renderList[i];
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

	void ForwardRenderPath::BasePassRender(const CameraPtr& camera, const std::vector<GeometryRendererPtr>& renderList)
	{
		if (renderList.size() == 0)
			return;

		for (unsigned int i = 0; i< renderList.size(); i++)
		{
			GeometryRendererPtr obj = renderList[i];
			obj->BuildRenderLayout(_BasePassEffect, false);
			obj->UpdateShaderProperty();
 
			obj->SetRenderEffect(_BasePassEffect);
			obj->Render(camera);
		}
	}

	void ForwardRenderPath::Render()
	{
		CameraPtr mainCamera = GSceneManager->GetDefaultCamera();

		if( mainCamera == NULL )
			return;

		GSceneManager->UpdateBoundingBox();

		GEngine->RenderEngine->OnFrameBegin();

		GEngine->RenderEngine->SetRenderTarget(GEngine->RenderSurfaceCache->MainTarget->RenderTargetSurface);
		GEngine->RenderEngine->ClearRenderSurface(GEngine->RenderSurfaceCache->MainTarget->RenderTargetSurface, glm::vec4(0.f, 0.f, 0.f, 1.0f), true, 1.0f, false, 0);
 
		GSceneManager->UpdateShaderProperty();
		mainCamera->UpdateShaderProperty();
 
		// skybox
		GSceneManager->GetSkybox()->Render();

		std::vector<GeometryRendererPtr> rendererList;
		GSceneManager->GetRendererList(mainCamera,rendererList);

		// base pass for ambient light and diffuse texture etc
		BasePassRender(mainCamera, rendererList);
		RenderLights(mainCamera, rendererList);
 

		GEngine->GameCanvas->DrawString(5, 35, "Forward Lighting Mode");
		GSceneManager->DebugDraw();
		GEngine->Stat.DrawStat();
		GEngine->GameCanvas->RenderLines(mainCamera);

		// postprocess not include the string renders.
		if (GConfig->pRenderConfig->FXAA)
		{
			_aaRender->Render(mainCamera, GEngine->RenderSurfaceCache->MainTarget->RenderTargetShaderView);
			GEngine->RenderEngine->SetEffect(NULL);
			GEngine->RenderEngine->SetRenderTarget(GEngine->RenderSurfaceCache->MainTarget->RenderTargetSurface);
		}
		// the last ... render string.
		GEngine->GameCanvas->RenderStrings(mainCamera);

		GEngine->RenderEngine->OnFrameEnd();
	}
 

	ForwardRenderPathPtr ForwardRenderPath::Create()
	{
		ForwardRenderPath *pPath = new ForwardRenderPath;
		return ForwardRenderPathPtr(pPath);
	}

	ForwardRenderPath::ForwardRenderPath()
	{
		_type = RPT_ForwardLighting;

	 
		_BasePassEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
		ShaderObjectPtr vertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader, "ForwardLighting", SM_4_0, "SceneVS");
		ShaderObjectPtr pixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader, "ForwardLighting", SM_4_0, "BasePassPS");
		 
		_BasePassEffect->BindShader(vertexShader);
		_BasePassEffect->BindShader(pixelShader);
		_BasePassEffect->LinkShaders();

		_DirectionLightEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
		ShaderObjectPtr dvertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader,"ForwardLighting",SM_4_0,"SceneVS");
		ShaderObjectPtr dpixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader,"ForwardLighting",SM_4_0,"DirectionLightingPS");
		_DirectionLightEffect->BindShader(dvertexShader);
		_DirectionLightEffect->BindShader(dpixelShader);
		_DirectionLightEffect->LinkShaders();
 
		_PointLightEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
		ShaderObjectPtr pVertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader,"ForwardLighting",SM_4_0,"SceneVS");
		ShaderObjectPtr pPixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader,"ForwardLighting",SM_4_0,"PointLightingPS");
		_PointLightEffect->BindShader(pVertexShader);
		_PointLightEffect->BindShader(pPixelShader);
		_PointLightEffect->LinkShaders();

		_SpotLightEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
		ShaderObjectPtr sVertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader, "ForwardLighting", SM_4_0, "SceneVS");
		ShaderObjectPtr sPixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader, "ForwardLighting", SM_4_0, "SpotLightingPS");
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
		BlendStatePtr blendState = GEngine->RenderResourceMgr->CreateBlendState(&bDesc,1);
		_DirectionLightEffect->BindBlendState(blendState);
		_PointLightEffect->BindBlendState(blendState);
		_SpotLightEffect->BindBlendState(blendState);
 
		DepthStencilDesc dsDesc;
		dsDesc.DepthFunc = CF_Less_Equal;
		dsDesc.DepthWrite = false;
		DepthStencilStatePtr noDepthState = GEngine->RenderResourceMgr->CreateDepthStencilState(&dsDesc,0);
		_DirectionLightEffect->BindDepthStencilState(noDepthState);
		_PointLightEffect->BindDepthStencilState(noDepthState);
		_SpotLightEffect->BindDepthStencilState(noDepthState);
 
		_aaRender = FXAA::Create();
 
	}

	////////////////////////////////////////////////////////////////////////////////////////

	DeferredShadingPtr DeferredShading::Create()
	{
		DeferredShading *pShading = new DeferredShading;
		return DeferredShadingPtr(pShading);
	}

	DeferredShading::DeferredShading()
	{
		_type = RPT_DeferredShading;

	 
		_RenderSceneEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
		ShaderObjectPtr vertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader,"DeferredShading",SM_4_0,"SceneVS");
		ShaderObjectPtr pixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader,"DeferredShading",SM_4_0,"ScenePS");
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
		DepthStencilStatePtr _DepthWriteState = GEngine->RenderResourceMgr->CreateDepthStencilState(&depthDesc,0);
		_RenderSceneEffect->BindDepthStencilState(_DepthWriteState);

		_BasePassEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
	     vertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader,"DeferredShading",SM_4_0,"LightingVS");
		 pixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader,"DeferredShading",SM_4_0,"BasePassPS");
		 _BasePassEffect->BindShader(vertexShader);
		 _BasePassEffect->BindShader(pixelShader);
		 _BasePassEffect->LinkShaders();

		 _DirectionLightEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
		 vertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader, "DeferredShading", SM_4_0, "LightingVS");
		 pixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader, "DeferredShading", SM_4_0, "DirectionLightingPS");
		 _DirectionLightEffect->BindShader(vertexShader);
		 _DirectionLightEffect->BindShader(pixelShader);
		 _DirectionLightEffect->LinkShaders();

		 _PointLightEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
		 vertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader, "DeferredShading", SM_4_0, "LightingVS");
		 pixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader, "DeferredShading", SM_4_0, "PointLightingPS");
		 _PointLightEffect->BindShader(vertexShader);
		 _PointLightEffect->BindShader(pixelShader);
		 _PointLightEffect->LinkShaders();

		 _SpotLightEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
		 vertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader, "DeferredShading", SM_4_0, "LightingVS");
		 pixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader, "DeferredShading", SM_4_0, "SpotLightingPS");
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
		DepthStencilStatePtr _noDepthWriteState = GEngine->RenderResourceMgr->CreateDepthStencilState(&nodepthWriteDesc, 0);
		_BasePassEffect->BindDepthStencilState(_noDepthWriteState);
 
		BlendDesc bBaseDesc;
		bBaseDesc.BlendEnable = true;
		bBaseDesc.BlendOp = BLEND_OP_ADD;
		bBaseDesc.SrcBlend = BLEND_ONE;
		bBaseDesc.DestBlend = BLEND_INV_SRC_ALPHA;
		bBaseDesc.BlendOpAlpha = BLEND_OP_ADD;
		bBaseDesc.SrcBlendAlpha = BLEND_ONE;
		bBaseDesc.DestBlendAlpha = BLEND_ONE;
		BlendStatePtr blendStateBase = GEngine->RenderResourceMgr->CreateBlendState(&bBaseDesc, 1);
		_BasePassEffect->BindBlendState(blendStateBase);

		BlendDesc bDesc;
		bDesc.BlendEnable = true;
		bDesc.BlendOp = BLEND_OP_ADD;
		bDesc.SrcBlend = BLEND_ONE;
		bDesc.DestBlend = BLEND_ONE;
		bDesc.BlendOpAlpha = BLEND_OP_ADD;
		bDesc.SrcBlendAlpha = BLEND_ONE;
		bDesc.DestBlendAlpha = BLEND_ONE;
		BlendStatePtr blendState = GEngine->RenderResourceMgr->CreateBlendState(&bDesc,1);
		
		_DirectionLightEffect->BindBlendState(blendState);
		_PointLightEffect->BindBlendState(blendState);
		_SpotLightEffect->BindBlendState(blendState);
 
		DepthStencilDesc dsDesc;
		dsDesc.DepthFunc = CF_Less_Equal;
		DepthStencilStatePtr noDepthState = GEngine->RenderResourceMgr->CreateDepthStencilState(&dsDesc,0);
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
 
		GEngine->RenderSurfaceCache->GBuffer = RenderGBuffer::Create(GConfig->pRenderConfig->SizeX,GConfig->pRenderConfig->SizeY);

		_aaRender = FXAA::Create();
	}

	void DeferredShading::RenderLights(const CameraPtr& camera, const std::vector<GeometryRendererPtr>& renderList)
	{
		const std::vector<LightPtr>& lightList = GSceneManager->GetLightsList();
		if (lightList.size() == 0)
			return;

		std::vector<GeometryRendererPtr> allGeometryList;
		GSceneManager->GetRendererList(allGeometryList);

		for (size_t i = 0; i < lightList.size(); i++)
		{
			LightPtr light = lightList[i];

			// render shadow
			if (!light->CastShadows)
				continue;

			light->CalculateShadowMatrix();
			// begin render depth
			GEngine->RenderSurfaceCache->ShadowMapBuffer->RenderDepth(camera, allGeometryList, light);

			// render object
			GEngine->RenderEngine->SetRenderTarget(GEngine->RenderSurfaceCache->MainTarget->RenderTargetSurface);
			GEngine->RenderEngine->SetViewport((float)GConfig->pRenderConfig->SizeX, (float)GConfig->pRenderConfig->SizeY, 0.f, 1.f, 0.f, 0.f);
			GEngine->RenderSurfaceCache->ShadowMapBuffer->PrepareRenderLight(light);

			if (light->LightType == LT_Directional)
			{
				DirectionLightPtr dirLight = boost::dynamic_pointer_cast<DirectionLight>(light);
				SetDirectionLight(dirLight);
				_LightingTile.SetRenderEffect(_DirectionLightEffect);
				_LightingTile.Render(camera);
			}
			else if (light->LightType == LT_Point)
			{
				PointLightPtr pointLight = boost::dynamic_pointer_cast<PointLight>(light);
				SetPointLight(pointLight);
				_LightingTile.SetRenderEffect(_PointLightEffect);
				_LightingTile.Render(camera);
			}
			else if (light->LightType == LT_Spot)
			{
				SpotLightPtr spotLight = boost::dynamic_pointer_cast<SpotLight>(light);
				SetSpotLight(spotLight);
				_LightingTile.SetRenderEffect(_SpotLightEffect);
				_LightingTile.Render(camera);
			}
 
		}
	}

	void DeferredShading::Render()
	{
		CameraPtr mainCamera = GSceneManager->GetDefaultCamera();

		if( mainCamera == NULL )
			return;

		GEngine->RenderEngine->OnFrameBegin();

		GSceneManager->UpdateShaderProperty();
		mainCamera->UpdateShaderProperty();

		GEngine->RenderEngine->SetRenderTarget(GEngine->RenderSurfaceCache->MainTarget->RenderTargetSurface);
		GEngine->RenderEngine->ClearRenderSurface(GEngine->RenderSurfaceCache->MainTarget->RenderTargetSurface, glm::vec4(0.f, 0.f, 0.f, 1.0f), true, 1.0f, false, 0);
		
		GSceneManager->GetSkybox()->Render();

		// draw to GBuffer
		RenderSurfacePtr renderTargetPtr = GEngine->RenderSurfaceCache->GBuffer->RenderTargetBuffer;
		GEngine->RenderEngine->SetRenderTarget(renderTargetPtr);
		GEngine->RenderEngine->ClearRenderSurface(renderTargetPtr, glm::vec4(0.f),true, 1.f, true, 0);
 
		std::vector<GeometryRendererPtr> rendererList;
		GSceneManager->GetRendererList(mainCamera, rendererList);
		
		RenderScene(mainCamera,rendererList);

		GEngine->RenderSurfaceCache->GBuffer->UpdateShaderProperty();
	
		//lighting pass, draw to main render target
		GEngine->RenderEngine->SetRenderTarget(GEngine->RenderSurfaceCache->MainTarget->RenderTargetSurface);

		//base pass light
		_LightingTile.SetRenderEffect(_BasePassEffect);
		_LightingTile.Render(mainCamera);

		RenderLights(mainCamera, rendererList);
 
		// the last hud things, must use GBuffer's depth buffer surface view
		GEngine->RenderEngine->SetEffect(NULL);
		GEngine->RenderEngine->SetRenderTarget(GEngine->RenderSurfaceCache->GBuffer->MainTargetGDepth, true);
		GEngine->GameCanvas->DrawString(5,35,"Deferred Shading Mode");
		GSceneManager->DebugDraw();
		GEngine->Stat.DrawStat();

		GEngine->RenderSurfaceCache->GBuffer->DebugVisual();

		GEngine->GameCanvas->RenderLines(mainCamera);

		// postprocess not include the string renders.
		if (GConfig->pRenderConfig->FXAA)
		{
			_aaRender->Render(mainCamera, GEngine->RenderSurfaceCache->MainTarget->RenderTargetShaderView);
			GEngine->RenderEngine->SetEffect(NULL);
			GEngine->RenderEngine->SetRenderTarget(GEngine->RenderSurfaceCache->GBuffer->MainTargetGDepth);
		}
		// the last ... render string.

		GEngine->GameCanvas->RenderStrings(mainCamera);

		GEngine->RenderEngine->SetEffect(NULL);
		GEngine->RenderEngine->OnFrameEnd();
	}

	void DeferredShading::RenderScene(const CameraPtr& mainCamera,const std::vector<GeometryRendererPtr>& rendererList)
	{	
		for(size_t i=0;i< rendererList.size(); i++ )
		{
			GeometryRendererPtr obj = rendererList[i];
			obj->BuildRenderLayout(_RenderSceneEffect,false);
			obj->UpdateShaderProperty();
			obj->SetRenderEffect(_RenderSceneEffect);
			obj->Render(mainCamera);
			 
		}
	}
 
}