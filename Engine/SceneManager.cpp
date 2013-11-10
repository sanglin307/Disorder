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

	void SceneManager::UpdateLight()
	{
		for(unsigned int i=0;i<_vRenderObjects.size();i++ )
		{
			_vRenderObjects[i]->ClearLight();
			for(unsigned int j=0;j<_vDirectLights.size();j++)
			{
				_vRenderObjects[i]->AddLight(_vDirectLights[j]);
			}

			for(unsigned int k=0;k<_vNonDirectLights.size();k++)
			{
				if( _vNonDirectLights[k]->Touch(_vRenderObjects[i]))
					_vRenderObjects[i]->AddLight(_vNonDirectLights[k]);
			}
		}
	}

	void SceneManager::AddLight(LightPtr const& light)
	{
		BOOST_ASSERT(light->Name != "");

		if( _mLightObjects.find(light->Name) == _mLightObjects.end() )
		{
			_mLightObjects.insert(std::pair<std::string,LightPtr>(light->Name,light));
			if( light->LightType == LT_Parallel )
				_vDirectLights.push_back(light);
			else
				_vNonDirectLights.push_back(light);
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