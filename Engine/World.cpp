#include "EngineInclude.h"

namespace Disorder
{
	 InitialiseSingleton(World);

	 void World::Init()
	 {
		 // temp code to init a world
		
		 LevelPtr level = GEngine->SceneImporter->Import(std::string("TorusMesh.fbx"));
		 LightPtr light = boost::make_shared<Light>();

		 //camera
		 GameObjectPtr camera = SceneObjectGenerator::CreateCamera("MainCamera");
		 level->AddGameObject(camera);

		 GameObjectPtr go = boost::make_shared<GameObject>("test",Vector3(4.0f,0,0));
		 go->GetTransform()->SetLocalRotation(30,30,0);
		 light->Color = Vector3(1.0f,1.0f,1.0f);
		 light->Name = "light";
		 light->LType = LT_Parallel;
		 light->Intensity = 0.7f;
		 light->Range = 100;
		 go->AddComponent(light);
		 level->AddGameObject(go);

		 LightPtr light2 = boost::make_shared<Light>();
		 GameObjectPtr go2 = boost::make_shared<GameObject>("test2",Vector3(-4.0f,0,0));
		 light2->Color = Vector3(1.0f,1.0f,0.0f);
		 light2->Name = "light2";
		 light2->LType = LT_Parallel;
		 light2->Intensity = 0.5f;
		 light2->Range = 100;
		 go2->AddComponent(light2);
		 level->AddGameObject(go2);

		 LightPtr light3 = boost::make_shared<Light>();
		 GameObjectPtr go3 = boost::make_shared<GameObject>("test3",Vector3(0.0f,4.0f,0));
		 light3->Color = Vector3(1.0f,1.0f,0.0f);
		 light3->Name = "light3";
		 light3->LType = LT_Point;
		 light3->Intensity = 0.5f;
		 light3->Range = 100;
		 go3->AddComponent(light3);
		 level->AddGameObject(go3);

		 LightPtr light4 = boost::make_shared<Light>();
		 GameObjectPtr go4 = boost::make_shared<GameObject>("test4",Vector3(8.0f,2.0f,-1));
		 light4->Color = Vector3(0.0f,1.0f,1.0f);
		 light4->Name = "light4";
		 light4->LType = LT_Point;
		 light4->Intensity = 0.5f;
		 light4->Range = 100;
		 go4->AddComponent(light4);
		 level->AddGameObject(go4);

		 
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