#include "EngineInclude.h"

namespace Disorder
{
	RenderPath::RenderPath()
	{
		_DirectionLightPropertyManager = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerDirectionLight);
		_DirectionLightIntensityProperty = _DirectionLightPropertyManager->CreateProperty(ShaderPropertyManager::sDirectionLightIntensity,eSP_Float,1);
		_DirectionLightDirProperty = _DirectionLightPropertyManager->CreateProperty(ShaderPropertyManager::sDirectionLightDir,eSP_Float,3);
		_DirectionLightColorProperty = _DirectionLightPropertyManager->CreateProperty(ShaderPropertyManager::sDirectionLightColor,eSP_Float,3);

		
		_LightFourPropertyManager = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerForwardFourLight);
		ForwardLightPosX = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightPosX,eSP_Float,4);
		ForwardLightPosY = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightPosY,eSP_Float,4);
		ForwardLightPosZ = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightPosZ,eSP_Float,4);
		ForwardLightDirX = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightDirX,eSP_Float,4);
		ForwardLightDirY = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightDirY,eSP_Float,4);
		ForwardLightDirZ = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightDirZ,eSP_Float,4);
		ForwardLightRangeRcp = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightRangeRcp,eSP_Float,4);
		ForwardSpotCosOuterCone = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardSpotCosOuterCone,eSP_Float,4);
		ForwardSpotCosInnerConeRcp = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardSpotCosInnerConeRcp,eSP_Float,4);
		ForwardCapsuleLen = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardCapsuleLen,eSP_Float,4);
		ForwardLightColorR = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightColorR,eSP_Float,4);
		ForwardLightColorG = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightColorG,eSP_Float,4);
		ForwardLightColorB = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightColorB,eSP_Float,4);

	}

	void RenderPath::SetDirectionLight(const std::vector<DirectionLightPtr>& directionLightArray)
	{
		BOOST_ASSERT(directionLightArray.size() <=1 );

		if( directionLightArray.size() == 0 )
			return;

		DirectionLightPtr dLight = directionLightArray[0];
		_DirectionLightPropertyManager->ClearShaderPropertyValue();
		_DirectionLightIntensityProperty->SetData(&(dLight->Intensity));
		glm::vec3 dir = dLight->GetDirection();
		_DirectionLightDirProperty->SetData(glm::value_ptr(dir));
		_DirectionLightColorProperty->SetData(glm::value_ptr(dLight->Color));

		_DirectionLightPropertyManager->UpdateShaderProperty();
	}

	void RenderPath::SetFourLight(const std::vector<LightPtr>& lightArray)
	{
		if( lightArray.size() == 0 )
			return;

		BOOST_ASSERT(lightArray.size() <= 4);
	
		glm::vec4 LightPosX(0.f);
		glm::vec4 LightPosY(0.f); 
		glm::vec4 LightPosZ(0.f); 
		glm::vec4 LightDirX(0.f);  
		glm::vec4 LightDirY(0.f);  
		glm::vec4 LightDirZ(0.f); 
		glm::vec4 LightRangeRcp(0.f);  
		glm::vec4 SpotCosOuterCone(-2.f); 
		glm::vec4 SpotCosInnerConeRcp(1.f); 
		glm::vec4 CapsuleLen(0.f);  
		glm::vec4 LightColorR(0.f);  
		glm::vec4 LightColorG(0.f);  
		glm::vec4 LightColorB(0.f); 

		for(size_t i=0;i<lightArray.size();i++)
		{
			LightPtr light = lightArray[i];
			if(light->LightType == LT_Point )
			{
				PointLightPtr pLight = boost::dynamic_pointer_cast<PointLight>(light);
				glm::vec3 pos = pLight->GetPosition();
				LightPosX[i] = pos.x;
				LightPosY[i] = pos.y;
				LightPosZ[i] = pos.z;
				LightRangeRcp[i] = 1.0f / pLight->Range;
				LightColorR[i] = pLight->Color.x;
				LightColorG[i] = pLight->Color.y;
				LightColorB[i] = pLight->Color.z;
			}
			else if(light->LightType == LT_Spot )
			{
				SpotLightPtr sLight = boost::dynamic_pointer_cast<SpotLight>(light);
				glm::vec3 pos = sLight->GetPosition();
				LightPosX[i] = pos.x;
				LightPosY[i] = pos.y;
				LightPosZ[i] = pos.z;
				glm::vec3 dir = sLight->GetDirection();
				LightDirX[i] = dir.x;
				LightDirY[i] = dir.y;
				LightDirZ[i] = dir.z;
				LightRangeRcp[i] = 1.0f / sLight->Range;
				SpotCosInnerConeRcp[i] = 1.0f / Math::Cosf(sLight->SpotInnerAngle);
				SpotCosOuterCone[i] = Math::Cosf(sLight->SpotOuterAngle);
				LightColorR[i] = sLight->Color.x;
				LightColorG[i] = sLight->Color.y;
				LightColorB[i] = sLight->Color.z;
			}
			else
			{
				BOOST_ASSERT(0);
			}
			
		}

		_LightFourPropertyManager->ClearShaderPropertyValue();
		ForwardLightPosX->SetData(glm::value_ptr(LightPosX));
		ForwardLightPosY->SetData(glm::value_ptr(LightPosY));
		ForwardLightPosZ->SetData(glm::value_ptr(LightPosZ));
		ForwardLightDirX->SetData(glm::value_ptr(LightDirX));
		ForwardLightDirY->SetData(glm::value_ptr(LightDirY));
		ForwardLightDirZ->SetData(glm::value_ptr(LightDirZ));
		ForwardLightRangeRcp->SetData(glm::value_ptr(LightRangeRcp));
		ForwardSpotCosOuterCone->SetData(glm::value_ptr(SpotCosOuterCone));
		ForwardSpotCosInnerConeRcp->SetData(glm::value_ptr(SpotCosInnerConeRcp));
		ForwardCapsuleLen->SetData(glm::value_ptr(CapsuleLen));
		ForwardLightColorR->SetData(glm::value_ptr(LightColorR));
		ForwardLightColorG->SetData(glm::value_ptr(LightColorG));
		ForwardLightColorB->SetData(glm::value_ptr(LightColorB));
		_LightFourPropertyManager->UpdateShaderProperty();

	}

	//////////////////////////////////////////////////////////////////////////////////////////
	void ForwardRenderPath::Render()
	{
		CameraPtr mainCamera = GSceneManager->GetDefaultCamera();

		if( mainCamera == NULL )
			return;

		GEngine->RenderEngine->OnDrawBegin();

		GEngine->RenderEngine->SetRenderTarget(GEngine->RenderSurfaceCache->RenderTarget,GEngine->RenderSurfaceCache->DepthStencilBuffer);
		GEngine->RenderEngine->ClearRenderTarget(GEngine->RenderSurfaceCache->RenderTarget,glm::vec4(0.f,0.f,0.f,1.0f));
		GEngine->RenderEngine->ClearDepthStencil(GEngine->RenderSurfaceCache->DepthStencilBuffer,true,1.0f,false,0);
 
		GSceneManager->UpdateShaderProperty();
		mainCamera->UpdateShaderProperty();

		std::vector<RendererPtr> rendererList;
		GSceneManager->GetRendererList(mainCamera,rendererList);
		for(unsigned int i=0;i< rendererList.size(); i++ )
		{
			RendererPtr obj = rendererList[i];
			obj->BuildRenderLayout(_DirectionLightEffect->GetVertexShader(),false);
			obj->PreRender(mainCamera);

			// render lights
			const std::vector<DirectionLightPtr>& directionLightList = obj->GetDirectionLights();
			if( directionLightList.size() > 0 )
			{
				SetDirectionLight(directionLightList);
				obj->SetRenderEffect(_DirectionLightEffect);
				obj->Render(mainCamera);	 
			}

			//non direction lights
			const std::vector<LightPtr>& nonDirectionLights = obj->GetNonDirectionLights();
			size_t lightIndex = 0;
			std::vector<LightPtr> lightArray;
			while( lightIndex < nonDirectionLights.size() )
			{
				if(lightArray.size() == 4 )
				{
					SetFourLight(lightArray);
					obj->SetRenderEffect(_FourLightEffect);
					obj->Render(mainCamera);
					lightArray.clear();
				}
				else
				{
					lightArray.push_back(nonDirectionLights[lightIndex]);
				}

				lightIndex++;
			}

			if(lightArray.size() > 0 )
			{
				SetFourLight(lightArray);
				obj->SetRenderEffect(_FourLightEffect);
				obj->Render(mainCamera);
			}

			obj->PostRender(mainCamera);
		}

		GEngine->GameCanvas->DrawString(0.005f, 0.04f, 0.04f, glm::vec4(1.f), "Forward Lighting Mode");

		GSceneManager->DebugDraw();

		// before we call canvas draw ,we should check if we should add stat info to canvas.
		if(GEngine->Stat.bEnable())
		{
			GDrawTriNumber += GEngine->GameCanvas->GetCurrentDrawTriNumber();
			GEngine->Stat.DrawStat();
		}

		GEngine->GameCanvas->Render(mainCamera);

		GEngine->RenderEngine->OnDrawEnd();
	}
 

	ForwardRenderPathPtr ForwardRenderPath::Create()
	{
		ForwardRenderPath *pPath = new ForwardRenderPath;
		return ForwardRenderPathPtr(pPath);
	}

	ForwardRenderPath::ForwardRenderPath()
	{
		_type = RPT_ForwardLighting;
 
		_DirectionLightEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
		ShaderObjectPtr vertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader,"ForwardLighting",SM_4_0,"RenderSceneVS");
		ShaderObjectPtr pixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader,"ForwardLighting",SM_4_0,"DirectionalLightPS");
		_DirectionLightEffect->BindShader(vertexShader);
		_DirectionLightEffect->BindShader(pixelShader);
 
		_FourLightEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
		ShaderObjectPtr fVertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader,"ForwardLighting",SM_4_0,"RenderSceneVS");
		ShaderObjectPtr fPixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader,"ForwardLighting",SM_4_0,"ForwardFourLightPS");
		_FourLightEffect->BindShader(fVertexShader);
		_FourLightEffect->BindShader(fPixelShader);
		BlendDesc bDesc;
		bDesc.BlendEnable = true;
		bDesc.BlendOp = BLEND_OP_ADD;
		bDesc.SrcBlend = BLEND_ONE;
		bDesc.DestBlend = BLEND_ONE;
		bDesc.BlendOpAlpha = BLEND_OP_ADD;
		bDesc.SrcBlendAlpha = BLEND_ONE;
		bDesc.DestBlendAlpha = BLEND_ONE;
		BlendStatePtr blendState = GEngine->RenderResourceMgr->CreateBlendState(&bDesc,1);
		_FourLightEffect->BindBlendState(blendState);
 
		DepthStencilDesc dsDesc;
		dsDesc.DepthFunc = CF_Less_Equal;
		DepthStencilStatePtr noDepthState = GEngine->RenderResourceMgr->CreateDepthStencilState(&dsDesc,0);
		_FourLightEffect->BindDepthStencilState(noDepthState);


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
		ShaderObjectPtr vertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader,"DeferredShading",SM_4_0,"RenderSceneVS");
		ShaderObjectPtr pixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader,"DeferredShading",SM_4_0,"RenderScenePS");
		_RenderSceneEffect->BindShader(vertexShader);
		_RenderSceneEffect->BindShader(pixelShader);
		DepthStencilDesc depthDesc;
		depthDesc.DepthEnable = true;
		depthDesc.DepthWrite = true;
		depthDesc.DepthFunc = CF_Less;
		depthDesc.StencilEnable = true;
		depthDesc.BackFaceStencilDepthFailOp = depthDesc.FrontFaceStencilDepthFailOp = STENCIL_OP_REPLACE;
		depthDesc.BackFaceStencilFailOp = depthDesc.FrontFaceStencilFailOp = STENCIL_OP_REPLACE;
		depthDesc.BackFaceStencilPassOp = depthDesc.FrontFaceStencilPassOp = STENCIL_OP_REPLACE;
		depthDesc.BackFaceStencilFunc = depthDesc.BackFaceStencilFunc = CF_Always;
		DepthStencilStatePtr _DepthWriteState = GEngine->RenderResourceMgr->CreateDepthStencilState(&depthDesc,1);
		_RenderSceneEffect->BindDepthStencilState(_DepthWriteState);

		_LightingEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
	     vertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader,"DeferredShading",SM_4_0,"LightingVS");
		 pixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader,"DeferredShading",SM_4_0,"BaseLightingPS");
		_LightingEffect->BindShader(vertexShader);
		_LightingEffect->BindShader(pixelShader); 

		_FourLightEffect = GEngine->RenderResourceMgr->CreateRenderEffect();
		ShaderObjectPtr fVertexShader = GEngine->RenderResourceMgr->CreateShader(ST_VertexShader,"DeferredShading",SM_4_0,"LightingVS");
		ShaderObjectPtr fPixelShader = GEngine->RenderResourceMgr->CreateShader(ST_PixelShader,"DeferredShading",SM_4_0,"DeferFourLightPS");
		_FourLightEffect->BindShader(fVertexShader);
		_FourLightEffect->BindShader(fPixelShader);
		BlendDesc bDesc;
		bDesc.BlendEnable = true;
		bDesc.BlendOp = BLEND_OP_ADD;
		bDesc.SrcBlend = BLEND_ONE;
		bDesc.DestBlend = BLEND_ONE;
		bDesc.BlendOpAlpha = BLEND_OP_ADD;
		bDesc.SrcBlendAlpha = BLEND_ONE;
		bDesc.DestBlendAlpha = BLEND_ONE;
		BlendStatePtr blendState = GEngine->RenderResourceMgr->CreateBlendState(&bDesc,1);
		_FourLightEffect->BindBlendState(blendState);
 
		DepthStencilDesc dsDesc;
		dsDesc.DepthFunc = CF_Less_Equal;
		DepthStencilStatePtr noDepthState = GEngine->RenderResourceMgr->CreateDepthStencilState(&dsDesc,0);
		_FourLightEffect->BindDepthStencilState(noDepthState);


		TileTexVertex vertex[4];
		vertex[0].position = glm::vec3(-1.f, 1.f, 0.f);
		vertex[1].position = glm::vec3(-1.0f, -1.0f, 0.0f);
		vertex[2].position = glm::vec3(1.0f, 1.0f, 0.0f);
		vertex[3].position = glm::vec3(1.0f, -1.0f, 0.0f);
		_LightingTile = SimpleTile("DeferLightingTile",vertex,_LightingEffect);

		DepthStencilDesc nodepthWriteDesc;
		nodepthWriteDesc.DepthEnable = true;
		nodepthWriteDesc.DepthWrite = false;
		nodepthWriteDesc.DepthFunc = CF_Less;
		nodepthWriteDesc.StencilEnable = true;
		nodepthWriteDesc.BackFaceStencilDepthFailOp = nodepthWriteDesc.FrontFaceStencilDepthFailOp = STENCIL_OP_KEEP;
		nodepthWriteDesc.BackFaceStencilFailOp = nodepthWriteDesc.FrontFaceStencilFailOp = STENCIL_OP_KEEP;
		nodepthWriteDesc.BackFaceStencilPassOp = nodepthWriteDesc.FrontFaceStencilPassOp = STENCIL_OP_KEEP;
		nodepthWriteDesc.BackFaceStencilFunc = nodepthWriteDesc.BackFaceStencilFunc = CF_Equal;
		DepthStencilStatePtr _noDepthWriteState = GEngine->RenderResourceMgr->CreateDepthStencilState(&nodepthWriteDesc,0);
		_LightingEffect->BindDepthStencilState(_noDepthWriteState);

		GEngine->RenderSurfaceCache->InitGBuffer(GConfig->pRenderConfig->SizeX,GConfig->pRenderConfig->SizeY);
	}

	void DeferredShading::Render()
	{
		CameraPtr mainCamera = GSceneManager->GetDefaultCamera();

		if( mainCamera == NULL )
			return;

		GSceneManager->UpdateShaderProperty();
		mainCamera->UpdateShaderProperty();

		GEngine->RenderEngine->OnDrawBegin();
	
		RenderSurfacePtr depthSurfacePtr = GEngine->RenderSurfaceCache->GBuffer->DepthStencilBuffer;
		RenderSurfacePtr colorSurfacePtr = GEngine->RenderSurfaceCache->GBuffer->BasicColorBuffer;
		RenderSurfacePtr normalSurfacePtr = GEngine->RenderSurfaceCache->GBuffer->NormalDataBuffer;
		RenderSurfacePtr specularSurfacePtr = GEngine->RenderSurfaceCache->GBuffer->SpecularDataBuffer;

		GEngine->RenderEngine->ClearDepthStencil(depthSurfacePtr,true,1.f,true,0);
		GEngine->RenderEngine->ClearRenderTarget(colorSurfacePtr,glm::vec4(0.f));
		GEngine->RenderEngine->ClearRenderTarget(normalSurfacePtr, glm::vec4(0.f));
		GEngine->RenderEngine->ClearRenderTarget(specularSurfacePtr, glm::vec4(0.f));

		std::vector<RenderSurfacePtr> vRenderSurface;
		vRenderSurface.push_back(colorSurfacePtr);
		vRenderSurface.push_back(normalSurfacePtr);
		vRenderSurface.push_back(specularSurfacePtr);

		GEngine->RenderEngine->SetRenderTarget(vRenderSurface,depthSurfacePtr);
 
		RenderScene(mainCamera);

		static bool sSaveTest = false;
		if( sSaveTest )
		{
			GEngine->RenderEngine->SaveRenderSurface(depthSurfacePtr,"GBuffer_DepthStencilBuffer.bmp");
			GEngine->RenderEngine->SaveRenderSurface(colorSurfacePtr,"GBuffer_BaseColorBuffer.bmp");
			GEngine->RenderEngine->SaveRenderSurface(normalSurfacePtr,"GBuffer_NormalBuffer.bmp");
			GEngine->RenderEngine->SaveRenderSurface(specularSurfacePtr,"GBuffer_SpecularBuffer.bmp");

			sSaveTest = false;
		}

		GEngine->RenderSurfaceCache->GBuffer->UpdateShaderProperty();
	
		//lighting pass
		GEngine->RenderEngine->SetRenderTarget(GEngine->RenderSurfaceCache->RenderTarget,GEngine->RenderSurfaceCache->DepthStencilBuffer);
		GEngine->RenderEngine->ClearRenderTarget(GEngine->RenderSurfaceCache->RenderTarget, glm::vec4(0.f, 0.f, 0.f, 1.0f));
		GEngine->RenderEngine->ClearDepthStencil(GEngine->RenderSurfaceCache->DepthStencilBuffer,true,1.0f,false,0);
	
		const std::vector<LightPtr>& vLights = GSceneManager->GetLightsList();
		std::vector<DirectionLightPtr> directionLightArray;
		std::vector<LightPtr> nonDirectionLights;
		for(size_t i=0;i<vLights.size();i++ )
		{
			if( vLights[i]->LightType == LT_Directional )
			{
				directionLightArray.push_back(boost::dynamic_pointer_cast<DirectionLight>(vLights[i]));
			}
			else
			{
				nonDirectionLights.push_back(vLights[i]);
			}
		}
		SetDirectionLight(directionLightArray);
		_LightingTile.SetRenderEffect(_LightingEffect);
		_LightingTile.Render(mainCamera);

		if( nonDirectionLights.size() > 0 )
		{
			size_t lightIndex = 0;
			std::vector<LightPtr> lightArray;
			while( lightIndex < nonDirectionLights.size() )
			{
				if(lightArray.size() == 4 )
				{
					SetFourLight(lightArray);
					_LightingTile.SetRenderEffect(_FourLightEffect);
					_LightingTile.Render(mainCamera);
					lightArray.clear();
				}
				else
				{
					lightArray.push_back(nonDirectionLights[lightIndex]);
				}

				lightIndex++;
			}

			if(lightArray.size() > 0 )
			{
				SetFourLight(lightArray);
				_LightingTile.SetRenderEffect(_FourLightEffect);
				_LightingTile.Render(mainCamera);
			}
		}

		GEngine->RenderEngine->SetEffect(NULL);
		
		GEngine->RenderEngine->SetRenderTarget(GEngine->RenderSurfaceCache->RenderTarget,depthSurfacePtr,true);
		GEngine->GameCanvas->DrawString(0.005f,0.04f,0.04f,glm::vec4(1.f),"Deferred Shading Mode");
		GSceneManager->DebugDraw();

		// before we call canvas draw ,we should check if we should add stat info to canvas.
		if(GEngine->Stat.bEnable())
		{
			GDrawTriNumber += GEngine->GameCanvas->GetCurrentDrawTriNumber();
			GEngine->Stat.DrawStat();
		}

		GEngine->GameCanvas->Render(mainCamera);

		if( GSceneManager->EnableDebugDraw )
		    GEngine->RenderSurfaceCache->GBuffer->DebugVisual();

		GEngine->RenderEngine->SetEffect(NULL);
		GEngine->RenderEngine->OnDrawEnd();

	}

	void DeferredShading::RenderScene(const CameraPtr& mainCamera)
	{
		std::vector<RendererPtr> rendererList;
		GSceneManager->GetRendererList(mainCamera,rendererList);
		for(unsigned int i=0;i< rendererList.size(); i++ )
		{
			RendererPtr obj = rendererList[i];
			obj->BuildRenderLayout(_RenderSceneEffect->GetVertexShader(),false);
			obj->PreRender(mainCamera);
			obj->SetRenderEffect(_RenderSceneEffect);
			obj->Render(mainCamera);
			obj->PostRender(mainCamera);
			 
		}
	}
 
}