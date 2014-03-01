#include "EngineInclude.h"

namespace Disorder
{
	InitialiseSingleton(SceneManager);

	void SceneManager::Init()
	{
		_sceneImporter = FbxSceneImporter::Create();

		_vAmbientLowerColor = Eigen::Vector3f::Constant(0.f);
		_vAmbientUpperColor = Eigen::Vector3f::Constant(0.25f);
		_propertyManager = GEngine->RenderResourceMgr->GetPropertyManager(ShaderPropertyManager::sManagerScene);
		_sAmbientLowerProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sAmbientLowColor,eSP_Float,3);
		_sAmbientUpperProperty = _propertyManager->CreateProperty(ShaderPropertyManager::sAmbientUpperColor,eSP_Float,3);

		EnableDebugDraw = true;
	}

	void SceneManager::Exit()
	{
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

	void SceneManager::GetRendererList(CameraPtr const camera,std::vector<RendererPtr>& renderObjList) const
	{
		if( _vRenderObjects.size() == 0 )
			return;

		renderObjList.reserve(_vRenderObjects.size());
		for(size_t i=0;i<_vRenderObjects.size();i++)
		{
			//if(_vRenderObjects[i]->Overlaps(camera->CameraFrustrum))
				renderObjList.push_back(_vRenderObjects[i]);
		}
	}

	SceneManagerPtr SceneManager::Create()
	{
		SceneManager *pManager = new SceneManager;
		return SceneManagerPtr(pManager);
	}

	void SceneManager::UpdateShaderProperty()
	{
		_sAmbientLowerProperty->SetData(_vAmbientLowerColor.data());
		_sAmbientUpperProperty->SetData(_vAmbientUpperColor.data());
		_propertyManager->UpdateShaderProperty();
	}

	void SceneManager::CreateDefaultCamera()
	{
		BOOST_ASSERT(_mDefaultCamera == NULL);

	    GameObjectPtr go = GameObject::Create("DefaultCamera");
	    CameraPtr sceneCamera = Camera::Create("DefaultCamera");
	    go->AddComponent(sceneCamera);
		GWorld->GetLevel()->AddGameObject(go);

		_mDefaultCamera = sceneCamera;
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
			for(unsigned int j=0;j<_vLightList.size();j++)
			{
				if( _vLightList[j]->Touch(_vRenderObjects[i]) )
				   _vRenderObjects[i]->AddLight(_vLightList[j]);
			}
		}
	}

	void SceneManager::AddLight(LightPtr const& light)
	{
		BOOST_ASSERT(light->Name != "");

		if( _mLightObjects.find(light->Name) == _mLightObjects.end() )
		{
			_mLightObjects.insert(std::pair<std::string,LightPtr>(light->Name,light));
			_vLightList.push_back(light);
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