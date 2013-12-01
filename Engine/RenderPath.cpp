#include "EngineInclude.h"

namespace Disorder
{
	void RenderPath::Render(RenderPathType type)
	{
		if( type == RPT_ForwardLighting )
			_ForwardLighting();
	}

	void RenderPath::_ForwardLighting()
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

		std::vector<RendererPtr> rendererList = GSceneManager->GetRendererList(mainCamera);
		for(unsigned int i=0;i< rendererList.size(); i++ )
		{
			RendererPtr obj = rendererList[i];
			obj->PreDraw(mainCamera);

			for( unsigned int j=0;j< obj->GetPassNumber();j++ )
			{
				obj->Draw(j,mainCamera);
			}

			obj->PostDraw(mainCamera);
		}

		mainCamera->DrawAxis();

		// before we call canvas draw ,we should check if we should add stat info to canvas.
		if(GEngine->Stat.bEnable())
		{
			GDrawTriNumber += GEngine->GameCanvas->GetCurrentDrawTriNumber();
			GEngine->Stat.DrawStat();
		}

		GEngine->GameCanvas->Draw(mainCamera);

		GEngine->RenderEngine->OnDrawEnd();
	}
}