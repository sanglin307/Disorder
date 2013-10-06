#include "EngineInclude.h"

namespace Disorder
{
	void RenderPath::Render(RenderPathType type)
	{
		if( type == RPT_ForwardMultiPassLight )
			_ForwardMultiPassLight();
	}

	void RenderPath::_ForwardMultiPassLight()
	{
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
				obj->Draw(RPT_ForwardMultiPassLight,j,mainCamera);
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