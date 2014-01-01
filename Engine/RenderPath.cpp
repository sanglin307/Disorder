#include "EngineInclude.h"

namespace Disorder
{
	//////////////////////////////////////////////////////////////////////////////////////////
	void ForwardRenderPath::Render()
	{
		GEngine->RenderEngine->SetRenderTarget(GRenderSurface.RenderTarget,GRenderSurface.DepthStencilBuffer);
		GEngine->RenderEngine->ClearRenderTarget(GRenderSurface.RenderTarget,Vector4(0.1f,0.1f,0.1f,1.0f));
		GEngine->RenderEngine->ClearDepthStencil(GRenderSurface.DepthStencilBuffer,true,1.0f,false,0);
		GEngine->RenderEngine->OnDrawBegin();

		CameraPtr mainCamera = GSceneManager->GetDefaultCamera();

		if( mainCamera == NULL )
			return;

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

	void ForwardRenderPath::SetDirectionLight(const std::vector<DirectionLightPtr>& directionLightArray)
	{
		BOOST_ASSERT(directionLightArray.size() <=1 );

		if( directionLightArray.size() == 0 )
			return;

		DirectionLightPtr dLight = directionLightArray[0];
		_DirectionLightPropertyManager->ClearShaderPropertyValue();
		_DirectionLightIntensityProperty->SetData(dLight->Intensity);
		_DirectionLightDirProperty->SetData(dLight->GetDirection());
		_DirectionLightColorProperty->SetData(dLight->Color);

		_DirectionLightPropertyManager->UpdateShaderProperty();
	}
 
	void ForwardRenderPath::SetFourLight(const std::vector<LightPtr>& lightArray)
	{
		if( lightArray.size() == 0 )
			return;

		BOOST_ASSERT(lightArray.size() <= 4);
	
		Vector4 LightPosX(0.0f);
		Vector4 LightPosY(0.0f);
		Vector4 LightPosZ(0.0f);
		Vector4 LightDirX(0.0f);
		Vector4 LightDirY(0.0f);
		Vector4 LightDirZ(0.0f);
		Vector4 LightRangeRcp(0.0f);
		Vector4 SpotCosOuterCone(-2.0f);
		Vector4 SpotCosInnerConeRcp(1.0f);
		Vector4 CapsuleLen(0.0f);
		Vector4 LightColorR(0.0f);
		Vector4 LightColorG(0.0f);
		Vector4 LightColorB(0.0f);

		for(size_t i=0;i<lightArray.size();i++)
		{
			LightPtr light = lightArray[i];
			if(light->LightType == LT_Point )
			{
				PointLightPtr pLight = boost::dynamic_pointer_cast<PointLight>(light);
				Vector3 pos = pLight->GetPosition();
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
				Vector3 pos = sLight->GetPosition();
				LightPosX[i] = pos.x;
				LightPosY[i] = pos.y;
				LightPosZ[i] = pos.z;
				Vector3 dir = sLight->GetDirection();
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
		ForwardLightPosX->SetData(LightPosX);
		ForwardLightPosY->SetData(LightPosY);
		ForwardLightPosZ->SetData(LightPosZ);
		ForwardLightDirX->SetData(LightDirX);
		ForwardLightDirY->SetData(LightDirY);
		ForwardLightDirZ->SetData(LightDirZ);
		ForwardLightRangeRcp->SetData(LightRangeRcp);
		ForwardSpotCosOuterCone->SetData(SpotCosOuterCone);
		ForwardSpotCosInnerConeRcp->SetData(SpotCosInnerConeRcp);
		ForwardCapsuleLen->SetData(CapsuleLen);
		ForwardLightColorR->SetData(LightColorR);
		ForwardLightColorG->SetData(LightColorG);
		ForwardLightColorB->SetData(LightColorB);
		_LightFourPropertyManager->UpdateShaderProperty();

	}

	ForwardRenderPathPtr ForwardRenderPath::Create()
	{
		ForwardRenderPath *pPath = new ForwardRenderPath;
		return ForwardRenderPathPtr(pPath);
	}

	ForwardRenderPath::ForwardRenderPath()
	{
		_type = RPT_ForwardLighting;

		_DirectionLightPropertyManager = GEngine->RenderResManager->GetPropertyManager(ShaderPropertyManager::sManagerDirectionLight);

		_DirectionLightIntensityProperty = _DirectionLightPropertyManager->CreateProperty(ShaderPropertyManager::sDirectionLightIntensity,eSP_Float);
		_DirectionLightDirProperty = _DirectionLightPropertyManager->CreateProperty(ShaderPropertyManager::sDirectionLightDir,eSP_Vector3);
		_DirectionLightColorProperty = _DirectionLightPropertyManager->CreateProperty(ShaderPropertyManager::sDirectionLightColor,eSP_Vector3);

		_DirectionLightEffect = RenderEffect::Create();
		ShaderObjectPtr vertexShader = GEngine->RenderResManager->CreateShader(ST_VertexShader,"ForwardLightPass",SM_4_0,"RenderSceneVS");
		ShaderObjectPtr pixelShader = GEngine->RenderResManager->CreateShader(ST_PixelShader,"ForwardLightPass",SM_4_0,"DirectionalLightPS");
		_DirectionLightEffect->BindShader(vertexShader);
		_DirectionLightEffect->BindShader(pixelShader);


		_LightFourPropertyManager = GEngine->RenderResManager->GetPropertyManager(ShaderPropertyManager::sManagerForwardFourLight);
		ForwardLightPosX = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightPosX,eSP_Vector4);
		ForwardLightPosY = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightPosY,eSP_Vector4);
		ForwardLightPosZ = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightPosZ,eSP_Vector4);
		ForwardLightDirX = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightDirX,eSP_Vector4);
		ForwardLightDirY = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightDirY,eSP_Vector4);
		ForwardLightDirZ = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightDirZ,eSP_Vector4);
		ForwardLightRangeRcp = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightRangeRcp,eSP_Vector4);
		ForwardSpotCosOuterCone = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardSpotCosOuterCone,eSP_Vector4);
		ForwardSpotCosInnerConeRcp = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardSpotCosInnerConeRcp,eSP_Vector4);
		ForwardCapsuleLen = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardCapsuleLen,eSP_Vector4);
		ForwardLightColorR = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightColorR,eSP_Vector4);
		ForwardLightColorG = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightColorG,eSP_Vector4);
		ForwardLightColorB = _LightFourPropertyManager->CreateProperty(ShaderPropertyManager::sForwardLightColorB,eSP_Vector4);

		_FourLightEffect = RenderEffect::Create();
		ShaderObjectPtr fVertexShader = GEngine->RenderResManager->CreateShader(ST_VertexShader,"ForwardLightPass",SM_4_0,"RenderSceneVS");
		ShaderObjectPtr fPixelShader = GEngine->RenderResManager->CreateShader(ST_PixelShader,"ForwardLightPass",SM_4_0,"ForwardFourLightPS");
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
		BlendStatePtr blendState = GEngine->RenderResManager->CreateBlendState(&bDesc,1);
		_FourLightEffect->BindBlendState(blendState);

		RasterizeDesc rDesc;

		DepthStencilDesc dsDesc;
		dsDesc.DepthFunc = CF_Less_Equal;
		DepthStencilStatePtr noDepthState = GEngine->RenderResManager->CreateDepthStencilState(&dsDesc,0);
		_FourLightEffect->BindDepthStencilState(noDepthState);


	}
}