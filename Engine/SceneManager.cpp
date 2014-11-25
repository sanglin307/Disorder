#include "EngineInclude.h"

namespace Disorder
{
	SceneManager* GSceneManager = NULL;

	void SceneManager::Init()
	{
		_sceneImporter = new FbxSceneImporter;

		_vAmbientLowerColor = glm::vec3(0.f);
		_vAmbientUpperColor = glm::vec3(0.25f);
		_propertyManager = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerScene);
		_sAmbientLowerProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sAmbientLowColor,eSP_Float,3);
		_sAmbientUpperProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sAmbientUpperColor,eSP_Float,3);
		_sScreenWidthProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sScreenWidth, eSP_Int, 1);
		_sScreenHeightProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sScreenHeight, eSP_Int, 1);
		_sShadowMapSizeProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sShadowMapSize, eSP_Int, 1);
		_sLineRadius = _propertyManager->CreateProperty(ShaderPropertyManager::sLineRadius, eSP_Float, 1);
		_skyBox = new Skybox;

		EnableDebugDraw = true;
	}

	void SceneManager::Exit()
	{
		_mRenderObjects.empty();
		_mLightObjects.empty();;
		_mCameraObjects.empty();
		_mDefaultCamera = NULL;
		_sceneImporter = NULL;
		_vLightList.empty();
		_vRenderObjects.empty();
	}

	void SceneManager::UpdateBoundingBox()
	{
		_sceneBounds.BMin = _sceneBounds.BMax = glm::vec3(0, 0, 0);
		for (size_t i = 0; i < _vRenderObjects.size(); i++)
		{
			const BoxBounds& box = _vRenderObjects[i]->GetBoxBounds();
			_sceneBounds.Union(box);
		}
	}

	void SceneManager::DebugDraw()
	{
		if( !EnableDebugDraw )
			return;
        
		for(size_t i=0;i<_vLightList.size();i++)
			_vLightList[i]->DebugDraw();

		for(size_t j=0;j<_vRenderObjects.size();j++ )
			_vRenderObjects[j]->DebugDraw();

		if( _mDefaultCamera != NULL )
			_mDefaultCamera->DebugDraw();
	}

	void SceneManager::GetRendererList(std::vector<GeometryRenderer*>& renderObjList) const
	{
		if (_vRenderObjects.size() == 0)
			return;

		renderObjList.reserve(_vRenderObjects.size());
		for (size_t i = 0; i<_vRenderObjects.size(); i++)
		{
		     renderObjList.push_back(_vRenderObjects[i]);
		}
	}

	void SceneManager::GetRendererList(Camera* camera, std::vector<GeometryRenderer*>& renderObjList) const
	{
		if( _vRenderObjects.size() == 0 )
			return;

		renderObjList.reserve(_vRenderObjects.size());
		for(size_t i=0;i<_vRenderObjects.size();i++)
		{
			if(_vRenderObjects[i]->Overlaps(camera->CameraFrustrum))
				renderObjList.push_back(_vRenderObjects[i]);
		}
	}

 

	void SceneManager::UpdateShaderProperty()
	{
		_sAmbientLowerProperty->SetData(glm::value_ptr(_vAmbientLowerColor));
		_sAmbientUpperProperty->SetData(glm::value_ptr(_vAmbientUpperColor));
 
		_sScreenWidthProperty->SetData(&GConfig->pRenderConfig->SizeX);
		_sScreenHeightProperty->SetData(&GConfig->pRenderConfig->SizeY);
		_sShadowMapSizeProperty->SetData(&GConfig->pRenderConfig->ShadowMapSize);
		_sLineRadius->SetData(&GConfig->pSceneConfig->LineRadius);
		_propertyManager->UpdateShaderProperty();
	}

	void SceneManager::CreateDefaultLight()
	{
		BOOST_ASSERT(_vLightList.size() == 0);
		GameObject* go = new GameObject("DefaultDirLight");
		DirectionLight* light = new DirectionLight("DefaultDirLight");
		go->AddComponent(light);
		GWorld->GetLevel()->AddGameObject(go);
	}

	void SceneManager::CreateDefaultCamera()
	{
		BOOST_ASSERT(_mDefaultCamera == NULL);

	    GameObject* go = new GameObject("DefaultCamera");
	    Camera* sceneCamera = new Camera("DefaultCamera");
	    go->AddComponent(sceneCamera);
		GWorld->GetLevel()->AddGameObject(go);

		_mDefaultCamera = sceneCamera;
		AddCamera(_mDefaultCamera);

		GClient->AddInputListener(_mDefaultCamera);
	}

	void SceneManager::SetDefaultCamera(Camera* camera)
	{
		if( _mDefaultCamera != NULL )
		{
			GClient->ReleaseInputListener(_mDefaultCamera);		
		}

		GClient->AddInputListener(camera);
	   _mDefaultCamera = camera;

	}

	GeometryRenderer* SceneManager::GetRenderer(std::string const& name)
	{
		if( _mRenderObjects.find(name) != _mRenderObjects.end() )
			return _mRenderObjects.at(name);

		return NULL;
	}

	Light* SceneManager::GetLight(std::string const& name)
	{
		if( _mLightObjects.find(name) != _mLightObjects.end() )
			return _mLightObjects.at(name);

		return NULL;
	}

	Camera* SceneManager::GetCamera(std::string const& name)
	{
		if( _mCameraObjects.find(name) != _mCameraObjects.end() )
			return _mCameraObjects.at(name);

		return NULL;
	}

	void SceneManager::AddCamera(Camera* camera)
	{
		BOOST_ASSERT(camera->Name != "");

		if(_mCameraObjects.find(camera->Name) == _mCameraObjects.end())
			_mCameraObjects.insert(std::pair<std::string,Camera*>(camera->Name,camera));
	}
 
	void SceneManager::AddLight(Light* light)
	{
		BOOST_ASSERT(light->Name != "");

		if( _mLightObjects.find(light->Name) == _mLightObjects.end() )
		{
			_mLightObjects.insert(std::pair<std::string,Light*>(light->Name,light));
			_vLightList.push_back(light);
		}
	}

	void SceneManager::AddRenderer(GeometryRenderer* renderer)
	{
		BOOST_ASSERT(renderer->Name != "");

		if( _mRenderObjects.find(renderer->Name) == _mRenderObjects.end() )
		{
			_mRenderObjects.insert(std::pair<std::string, GeometryRenderer*>(renderer->Name, renderer));
			_vRenderObjects.push_back(renderer);
			renderer->UpdateBoundingBox();
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
	}
 

}