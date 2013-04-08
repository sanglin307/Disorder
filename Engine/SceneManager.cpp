#include "EngineInclude.h"

namespace Disorder
{
	InitialiseSingleton(SceneManager);

	void SceneManager::Init()
	{
		_sceneImporter = boost::make_shared<FbxSceneImporter>();
	}

	void SceneManager::Exit()
	{
	}

	void SceneManager::AddRenderer(RendererPtr const& renderer)
	{
		_vRenderObjects.push_back(renderer);
	}

	void SceneManager::Render()
	{
		GEngine->RenderEngine->OnDrawBegin();

		//road on the render tree.
		GEngine->RenderEngine->Draw(_vRenderObjects);

		// before we call canvas draw ,we should check if we should add stat info to canvas.
		/*if(GEngine->Stat.bEnable())
		{
			GDrawTriNumber += GEngine->GameCanvas->GetCurrentDrawTriNumber();
			GEngine->Stat.DrawStat();
		}

		GEngine->GameCanvas->Draw();*/

		GEngine->RenderEngine->OnDrawEnd();
	}


}