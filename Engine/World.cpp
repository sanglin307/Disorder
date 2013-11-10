#include "EngineInclude.h"

namespace Disorder
{
	 InitialiseSingleton(World);

	 void World::Init()
	 {
		 // temp code to init a world
		
		 LevelPtr level = GEngine->SceneImporter->Import(std::string("MiniGun.fbx"));
		 LightPtr light = boost::make_shared<Light>();

		 //camera
		 GameObjectPtr camera = SceneObjectGenerator::CreateCamera("MainCamera");
		 level->AddGameObject(camera);

		 GameObjectPtr go = boost::make_shared<GameObject>("test",Vector3(0.0f,4,-4.0f));
		 
		 light->Color = Vector3(1.0f,1.0f,1.0f);
		 light->Name = "light";
		 light->LightType = LT_Parallel;
		 light->Intensity = 1.0f;
		 light->Range = 1000;
		 go->AddComponent(light);
		 level->AddGameObject(go);
 
		 Levels.push_back(level);
	 }

	 void World::Exit()
	 {
	 }

	 void World::Tick(float deltaSeconds)
	 {
		 LevelContainer::iterator iter;
		 for(iter=Levels.begin();iter!= Levels.end();iter++)
		 {
			 (*iter)->Tick(deltaSeconds);
		 }
	 }



}