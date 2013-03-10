#include "EngineInclude.h"

namespace Disorder
{
	 InitialiseSingleton(World);

	 void World::Init()
	 {
		 // temp code to init a world
		
		 LevelPtr level = boost::make_shared<Level>();
		 Vector3 position = Vector3(0.0f);
		 GameObjectPtr gameObject = boost::make_shared<GameObject>(position);
		 CubeRendererPtr cube = boost::make_shared<CubeRenderer>(gameObject,1.0f);
		 gameObject->AddComponent(cube);
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