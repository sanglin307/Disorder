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
		GEngine->RenderEngine->ClearRenderTarget(GRenderSurface.RenderTarget,Vector4(0.1f,0.2f,0.6f,1.0f));
		GEngine->RenderEngine->ClearDepthStencil(GRenderSurface.DepthStencilBuffer,true,1.0f,false,0);
		GEngine->RenderEngine->OnDrawBegin();

		CameraPtr mainCamera = GSceneManager->GetCamera("MainCamera");

		if( mainCamera == NULL )
			return;

		std::vector<RendererPtr> rendererList = GSceneManager->GetRendererList(mainCamera);
		for(unsigned int i=0;i< rendererList.size(); i++ )
		{
			RendererPtr obj = rendererList[i];
			obj->PreDraw(mainCamera);

			for( int j=0;j< FRP_Max;j++ )
			{
				obj->Draw(RPT_ForwardLighting,j,mainCamera);
			}

			obj->PostDraw(mainCamera);
		}

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