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

	RendererPtr SceneManager::GetRenderer(std::string const& name)
	{
		if( _mRenderObjects.find(name) != _mRenderObjects.end() )
			return _mRenderObjects.at(name);

		return NULL;
	}

	LightPtr SceneManager::GetLight(std::string const& name)
	{
		if( _mLightObjects.find(name) != _mLightObjects.end() )
			return _mLightObjects.at(name);

		return NULL;
	}

	CameraPtr SceneManager::GetCamera(std::string const& name)
	{
		if( _mCameraObjects.find(name) != _mCameraObjects.end() )
			return _mCameraObjects.at(name);

		return NULL;
	}

	void SceneManager::AddCamera(CameraPtr const& camera)
	{
		BOOST_ASSERT(camera->Name != "");

		if(_mCameraObjects.find(camera->Name) == _mCameraObjects.end())
			_mCameraObjects.insert(std::pair<std::string,CameraPtr>(camera->Name,camera));
	}

	void SceneManager::AddLight(LightPtr const& light)
	{
		BOOST_ASSERT(light->Name != "");

		if( _mLightObjects.find(light->Name) == _mLightObjects.end() )
		{
			_mLightObjects.insert(std::pair<std::string,LightPtr>(light->Name,light));
			if( light->Type == LT_Parallel )
				_mDirectLights.push_back(light);
			else
				_mNonDirectLights.push_back(light);
		}
	}

	void SceneManager::AddRenderer(RendererPtr const& renderer)
	{
		BOOST_ASSERT(renderer->Name != "");

		if( _mRenderObjects.find(renderer->Name) == _mRenderObjects.end() )
			_mRenderObjects.insert(std::pair<std::string,RendererPtr>(renderer->Name,renderer));
	}

	void SceneManager::Tick(float deltaSeconds)
	{
		CameraMap::const_iterator iter = _mCameraObjects.begin();
		while( iter != _mCameraObjects.end())
		{
			iter->second->Tick(deltaSeconds);
			iter++;
		}
	}

	void SceneManager::Render()
	{
		GEngine->RenderEngine->OnDrawBegin();

		CameraPtr mainCamera = _mCameraObjects.at("MainCamera");

		RendererMap::const_iterator iter = _mRenderObjects.begin();
		while (iter != _mRenderObjects.end())
		{
			for( int i = 0;i<MVT_NUM_VIEW_TYPES;i++)
			{
				if( i == MVT_Perspective )
				{
					iter->second->SetDirectLightParam(_mDirectLights);
				}

				if( i != MVT_Lights )
				{
					iter->second->Draw((MaterialViewType)i,mainCamera);
				}
				else
				{
					for(int lightIndex=0;lightIndex<_mNonDirectLights.size();lightIndex++)
					{
						iter->second->SetLightParam(_mNonDirectLights[lightIndex]);
						iter->second->Draw((MaterialViewType)i,mainCamera);
					}
				}
			}
			
			iter++;
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