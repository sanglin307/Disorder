#include "EngineInclude.h"

namespace Disorder
{
	 InitialiseSingleton(World);

	 WorldPtr World::Create()
	 {
		 World* pWorld = new World;
		 return WorldPtr(pWorld);
	 }

	 void World::Init()
	 {
		 // temp code to init a world		
		 LevelPtr level = GEngine->SceneImporter->Import(std::string("MiniGun_CameraLight.fbx"));

		 if( GSceneManager->GetDefaultCamera() == NULL ) // create default camera
		 {
			 GSceneManager->CreateDefaultCamera();
		 }

    	/* GameObjectPtr go = GameObject::Create("test",Eigen::Vector3f(0.0f,4,-4.0f));
 		 LightPtr light = Light::Create("DefaultLight");
    	 light->Color = Eigen::Vector3f(1.0f,1.0f,1.0f);
 		 
 		 light->LightType = LT_Directional;
 		 light->Intensity = 1.0f;
 		 light->Range = 1000;
 		 go->AddComponent(light);

 		 level->AddGameObject(go);*/

		 GEngine->RenderResourceMgr->DumpContent();
	 }

	 void World::Exit()
	 {
	 }
 
	 void World::Tick(float deltaSeconds)
	 {
		 LevelContainer::iterator iter;
		 for(iter = _Levels.begin();iter!= _Levels.end();iter++)
		 {
			 (*iter)->Tick(deltaSeconds);
		 }
	 }

	 void World::AddLevel(LevelPtr const& level)
	 {
		 _Levels.push_back(level);
	 }

	 LevelPtr World::GetLevel()
	 {
		 if(_Levels.size() > 0 )
			 return *(_Levels.begin());

		 return NULL;
	 }

	LevelPtr World::GetLevel(std::string const& name)
	{
		 LevelContainer::iterator iter;
		 for(iter = _Levels.begin();iter!= _Levels.end();iter++)
		 {
			 return *iter;
		 }

		 return NULL;
	}
}