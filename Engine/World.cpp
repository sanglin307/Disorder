#include "EngineInclude.h"

namespace Disorder
{
	 World *GWorld = NULL;

	 World::World()
	 {
		 // temp code to init a world		
		 GSceneImporter->Import(GConfig->pSceneConfig->LevelName);

		 if( GSceneManager->GetDefaultCamera() == NULL ) // create default camera
		 {
			 GSceneManager->CreateDefaultCamera();
		 }

		 if (GSceneManager->GetLightsList().size() == 0)
		 {
			 GSceneManager->CreateDefaultLight();
		 }
 

		 GRenderResourceMgr->DumpContent();
	 }

	 World::~World()
	 {
		 for (size_t i = 0; i < _levels.size(); i++)
		 {
			 delete _levels[i];
		 }

		 _levels.empty();
	 }
 
	 void World::Tick(float deltaSeconds)
	 {
		 for (size_t i = 0; i < _levels.size(); i++)
		 {
			 _levels[i]->Tick(deltaSeconds);
		 }
	 }

	 void World::AddLevel(Level *level)
	 {
		 _levels.push_back(level);
	 }

	 Level* World::GetLevel()
	 {
		 if(_levels.size() > 0 )
			 return _levels[0];

		 return NULL;
	 }

	Level* World::GetLevel(std::string const& name)
	{
		for (size_t i = 0; i < _levels.size();i++)
		 {
			if (_levels[i]->Name == name)
			   return _levels[i];
		 }

		 return NULL;
	}
}