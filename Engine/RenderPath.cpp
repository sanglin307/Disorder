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
			const std::vector<LightPtr>& lightList = obj->GetLightArray();
			for( size_t i=0;i<lightList.size();i++ )
			{
				lightList[i]->DebugDraw();

				if(lightList[i]->LightType == LT_Directional)
				{
					SetDirectionLight(lightList[i]);
					obj->SetRenderEffect(_DirectionLightEffect);
					obj->Render(mainCamera);
				}
			}

			obj->PostRender(mainCamera);
		}

		mainCamera->DrawAxis();

		// before we call canvas draw ,we should check if we should add stat info to canvas.
		if(GEngine->Stat.bEnable())
		{
			GDrawTriNumber += GEngine->GameCanvas->GetCurrentDrawTriNumber();
			GEngine->Stat.DrawStat();
		}

		GEngine->GameCanvas->Render(mainCamera);

		GEngine->RenderEngine->OnDrawEnd();
	}

	void ForwardRenderPath::SetDirectionLight(LightPtr const& light)
	{
		if( light->LightType != LT_Directional )
			return;

		_DirectionLightPropertyManager->ClearShaderPropertyValue();
		_DirectionLightIntensityProperty->SetData(light->Intensity);
		_DirectionLightDirProperty->SetData(light->GetDirection());
		_DirectionLightColorProperty->SetData(light->Color);

		_DirectionLightPropertyManager->UpdateShaderProperty();
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
	}
}