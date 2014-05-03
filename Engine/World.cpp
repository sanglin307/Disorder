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
		 LevelPtr level = GEngine->SceneImporter->Import(GConfig->pSceneConfig->LevelName);

		 if( GSceneManager->GetDefaultCamera() == NULL ) // create default camera
		 {
			 GSceneManager->CreateDefaultCamera();
		 }
 

		 GEngine->RenderResourceMgr->DumpContent();
	 }

	 void World::Exit()
	 {
		 _Levels.empty();
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