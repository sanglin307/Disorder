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

	void SceneManager::CreateDefaultCamera()
	{
		BOOST_ASSERT(_mDefaultCamera == NULL);

		GameObjectPtr camera = SceneObjectGenerator::CreateCamera("DefaultCamera");
		GWorld->GetLevel()->AddGameObject(camera);
		_mDefaultCamera = boost::dynamic_pointer_cast<Camera>(camera->GetComponent("DefaultCamera"));
		AddCamera(_mDefaultCamera);
		GEngine->GameClient->AddInputListener(_mDefaultCamera);
	}

	void SceneManager::SetDefaultCamera(CameraPtr const& camera)
	{
		if( _mDefaultCamera != NULL )
		{
			GEngine->GameClient->ReleaseInputListener(_mDefaultCamera);
			
		}

		GEngine->GameClient->AddInputListener(camera);
	   _mDefaultCamera = camera;

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

	void SceneManager::UpdateLight()
	{
		for(unsigned int i=0;i<_vRenderObjects.size();i++ )
		{
			_vRenderObjects[i]->ClearLight();
			for(unsigned int j=0;j<_vDirectionalLights.size();j++)
			{
				_vRenderObjects[i]->AddLight(_vDirectionalLights[j]);
			}

			for(unsigned int k=0;k<_vNonDirectionalLights.size();k++)
			{
				if( _vNonDirectionalLights[k]->Touch(_vRenderObjects[i]))
					_vRenderObjects[i]->AddLight(_vNonDirectionalLights[k]);
			}
		}
	}

	void SceneManager::AddLight(LightPtr const& light)
	{
		BOOST_ASSERT(light->Name != "");

		if( _mLightObjects.find(light->Name) == _mLightObjects.end() )
		{
			_mLightObjects.insert(std::pair<std::string,LightPtr>(light->Name,light));
			if( light->LightType == LT_Directional )
				_vDirectionalLights.push_back(light);
			else
				_vNonDirectionalLights.push_back(light);
		}
	}

	void SceneManager::AddRenderer(RendererPtr const& renderer)
	{
		BOOST_ASSERT(renderer->Name != "");

		if( _mRenderObjects.find(renderer->Name) == _mRenderObjects.end() )
		{
			_mRenderObjects.insert(std::pair<std::string,RendererPtr>(renderer->Name,renderer));
			_vRenderObjects.push_back(renderer);
		}
	}

	void SceneManager::Tick(float deltaSeconds)
	{
		CameraMap::const_iterator iter = _mCameraObjects.begin();
		while( iter != _mCameraObjects.end())
		{
			iter->second->Tick(deltaSeconds);
			iter++;
		}

		UpdateLight();
	}
 

}