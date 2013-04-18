#include "EngineInclude.h"

namespace Disorder
{
	 InitialiseSingleton(World);

	 void World::Init()
	 {
		 // temp code to init a world
		
		 GEngine->SceneImporter->Import(std::string("TorusMesh.fbx"));
		 LevelPtr level = boost::make_shared<Level>();
		 Vector3 position = Vector3(0.0f);
		 GameObjectPtr gameObject = boost::make_shared<GameObject>("",position);
		 GeometryPtr cube = GeometryGenerator::CreateCube(1.0);
		 GeometryRendererPtr cuberender = boost::make_shared<GeometryRenderer>(gameObject);
		 MaterialPtr mat = MaterialGenerator::GenerateLambert();
		 cuberender->SetGeometry(cube,mat);
		 gameObject->AddComponent(cuberender);
		 level->AddGameObject(gameObject);
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